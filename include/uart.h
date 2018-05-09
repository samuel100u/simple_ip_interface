#ifndef UART_H_INCLUDED

#define UART_H_INCLUDED

#ifdef __uart

extern "C" {


#endif

int uart_init(char *path,int baud_rate);

int uart_write(int uart0_filestream,unsigned char * tx_buffer, int count);

int uart_read(int uart0_filestream,unsigned char * rx_buffer,int length);

void uart_close(int fd);

#ifdef __uart

}

#endif


#endif 