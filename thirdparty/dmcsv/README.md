# dmcsv

Copyright (c) 2013-2018 brinkqiang (brink.qiang@gmail.com)

[dmcsv GitHub](https://github.com/brinkqiang/dmcsv)

## Build status
| [Linux][lin-link] | [MacOSX][osx-link] | [Windows][win-link] |
| :---------------: | :----------------: | :-----------------: |
| ![lin-badge]      | ![osx-badge]       | ![win-badge]        |

[lin-badge]: https://travis-ci.org/brinkqiang/dmcsv.svg?branch=master "Travis build status"
[lin-link]:  https://travis-ci.org/brinkqiang/dmcsv "Travis build status"
[osx-badge]: https://travis-ci.org/brinkqiang/dmcsv.svg?branch=master "Travis build status"
[osx-link]:  https://travis-ci.org/brinkqiang/dmcsv "Travis build status"
[win-badge]: https://ci.appveyor.com/api/projects/status/github/brinkqiang/dmcsv?branch=master&svg=true "AppVeyor build status"
[win-link]:  https://ci.appveyor.com/project/brinkqiang/dmcsv "AppVeyor build status"

## Intro
dmcsv
```cpp
#include "dmcsv_parser.hpp"
#include "dmcsv_writer.hpp"
#include <vector>
#include <queue>

int main()
{
    std::string strFile = "slist.csv";
    std::ofstream out(strFile);

    std::queue<std::vector<std::string>> q;
    q.push({ "name", "age", "ip" });
    q.push({ "Tom", std::to_string(std::numeric_limits<uint64_t>::max()).c_str(), "172.30.10.29" });
    q.push({ "Plus", "22", "172.30.10.18" });
    q.push({ "Andy", "27", "172.30.10.21" });

    auto writer = csv::make_csv_writer(out);
    for (; !q.empty(); q.pop())
        writer.write_row(q.front());

    out.close();

    try
    {
        csv::CSVReader reader(strFile);
        csv::CSVRow rows;

        for (size_t i = 0; reader.read_row(rows); i++) {
            std::cout << rows["name"].get<std::string>() << "\t"
                << rows["age"].get<uint64_t>() << "\t"
                << rows["ip"].get<std::string>() << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;
}
```
## Contacts
[![Join the chat](https://badges.gitter.im/brinkqiang/dmcsv/Lobby.svg)](https://gitter.im/brinkqiang/dmcsv)

## Thanks
