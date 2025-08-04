#include "gtest.h"
#include "dmsingleipc.h"
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <iostream>
#include <future>

// 定义要传输的数据结构
struct PerformanceTestData {
    uint64_t sequence;
    char     payload[120]; // 凑齐128字节
};

class IPCPerformanceTest : public ::testing::Test {
protected:
    // 为生产者和消费者分别创建模块实例
    dmsingleipcPtr producer_ipc_;
    dmsingleipcPtr consumer_ipc_;

    const std::string shm_name_ = "ipc_perf_test_shm";
    static constexpr size_t kQueueCapacity = 1024; // 队列容量

    void SetUp() override {
        // 使用您偏好的指针断言风格
        ASSERT_TRUE(producer_ipc_.get() == nullptr);
        ASSERT_TRUE(consumer_ipc_.get() == nullptr);

        producer_ipc_.reset(dmSingleIPCGetModule());
        consumer_ipc_.reset(dmSingleIPCGetModule());

        ASSERT_TRUE(producer_ipc_.get() != nullptr);
        ASSERT_TRUE(consumer_ipc_.get() != nullptr);

        // 由生产者创建共享内存队列
        bool created = producer_ipc_->Create(shm_name_, sizeof(PerformanceTestData), kQueueCapacity);
        ASSERT_TRUE(created);

        // 消费者打开同一个队列
        bool opened = consumer_ipc_->Open(shm_name_);
        ASSERT_TRUE(opened);
    }

    void TearDown() override {
        // Close 会被 dmsingleipcPtr 的析构函数自动调用
        // Teardown 会在 producer_ipc_ 和 consumer_ipc_ 析构时自动完成清理
    }
};

// 测试极限QPS的测试用例
TEST_F(IPCPerformanceTest, MeasuresQpsUnderStress) {
    std::cout << std::endl;
    std::cout << "[ IPC Performance Test ]" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Queue Capacity: " << kQueueCapacity << " items" << std::endl;
    std::cout << "Item Size: " << sizeof(PerformanceTestData) << " bytes" << std::endl;
    
    // 原子停止标志
    std::atomic<bool> stop_flag(false);
    const auto test_duration = std::chrono::seconds(5);

    // 生产者任务 (使用 std::async 以方便地获取返回值)
    auto producer_task = std::async(std::launch::async, [&]() -> uint64_t {
        PerformanceTestData data{0, "producer_data"};
        uint64_t success_count = 0;
        while (!stop_flag.load(std::memory_order_relaxed)) {
            data.sequence = success_count;
            // 持续尝试推送，不间断
            if (producer_ipc_->Push(&data, sizeof(data))) {
                success_count++;
            } else {
                // 如果队列满了，让出CPU时间片，避免100%空转消耗
                std::this_thread::yield();
            }
        }
        return success_count;
    });

    // 消费者任务
    auto consumer_task = std::async(std::launch::async, [&]() -> uint64_t {
        PerformanceTestData data;
        uint64_t success_count = 0;
        while (!stop_flag.load(std::memory_order_relaxed)) {
            // 持续尝试弹出，不间断
            if (consumer_ipc_->Pop(&data, sizeof(data))) {
                success_count++;
            } else {
                // 如果队列空了，让出CPU时间片
                std::this_thread::yield();
            }
        }
        return success_count;
    });

    std::cout << "Test running for " << test_duration.count() << " seconds..." << std::endl;
    std::this_thread::sleep_for(test_duration);

    // 时间到，通知所有线程停止
    stop_flag.store(true);

    // 等待线程结束并获取结果
    uint64_t total_produced = producer_task.get();
    uint64_t total_consumed = consumer_task.get();

    // 计算QPS
    double producer_qps = static_cast<double>(total_produced) / test_duration.count();
    double consumer_qps = static_cast<double>(total_consumed) / test_duration.count();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Producer finished." << std::endl;
    std::cout << "  - Total items pushed: " << total_produced << std::endl;
    std::cout << "  - Producer QPS: " << std::fixed << producer_qps / 1e6 << " M ops/sec" << std::endl;
    
    std::cout << "Consumer finished." << std::endl;
    std::cout << "  - Total items consumed: " << total_consumed << std::endl;
    std::cout << "  - Consumer QPS: " << std::fixed << consumer_qps / 1e6 << " M ops/sec" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // 断言检查：确保测试至少成功运行并处理了一些数据
    ASSERT_GT(total_produced, 0);
    ASSERT_GT(total_consumed, 0);
}