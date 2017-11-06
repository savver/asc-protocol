/**
  ************************************************************
  @file         ASC_protocol.c
  @author       savver (savver.engineer@gmail.com)
  @date         14.10.2017
  @brief        main processing
  
  @copyright    savver (Anton S.), MIT License
  ************************************************************
 */
/* --- Includes ----------------------------------------------------------*/

    #include "asc_protocol.h"
    #include "asc_user_cmds.h"
    #include <string.h>

/* --- Defines ----------------------------------------------------------*/

// protocol reserved symbols
// ex.  $12@led_on:1#
//
    #define ASC_START_ID        '$'
    #define ASC_END_ID          '#'
    #define ASC_PARAM_ID        ':'
    #define ASC_ADDR_ID         '@'
    
    #define ASC_NEWPACK_FLAG     1
    
    #define ASC_MIN_PACK_BSIZE   3  // '$' + cmd_byte + '#'

/* --- Variables ----------------------------------------------------------*/

// asnwer templates
//
    const uint8_t
    ASC_Answ1_Rx[]  = ">RX_ER"; // 1st + one of 2d part for less coping data
                                // both 2d parts length are equal!
 
    #define ASC_ANSW1_RX_INSERT     4  // after ">RX:"
    #define ASC_ANSW1_RX_BSIZE      (sizeof(ASC_Answ1_Rx) - 1)
    
    const uint8_t
    ASC_Answ2_Ok[]  = "OK";
    
    #define ASC_ANSW2_OK_BSIZE      (sizeof(ASC_Answ2_Ok) - 1)
    
    const uint8_t
    ASC_Answ2_Err[]  = "ER";
    
    #define ASC_ANSW2_ERR_BSIZE     (sizeof(ASC_Answ2_Err) - 1)

// local buffers for processing
//
    uint8_t     ASC_RxBuf[ ASC_RX_BUF_BSIZE ];
    uint8_t     ASC_TxBuf[ ASC_TX_BUF_BSIZE ];
    
    uint16_t    ASC_Rx_Cnt = 0;
    uint8_t     ASC_NewPack_flag = 0;
    
    uint8_t *   ASC_Rx_Param_p = NULL;
    uint16_t    ASC_Rx_Param_size = 0;
    
    uint8_t *   ASC_Rx_Cmd_p = NULL;
    uint16_t    ASC_Rx_Cmd_size = 0;
    
    uint16_t    ASC_Tx_Answ_size = 0;
    
// statistics
//
    uint32_t    ASC_Rx_Handler_cntr = 0;

/*--- Private Prototypes ----------------------------------- */

    static int 
    ASC_Decode(void);

    static int 
    ASC_Execute(void);

/*=== Func ================================================= */
// < 0 - not search in this buffer, searching complete with error
// > 0 - end idx of searching, for later searching in same buffer
//
static
int
ASC_SearchPack( uint8_t *   rx_buf,
                uint16_t    rx_bsize )
{
    int idx = 0;
    
    if( rx_bsize == 0)
        return ASC_RES_ERR;
    
    // 1. search start of packet
    while( (rx_buf[idx++] != ASC_START_ID) &&
           (idx < rx_bsize) );
    if( idx >= rx_bsize )
        return ASC_RES_ERR;
    
    // 2.   search end of packet
    // 2.1. check length of packet
    while( (rx_buf[idx++] != ASC_END_ID) &&
           (idx < rx_bsize) );
    if( (idx > rx_bsize)          || /* '>' instead '>=' because of auto_inc */        (idx >= ASC_RX_BUF_BSIZE) || 
        (idx < ASC_MIN_PACK_BSIZE) )
        return ASC_RES_ERR;
    
    // 3. copy packet to local buffer
    memcpy( ASC_RxBuf, &rx_buf[1], idx - 1 ); // only payload, wo '$','#'
    ASC_Rx_Cnt = idx - 2;
    ASC_NewPack_flag = ASC_NEWPACK_FLAG;
    
    // 4. return end of packet in this rx_buf, for later searching
    return idx;
}
/****************************************************************/
// now not search start of packet, only end
//
static
int
ASC_CheckPack( uint8_t *   rx_buf,
               uint16_t    rx_bsize )
{
    int idx = 0;
    
    if( rx_bsize == 0)
        return ASC_RES_ERR;

    // 1. search start of packet
    if( rx_buf[idx++] != ASC_START_ID )
        return ASC_RES_ERR;
    
    // 2.   search end of packet
    // 2.1. check length of packet
    while( (rx_buf[idx++] != ASC_END_ID) &&
           (idx < rx_bsize) );
    if( (idx > rx_bsize)          || /* '>' instead '>=' because of auto_inc */
        (idx >= ASC_RX_BUF_BSIZE) || 
        (idx < ASC_MIN_PACK_BSIZE) )
        return ASC_RES_ERR;
    
    // 3. copy packet to local buffer
    memcpy( ASC_RxBuf, &rx_buf[1], idx - 1 ); // only payload, wo '$','#'
    ASC_Rx_Cnt = idx - 2;
    ASC_NewPack_flag = ASC_NEWPACK_FLAG;
    
    return ASC_RES_OK;
}
/****************************************************************/
// < 0 - decoding complete with errors, break next processing
//
static
int 
ASC_Decode(void)
{
    int idx = 0;
    
    // 1. search parametrs separator
    while( (ASC_RxBuf[idx++] != ASC_PARAM_ID) &&
           (idx < ASC_Rx_Cnt) );
    
    if( idx >= ASC_Rx_Cnt )
    {
        ASC_Rx_Param_size = 0;
        ASC_Rx_Cmd_size = idx;
    }
    else
    {
        ASC_Rx_Param_size = ASC_Rx_Cnt - idx;           // idx after dummy increment
        ASC_Rx_Param_p = (uint8_t *)(ASC_RxBuf + idx);  // so no need +/-1 correction
        ASC_Rx_Cmd_size = idx - 1; // idx after dummy increment
        
        // 1.1 checking
        if( ASC_Rx_Param_size == 0)
            return ASC_RES_ERR;
    }
    
    // 2. command filed size
    ASC_Rx_Cmd_p = ASC_RxBuf;
    
    // 2.1 checking
    if( ASC_Rx_Cmd_size == 0)
        return ASC_RES_ERR;
    
    // 3. return OK, continue processing in caller
    return ASC_RES_OK;
}
/****************************************************************/
static 
int 
ASC_Execute(void)
{
    int         res;
    uint16_t    ands_len = 0;
    
    // 1. 1st part of answer ">rx"
    memcpy( ASC_TxBuf, ASC_Answ1_Rx, ASC_ANSW1_RX_BSIZE );
    ASC_Tx_Answ_size = ASC_ANSW1_RX_BSIZE;
    
    // 2. search command
    for(int i = 0; i < ASC_US_CMDs_COUNT; i++)
    {
        if( ASC_Rx_Cmd_size == ASC_US_Cmds[i].len )
        {
            if( memcmp( ASC_Rx_Cmd_p, ASC_US_Cmds[i].str,
                        ASC_US_Cmds[i].len ) == 0 )
            {
                res = ASC_US_Cmds[i].func( ASC_Rx_Param_p, ASC_Rx_Param_size,
                                           &ASC_TxBuf[ ASC_Tx_Answ_size ],
                                           &ands_len );
                break; //for
            }
        }
    }//for
    
    // 3. correct 1st part of answer
    if(res == ASC_RES_OK) 
        memcpy( &ASC_TxBuf[ ASC_ANSW1_RX_INSERT ], 
                ASC_Answ2_Ok, ASC_ANSW2_OK_BSIZE );
    
    // 4. check of overflow
    ASC_Tx_Answ_size += ands_len;
    if( ASC_Tx_Answ_size > ASC_TX_BUF_BSIZE - 1 )
        while(1);  // crash, RAM was demages
    
    // 5. add end_of_packet
    ASC_TxBuf[ ASC_Tx_Answ_size++ ] = ASC_END_ID;
    
    return ASC_RES_OK;
}
/****************************************************************/
int
ASC_Rx_Handler( uint8_t *   rx_data,    uint16_t    rx_data_bsize,
                uint8_t **  pp_tx_data, uint16_t *  tx_data_bsize )
{
    *tx_data_bsize = 0;
    ASC_Rx_Handler_cntr++;
    
    if( ASC_CheckPack( rx_data, rx_data_bsize) == ASC_RES_OK )
    {
        if( ASC_Decode() == ASC_RES_OK )
        {
            ASC_Execute();
            
            // NO copy answer to driver buffers
            //memcpy( tx_data, ASC_TxBuf, ASC_Tx_Answ_size );
            *pp_tx_data = ASC_TxBuf;
            
            *tx_data_bsize = ASC_Tx_Answ_size;
            return ASC_RES_OK;
        }
    }
    return ASC_RES_ERR;
}
/****************************************************************/
