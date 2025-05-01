
// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
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

#ifndef __DMIPCSHM_H_INCLUDE__
#define __DMIPCSHM_H_INCLUDE__

#include "dmos.h"
#include <string>
#include <vector>
#include <cstdint>

// 共享内存消息结构
struct DMShmMessage {
    uint32_t msg_id;        // 消息ID
    uint32_t sender_pid;    // 发送者进程ID
    uint32_t receiver_pid;  // 接收者进程ID(0表示广播)
    uint32_t data_len;      // 数据长度
    uint8_t  data[1];       // 可变长度数据(实际使用时需要动态分配)
};

// 共享内存区域信息
struct DMShmRegion {
    std::string name;       // 共享内存区域名称
    size_t size;            // 共享内存大小
    void* address;          // 映射地址
    bool is_creator;        // 是否是创建者
};

// 共享内存连接信息
struct DMShmConnection {
    std::string name;       // 连接名称
    uint32_t local_pid;     // 本地进程ID
    uint32_t remote_pid;    // 远程进程ID
    bool is_connected;      // 是否已连接
};

// 回调函数类型定义
typedef void (*DMShmMessageCallback)(const DMShmMessage* msg, void* user_data);

// 接口类
class Idmipcshm {
public:
    virtual ~Idmipcshm() {}
    
    // 释放接口
    virtual void DMAPI Release(void) = 0;
    
    // 测试接口
    virtual void DMAPI Test(void) = 0;
    
    // 创建共享内存区域
    // @param name 共享内存名称
    // @param size 共享内存大小
    // @return 是否创建成功
    virtual bool DMAPI CreateShmRegion(const std::string& name, size_t size) = 0;
    
    // 打开已存在的共享内存区域
    // @param name 共享内存名称
    // @return 是否打开成功
    virtual bool DMAPI OpenShmRegion(const std::string& name) = 0;
    
    // 关闭共享内存区域
    // @param name 共享内存名称
    virtual void DMAPI CloseShmRegion(const std::string& name) = 0;
    
    // 获取共享内存区域信息
    // @param name 共享内存名称
    // @return 共享内存区域信息指针，失败返回nullptr
    virtual const DMShmRegion* DMAPI GetShmRegion(const std::string& name) = 0;
    
    // 发送消息到共享内存
    // @param region_name 共享内存区域名称
    // @param msg 要发送的消息
    // @return 是否发送成功
    virtual bool DMAPI SendMessage(const std::string& region_name, const DMShmMessage* msg) = 0;
    
    // 接收消息
    // @param region_name 共享内存区域名称
    // @param timeout_ms 超时时间(毫秒)，-1表示无限等待
    // @param msg 输出参数，接收到的消息
    // @return 是否接收到消息
    virtual bool DMAPI ReceiveMessage(const std::string& region_name, int timeout_ms, DMShmMessage** msg) = 0;
    
    // 注册消息回调函数
    // @param region_name 共享内存区域名称
    // @param callback 消息回调函数
    // @param user_data 用户数据指针
    virtual void DMAPI RegisterMessageCallback(const std::string& region_name, 
                                             DMShmMessageCallback callback, 
                                             void* user_data) = 0;
    
    // 取消注册消息回调函数
    // @param region_name 共享内存区域名称
    virtual void DMAPI UnregisterMessageCallback(const std::string& region_name) = 0;
    
    // 列出所有共享内存区域
    // @return 共享内存区域名称列表
    virtual std::vector<std::string> DMAPI ListShmRegions() = 0;
    
    // 设置进程间同步对象(信号量/互斥锁等)超时时间
    // @param timeout_ms 超时时间(毫秒)
    virtual void DMAPI SetSyncTimeout(uint32_t timeout_ms) = 0;
    
    // 获取当前进程ID
    // @return 当前进程ID
    virtual uint32_t DMAPI GetCurrentPid() = 0;
    
    // 设置调试日志级别
    // @param level 日志级别(0-关闭,1-错误,2-警告,3-信息,4-调试)
    virtual void DMAPI SetDebugLevel(int level) = 0;
};

extern "C" DMEXPORT_DLL Idmipcshm* DMAPI dmipcshmGetModule();

typedef Idmipcshm* (DMAPI* PFN_dmipcshmGetModule)();

#endif // __DMIPCSHM_H_INCLUDE__
