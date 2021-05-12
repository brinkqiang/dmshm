
#include "dmos.h"
#include "dmutil.h"
#include "dmtypes.h"
#include "dmshmagent.h"
#include "dmformat.hpp"

int main( int argc, char* argv[] )
{
    DMSHM_AGENT_INIT();
    std::cout << fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time(NULL))) << std::endl;
    for (int i=0; i < 100; ++i)
    {
        SHM_WRITE(fmt::format("{}", i), fmt::format("this is a test {}", i));
    }

    std::string msg;
    SHM_READ("7", msg);
    std::cout << fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(time(NULL))) << std::endl;
    return 0;
}
