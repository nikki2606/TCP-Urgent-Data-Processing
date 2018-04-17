#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include<netinet/tcp.h>
int cl_sock;

void bail(const char *err_msg) {
  if ( errno != 0 ) {
    fputs(strerror(errno),stderr);
  fputs(": ",stderr);
   }
  fputs(err_msg,stderr);
  fputc('\n',stderr);
  exit(1);
  }
static void sigurg(int signo) {
   int m;
   char buff[200];
   m = recv(cl_sock,buff,sizeof buff,MSG_OOB);
   //printf("cl_sock:%d\n",cl_sock);
   buff[m] = 0;
   printf("\nURG '%s' (%d)\n",buff,m);
   signal(SIGURG,sigurg);
 }

int main(int argc , char *argv[]){
	int sc_desc,ch,rd_size,z;
	struct sockaddr_in srv,cl;
	char buffer[2000];
	
	sc_desc= socket(AF_INET,SOCK_STREAM,0); 
	if(sc_desc == -1){
		printf("\nFAILED SOCKET CREATION");	
	}
	printf("SUCCESSFULLY CREATED");
  	srv.sin_family = AF_INET;
  	srv.sin_addr.s_addr = INADDR_ANY;
	srv.sin_port = htons( 8888 ); 

	if( bind (sc_desc,(struct sockaddr *)&srv ,sizeof(srv))<0){
		perror( "Failed 2");
		return 1;
	}
	puts("\nREADY TO CONNECT");

	listen(sc_desc,3);

	ch= sizeof(struct sockaddr_in);
	cl_sock=accept(sc_desc,(struct sockaddr *)&cl,(socklen_t*)&ch);
	if(cl_sock <0){
		perror( "\nFAILED");
		return 1;
	}	
	puts("CLIENT CONNECTED");
  z = fcntl(cl_sock,F_SETOWN,getpid());
  if ( z == -1 )
    bail("fcntl(2)");
  signal(SIGURG,sigurg);
  while(1){
      z = recv(cl_sock,buffer,sizeof buffer,0);
      if ( z == -1 )
	bail("recv(2)");
      if ( z == 0 )
	break;
      buffer[z] = 0;
    printf("\nRECEIVED: '%s' (BYTES: %d)\n",buffer, z);
    }
  close(cl_sock);
  return 0;
}	


