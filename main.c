#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "sockets.h"

#define BUFFER_LEN 99
 
    char Packet[15];
char query69[200];
char query70[15];
char query63[11];

int portsrv = 7777; // <----- AQUI PONEMOS EL PUERTO DE NUESTRO SERVIDOR
 char *iprecv;
 char *portdst;
char modesrv[] = "Rol en espa9ol"; // <---------- AQUI PONEMOS EL NOMBRE DEL GM
char namesrv[] = "[Â»] City World Roleplay | Perfeccionando el Rol[Â«]"; // <----- AQUI EL NOMBRE DEL SERVIDOR
char lenguage[] = "Espa9ol - Spanish"; // <------- LENGUAJE DEL SERVIDOR
    int numbytex = 0;
int playerson = 100;  // <----------- PODEMOS EDITAR LOS USUARIOS QUE QUEREMOS QUE APAREZCAN "ON" (no hace falta que haya$
int maxplayers = 600; // <------------- USUARIOS MAXIMOS

 /*
char modesrv[] = "FSAMP"; // <---------- AQUI PONEMOS EL NOMBRE DEL GM
char namesrv[] = "[RP] Frankito/wachichorro roleplay"; // <----- AQUI EL NOMBRE DEL SERVIDOR
char lenguage[] = "Arg villero"; // <------- LENGUAJE DEL SERVIDOR
    int numbytex = 0;
int playerson = 200;  // <----------- PODEMOS EDITAR LOS USUARIOS QUE QUEREMOS QUE APAREZCAN "ON" (no hace falta que haya nadie para que funcione)
int maxplayers = 200; // <------------- USUARIOS MAXIMOS */
char byteips[4];


 
void processpacket(char *h, char *i)
{
	char addip[100];  
  int querylen = numbytex;
    int query70len = 15;
    int query63len = 13;
      if ( Packet[0] == 'S' && Packet[1] == 'A' && Packet[2] == 'M' && Packet[3] == 'P' )
{
//	char addip[100];
    switch(Packet[10])
    {
        case '\x69':
	query69[8] = Packet[8];
        query69[9] = Packet[9];
        enviopack(iprecv, portsrv, portdst, query69, querylen);
//	printf("%s - Enviado", iprecv);        
       sprintf(addip, "echo %s >> trampa.txt", iprecv);
        system(addip);
//printf("%s // %d // %d // %s \n", i, SERVER_PORT, h, query69);
        break;
        case '\x70':
        query70[8] = Packet[8];
        query70[9] = Packet[9];
        query70[11] = Packet[11];
        query70[12] = Packet[12];
        query70[13] = Packet[13];
        query70[14] = Packet[14];
        enviopack(iprecv, portsrv, portdst, query70, query70len);
        break;
	//printf("%s \n", iprecv);
    }
}
}
 
void makeArrayIP(char* inIP,unsigned char* outIP){
    short i,nTemp=0; char cbool=0;
    for(i=0;inIP[i] != NULL;i++){
        if(inIP[i] != '.'){
            nTemp = nTemp*10 + (short)(inIP[i] - '0');
        }else{
            outIP[cbool] = (char) nTemp;
            nTemp = 0; /*Reiniciar el valor temporal*/
            cbool++;
           }
     
 
    }
    outIP[cbool] = (char) nTemp;
}
 
 
int escucha()
{
 int sockfd;
 struct sockaddr_in my_addr;
 struct sockaddr_in their_addr;
 
 int addr_len, numbytes;
 char buf[BUFFER_LEN];
 
 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 17)) == -1) {
 perror("socket");
 exit(1);
 }
 
 my_addr.sin_family = AF_INET;
 my_addr.sin_port = htons(portsrv);
 my_addr.sin_addr.s_addr = INADDR_ANY;
 bzero(&(my_addr.sin_zero), 8);
 
 if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
 perror("bind");
 exit(1);
 }
 
 addr_len = sizeof(struct sockaddr);
 
while(1)
{
 if ((numbytes=recvfrom(sockfd, Packet, 15, 0, (struct sockaddr *)&their_addr, (socklen_t *)&addr_len)) == -1) {
 perror("recvfrom");
 exit(1);
 }
 //printf("recibido:%s\n", Packet);
  iprecv = inet_ntoa(their_addr.sin_addr);
  portdst = ntohs(their_addr.sin_port);
  //printf("recibido %s \n", Packet);
  processpacket(portdst, iprecv);
}
 close(sockfd);
 
 }
 
 
 
int main(int argc, char *argv[])
{
    /*
    printf("Ingrese el nombre del server:");
        fgets(namesrv, 50, stdin);
    printf("Ingrese gamemode del server:");
        fgets(modesrv, 21, stdin);
    printf("Ingrese lenguaje del server:");
        fgets(lenguage, 15, stdin);*/
        makeArrayIP(argv[1], &byteips[0]);
        query70[0] = 0x53;
        query69[0] = 0x53; // 0
    numbytex++;
    query69[1] = 0x41; // 1
    query70[1] = 0x41;
    numbytex++;
    query70[2] = 0x4d;
    query69[2] = 0x4d; // 2
    numbytex++;
    query70[3] = 0x50;
    query69[3] = 0x50; // 3
    numbytex++;
    query69[4] = byteips[0]; // IP
    query70[4] = byteips[0];
    numbytex++;
    query69[5] = byteips[1];
    query70[5] = byteips[1];
    numbytex++;
    query69[6] = byteips[2];
    query70[6] = byteips[2];
    numbytex++;
    query69[7] = byteips[3];
    query70[7] = byteips[3];
    numbytex++;
    numbytex++;
    numbytex++;
    query70[10] = 0x70;
    query69[10] = 0x69; // 10
    numbytex++;
    query70[11] = 0x44;
    query70[12] = 0x44;
    query70[13] = 0x44;
    query70[14] = 0x44;
    query69[numbytex] = 0x00; // 11 PASSWORD
    numbytex++;
    query69[numbytex] = (char)(playerson & 0xFFFF); //PLAYERS
    numbytex++;
    query69[numbytex] = (char)(playerson  >> 8 & 0xFFFF); // PLAYERS
    numbytex++;
    query69[numbytex] = (char)(maxplayers & 0xFFFF); // MAX PLAYERS
    numbytex++;
    query69[numbytex] = (char)(maxplayers >> 8 & 0xFFFF); // MAX PLAYERS
    numbytex++;
    query69[numbytex] = (char)(sizeof(namesrv) & 0xFFFF); // TAMANIO DE NOMBRE
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO DENOMBRE
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO DE NOMBRE
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO DENOMBRE
    int xcz = numbytex;
    //printf("nameserver:");
for(int i = 0; i != sizeof(namesrv); i++)
{
switch(namesrv[i])
{
case '\x39':
namesrv[i] = 0xf1;
printf("remplazado en byte [%d] quedando \n", i);
break;

}
}


for(int i = 0; i != sizeof(modesrv); i++)
{
switch(modesrv[i])
{
case '\x39':
modesrv[i] = 0xf1;
printf("remplazado en byte [%d] quedando \n", i);
break;

}
}
	 
for(int i = 0; i != sizeof(namesrv); i++)
    {
    numbytex++;
    query69[numbytex] = namesrv[i];
//  printf("%d - %d | %c |-> %c \n", i, xcz, namesrv[i], query69[numbytexz]);
    }
    numbytex++;
    query69[numbytex] = (char)(sizeof(modesrv) & 0xFFFF); // TAMANIO DE GM NAME
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO ED GM NAME
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO ED GM NAME
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO ED GM NAME
    //printf("\n modeserver:");
    for(int i = 0; i != sizeof(modesrv); i++)
    {
    numbytex++;
    query69[numbytex] = modesrv[i];
    //printf("%d - %d | %c \n", i, xcx, modesrv[i]);
    }
    numbytex++;
    query69[numbytex] = (char)(sizeof(lenguage) & 0xFFFF) ; // TAMANIO SRV LENGUAGE
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO SRV LENGUAGE
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO SRV LENGUAGE
    numbytex++;
    query69[numbytex] = 0x00; // TAMANIO SRV LENGUAGE
    //printf("\n lenguague:");
    for(int i = 0; i != sizeof(lenguage); i++)
    {
        numbytex++;
    query69[numbytex] = lenguage[i];
        //printf("%d - %d | %c \n", i, xcv, lenguage[i]);
    }
    numbytex++;
	   query63[0] = 0x53;
    query63[1] = 0x41;
    query63[2] = 0x4d;
    query63[3] = 0x50;
    query63[4] = byteips[0];
    query63[5] = byteips[1];
    query63[6] = byteips[2];
    query63[7] = byteips[3];
    query63[8] = (char)(SERVER_PORT & 0xFFFFFF);
    query63[9] = (char)(SERVER_PORT >> 8 & 0xFFFFFF);
    query63[10] = 0x63;
    query63[11] = 0x00;
    query63[12] = 0x00;
   
 printf("%s \n", query69);
//	remplaze(query69); 
   escucha();
}
