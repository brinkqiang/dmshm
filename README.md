# dmshm

Copyright (c) 2013-2018 brinkqiang (brink.qiang@gmail.com)

[dmshm GitHub](https://github.com/brinkqiang/dmshm)

## Build status
| [Linux][lin-link] | [Mac][mac-link] | [Windows][win-link] |
| :---------------: | :----------------: | :-----------------: |
| ![lin-badge]      | ![mac-badge]       | ![win-badge]        |

[lin-badge]: https://github.com/brinkqiang/dmshm/workflows/linux/badge.svg "linux build status"
[lin-link]:  https://github.com/brinkqiang/dmshm/actions/workflows/linux.yml "linux build status"
[mac-badge]: https://github.com/brinkqiang/dmshm/workflows/mac/badge.svg "mac build status"
[mac-link]:  https://github.com/brinkqiang/dmshm/actions/workflows/mac.yml "mac build status"
[win-badge]: https://github.com/brinkqiang/dmshm/workflows/win/badge.svg "win build status"
[win-link]:  https://github.com/brinkqiang/dmshm/actions/workflows/win.yml "win build status"

## Intro
dmshm

dmshmserver  
config filename: dmshmserver.csv
```cpp

#include "dmos.h"
#include "dmutil.h"
#include "dmtypes.h"
#include "dmformat.h"
#include "dmshmserver.h"

int main( int argc, char* argv[] )
{
    CDMShmServer::Instance()->Start(CDMShmServer::Instance());
    CDMShmServer::Instance()->WaitFor();
    return 0;
}

```

dmshmagent  
config filename: dmshmagent.csv
```cpp

#include "dmos.h"
#include "dmutil.h"
#include "dmtypes.h"
#include "dmshmagent.h"
#include "dmtime.h"
int main( int argc, char* argv[] )
{
    DMSHM_AGENT_INIT();
    fmt::print("{:%Y-%m-%d %H:%M:%S}\n", fmt::localtime(time(NULL)));
    for (int i=0; i < 10; ++i)
    {
        SHM_WRITE(fmt::format("{}", i), fmt::format("this is a test {}", i));
    }
    for (int i = 0; i < 10; ++i)
    {
        std::string msg;
        SHM_READ(std::to_string(i), msg);
        fmt::print("{} = {}\n", i, msg);
    }

    fmt::print("{:%Y-%m-%d %H:%M:%S}\n", fmt::localtime(time(NULL)));
    return 0;
}

```
## Contacts
[![Join the chat](https://badges.gitter.im/brinkqiang/dmshm/Lobby.svg)](https://gitter.im/brinkqiang/dmshm)

## Thanks
