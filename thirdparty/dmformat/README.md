# dmformat

Copyright (c) 2013-2018 brinkqiang (brink.qiang@gmail.com)

[![dmformat](https://img.shields.io/badge/brinkqiang-dmformat-blue.svg?style=flat-square)](https://github.com/brinkqiang/dmformat)
[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/brinkqiang/dmformat/blob/master/LICENSE)
[![blog](https://img.shields.io/badge/Author-Blog-7AD6FD.svg)](https://brinkqiang.github.io/)
[![Open Source Love](https://badges.frapsoft.com/os/v3/open-source.png)](https://github.com/brinkqiang)
[![GitHub stars](https://img.shields.io/github/stars/brinkqiang/dmformat.svg?label=Stars)](https://github.com/brinkqiang/dmformat) 
[![GitHub forks](https://img.shields.io/github/forks/brinkqiang/dmformat.svg?label=Fork)](https://github.com/brinkqiang/dmformat)

## Build status
| [Linux][lin-link] | [Mac][mac-link] | [Windows][win-link] |
| :---------------: | :----------------: | :-----------------: |
| ![lin-badge]      | ![mac-badge]       | ![win-badge]        |

[lin-badge]: https://github.com/brinkqiang/dmformat/workflows/linux/badge.svg "linux build status"
[lin-link]:  https://github.com/brinkqiang/dmformat/actions/workflows/linux.yml "linux build status"
[mac-badge]: https://github.com/brinkqiang/dmformat/workflows/mac/badge.svg "mac build status"
[mac-link]:  https://github.com/brinkqiang/dmformat/actions/workflows/mac.yml "mac build status"
[win-badge]: https://github.com/brinkqiang/dmformat/workflows/win/badge.svg "win build status"
[win-link]:  https://github.com/brinkqiang/dmformat/actions/workflows/win.yml "win build status"

## Intro
dmformat 是一个基于fmt 轻量级的 C++ 格式化库，提供了高效、易用的字符串格式化功能。主要特点包括：

- 支持多种数据类型格式化
- 高性能，低内存占用
- 跨平台支持（Linux, Windows, macOS）
- 模块化设计，易于扩展

## 使用说明
### 构建
```bash
# 使用 CMake 构建
mkdir build
cd build
cmake ..
make
```

### 使用
```cpp
#include "dmformat.h"

int main() {
    std::string str = fmt::format("Hello, {}!", "world");
    std::cout << str << std::endl;
    return 0;
}
```

## 模块说明
- dmchrono: 时间相关格式化
- dmcolor: 颜色格式化
- dmcompile: 编译时格式化
- dmcore: 核心格式化功能
- dmlocale: 本地化支持
- dmprintf: printf 风格格式化
- dmranges: 范围格式化
- dmscan: 输入解析

## 贡献指南
欢迎提交 issue 和 PR。请确保：
1. 代码风格与现有代码一致
2. 包含必要的测试用例
3. 更新相关文档

## Contacts

## Thanks
[fmtlib](https://github.com/fmtlib/fmt)
