
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
