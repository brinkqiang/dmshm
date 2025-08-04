#ifndef __DMATOMIC_SHM_QUEUE_H_INCLUDE__
#define __DMATOMIC_SHM_QUEUE_H_INCLUDE__

#include <atomic>
#include <cassert>
#include <cstddef>
#include <new>      // Required for placement new
#include <cstring>  // Required for memcpy

/**
 * @class CDMSharedAtomicByteQueue
 * @brief һ��Ϊ�����ڴ���Ƶĵ�������/�������������ֽڶ��С�
 *
 * ��������˼��Դ�����ṩ�� dmatomic_queue.h�������˹ؼ��޸ģ�
 * 1. �������з����ڴ棬����ͨ�� placement new ��һ������Ĺ����ڴ��ϴ�����
 * 2. �������Ϊ����ԭʼ�ֽ�����ͨ�� memcpy�����������ض����͵Ķ���
 */
class CDMSharedAtomicByteQueue {
public:
    /**
     * @brief (�ɴ����ߵ���) �ڸ������ڴ���ϳ�ʼ�����С�
     * @param memory ָ�����ڴ���ָ�롣
     * @param total_size �����ڴ����ܴ�С��
     * @param item_size ÿ��������Ĵ�С��
     * @param item_count ���е�������
     * @return �ɹ��򷵻�ָ����õĶ��ж����ָ�룬ʧ���򷵻� nullptr��
     */
    static CDMSharedAtomicByteQueue* InitAt(void* memory, size_t total_size, size_t item_size, size_t item_count) {
        if (!memory || total_size < get_required_size(item_size, item_count)) {
            return nullptr;
        }
        // ʹ�� placement new �ڹ����ڴ����ʼλ�ù�����ж�����
        return new (memory) CDMSharedAtomicByteQueue(item_size, item_count);
    }

    /**
     * @brief ��������ָ�������ʹ�С��Ԫ��������ܹ����ڴ��С��
     * @param item_size ÿ��������Ĵ�С��
     * @param item_count ���е�������
     * @return ��������ֽ�����
     */
    static size_t get_required_size(size_t item_size, size_t item_count) {
        // �ܴ�С = ���ж�����Ĵ�С + ���λ������Ĵ�С
        // ����+1 ��Ϊ�����ֶ����Ͷӿ�
        return sizeof(CDMSharedAtomicByteQueue) + (item_count + 1) * item_size;
    }

    // ��ֹ�������ƶ�����Ϊ����״̬���ض��ڴ�λ�ð�
    CDMSharedAtomicByteQueue(const CDMSharedAtomicByteQueue&) = delete;
    CDMSharedAtomicByteQueue& operator=(const CDMSharedAtomicByteQueue&) = delete;

    /**
     * @brief ���Խ�һ��������������С�
     * @param data ָ��Ҫ��������ݵ�ָ�롣
     * @return �ɹ����� true��������������򷵻� false��
     */
    bool try_push(const void* data) {
        const auto head = head_.load(std::memory_order_relaxed);
        auto next_head = head + 1;
        if (next_head == capacity_) {
            next_head = 0;
        }
        if (next_head == tail_.load(std::memory_order_acquire)) {
            return false; // ��������
        }

        // �����ݿ�����������
        memcpy(get_slot(head), data, item_size_);
        
        head_.store(next_head, std::memory_order_release);
        return true;
    }

    /**
     * @brief ���ԴӶ����е���һ�������
     * @param buffer ���ڽ������ݵĻ�������
     * @return �ɹ����� true���������Ϊ���򷵻� false��
     */
    bool try_pop(void* buffer) {
        const auto tail = tail_.load(std::memory_order_relaxed);
        if (head_.load(std::memory_order_acquire) == tail) {
            return false; // ����Ϊ��
        }
        
        // �ӻ�������������
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
    // ˽�й��캯����ֻ��ͨ�� InitAt ������������
    CDMSharedAtomicByteQueue(size_t item_size, size_t item_count)
        : item_size_(item_size),
          capacity_(item_count + 1), // ʵ������������Ķ�һ�����������ֿ�/��
          head_(0),
          tail_(0) {}

    // ˽��������������ֹ����� delete ����
    ~CDMSharedAtomicByteQueue() {}

    /**
     * @brief ��ȡ���λ�������ָ������λ�õ�ָ�롣
     */
    void* get_slot(size_t index) {
        // ������������ CDMSharedAtomicByteQueue ����֮��
        return reinterpret_cast<char*>(this) + sizeof(CDMSharedAtomicByteQueue) + index * item_size_;
    }

private:
    // Ϊ�˷�ֹα����false sharing������ԭ�ӱ����ڲ�ͬ�Ļ�����
    static constexpr size_t kCacheLineSize = 128;

    const size_t item_size_;
    const size_t capacity_;

    // head_ �� tail_ �ֲ��ڲ�ͬ�Ļ������ϣ��Ա�����CPU����α�����µ������½�
    alignas(kCacheLineSize) std::atomic<size_t> head_;
    alignas(kCacheLineSize) std::atomic<size_t> tail_;
};

#endif // __DMATOMIC_SHM_QUEUE_H_INCLUDE__