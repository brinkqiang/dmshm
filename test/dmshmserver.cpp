
#include "dmshmserver.h"
#include "dmshm.h"
#include "dmcsv_parser.hpp"
#include "dmutil.h"
#include "dmformat.h"

#define  GSHM_SERVER_CSV "gshmserver.csv"

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
            fmt::fprintf(stdout, "CreateShmem name=%s, bufsize=%d, bufcount=%d\n", oConfig.name.c_str(), oConfig.bufsize, oConfig.bufcount);
        }
    }
    return true;
}

bool CDMShmServer::Run()
{
    return true;
}

void CDMShmServer::ThrdProc()
{
    if (!Init())
    {
        return;
    }
    fmt::fprintf(stdout, "GShmServer Begin\n");
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
    fmt::fprintf(stdout, "GShmServer End\n");
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
        std::string strPath = DMGetRootPath() + PATH_DELIMITER_STR + GSHM_SERVER_CSV;
        csv::CSVReader reader(strPath);
        csv::CSVRow rows;

        for (size_t i = 0; reader.read_row(rows); i++) {
            DMShmConfig oConfig;
            oConfig.name = DMGetRootPath() + PATH_DELIMITER_STR + rows["name"].get<std::string>();
            oConfig.bufsize = rows["bufsize"].get<int32_t>();
            oConfig.bufcount = rows["bufcount"].get<int32_t>();

            m_vecShmConfig.push_back(oConfig);
        }
    }
    catch (std::exception& e)
    {
        fmt::fprintf(stdout, "Load %s failed. ? %s.\n", GSHM_SERVER_CSV, e.what());
        return false;
    }
    return true;
}
