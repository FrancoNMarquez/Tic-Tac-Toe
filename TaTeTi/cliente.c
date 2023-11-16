#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


#define PORT 8080
#define MAXLINE 1024
#define SERV_HOST_ADDR //Aca iria el ?
char matriz[9]; //Aca guardo la matriz
int yo;

int main(int argc, char *argv[]){

	int sockfd, puertoNro, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	
	if (argc < 3) {
       printf("Para %s ingrese nombre del servidor y el numero de puerto\n", argv[0]);
       exit(1);
                  }
    puertoNro = atoi(argv[2]); //Captura del numero de puerto de salida

	// hasta aca entiendo
 	
  
    server = gethostbyname(argv[1]); // Recuperacion del nombre del servidor
    if (server == NULL) {
        fprintf(stderr,"ERROR, %s no conoce ese servidor\n", argv[1]);
        exit(2);
    }
	/* Rellenamos con valores el socket del cliente */

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port= htons(puertoNro);
	serv_addr.sin_addr.s_addr = inet_addr("172.");


	 /*
     * Get a stream socket.
     */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        tcperror("Socket()");
        exit(3);
	}

	//Ahora me conecto al socket del server
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        tcperror("Connect()");
        exit(4);
    }
	//Ya conectado, empieza la logica del juego
  
  recv(sockfd,&yo,1,0);
	int jugada;

	imprimir_tablero();
	int actualizar_jugada=-1;
	while(hayGanador()!=0){ // Falta implementar un Esta llena el tablero
		recv(sockfd,&actualizar_jugada,1,0);
    if(actualizar_jugada!=-1){
      actualizarjugada(actualizar_jugada);
      if(hayGanador()){
        break;
      }
    }
    
		do{
		printf("Elija una posicion: ");
    	scanf("%i", &jugada);
		}while((jugada<0 || jugada>8)&&(matriz[jugada]==-1));
		send(sockfd,&jugada,1,0); //Aca mando la posicion donde juego
		
    
    actualizarjugada(jugada);
		imprimir_tablero(); // Imprimo la matriz actualizada
	}
	
  close(sockfd);
	printf("Se cerro la conexion correctamente");
    return 0;
}

void imprimir_tablero() {
  //chequaer esto
  // Imprime una línea horizontal del tablero.
  for (int i = 0; i < 3; i++) {
    // Imprime los tres símbolos de la fila.
    for (int j = 0; j < 3; j++) {
      // Imprime el símbolo correspondiente a la posición del tablero.
      printf("%c ", simbolo_tablero(matriz[i * 3 + j]));

      // Imprime una barra vertical si es necesario.
      if (j < 2 || matriz[i * 3 + j] == -1) {
        printf("|");
      }
    }
    // Imprime una barra vertical al final de la fila.
    printf("|");
    printf("\n");

    // Imprime guiones para separar las filas.
    if (i < 2) {
      printf("---+---+---\n");
    }
  }
}

int hayGanador(int jugada, int j)
{
    // j=1 X , j=0 O
    //  Chequeo si hay ganador, y retorno 1 o 0
    /*012,345,678,036,147,258,045,246*/

    switch (jugada)
    {
    case '0':
        if ((matriz[1] == j && matriz[2] == j) ||
            ((matriz[3] == j) && (matriz[6] == j)) ||
            ((matriz[4] == j) && (matriz[8] == j)))
        {
            return 0;
        }
        break;
    case '1':
        if ((matriz[0] == j && matriz[2] == j) ||
            (matriz[4] == j && matriz[7] == j))
        {
            return 0;
        }
        break;
    case '2':
        if ((matriz[1] == j && matriz[0] == j) ||
            ((matriz[4] == j) && (matriz[6] == j)) ||
            ((matriz[5] == j) && (matriz[8] == j)))
        {
            return 0;
        }
        break;
    case '3':
        if ((matriz[0] == j && matriz[6] == j) ||
            (matriz[4] == j && matriz[5] == j))
        {
            return 0;
        }
        break;
    case '4':
        if ((matriz[1] == j && matriz[7] == j) ||
            ((matriz[3] == j) && (matriz[5] == j)) ||
            ((matriz[0] == j) && (matriz[8] == j)) ||
            ((matriz[2] == j) && matriz[6] == j))
        {
            return 0;
        }
        break;
    case '5':
        if ((matriz[8] == j && matriz[2] == j) ||
            (matriz[4] == j && matriz[3] == j))
        {
            return 0;
        }
        break;
    case '6':
        if ((matriz[3] == j && matriz[0] == j) ||
            ((matriz[4] == j) && (matriz[2] == j)) ||
            ((matriz[7] == j) && (matriz[8] == j)))
        {
            return 0;
        }
        break;
    case '7':
        if ((matriz[6] == j && matriz[8] == j) ||
            (matriz[4] == j && matriz[1] == j))
        {
            return 0;
        }
        break;
    case '8':
        if ((matriz[4] == j && matriz[0] == j) ||
            ((matriz[2] == j) && (matriz[5] == j)) ||
            ((matriz[6] == j) && (matriz[7] == j)))
        {
            return 0;
        }
        break;
    default:
        return -1;
        break;
    }
    return -1; //Esto seria si entra a los if y no era ganador
}