#include <stdio.h>
#include <winsock2.h>
#include "math.h"
#include "BD/funcionesBD.h"
#include "Actividad.h"
#include "string.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main()
{
	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		fflush(stdout);
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		fflush(stdout);
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");
	fflush(stdout);

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n");
	fflush(stdout);

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	fflush(stdout);
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));
	fflush(stdout);

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	//SEND and RECEIVE data (CLIENT/SERVER PROTOCOL)
	printf("Waiting for incoming commands from client... \n");
	fflush(stdout);
	fflush(stdout);

	iniciarBD();

	do
	{
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

		printf("Command received: %s \n", recvBuff);
		fflush(stdout);

		if (strcmp(recvBuff, "ComprobarCliente") == 0)
		{
			char dni[10];
			char contra[20];
			char respuesta[20];

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(dni, recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(contra, recvBuff);

			strcpy(respuesta, comprobarCliente(dni, contra));
			strcpy(recvBuff, respuesta);


			sprintf(sendBuff, " %s", respuesta);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
			fflush(stdout);
		}

		if (strcmp(recvBuff, "VisualizarActividades") == 0)
		{
			int tamanyo = getNActividades();
			Actividad* actividades = new Actividad[tamanyo];

			actividades = getActividades();

			int i;
			for(i = 0; i < tamanyo; i++)
			{
				strcpy(sendBuff, actividades[i].nombre);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, actividades[i].dificultad);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, (char*) actividades[i].limitePerMin);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, (char*)actividades[i].limitePerMax);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, (char*)actividades[i].edadMin);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			strcpy(sendBuff, "FIN");
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %d actividades \n", tamanyo);
			fflush(stdout);
		}

		if (strcmp(recvBuff, "VisualizarActividadesPorCiudad") == 0)
		{

		}

		if (strcmp(recvBuff, "VisualizarActividadesPorDificultad") == 0)
		{

		}

		if (strcmp(recvBuff, "MostrarCiudades") == 0)
		{

		}

		if (strcmp(recvBuff, "EXIT") == 0)
			break;

	} while (1);

	cerrarBD();

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();

	return 0;
}
