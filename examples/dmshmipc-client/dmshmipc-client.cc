#include "dmsingleipc.h"
#include <iostream>
#include <thread> // For std::this_thread::sleep_for

struct MyData {
    int id;
    char message[60];
};

int main() {
    // 使用智能指针获取模块实例
    dmsingleipcPtr ipc(dmSingleIPCGetModule());
    if (!ipc) {
        std::cerr << "Failed to get IPC module." << std::endl;
        return 1;
    }

    const std::string shm_name = "my_test_ipc_queue";

    while (!ipc->Open(shm_name)) {
        std::cerr << "Consumer: Failed to open IPC channel. Retrying..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Consumer: IPC channel '" << shm_name << "' opened." << std::endl;

    for (int i = 0; i < 20; ++i) {
        MyData data;
        data.id = i;
        snprintf(data.message, sizeof(data.message), "Hello from producer, message %d", i);

        // 循环尝试推送，直到成功（因为队列可能满了）
        while (!ipc->Push(&data, sizeof(MyData))) {
            std::cout << "Producer: Queue is full, waiting..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "Producer: Pushed message " << data.id << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    // Close 会自动被 dmsingleipcPtr 的析构函数调用，从而释放共享内存
    std::cout << "Producer: Finished." << std::endl;
    return 0;
}