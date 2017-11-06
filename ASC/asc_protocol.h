/**
  ************************************************************
  @file         ASC_protocol.h
  @author       savver (savver.engineer@gmail.com)
  @date         14.10.2017
  @brief        main processing
  
  @copyright    savver (Anton S.), MIT License
  ************************************************************
 */
/* --- Define to prevent recursive inclusion ----------------------------------*/
#ifndef _ASC_PROTOCOL_H
#define _ASC_PROTOCOL_H

/*--- Includes ----------------------------------------------*/

    #include "stdint.h"

/* --- User Settings ------------------------------------------------------ */

    #define ASC_RX_BUF_BSIZE        64
    #define ASC_TX_BUF_BSIZE        64

/* --- Defines ------------------------------------------------------ */

    #define ASC_RES_OK              0
    #define ASC_RES_ERR             (-1)
    #define ASC_RES_CMD_NOT_FOUND   0xFF

/* --- Prototypes ------------------------------------------------------ */

    int
    ASC_Rx_Handler( uint8_t *  rx_data,     uint16_t    rx_data_bsize,
                    uint8_t **  pp_tx_data, uint16_t *  tx_data_bsize );

/*--- Define to prevent recursive inclusion ----------------------------------*/
#endif  /* _ASC_PROTOCOL_H */

/***********************END OF FILE*******************************************/

