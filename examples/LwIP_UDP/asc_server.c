/**
  ************************************************************
  @file         ASC_server.c
  @author       savver (savver.engineer@gmail.com)
  @date         11.11.2017
  @brief        example of ASC server for LwIP stack
  
  @copyright    savver (Anton S.), MIT License
  ************************************************************
 */
/* Includes ------------------------------------------------------------------*/

    #include "main.h"
    #include "lwip/pbuf.h"
    #include "lwip/udp.h"
    #include "lwip/tcp.h"
    #include <string.h>
    #include <stdio.h>

    #include "asc_server.h"
    #include "asc_protocol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

    void udp_asc_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void udp_asc_server_init(void)
{
   struct udp_pcb *upcb;
   err_t err;
   
   /* Create a new UDP control block  */
   upcb = udp_new();
   
   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_ASC_SERVER_PORT);
      
      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_asc_server_receive_callback, NULL);
      }
      else
      {
        udp_remove(upcb);
        //printf("can not bind pcb");
      }
   }
   else
   {
     //printf("can not create pcb");
   } 
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_asc_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
  struct pbuf *     p_answ;

  /* Connect to the remote client */
  udp_connect(upcb, addr, UDP_ASC_CLIENT_PORT);

  //-------------------------
  uint8_t *       temp_ptr = NULL;
  uint8_t **      pp_answ_data = &temp_ptr;

  uint16_t        answ_len;

  ASC_Rx_Handler( p->payload, p->len,
                  pp_answ_data, &answ_len );
    
 /* allocate pbuf from pool*/
  p_answ = pbuf_alloc(PBUF_TRANSPORT, answ_len, PBUF_POOL);
    
  pbuf_take(p_answ, (char*)(*pp_answ_data), answ_len );
  //-------------------------
    
  /* Tell the client that we have accepted it */
  udp_send(upcb, p_answ);

  /* free the UDP connection, so we can accept new clients */
  udp_disconnect(upcb);

  /* Free the p buffer */
  pbuf_free(p);
  pbuf_free(p_answ);
}
