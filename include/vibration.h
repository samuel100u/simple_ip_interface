#ifndef VIBRATION_H_INCLUDED

#define VIBRATION_H_INCLUDED

int vibration_init(int com_port);
int vibration_waveform(int fd, unsigned char *out,int n);
int vibration_spectrum(int fd, unsigned char *out,int n);
void vibration_close(int fd);

#endif 


