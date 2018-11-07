
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
        std::fprintf(stdout, "OpenShmem %s failed\n", m_oConfig.name.c_str());
        return false;
    }

    std::fprintf(stdout, "OpenShmem name=%s, bufsize=%d, bufcount=%d\n", m_oConfig.name.c_str(), m_oConfig.bufsize, m_oConfig.bufcount);

    DMAgentHead* pHead = NULL;
    for (int i=0; i < m_oConfig.bufcount; ++i)
    {
        pHead = (DMAgentHead*)(m_oShmem.mem + i * m_oConfig.bufsize);

        if (!pHead->flags.used)
        {
            continue;
        }

        DMAgentRecord oRecord((uint8_t*)(pHead));
        AddRecord(oRecord.GetKey(), oRecord);
    }

    return true;
}

bool CDMShmAgent::Write(const std::string &key, const std::string &message)
{
    if (NULL == m_oShmem.mem)
    {
        return false;
    }

    if (Full())
    {
        return false;
    }

    DMAgentRecord* poRecord = FindRecord(key);
    if (NULL != poRecord)
    {
        return false;
    }

    DMAgentHead* pHead = NULL;
    do 
    {
        pHead = (DMAgentHead*)(m_oShmem.mem + m_nIndex * m_oConfig.bufsize);

        if (!pHead->flags.used)
        {
            pHead->key_size = key.size();
            pHead->msg_size = message.size();
            break;
        }
        m_nIndex++;

        if (m_nIndex >= m_oConfig.bufcount)
        {
            m_nIndex = 0;
        }

        if (Full())
        {
            return false;
        }
    } while (true);

    memcpy(((uint8_t*)pHead) + sizeof(DMAgentHead), key.data(), key.size());
    memcpy(((uint8_t*)pHead) + sizeof(DMAgentHead) + key.size(), message.data(), message.size());
    pHead->flags.used = true;

    DMAgentRecord oRecord((uint8_t*)(pHead));
    AddRecord(key, oRecord);
    return true;
}

bool CDMShmAgent::Read(const std::string &key, std::string* message)
{
    if (NULL == m_oShmem.mem)
    {
        return false;
    }
    DMAgentRecord* poRecord = FindRecord(key);
    if (NULL == poRecord)
    {
        return false;
    }
    *message = std::string((char*)poRecord->GetMsg(), poRecord->GetMsgSize());
    return true;
}

bool CDMShmAgent::Remove(const std::string &key)
{
    MapDMAgentRecordIt It = m_mapRecord.find(key);
    if (It == m_mapRecord.end())
    {
        return false;
    }

    m_mapRecord.erase(key);

    return true;
}

DMAgentRecord* CDMShmAgent::FindRecord(const std::string &key)
{
    MapDMAgentRecordIt It = m_mapRecord.find(key);
    if (It == m_mapRecord.end())
    {
        return NULL;
    }

    return &It->second;
}

void CDMShmAgent::AddRecord(const std::string &key, DMAgentRecord& data)
{
    m_mapRecord[key] = data;
}

bool CDMShmAgent::Full()
{
    return m_mapRecord.size() >= m_oConfig.bufcount;
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
        std::fprintf(stdout, "Load %s failed. ? %s.\n", DMSHM_AGENT_CSV, e.what());
        return false;
    }

    return true;
}

bool DMShmAgentInit()
{
    return CDMShmAgent::Instance()->Init();
}
