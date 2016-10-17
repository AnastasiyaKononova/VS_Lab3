#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
 
int main ()
{
	char message[2048];
	char bufer[sizeof(message)];
	int ch;
 
	int sockets;           
	struct sockaddr_in addr;

    	sockets = socket(AF_INET, SOCK_STREAM, 0);
	if(sockets < 0){
		perror("Ошибка socket\n");
		exit(1);
	}

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    	addr.sin_family = AF_INET;
    	addr.sin_port = htons(3425);
    
   	if(connect(sockets, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("Ошибка подключения\n");
		exit(2);
	}
 
	printf("Введите команду: ");
	fgets(message, 2048, stdin);
	send(sockets, message, sizeof(message), 0);
	int read = 0;
	read = recv(sockets, bufer, sizeof(message), 0);
	printf("Результат: %s\n", bufer);
    
	return 0;
}
