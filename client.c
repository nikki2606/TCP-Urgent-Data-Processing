#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

 void bail(const char *err_msg) {
    if ( errno != 0 ) {
       fputs(strerror(errno),stderr);
          fputs(": ",stderr);
   }
     fputs(err_msg,stderr);
      fputc('\n',stderr);
      exit(1);
  }

 static void iband(int s,char *str) {
    int z;
    z = send(s,str,strlen(str),0);
   if ( z == -1 )
         bail("send(2)");
   printf("IN BAND DATA: '%s' (BYTES: %d)\n",str,z);
  }

 static void oband(int s,char *str) {
      int z;

     z = send(s,str,strlen(str),MSG_OOB);
      if ( z == -1 )
         bail("send(2)");
     
    printf("OOB '%s' (%d)\n",str,z);
 }


int main(int argc , char *argv[]){
	int sc,s=-1,i=0,ans;//
	struct sockaddr_in srv;
	char msg[2000] , msgs[100][50], reply[2000], dump;

	srv.sin_family = AF_INET;
  	srv.sin_addr.s_addr = inet_addr("192.168.43.145");
	srv.sin_port = htons( 8888 ); 
	
	sc= socket(AF_INET,SOCK_STREAM,0);
	if(sc == -1){
		printf("FAILED SOCKET CREATION");	
	}
	else
		printf("Successfully created");

	if( connect(sc, (struct sockaddr *)&srv , sizeof(srv))<0){
		perror("Error");
	}
	else
		printf("\nSuccessfully connected");

	while(1)
	    {
		printf("\nEnter message: ");
		scanf("%[^\n]s" , msg);
		if( send(sc , msg , strlen(msg) , 0) < 0)
			{
			    puts("\nSend failed");
			    return 1;
			}

		printf("\nWant to send more data?(1/0):");
		scanf("%d",&ans);
		while(ans!=0)
		{
			i++;
			printf("\nEnter message: ");
			//scanf("%0s",msg);

			scanf("%c", &dump);
			scanf("%[^\n]s",msg);

			if(i%2==0)
			 iband(sc,msg);
			else
			 oband(sc,msg);

			memset(msg,0,strlen(msg));

			printf("\nWant to send more data?(1/0):");
			scanf("%d",&ans);	
		}
		if(ans==0)
			break;
	    }
	     close(sc);
	    return 0;
	}						
