===============================================================================
                    Ascii String Command (ASC) Protocol
===============================================================================

Initial purpose of protocol is communication with electronic devices (based on
microcontrollers, FPGAs with cores) via serial interfaces (UART/RS485) using 
any(!) terminal programs on PC. Protocol does not use specific symbols such as
CR,LF because they are difficult to use in some PC clients.

---------------------------------------
                Terms
---------------------------------------
    Command packet  - request from PC to device, command for device
    Command         - field in command packet, short name 'cmd'
    Response packet - response from device to PC
    Response        - field in response packet
    Parameter       - filed in command/answer packet, short name 'param'
    
---------------------------------------
               Protocol
---------------------------------------
Command packet structure:
    <start_of_cmd> <command> [<param_separ>] [<parameter>] <end_of_cmd>

Response packet structure:
    <start_of_responce> <response> [<param_separ>] [<parameter>] <end_of_cmd>

Service symbols:
    $ - start of command packet
    # - end of command/response packets
    : - parameter separator
    > - start response

So command packets view: 
$ ... #      $ ... : ..... #
  cmd          cmd   param
                           
If parameter is a number, it is transmitted as ascii symbols:
115200 --> '1','1','5','2','0','0' (6 ascii symbols, 6 bytes)
                           
Examples of command packets:
$help#
$led_on:1#                  //turn on led number 1
$set_uart_speed:115200#     //set uart speed equal to 115200 bit/s
    
For response packets may be used templates: RX_OK, RX_ERR, so packet view:
>RX_OK#
>RX_OK:115200#

For every command packet device must send response packet:
$led_on:1#
>RX_OK#

$get_uart_speed#
>RX_OK:115200# 

---------------------------------------
         Address Extension
---------------------------------------
Address support will be added in future revisions.
@  - separator between start of packet and command or answer fields
Example:
$12@led_on:1#
>12@RX_OK#

---------------------------------------
           Reserved Commands
---------------------------------------
Some universal commands may be used for all devices. They need to help device 
operator.

$help#
----------
Get from device list of all supported commands. Response can be generated 
automatically from command name strings, stored in device flash memory.
Example:
    >RX_OK: help, info, led_on, led_off, reset, set_uart_speed#
Also device may send extended information about commands, for example examples
of usage:
    >RX_OK: help, info, led_on:1, led_off:1, set_uart_speed:115200#

$info
----------
Common information about device, such as device name, firmware version, 
board serial number. Example:
    >RX_OK:device=USB_COM_CONVERTOR,fw=12,brd=1#

-------------------------------------------------------------------------------
Copyright (c) 2017 by savver (savver.engineer@gmail.com)

