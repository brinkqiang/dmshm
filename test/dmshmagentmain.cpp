
#include "dmos.h"
#include "dmutil.h"
#include "dmtypes.h"
#include "dmformat.h"
#include "dmshmagent.h"

int main( int argc, char* argv[] )
{
    DMSHM_AGENT_INIT();

    for (int i=0; i < 100; ++i)
    {
        LOG_WARN("this is a test {}", i);
    }

    return 0;
}
