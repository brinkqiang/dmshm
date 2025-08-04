#ifndef __DMATOMIC_SHM_QUEUE_H_INCLUDE__
#define __DMATOMIC_SHM_QUEUE_H_INCLUDE__

#include <atomic>
#include <cassert>
#include <cstddef>
#include <new>      // Required for placement new
#include <cstring>  // Required for memcpy

/**
 * @class CDMSharedAtomicByteQueue
 * @brief 一个为共享内存设计的单生产者/单消费者无锁字节队列。
 *
 * 该类的设计思想源自您提供的 dmatomic_queue.h，但做了关键修改：
 * 1. 它不自行分配内存，而是通过 placement new 在一块给定的共享内存上创建。
 * 2. 它被设计为处理原始字节流（通过 memcpy），而不是特定类型的对象。
 */
class CDMSharedAtomicByteQueue {
public:
    /**
     * @brief (由创建者调用) 在给定的内存块上初始化队列。
     * @param memory 指向共享内存块的指针。
     * @param total_size 共享内存块的总大小。
     * @param item_size 每个数据项的大小。
     * @param item_count 队列的容量。
     * @return 成功则返回指向构造好的队列对象的指针，失败则返回 nullptr。
     */
    static CDMSharedAtomicByteQueue* InitAt(void* memory, size_t total_size, size_t item_size, size_t item_count) {
        if (!memory || total_size < get_required_size(item_size, item_count)) {
            return nullptr;
        }
        // 使用 placement new 在共享内存的起始位置构造队列对象本身
        return new (memory) CDMSharedAtomicByteQueue(item_size, item_count);
    }

    /**
     * @brief 计算容纳指定数量和大小的元素所需的总共享内存大小。
     * @param item_size 每个数据项的大小。
     * @param item_count 队列的容量。
     * @return 所需的总字节数。
     */
    static size_t get_required_size(size_t item_size, size_t item_count) {
        // 总大小 = 队列对象本身的大小 + 环形缓冲区的大小
        // 容量+1 是为了区分队满和队空
        return sizeof(CDMSharedAtomicByteQueue) + (item_count + 1) * item_size;
    }

    // 禁止拷贝和移动，因为队列状态与特定内存位置绑定
    CDMSharedAtomicByteQueue(const CDMSharedAtomicByteQueue&) = delete;
    CDMSharedAtomicByteQueue& operator=(const CDMSharedAtomicByteQueue&) = delete;

    /**
     * @brief 尝试将一个数据项推入队列。
     * @param data 指向要推入的数据的指针。
     * @return 成功返回 true，如果队列已满则返回 false。
     */
    bool try_push(const void* data) {
        const auto head = head_.load(std::memory_order_relaxed);
        auto next_head = head + 1;
        if (next_head == capacity_) {
            next_head = 0;
        }
        if (next_head == tail_.load(std::memory_order_acquire)) {
            return false; // 队列已满
        }

        // 将数据拷贝到缓冲区
        memcpy(get_slot(head), data, item_size_);
        
        head_.store(next_head, std::memory_order_release);
        return true;
    }

    /**
     * @brief 尝试从队列中弹出一个数据项。
     * @param buffer 用于接收数据的缓冲区。
     * @return 成功返回 true，如果队列为空则返回 false。
     */
    bool try_pop(void* buffer) {
        const auto tail = tail_.load(std::memory_order_relaxed);
        if (head_.load(std::memory_order_acquire) == tail) {
            return false; // 队列为空
        }
        
        // 从缓冲区拷贝数据
        memcpy(buffer, get_slot(tail), item_size_);

        auto next_tail = tail + 1;
        if (next_tail == capacity_) {
            next_tail = 0;
        }
        tail_.store(next_tail, std::memory_order_release);
        return true;
    }
    
    size_t get_item_size() const { return item_size_; }

private:
    // 私有构造函数，只能通过 InitAt 工厂方法调用
    CDMSharedAtomicByteQueue(size_t item_size, size_t item_count)
        : item_size_(item_size),
          capacity_(item_count + 1), // 实际容量比请求的多一个，用于区分空/满
          head_(0),
          tail_(0) {}

    // 私有析构函数，防止意外的 delete 调用
    ~CDMSharedAtomicByteQueue() {}

    /**
     * @brief 获取环形缓冲区中指定索引位置的指针。
     */
    void* get_slot(size_t index) {
        // 缓冲区紧跟在 CDMSharedAtomicByteQueue 对象之后
        return reinterpret_cast<char*>(this) + sizeof(CDMSharedAtomicByteQueue) + index * item_size_;
    }

private:
    // 为了防止伪共享（false sharing），让原子变量在不同的缓存行
    static constexpr size_t kCacheLineSize = 128;

    const size_t item_size_;
    const size_t capacity_;

    // head_ 和 tail_ 分布在不同的缓存行上，以避免多核CPU下因伪共享导致的性能下降
    alignas(kCacheLineSize) std::atomic<size_t> head_;
    alignas(kCacheLineSize) std::atomic<size_t> tail_;
};

#endif // __DMATOMIC_SHM_QUEUE_H_INCLUDE__