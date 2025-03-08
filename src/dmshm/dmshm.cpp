#include <stdio.h>
#include "dmshm.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/shm.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#endif


#ifdef _WIN32
static void ThansPath(std::string& file)
{
    for (std::string::iterator itr = file.begin(); itr != file.end(); ++itr)
    {
        if (*itr == '/')
        {
            *itr = '\\';
        }
    }
}
#else
static void ThansPath(std::string& file)
{
    for (std::string::iterator itr = file.begin(); itr != file.end(); ++itr)
    {
        if (*itr == '\\')
        {
            *itr = '/';
        }
    }
}
#endif

static bool DMAttachShm(DMShmem *shm, const char *file)
{
    std::string path = file;
    ThansPath(path);
#ifdef _WIN32

    shm->handle = OpenFileMappingA(FILE_MAP_READ|FILE_MAP_WRITE, false, path.c_str());
    if (shm->handle == NULL)
    {
        return false;
    }

    shm->mem = (void*)MapViewOfFile(shm->handle, FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);

    if (shm->mem == NULL)
    {
        CloseHandle(shm->handle);
        return false;
    }
#else
    key_t shmkey = ftok(path.c_str(), 'a');
    if(shmkey == -1)
    {
        return false;
    }

    struct stat st;
    if (stat(file, &st) == -1)
    {
        return false;
    }

    int id = shmget(shmkey, st.st_size, SHM_R | SHM_W);
    if (id == -1)
    {
        return false;
    }

    shm->mem = (void*)shmat(id, NULL, 0);
    if ((char*)shm->mem == (char*)-1)
    {
        return false;
    }
    shm->handle = id;
#endif
    shm->file = path;

    return true;;
}

static bool DMCreateShm(DMShmem *shm, const char *file, uint32_t size)
{
    std::string path = file;
    ThansPath(path);
#ifdef _WIN32
    HANDLE filehandle = CreateFileA(file, GENERIC_READ | GENERIC_WRITE,
                                    FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_HIDDEN, NULL);
    if (filehandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    SetFilePointer(filehandle, size, NULL, FILE_BEGIN);

    shm->handle = CreateFileMappingA(filehandle, NULL,
                                        PAGE_READWRITE, 0, size, path.c_str());

    if (NULL == shm->handle)
    {
        CloseHandle(filehandle);
        return false;
    }

    shm->mem = (void*)MapViewOfFile(shm->handle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

    if (NULL == shm->mem)
    {
        CloseHandle(filehandle);
        CloseHandle(shm->handle);
        return false;
    }

    CloseHandle(filehandle);

#else
    FILE *fp = fopen(path.c_str(), "w+");
    if (file == NULL)
    {
        return false;
    }
    fclose(fp);

    size += 4;
    if (truncate(file, size) == -1)
    {
        return false;
    }

    key_t shmkey = ftok(file, 'a');
    if(shmkey == -1)
    {
        return false;
    }

    int id = shmget(shmkey, size, SHM_R | SHM_W | IPC_CREAT);
    if (id == -1)
    {
        return false;
    }

    shm->mem = (void*)shmat(id, NULL, 0);
    if ((char*)shm->mem == (char*)-1)
    {
        shmctl(id, IPC_RMID, NULL);
        return false;
    }
    shm->handle = id;
#endif
    shm->file = path;

    return true;
}

DMShmem DMAPI DMOpenShmem( const char *file, bool bCreate, uint32_t size  )
{
    if (DMShmemExist(file))
    {
        DMShmem handle;
        DMAttachShm(&handle, file);
        return handle;
    }
    else if(bCreate)
    {
        return DMCreateShmem(file, size);
    }
    else
    {
        return DMShmem();
    }
}

DMShmem DMAPI DMCreateShmem(const char *pFile, uint32_t size )
{
    DMShmem handle;
    DMCreateShm(&handle, pFile, size);
    return handle;
}

bool DMAPI DMShmemExist(const char *file)
{
    std::string path = file;
    ThansPath(path);
#ifdef _WIN32
    DMHANDLE handle= OpenFileMappingA(FILE_MAP_READ | FILE_MAP_WRITE, false, path.c_str());
    bool ret = (handle!= NULL);
    CloseHandle(handle);

    return ret;
#else
    FILE *fp = fopen(path.c_str(), "w+");
    bool ret = (fp !=NULL);
    fclose(fp);
    return ret;
#endif // 

}

void DMAPI DMCloseShmem(DMShmem *shm)
{
    if (shm != NULL)
    {
#ifdef _WIN32
        UnmapViewOfFile(shm->mem);
        CloseHandle(shm->handle);
        DeleteFileA(shm->file.c_str());
#else
        shmdt(shm->mem);
        shmctl(shm->handle, IPC_RMID, NULL);
        unlink(shm->file.c_str());
#endif
    }
}

CDMShmem::CDMShmem()
{
    m_shm.mem    = NULL;
    m_shm.handle = DMINVALID_HANDLE;
}

void* DMAPI CDMShmem::Open(const char *file)
{
    m_shm = DMOpenShmem(file);
    return m_shm.mem;
}

void* DMAPI CDMShmem::Create(const char *file, uint32_t size )
{
    m_shm = DMCreateShmem(file,size);
    return m_shm.mem;
}

void DMAPI CDMShmem::Close()
{
    DMCloseShmem(&m_shm);
}

DMHANDLE CDMShmem::GetHandle()
{
    return m_shm.handle;
}
