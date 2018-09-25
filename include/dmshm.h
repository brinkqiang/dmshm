#ifndef __DMSHM_H_INCLUDE__
#define __DMSHM_H_INCLUDE__

#include <dmos.h>
#include <dmtypes.h>
/**
* @brief 共享内存的结构体
*
*/
struct SShmem
{
    DMHANDLE    handle;		/**<文件对应的handle*/
    void *      mem;		/**<共享内存的起始地址*/
    std::string file;		/**<共享内存对应的文件路径*/
};

/**
* @brief
* 打开一个共享内存
* @param pszFileName : 共享内存映射文件
* @param bCreate : 如果没有该共享内存时，是否创建共享内存
* @param size : 共享内存大小
* @return 共享内存的结构体
*/
SShmem DMAPI DMOpenShmem( const char * pszFileName, bool bCreate = false, uint32_t size = 0 );

/**
* @brief
* 创建一个共享内存
* @param pszFileName : 共享内存映射文件
* @param size : 共享内存大小
* @return 共享内存的结构体
*/
SShmem DMAPI DMCreateShmem( const char * pszFileName, uint32_t size );

/**
* @brief
* 创建一个共享内存
* @param file : 共享内存映射文件
* @param size : 共享内存大小
* @return 共享内存的结构体
*/
bool DMAPI DMShmemExist( const char * pszFileName );

/**
* @brief
* 关闭一个共享内存
* 在windows操作系统中，如果打开次数等于关闭次数，内存将被销毁
* 在linux操作系统中， 则直接被销毁
* @param shm : 共享内存的结构体
* @return void
*/
void DMAPI DMCloseShmem( SShmem * shm );

/**
* @brief
* 这是一个共享内存类
*/
class CDMShmem
{
public:
    CDMShmem();

    /**
    * @brief
    * 打开一个共享内存
    * @param pszFileName : 文件路径
    * @return 指向共享内存的指针
    */
    void * DMAPI Open( const char * pszFileName );

    /**
    * @brief
    * 创建一个共享内存
    * @param pszFileName : 文件路径
    * @param size : 文件大小
    * @return 指向共享内存的指针
    */
    void * DMAPI Create( const char * pszFileName, uint32_t size );

    /**
    * @brief
    * 关闭共享内存
    * @return void
    */
    void DMAPI Close();

    /**
    * @brief
    * 获取共享内存句柄
    * @return 共享内存句柄
    */
    DMHANDLE GetHandle();

private:
    SShmem m_shm;
};

#endif // __DMSHM_H_INCLUDE__
