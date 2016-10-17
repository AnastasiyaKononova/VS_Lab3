#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 2048

void command(int sockets)
{
	char bufer[2048];
	int read;
	FILE * file;
	printf("Ожидание команды...\n");
	read = recv(sockets, bufer, 2048, 0);
	if(read <= 0) 
		return;
	printf("Получена команда: %s", bufer);
	file = popen(bufer, "r");
	if (file == NULL) {
		perror("Ошибка popen:\n");
		return;
	}
	int len = -1;
	while (len!= 0){
		len = fread(bufer, 1, BUF_SIZE, file);						
	}
	pclose(file);
	printf("Отправление принятой команды клиенту...\n\n");
	send(sockets, bufer, read, 0);
	close(socket);
}

int main()
{
	int sockets, listener;
	struct sockaddr_in addr; 
	
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0){
		perror("Ошибка socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("Ошибка bind");
		exit(2);
	}

	listen(listener, 1);
	while(1){
		sockets = accept(listener, NULL, NULL);
		if(sockets < 0){
			perror("Прием входящих подключений");
			exit(3);
		}
 		#ifdef PROCESS
			pid_t child = 0;
			if ((child=fork())==-1) {
				perror("Ошибка fork");
				exit(1);
			}
			if (child == 0) {
				command(sockets);
				return 0;
			}
		#else
			pthread_t thread1;
			int result = pthread_create(&thread1, NULL, command, sockets);
			if (result != 0) {
				perror("Ошибка создания потока");
			}
		#endif
	}
    
	return 0;
}
