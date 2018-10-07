
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
