/**
  ************************************************************
  @file         ASC_server.c
  @author       savver (savver.engineer@gmail.com)
  @date         11.11.2017
  @brief        example of ASC server for LwIP stack
  
  @copyright    savver (Anton S.), MIT License
  ************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef __UDP_ASC_SERVER_H__
#define __UDP_ASC_SERVER_H__

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

    #define UDP_ASC_SERVER_PORT    60000   /* define the UDP local connection port */
    #define UDP_ASC_CLIENT_PORT    60000   /* define the UDP remote connection port */

/* Exported functions ------------------------------------------------------- */

    void udp_asc_server_init(void);

#endif /* __UDP_ASC_SERVER_H__ */
