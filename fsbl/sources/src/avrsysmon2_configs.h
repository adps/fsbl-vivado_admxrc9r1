#ifndef AVRSYSMON_CONFIGS
#define AVRSYSMON_CONFIGS

//Prints the ASCII Alpha Data logo at boot
#define PRINT_ALPHADATA_BANNER

//Disables optimization on certain sysmon files.
//#define DISABLE_OPTIMIZATION


//Enables UART interfaces debug messages
//#define _UART_DEBUG


//Enables debug messages on AVR responses.
//#define DEBUG_RESPS


//Enables support for extra AVR commands.

#define USE_CMDS_FLASH
#define FULL_CMDS_USER_EXT
#define FULL_CMDS_USER
#define FULL_RESPS_AUTO_EXT
#define FULL_RESPS_AUTO
#define FULL_RESPS_FLASH
#define FULL_RESPS_USER_EXT
#define FULL_RESPS_USER


#endif
