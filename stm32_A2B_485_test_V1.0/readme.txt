测试环境：
max485收发芯片

MCU：
STM32F103xxxx，72Mhz
波特率9600

A机（使用串口2）定时发送数据，接收B机传回数据并使用串口1发送数据；
B机（使用串口1）接收数据，使用12864显示，并传回数据

A机接线：
PA2-->DI
PA3-->RO
PG9-->DE,RE

B接线：
PA9-->DI
PA10-->RO
PA0-->DE,RE

LCD12864(ST7565R)
PA3-->RS
PA4-->RESET
PA5-->CS
PA2-->SCLK
PA1-->SDA