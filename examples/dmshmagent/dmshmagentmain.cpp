
#include "dmos.h"
#include "dmutil.h"
#include "dmtypes.h"
#include "dmshmagent.h"

int main( int argc, char* argv[] )
{
    DMSHM_AGENT_INIT();
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

    return 0;
}
