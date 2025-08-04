#include "libdmsingleipc_impl.h"

DmSingleIPCImpl::DmSingleIPCImpl()
    : m_pQueue(nullptr), m_isCreator(false)
{
}

DmSingleIPCImpl::~DmSingleIPCImpl()
{
    internal_close();
}

void DmSingleIPCImpl::Release(void)
{
    delete this;
}

bool DmSingleIPCImpl::Create(const std::string& name, size_t item_size, size_t item_count)
{
    if (m_pQueue || name.empty() || item_size == 0 || item_count == 0)
    {
        return false;
    }

    // 使用新的队列类的静态方法计算所需的总内存
    const uint32_t total_size = CDMSharedAtomicByteQueue::get_required_size(item_size, item_count);
    m_shmName = name;
    
    // 创建共享内存
    void* shm_ptr = m_shm.Create(m_shmName.c_str(), total_size);
    if (!shm_ptr)
    {
        return false;
    }

    // 在共享内存上初始化队列
    m_pQueue = CDMSharedAtomicByteQueue::InitAt(shm_ptr, total_size, item_size, item_count);
    if (!m_pQueue)
    {
        m_shm.Close(); // 初始化失败，清理共享内存
        return false;
    }
    
    m_isCreator = true;
    return true;
}

bool DmSingleIPCImpl::Open(const std::string& name)
{
    if (m_pQueue || name.empty())
    {
        return false;
    }
    
    m_shmName = name;
    void* shm_ptr = m_shm.Open(m_shmName.c_str());
    if (!shm_ptr)
    {
        return false;
    }
    
    // 对于打开者，直接将共享内存指针转型为队列指针即可
    m_pQueue = static_cast<CDMSharedAtomicByteQueue*>(shm_ptr);
    m_isCreator = false;
    return true;
}

void DmSingleIPCImpl::internal_close()
{
    if (!m_pQueue)
    {
        return;
    }
    
    if (m_isCreator)
    {
        // 对于创建者，需要显式调用析构函数
        // 因为对象是通过 placement new 创建的
        m_pQueue->~CDMSharedAtomicByteQueue();
    }
    
    m_shm.Close();
    
    m_pQueue = nullptr;
    m_shmName.clear();
    m_isCreator = false;
}

void DmSingleIPCImpl::Close()
{
    internal_close();
}

bool DmSingleIPCImpl::Push(const void* data, size_t size)
{
    if (!m_pQueue || !data || size != m_pQueue->get_item_size())
    {
        return false;
    }
    // 将操作委托给队列对象
    return m_pQueue->try_push(data);
}

bool DmSingleIPCImpl::Pop(void* buffer, size_t buffer_size)
{
    if (!m_pQueue || !buffer || buffer_size < m_pQueue->get_item_size())
    {
        return false;
    }
    // 将操作委托给队列对象
    return m_pQueue->try_pop(buffer);
}


// --- Factory Function ---
extern "C" DMEXPORT_DLL IdmSingleIPC* DMAPI dmSingleIPCGetModule()
{
    return new DmSingleIPCImpl();
}