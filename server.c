#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 2048

void command(int sock)
{
	char buf[2048];
	int bytes_read;
	FILE * file;
	printf("Ожидание команды...\n");
	bytes_read = recv(sock, buf, 2048, 0);
	if(bytes_read <= 0) 
		return;
	printf("Получена команда: %s", buf);
	file = popen(buf, "r");
	if (file == NULL) {
		perror("Ошибка popen:\n");
		return;
	}
	int len = -1;
	while (len!= 0){
		len = fread(buf, 1, BUF_SIZE, file);						
	}
	pclose(file);
	printf("Отправление принятой команды клиенту...\n\n");
	send(sock, buf, bytes_read, 0);
	close(socket);
}

int main()
{
	int sock, listener;
	struct sockaddr_in addr; 
	
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0){
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("bind");
		exit(2);
	}

	listen(listener, 1);
	while(1){
		sock = accept(listener, NULL, NULL);
		if(sock < 0){
			perror("Прием входящих подключений");
			exit(3);
		}
 		#ifdef PROCESS
			pid_t childId = 0;
			if ((childId=fork())==-1) {
				perror("fork");
				exit(1);
			}
			if (childId == 0) {
				command(sock);
				return 0;
			}
		#else
			pthread_t thread1;
			int result = pthread_create(&thread1, NULL, command, sock);
			if (result != 0) {
				perror("Ошибка создания потока");
			}
		#endif
	}
    
	return 0;
}
