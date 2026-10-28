#include <utils/utilsClient.h>
#include <utils/structsCpu.h>

#ifndef PROTOCOLO_H
#define PROTOCOLO_H

// Funciones de conexión
void start_semaphores();
void read_configs_cpu(t_config * config);
int connect_cpu_as_client(char *ip, ID module, char *port, char *destiny, char *identifier);
int connect_cpu_as_client_kernel(char *ip, ID module, char *port, char *destiny, char *identifier);
handshakeMemory connect_cpu_as_client_memory(char *ip, ID module, char *port, char *destiny, char *identifier);
t_package *receive_package_CPU(int socket);
void * get_process_from_kernel(int socket);
void send_pic_pc_to_memory(int pid,int pc);

// Funciones de interpretación y ejecución
t_instruction* parse_instruction(char* instruccion);

// Funciones de comunicación con memoria
char* request_instruction(cpu_context_t* context);

#endif
