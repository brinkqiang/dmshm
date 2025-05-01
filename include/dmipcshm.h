
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

// �����ڴ���Ϣ�ṹ
struct DMShmMessage {
    uint32_t msg_id;        // ��ϢID
    uint32_t sender_pid;    // �����߽���ID
    uint32_t receiver_pid;  // �����߽���ID(0��ʾ�㲥)
    uint32_t data_len;      // ���ݳ���
    uint8_t  data[1];       // �ɱ䳤������(ʵ��ʹ��ʱ��Ҫ��̬����)
};

// �����ڴ�������Ϣ
struct DMShmRegion {
    std::string name;       // �����ڴ���������
    size_t size;            // �����ڴ��С
    void* address;          // ӳ���ַ
    bool is_creator;        // �Ƿ��Ǵ�����
};

// �����ڴ�������Ϣ
struct DMShmConnection {
    std::string name;       // ��������
    uint32_t local_pid;     // ���ؽ���ID
    uint32_t remote_pid;    // Զ�̽���ID
    bool is_connected;      // �Ƿ�������
};

// �ص��������Ͷ���
typedef void (*DMShmMessageCallback)(const DMShmMessage* msg, void* user_data);

// �ӿ���
class Idmipcshm {
public:
    virtual ~Idmipcshm() {}
    
    // �ͷŽӿ�
    virtual void DMAPI Release(void) = 0;
    
    // ���Խӿ�
    virtual void DMAPI Test(void) = 0;
    
    // ���������ڴ�����
    // @param name �����ڴ�����
    // @param size �����ڴ��С
    // @return �Ƿ񴴽��ɹ�
    virtual bool DMAPI CreateShmRegion(const std::string& name, size_t size) = 0;
    
    // ���Ѵ��ڵĹ����ڴ�����
    // @param name �����ڴ�����
    // @return �Ƿ�򿪳ɹ�
    virtual bool DMAPI OpenShmRegion(const std::string& name) = 0;
    
    // �رչ����ڴ�����
    // @param name �����ڴ�����
    virtual void DMAPI CloseShmRegion(const std::string& name) = 0;
    
    // ��ȡ�����ڴ�������Ϣ
    // @param name �����ڴ�����
    // @return �����ڴ�������Ϣָ�룬ʧ�ܷ���nullptr
    virtual const DMShmRegion* DMAPI GetShmRegion(const std::string& name) = 0;
    
    // ������Ϣ�������ڴ�
    // @param region_name �����ڴ���������
    // @param msg Ҫ���͵���Ϣ
    // @return �Ƿ��ͳɹ�
    virtual bool DMAPI SendMessage(const std::string& region_name, const DMShmMessage* msg) = 0;
    
    // ������Ϣ
    // @param region_name �����ڴ���������
    // @param timeout_ms ��ʱʱ��(����)��-1��ʾ���޵ȴ�
    // @param msg ������������յ�����Ϣ
    // @return �Ƿ���յ���Ϣ
    virtual bool DMAPI ReceiveMessage(const std::string& region_name, int timeout_ms, DMShmMessage** msg) = 0;
    
    // ע����Ϣ�ص�����
    // @param region_name �����ڴ���������
    // @param callback ��Ϣ�ص�����
    // @param user_data �û�����ָ��
    virtual void DMAPI RegisterMessageCallback(const std::string& region_name, 
                                             DMShmMessageCallback callback, 
                                             void* user_data) = 0;
    
    // ȡ��ע����Ϣ�ص�����
    // @param region_name �����ڴ���������
    virtual void DMAPI UnregisterMessageCallback(const std::string& region_name) = 0;
    
    // �г����й����ڴ�����
    // @return �����ڴ����������б�
    virtual std::vector<std::string> DMAPI ListShmRegions() = 0;
    
    // ���ý��̼�ͬ������(�ź���/��������)��ʱʱ��
    // @param timeout_ms ��ʱʱ��(����)
    virtual void DMAPI SetSyncTimeout(uint32_t timeout_ms) = 0;
    
    // ��ȡ��ǰ����ID
    // @return ��ǰ����ID
    virtual uint32_t DMAPI GetCurrentPid() = 0;
    
    // ���õ�����־����
    // @param level ��־����(0-�ر�,1-����,2-����,3-��Ϣ,4-����)
    virtual void DMAPI SetDebugLevel(int level) = 0;
};

extern "C" DMEXPORT_DLL Idmipcshm* DMAPI dmipcshmGetModule();

typedef Idmipcshm* (DMAPI* PFN_dmipcshmGetModule)();

#endif // __DMIPCSHM_H_INCLUDE__
