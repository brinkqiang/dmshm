#include "dmsingleipc.h"
#include <iostream>
#include <thread>

struct MyData {
    int id;
    char message[60];
};

int main() {
    dmsingleipcPtr ipc(dmSingleIPCGetModule());
    if (!ipc) {
        std::cerr << "Failed to get IPC module." << std::endl;
        return 1;
    }

    const std::string shm_name = "my_test_ipc_queue";
    const size_t item_count = 10;

        // 创建IPC通道
    if (!ipc->Create(shm_name, sizeof(MyData), item_count)) {
        std::cerr << "Failed to create IPC channel." << std::endl;
        return 1;
    }
    std::cout << "Producer: IPC channel '" << shm_name << "' created." << std::endl;
    for (int i = 0; i < 20; ++i) {
        MyData data;
        // 循环尝试弹出，直到成功（因为队列可能空了）
        while(!ipc->Pop(&data, sizeof(MyData))) {
            // 在实际应用中，这里可能是等待或做其他事
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "Consumer: Popped message id=" << data.id 
                  << ", msg='" << data.message << "'" << std::endl;
    }

    std::cout << "Consumer: Finished." << std::endl;
    return 0;
}