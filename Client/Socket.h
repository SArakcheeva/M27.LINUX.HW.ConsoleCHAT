#ifndef _MY_GLOBAL
#define _MY_GLOBAL

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

extern int socket_file_descriptor, connection;
extern struct sockaddr_in serveraddress, client;
extern char messageCS[MESSAGE_LENGTH];

#endif