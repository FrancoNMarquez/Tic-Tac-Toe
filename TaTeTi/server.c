#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080    // Ver si esta bien el 8080 o hay que cambiarlo
#define MAXLINE 1024 // Preguntar que es esto
int main()
{

    char matriz_actualizada[9]; // Aca guardo la matriz del tablero?
    char *pointer_matriz = &matriz_actualizada;
    int socketfd, socket2fd;

    char pos_recibida = 0;

    struct sockaddr_in direcciónCliente1, direcciónCliente2;
    unsigned int tamañoDirección;

    // Internet domain socket
    struct sockaddr_in serveraddr, clientaddr; // Asi defino las variables de tipo sockaddr

    // Creating socket file descriptor
    // AF_INET es para ipv4 , SOCK_DGRAM para socket UDP, SOCK_STREAM para TCP
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    if ((socket2fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configuracion del servidor
    serveraddr.sin_family = AF_INET; // IPv4
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    // Bindeo el socket con el server address
    // associates an address with the socket descriptor.
    if (bind(socketfd, (const struct sockaddr *)&serveraddr,
             sizeof(serveraddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
        // Aca puede ir close(conexion) return 1;
        //  Despues del bind , el socket esta listo para recibir
    }
    if (bind(socket2fd, (const struct sockaddr *)&serveraddr,
             sizeof(serveraddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
        // Aca puede ir close(conexion) return 1;
        //  Despues del bind , el socket esta listo para recibir
    }

    printf("Estoy escuchando");
    listen(socketfd, 1); // Cuantos request antes de hacer accept?
    int cliente1 = accept(socketfd, (void *)&direcciónCliente1, &tamañoDirección);
    listen(socket2fd, 1);
    int cliente2 = accept(socket2fd, (void *)&direcciónCliente2, &tamañoDirección);

    // En teoria ya tengo la conexion tcp con ambos jugadores
    send(socketfd,1,1,0);
    send(socket2fd,0,1,0); // Les asigno que ficha le toca
    

    int jugada;    // Aca guardo el valor que paso el cliente
    int turno = 1; // 1 juega el cliente1, 0 juega el otro
    int hay_ganador = -1;
    
    /*Cambiando cosas ... del lado del cliente chequeo de enviar
    una posicion valida y que no este ocupada, asi no envio error*/

    //send enviarle al cliente 1 para que arranque
    while (hay_ganador != 0)
    {
        // Turno 1 seria el turno del cliente con la X
        if (turno == 1)
        {
            recv_message(socketfd, &jugada,1);
            // En teoria queda en jugada, guardado el valor que mando el usuario
            printf("%i", jugada);
            matriz_actualizada[jugada] = 1; // 1 era para X
            hay_ganador = hayGanador(jugada, matriz_actualizada, 1);
            turno = 0;
        }
        else
        {
            // Le toca al cliente 2
            recv_message(socket2fd, &jugada);
            printf("%i", jugada);

            matriz_actualizada[jugada] = 0; // 0 era para 1
            hay_ganador = hayGanador(jugada, matriz_actualizada, 0);
            turno = 1;
        }
    }
    printf("gano alguien xd");
    close(socketfd);
    close(socket2fd);
}


