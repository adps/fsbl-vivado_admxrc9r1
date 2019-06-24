#ifndef _CMDS_USER_EXT
#define _CMDS_USER_EXT

#include "cmds.h"

CMD_FUNC( CmdI2CRead );
CMD_FUNC( CmdI2CWrite );
CMD_FUNC( CmdAbortProtectionProtocol );
CMD_FUNC( CmdInvokeProtectionProtocol );
CMD_FUNC( CmdRequestVPDUpdate );
CMD_FUNC( CmdRequestMezUpdate );
CMD_FUNC( CmdRequestSensors );
CMD_FUNC( CmdRequestBlockBox );
CMD_FUNC( CmdForceMezSupply );
CMD_FUNC( CmdRequestVPDData );
CMD_FUNC( CmdRequestMezData );
CMD_FUNC( CmdSetFrequency );
CMD_FUNC( CmdGetFrequency );
CMD_FUNC( CmdOverrideSensor );
CMD_FUNC( CmdSetFrequencyEx );
CMD_FUNC( CmdGetFrequencyEx );

#endif
