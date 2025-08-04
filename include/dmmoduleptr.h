
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

#ifndef __DMMODULEPTR_H_INCLUDE__
#define __DMMODULEPTR_H_INCLUDE__

#include <memory>

/**
 * @brief 一个通用的自定义删除器，用于调用对象的 Release() 方法。
 * @tparam T 任何包含 public Release() 方法的接口类型。
 */
template <typename T>
struct DmReleaseDeleter {
    void operator()(T* ptr) const {
        if (ptr) {
            ptr->Release();
        }
    }
};

/**
 * @brief 一个类型别名模板，用于创建管理模块接口的独占所有权智能指针。
 * 它会自动调用对象的 Release() 方法来释放资源。
 * @tparam T 模块接口类型，例如 Idmpath。
 */
template <typename T>
using DmModulePtr = std::unique_ptr<T, DmReleaseDeleter<T>>;

#endif // __DMMODULEPTR_H_INCLUDE__