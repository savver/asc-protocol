/**
  ************************************************************
  @file         ASC_user_cmds.h
  @author       savver (savver.engineer@gmail.com)
  @date         14.10.2017
  @brief        user commands, modified by user
  
  @copyright    savver (Anton S.), MIT License
  ************************************************************
 */
/* --- Define to prevent recursive inclusion ------------------------------ */
#ifndef _ASC_USER_CMDS_H
#define _ASC_USER_CMDS_H

/*--- Includes -------------------------------------------------------------*/

    #include "stdint.h"

/* --- User Settings ------------------------------------------------------ */

    #define ASC_US_CMDs_COUNT       4

/* --- User Types --------------------------------------------------------- */

typedef     int  (*ASC_Cmd_Func)( uint8_t *   data, uint16_t    data_len,
                                  uint8_t *   answ, uint16_t *  answ_len);

typedef struct
{
    const uint8_t *      str;
    const uint16_t       len;
    ASC_Cmd_Func         func;
}
ASC_US_Cmds_t;

/* --- Global Variables --------------------------------------------------- */

    extern const ASC_US_Cmds_t    ASC_US_Cmds[];

#ifndef ESC_US_CMDs_COUNT
    extern uint16_t               ASC_US_CMDs_COUNT;
#endif

/*--- Define to prevent recursive inclusion ------------------------------- */
#endif  /* _ASC_USER_CMDS_H */

/***********************END OF FILE*******************************************/
