# Ascii String Command (ASC) Protocol

Initial purpose of protocol is communication with electronic devices (based on
microcontrollers, FPGAs with cores) via serial interfaces (UART/RS485) using 
any(!) terminal programs on PC. Protocol does not use specific symbols such as
CR,LF because they are difficult to use in some PC clients.

Protocol description see in "description.txt"

Example of commands:

    $help#
    >RX_OK: help, info, led_on, led_off, reset, set_uart_speed#
    
    $led_on:1#
    >RX_OK#
    
    $set_uart_speed:115200#
    >RX_OK#
    
    $get_uart_speed#
    >RX_OK:115200# 
    
Address extension will be later:

    $12@led_on:1#
    >12@RX_OK#


