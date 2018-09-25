#ifndef __DMSHM_H_INCLUDE__
#define __DMSHM_H_INCLUDE__

#include <dmos.h>
#include <dmtypes.h>
/**
* @brief �����ڴ�Ľṹ��
*
*/
struct SShmem
{
    DMHANDLE    handle;		/**<�ļ���Ӧ��handle*/
    void *      mem;		/**<�����ڴ����ʼ��ַ*/
    std::string file;		/**<�����ڴ��Ӧ���ļ�·��*/
};

/**
* @brief
* ��һ�������ڴ�
* @param pszFileName : �����ڴ�ӳ���ļ�
* @param bCreate : ���û�иù����ڴ�ʱ���Ƿ񴴽������ڴ�
* @param size : �����ڴ��С
* @return �����ڴ�Ľṹ��
*/
SShmem DMAPI DMOpenShmem( const char * pszFileName, bool bCreate = false, uint32_t size = 0 );

/**
* @brief
* ����һ�������ڴ�
* @param pszFileName : �����ڴ�ӳ���ļ�
* @param size : �����ڴ��С
* @return �����ڴ�Ľṹ��
*/
SShmem DMAPI DMCreateShmem( const char * pszFileName, uint32_t size );

/**
* @brief
* ����һ�������ڴ�
* @param file : �����ڴ�ӳ���ļ�
* @param size : �����ڴ��С
* @return �����ڴ�Ľṹ��
*/
bool DMAPI DMShmemExist( const char * pszFileName );

/**
* @brief
* �ر�һ�������ڴ�
* ��windows����ϵͳ�У�����򿪴������ڹرմ������ڴ潫������
* ��linux����ϵͳ�У� ��ֱ�ӱ�����
* @param shm : �����ڴ�Ľṹ��
* @return void
*/
void DMAPI DMCloseShmem( SShmem * shm );

/**
* @brief
* ����һ�������ڴ���
*/
class CDMShmem
{
public:
    CDMShmem();

    /**
    * @brief
    * ��һ�������ڴ�
    * @param pszFileName : �ļ�·��
    * @return ָ�����ڴ��ָ��
    */
    void * DMAPI Open( const char * pszFileName );

    /**
    * @brief
    * ����һ�������ڴ�
    * @param pszFileName : �ļ�·��
    * @param size : �ļ���С
    * @return ָ�����ڴ��ָ��
    */
    void * DMAPI Create( const char * pszFileName, uint32_t size );

    /**
    * @brief
    * �رչ����ڴ�
    * @return void
    */
    void DMAPI Close();

    /**
    * @brief
    * ��ȡ�����ڴ���
    * @return �����ڴ���
    */
    DMHANDLE GetHandle();

private:
    SShmem m_shm;
};

#endif // __DMSHM_H_INCLUDE__
