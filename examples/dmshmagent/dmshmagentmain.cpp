
#include "dmos.h"
#include "dmutil.h"
#include "dmtypes.h"
#include "dmshmagent.h"

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
