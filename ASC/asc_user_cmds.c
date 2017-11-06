/**
  ************************************************************
  @file         ASC_user_cmds.c
  @author       savver (savver.engineer@gmail.com)
  @date         14.10.2017
  @brief        user commands, modified by user
  
  @copyright    savver (Anton S.), MIT License
  ************************************************************
 */
/* --- Includes ----------------------------------------------------------*/

    #include "asc_user_cmds.h"
    #include "asc_protocol.h"
    
    #include "board.h"
    
    #include <string.h>

/*--- Commands Process Functions ------------------------------------------*/

    static int 
    ASC_Cmd_Func_LedOn( uint8_t *   data, uint16_t    data_len,
                        uint8_t *   answ, uint16_t *  answ_len);
    
    static int 
    ASC_Cmd_Func_LedOff( uint8_t *   data, uint16_t    data_len,
                         uint8_t *   answ, uint16_t *  answ_len);
    
    static int 
    ASC_Cmd_Func_Info( uint8_t *   data, uint16_t    data_len,
                       uint8_t *   answ, uint16_t *  answ_len);
    
    static int 
    ASC_Cmd_Func_Help( uint8_t *   data, uint16_t    data_len,
                       uint8_t *   answ, uint16_t *  answ_len);

/* --- Standart Protocol --------------------------------------------------*/
    
#ifndef ASC_PARAM_ID
    #define ASC_PARAM_ID        ':'
#endif
    #define ASC_HELP_SEP        ' '
    
    const uint8_t
    ASC_UInf_Dev[] = "dev= TEST_BRD,";
    
    const uint8_t
    ASC_UInf_Sw[] = "sw= 1.0,";
    
    const uint8_t
    ASC_UInf_Brd[] = "brd= 1,";
    
/* --- Variables ----------------------------------------------------------*/

    const uint8_t 
    ASC_UC_LedOn[]  = "led_on";
    
    const uint8_t 
    ASC_UC_LedOff[] = "led_off";
    
    const uint8_t
    ASC_US_Info[]   = "info";
    
    const uint8_t
    ASC_US_Help[]   = "help";

    const ASC_US_Cmds_t
#ifdef ASC_US_CMDs_COUNT
    ASC_US_Cmds[ ASC_US_CMDs_COUNT ] = 
#else
    ASC_US_Cmds[] = 
#endif
    {
        { ASC_UC_LedOn,     sizeof(ASC_UC_LedOn) - 1,   ASC_Cmd_Func_LedOn },
        { ASC_UC_LedOff,    sizeof(ASC_UC_LedOff) - 1,  ASC_Cmd_Func_LedOff },
        { ASC_US_Info,      sizeof(ASC_US_Info) - 1,    ASC_Cmd_Func_Info },
        { ASC_US_Help,      sizeof(ASC_US_Help) - 1,    ASC_Cmd_Func_Help },
    };
#ifndef ASC_US_CMDs_COUNT
    const uint16_t         ASC_US_CMDs_COUNT;
#endif
    
/* === Func ========================================================== */
static int 
ASC_Cmd_Func_LedOn( uint8_t *   data, uint16_t    data_len,
                    uint8_t *   answ, uint16_t *  answ_len)
{
    switch( data[0] )
    {
        case '3':   LED3_On();    break;
        case '4':   LED4_On();    break;
        case '5':   LED5_On();    break;
        case '6':   LED6_On();    break;
        
        default:
            answ[0] = ASC_PARAM_ID;
            memcpy( &answ[1], "wrong_par", 9);
            *answ_len = 9+1;
            break;
    }

    return ASC_RES_OK;
}
/**************************************************************************/
static int 
ASC_Cmd_Func_LedOff( uint8_t *   data, uint16_t    data_len,
                     uint8_t *   answ, uint16_t *  answ_len)
{
    switch( data[0] )
    {
        case '3':   LED3_Off();    break;
        case '4':   LED4_Off();    break;
        case '5':   LED5_Off();    break;
        case '6':   LED6_Off();    break;
        
        default:
            answ[0] = ASC_PARAM_ID;
            memcpy( &answ[1], "wrong_par", 9);
            *answ_len = 9+1;
            break;
    }

    return ASC_RES_OK;
}
/**************************************************************************/
static int 
ASC_Cmd_Func_Info( uint8_t *   data, uint16_t    data_len,
                   uint8_t *   answ, uint16_t *  answ_len)
{
    answ[0] = ASC_PARAM_ID;
    
    uint16_t    idx = 1;
    memcpy( &answ[idx], ASC_UInf_Dev,  sizeof(ASC_UInf_Dev) - 1 );   idx += sizeof(ASC_UInf_Dev) - 1;
    memcpy( &answ[idx], ASC_UInf_Sw,   sizeof(ASC_UInf_Sw) - 1 );    idx += sizeof(ASC_UInf_Sw) - 1;
    memcpy( &answ[idx], ASC_UInf_Brd,  sizeof(ASC_UInf_Brd) - 1 );   idx += sizeof(ASC_UInf_Brd) - 1;
    *answ_len = idx;
    
    return ASC_RES_OK;
}
/**************************************************************************/
static int 
ASC_Cmd_Func_Help( uint8_t *   data, uint16_t    data_len,
                   uint8_t *   answ, uint16_t *  answ_len)
{
    answ[0] = ASC_PARAM_ID;
    int idx = 1;
    
    for(int i = 0; i < ASC_US_CMDs_COUNT; i++)
    {
        memcpy( &answ[idx], ASC_US_Cmds[i].str, ASC_US_Cmds[i].len );
        idx += ASC_US_Cmds[i].len;
        
        answ[idx++] = ASC_HELP_SEP;
    }
    
    *answ_len = idx;
    return ASC_RES_OK;
}
/**************************************************************************/
