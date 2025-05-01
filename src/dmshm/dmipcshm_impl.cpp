#include "dmipcshm_impl.h"

void DMAPI dmipcshmImpl::Release(void)
{

}

void DMAPI dmipcshmImpl::Test(void)
{

}

bool DMAPI dmipcshmImpl::CreateShmRegion(const std::string& name, size_t size)
{
    return false;
}

bool DMAPI dmipcshmImpl::OpenShmRegion(const std::string& name)
{
    return false;
}

void DMAPI dmipcshmImpl::CloseShmRegion(const std::string& name)
{

}

const DMShmRegion* DMAPI dmipcshmImpl::GetShmRegion(const std::string& name)
{
    return nullptr;
}

bool DMAPI dmipcshmImpl::SendMessage(const std::string& region_name, const DMShmMessage* msg)
{
    return false;
}

bool DMAPI dmipcshmImpl::ReceiveMessage(const std::string& region_name, int timeout_ms, DMShmMessage** msg)
{
    return false;
}

void DMAPI dmipcshmImpl::RegisterMessageCallback(const std::string& region_name, DMShmMessageCallback callback, void* user_data)
{

}

void DMAPI dmipcshmImpl::UnregisterMessageCallback(const std::string& region_name)
{

}

std::vector<std::string> DMAPI dmipcshmImpl::ListShmRegions()
{
    return {};
}

void DMAPI dmipcshmImpl::SetSyncTimeout(uint32_t timeout_ms)
{

}

uint32_t DMAPI dmipcshmImpl::GetCurrentPid()
{
    return 0;
}

void DMAPI dmipcshmImpl::SetDebugLevel(int level)
{

}

extern "C" DMEXPORT_DLL Idmipcshm* DMAPI dmipcshmGetModule()
{
    return new dmipcshmImpl();
}
