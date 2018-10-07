# dmshm

Copyright (c) 2013-2018 brinkqiang (brink.qiang@gmail.com)

[dmshm GitHub](https://github.com/brinkqiang/dmshm)

## Build status
| [Linux][lin-link] | [MacOSX][osx-link] | [Windows][win-link] |
| :---------------: | :----------------: | :-----------------: |
| ![lin-badge]      | ![osx-badge]       | ![win-badge]        |

[lin-badge]: https://travis-ci.org/brinkqiang/dmshm.svg?branch=master "Travis build status"
[lin-link]:  https://travis-ci.org/brinkqiang/dmshm "Travis build status"
[osx-badge]: https://travis-ci.org/brinkqiang/dmshm.svg?branch=master "Travis build status"
[osx-link]:  https://travis-ci.org/brinkqiang/dmshm "Travis build status"
[win-badge]: https://ci.appveyor.com/api/projects/status/github/brinkqiang/dmshm?branch=master&svg=true "AppVeyor build status"
[win-link]:  https://ci.appveyor.com/project/brinkqiang/dmshm "AppVeyor build status"

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
#include "dmformat.h"
#include "dmshmagent.h"
#include "dmtime.h"

int main( int argc, char* argv[] )
{
    DMSHM_AGENT_INIT();
    std::cout << fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time(NULL))) << std::endl;
    for (int i=0; i < 100; ++i)
    {
        LOG_WARN("this is a test {}", i);
    }
    std::cout << fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time(NULL))) << std::endl;
    return 0;
}

```
## Contacts
[![Join the chat](https://badges.gitter.im/brinkqiang/dmshm/Lobby.svg)](https://gitter.im/brinkqiang/dmshm)

## Thanks
