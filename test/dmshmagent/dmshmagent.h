
#ifndef __GSHMAGENT_H_INCLUDE__
#define __GSHMAGENT_H_INCLUDE__

#include "dmos.h"
#include "dmtypes.h"
#include "dmsingleton.h"
#include "dmshm.h"
#include "dmformat.h"

typedef struct tagDMAgentHead
{
    uint32_t key_size;
    uint32_t msg_size;

    struct
    {
        uint32_t used : 1;
    } flags;
}DMAgentHead;

struct DMAgentRecord
{
    DMAgentRecord()
        : data(NULL)
    {

    }
    DMAgentRecord(uint8_t * data_)
        : data(data_)
    {

    }
    DMAgentRecord(const DMAgentRecord& r)
    {
        data = r.data;
    }
    uint8_t *   data;

    DMAgentHead& GetHead() { return *(DMAgentHead*)data; }

    std::string GetKey(){ return std::move(std::string((char*)data + sizeof(DMAgentHead), GetHead().key_size));}
    uint32_t GetKeySize() { return GetHead().key_size; }

    uint8_t* GetMsg() { return data + sizeof(DMAgentHead) + GetKeySize(); }
    uint32_t GetMsgSize() { return GetHead().msg_size; }
};

typedef std::map<std::string, DMAgentRecord> MapDMAgentRecord;
typedef MapDMAgentRecord::iterator MapDMAgentRecordIt;

class CDMShmAgent : 
    public CDMSafeSingleton<CDMShmAgent>
{
    friend class CDMSafeSingleton<CDMShmAgent>;
public:
    CDMShmAgent();

    bool Init();

    bool Write(const std::string &key, const std::string &message);
    bool Read(const std::string &key, std::string* message);
    bool Remove(const std::string &key);

    DMAgentRecord* FindRecord(const std::string &key);
    void AddRecord(const std::string &key, DMAgentRecord& data);

    bool Full();
private:
    bool __LoadCSV();
private:
    DMShmem m_oShmem;
    DMShmConfig m_oConfig;
    int m_nIndex;

    MapDMAgentRecord m_mapRecord;
};

bool DMShmAgentInit();

#define DMSHM_AGENT_INIT() DMShmAgentInit()

#define SHM_WRITE(key, msg)   CDMShmAgent::Instance()->Write(key, msg)
#define SHM_READ(key, msg)    CDMShmAgent::Instance()->Read(key, &msg)
#define SHM_REMOVE(key, msg)  CDMShmAgent::Instance()->Remove(key, &msg)

#endif // __GSHMAGENT_H_INCLUDE__