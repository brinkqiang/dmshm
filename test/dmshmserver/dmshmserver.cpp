
#include "dmshmserver.h"
#include "dmshm.h"
#include "dmcsv_parser.hpp"
#include "dmutil.h"
#include "dmformat.h"

#define  DMSHM_SERVER_CSV "dmshmserver.csv"

CDMShmServer::CDMShmServer()
    : m_bStop(false)
{
    HDMConsoleMgr::Instance()->SetHandlerHook(this);
}

bool CDMShmServer::Init()
{
    if (!__LoadCSV())
    {
        return false;
    }

    for (int i=0; i < static_cast<int>(m_vecShmConfig.size()); ++i)
    {
        DMShmConfig& oConfig = m_vecShmConfig[i];

        DMShmInfo& oInfo = m_mapShmInfo[oConfig.name];
        oInfo.m_oConfig = oConfig;
        oInfo.m_oShmem = DMCreateShmem(oConfig.name.c_str(), oConfig.bufsize * oConfig.bufcount);
        if (NULL != oInfo.m_oShmem.mem)
        {
            memset(oInfo.m_oShmem.mem, 0, oConfig.bufsize * oConfig.bufcount);
            std::fprintf(stdout, "CreateShmem name=%s, bufsize=%d, bufcount=%d\n", oConfig.name.c_str(), oConfig.bufsize, oConfig.bufcount);
        }
    }
    return true;
}

void CDMShmServer::UnInit()
{
    for (int i = 0; i < static_cast<int>(m_vecShmConfig.size()); ++i)
    {
        DMShmConfig& oConfig = m_vecShmConfig[i];

        DMShmInfo& oInfo = m_mapShmInfo[oConfig.name];

        DMCloseShmem(&oInfo.m_oShmem);
    }
}

bool CDMShmServer::Run()
{
    return false;
}

void CDMShmServer::ThrdProc()
{
    if (!Init())
    {
        return;
    }
    std::fprintf(stdout, "ShmServer Begin\n");
    bool bBusy = false;
    while (!m_bStop) {
        bBusy = false;

        if (Run()) {
            bBusy = true;
        }

        if (!bBusy) {
            SleepMs(1);
        }
    }
    std::fprintf(stdout, "ShmServer Ending\n");

    UnInit();

    std::fprintf(stdout, "ShmServer End\n");
    SleepMs(2000);
}

void CDMShmServer::Terminate()
{
    m_bStop = true;
}

void CDMShmServer::OnCloseEvent()
{
    Stop();
}

bool CDMShmServer::__LoadCSV()
{
    try
    {
        std::string strPath = DMGetRootPath() + PATH_DELIMITER_STR + DMSHM_SERVER_CSV;
        csv::CSVReader reader(strPath);
        csv::CSVRow rows;

        for (size_t i = 0; reader.read_row(rows); i++) {
            DMShmConfig oConfig;
            oConfig.name = rows["name"].get<std::string>();
            oConfig.bufsize = rows["bufsize"].get<int32_t>();
            oConfig.bufcount = rows["bufcount"].get<int32_t>();

            m_vecShmConfig.push_back(oConfig);
        }
    }
    catch (std::exception& e)
    {
        std::fprintf(stdout, "Load %s failed. ? %s.\n", DMSHM_SERVER_CSV, e.what());
        return false;
    }
    return true;
}
