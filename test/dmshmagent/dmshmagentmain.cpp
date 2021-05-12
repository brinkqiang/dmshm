
#include "dmos.h"
#include "dmutil.h"
#include "dmtypes.h"
#include "dmshmagent.h"
#include "dmtime.h"
int main( int argc, char* argv[] )
{
    DMSHM_AGENT_INIT();
    fmt::print("{:%Y-%m-%d %H:%M:%S}\n", fmt::localtime(time(NULL)));
    for (int i=0; i < 100; ++i)
    {
        SHM_WRITE(fmt::format("{}", i), fmt::format("this is a test {}", i));
    }

    std::string msg;
    SHM_READ("7", msg);
    fmt::print("{:%Y-%m-%d %H:%M:%S}\n", fmt::localtime(time(NULL)));
    return 0;
}
