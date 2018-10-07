
// Copyright (c) 2018 brinkqiang
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __DMSHM_H_INCLUDE__
#define __DMSHM_H_INCLUDE__

#include <dmos.h>
#include <dmtypes.h>
/**
* @brief 共享内存的结构体
*
*/
struct DMShmem
{
    DMHANDLE    handle;		/**<文件对应的handle*/
    void *      mem;		/**<共享内存的起始地址*/
    std::string file;		/**<共享内存对应的文件路径*/
};

/**
* @brief 配置文件行结构
*
*/
struct DMShmConfig
{
    std::string name;       /**<共享内存的名字*/
    int bufsize;            /**<共享内存单行字节数*/
    int bufcount;           /**<共享内存最大行数*/
};


/**
* @brief
* 打开一个共享内存
* @param pszFileName : 共享内存映射文件
* @param bCreate : 如果没有该共享内存时，是否创建共享内存
* @param size : 共享内存大小
* @return 共享内存的结构体
*/
DMShmem DMAPI DMOpenShmem( const char * pszFileName, bool bCreate = false, uint32_t size = 0 );

/**
* @brief
* 创建一个共享内存
* @param pszFileName : 共享内存映射文件
* @param size : 共享内存大小
* @return 共享内存的结构体
*/
DMShmem DMAPI DMCreateShmem( const char * pszFileName, uint32_t size );

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
void DMAPI DMCloseShmem( DMShmem * shm );

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
    DMShmem m_shm;
};

#endif // __DMSHM_H_INCLUDE__
