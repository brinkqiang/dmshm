
#include "dmshmagent.h"
#include "dmshm.h"
#include "dmutil.h"
#include "dmcsv_parser.hpp"

#define  DMSHM_AGENT_CSV "dmshmagent.csv"

CDMShmAgent::CDMShmAgent()
    : m_nIndex(0)
{

}

bool CDMShmAgent::Init()
{
    if (!__LoadCSV())
    {
        return false;
    }

    m_oShmem = DMOpenShmem(m_oConfig.name.c_str());
    if (NULL == m_oShmem.mem)
    {
        fmt::fprintf(stdout, "OpenShmem %s failed\n", m_oConfig.name.c_str());
        return false;
    }

    fmt::fprintf(stdout, "OpenShmem name=%s, bufsize=%d, bufcount=%d\n", m_oConfig.name.c_str(), m_oConfig.bufsize, m_oConfig.bufcount);
    return true;
}

void CDMShmAgent::Log(DMLogMsgLevels level, const char *srcFilename, int srcLine, const std::string &message)
{
    if (NULL == m_oShmem.mem)
    {
        return;
    }
    snprintf(((char*)m_oShmem.mem) + m_nIndex * m_oConfig.bufcount, m_oConfig.bufsize, "%s", message.c_str());
    fmt::fprintf(stdout, "Log[%d]: %s\n", m_nIndex, ((char*)m_oShmem.mem) + m_nIndex * m_oConfig.bufcount);
    m_nIndex++;
    if (m_nIndex >= m_oConfig.bufcount)
    {
        m_nIndex = 0;
    }
}

bool CDMShmAgent::__LoadCSV()
{
    try
    {
        std::string strPath = DMGetRootPath() + PATH_DELIMITER_STR + DMSHM_AGENT_CSV;
        csv::CSVReader reader(strPath);
        csv::CSVRow rows;

        if (reader.read_row(rows))
        {
            m_oConfig.name = rows["name"].get<std::string>();
            m_oConfig.bufsize = rows["bufsize"].get<int32_t>();
            m_oConfig.bufcount = rows["bufcount"].get<int32_t>();
        }
    }
    catch (std::exception& e)
    {
        fmt::fprintf(stdout, "Load %s failed. ? %s.\n", DMSHM_AGENT_CSV, e.what());
        return false;
    }

    return true;
}
