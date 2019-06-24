#include "resps.h"


#include "../avrsysmon2_configs.h"

RESP_FUNC( RespServiceMode )
{
    if( *nProgress == 0 )
    {
        //std::cout << "\r\nService Mode Request response: " << std::dec << (unsigned int)nByte << "\r\n";
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}

RESP_FUNC( RespApplicationMode )
{
    if( *nProgress == 0 )
    {
        //std::cout << "\r\nApplication Mode Request response: " << std::dec << (unsigned int)nByte << "\r\n";
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}

RESP_FUNC( RespVersion )
{
    static unsigned short pVersion[4];
    switch( *nProgress )
    {
    case 0:
        pVersion[0] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 1:
        pVersion[0] = ( (unsigned short)nByte << 8 ) | pVersion[0];
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 2:
        pVersion[1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 3:
        pVersion[1] = ( (unsigned short)nByte << 8 ) | pVersion[1];
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 4:
        pVersion[2] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 5:
        pVersion[2] = ( (unsigned short)nByte << 8 ) | pVersion[2];
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 6:
        pVersion[3] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 7:
        pVersion[3] = ( (unsigned short)nByte << 8 ) | pVersion[3];
        (*nProgress)++;
        //std::cout << "\r\nVersion Number: " << std::dec << (unsigned int)pVersion[0] << "." << (unsigned int)pVersion[1] << "." << (unsigned int)pVersion[2] << "." << (unsigned int)pVersion[3] << "\r\n";
        *bFinished = true;
        return bEoT;
    }
    return false;
}

#ifdef FULL_RESPS_USER
RESP_FUNC( RespLoopback32x8 )
{
    static unsigned char pData[32];
    if( *nProgress < 31 )
    {
        pData[*nProgress] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else
    {
        pData[*nProgress] = nByte;
        (*nProgress)++;
        *bFinished = true;
        unsigned int nErrors = 0;
        for( unsigned int n = 0; n < 32; n++ )
        {
            if( (unsigned int)pData[n] != ( n + 1 ) )
            {
                //std::cout << "\r\n " << (unsigned int)pData[n] << " vs. " << ( n + 1 ) << "\r\n";
                nErrors++;
            }
        }
        //std::cout << "\r\nLoopback Data RXed: " << nErrors << " Errors\r\n";
        return bEoT;
    }
}
#endif
RESP_FUNC( RespGetProductID )
{
    static unsigned char pPID[4];
    switch( *nProgress )
    {
    case 0:
        pPID[0] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 1:
        pPID[1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 2:
        pPID[2] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 3:
        pPID[3] = nByte;
        (*nProgress)++;
        //std::cout << "\r\nProduct ID: " << std::dec << *((unsigned int*)&pPID[0]) << "\r\n";
        *bFinished = true;
        return bEoT;
    }
    return false;
}
