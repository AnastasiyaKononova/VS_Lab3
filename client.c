#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
 
int main ()
{
	char message[2048];
	char buf[sizeof(message)];
	int ch;
 
	int sock;           
	struct sockaddr_in addr;

    	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("Ошибка socket\n");
		exit(1);
	}

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    	addr.sin_family = AF_INET;
    	addr.sin_port = htons(3425);
    
   	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("Ошибка подключения\n");
		exit(2);
	}
 
	printf("Введите команду: ");
	fgets(message, 2048, stdin);
	send(sock, message, sizeof(message), 0);
	int bytes_read = 0;
	bytes_read = recv(sock, buf, sizeof(message), 0);
	printf("Результат: %s\n", buf);
    
	return 0;
}
