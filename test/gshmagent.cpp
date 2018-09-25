
#include "dmos.h"
#include "dmutil.h"
#include "dmtypes.h"
#include "dmformat.h"
#include "dmshm.h"

int main( int argc, char* argv[] )
{
    CDMShmem oShmem;
    void* pData = oShmem.Open("1-1-1-1");
    fmt::fprintf(stdout, "%s\n", (char*)pData);

    strcpy((char*)pData, "world hello");

    std::string strData;
    strData.assign((char*)pData, 1024);

    fmt::fprintf(stdout, "%s\n", strData.c_str());
    return 0;
}
