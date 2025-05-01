// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)

#ifndef __DMIPCSHM_IMPL_H_INCLUDE__
#define __DMIPCSHM_IMPL_H_INCLUDE__

#include "dmipcshm.h"
#include "dmos.h"
#include "dmtypes.h"
#include <unordered_map>
#include <string>
#include <vector>

class dmipcshmImpl : public Idmipcshm {
private:
    // 成员变量
    std::unordered_map<std::string, DMShmRegion> m_mRegions;
    std::unordered_map<std::string, DMShmConnection> m_mConnections;
    uint32_t m_nSyncTimeout = 3000;
    int m_nDebugLevel = 0;

public:
    virtual void DMAPI Release(void) override;
    virtual void DMAPI Test(void) override;
    
    virtual bool DMAPI CreateShmRegion(const std::string& name, size_t size) override;
    virtual bool DMAPI OpenShmRegion(const std::string& name) override;
    virtual void DMAPI CloseShmRegion(const std::string& name) override;
    
    virtual const DMShmRegion* DMAPI GetShmRegion(const std::string& name) override;
    
    virtual bool DMAPI SendMessage(const std::string& region_name, const DMShmMessage* msg) override;
    virtual bool DMAPI ReceiveMessage(const std::string& region_name, int timeout_ms, DMShmMessage** msg) override;
    
    virtual void DMAPI RegisterMessageCallback(const std::string& region_name, 
                                             DMShmMessageCallback callback, 
                                             void* user_data) override;
    virtual void DMAPI UnregisterMessageCallback(const std::string& region_name) override;
    
    virtual std::vector<std::string> DMAPI ListShmRegions() override;
    
    virtual void DMAPI SetSyncTimeout(uint32_t timeout_ms) override;
    virtual uint32_t DMAPI GetCurrentPid() override;
    virtual void DMAPI SetDebugLevel(int level) override;
};

#endif // __DMIPCSHM_IMPL_H_INCLUDE__
