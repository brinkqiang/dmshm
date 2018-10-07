#ifndef __GSHMSERVER_H_INCLUDE__
#define __GSHMSERVER_H_INCLUDE__
#include "dmos.h"
#include "dmtypes.h"
#include "dmshm.h"
#include "dmconsole.h"
#include "dmthread.h"

struct DMShmInfo 
{
    DMShmem m_oShmem;
    DMShmConfig m_oConfig;
};

class CDMShmServer :
    public IDMConsoleSink,
    public IDMThread,
    public CDMThreadCtrl,
    public CDMSafeSingleton<CDMShmServer>
{
    friend class CDMSafeSingleton<CDMShmServer>;
public:
    CDMShmServer();

    bool Init();
    void UnInit();
    bool Run();

    virtual void ThrdProc();
    virtual void Terminate();

    virtual void OnCloseEvent();
private:
    bool __LoadCSV();
private:
    std::vector<DMShmConfig> m_vecShmConfig;
    std::map<std::string, DMShmInfo> m_mapShmInfo;
    volatile bool   m_bStop;
};

#endif // __GSHMSERVER_H_INCLUDE__