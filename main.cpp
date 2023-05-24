#include <stdio.h>
#include <winsock2.h>
#include "math.h"
#include "BD/funcionesBD.h"
#include "Actividad.h"
#include "string.h"
#include "Reserva.h"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int main()
{
	char dni[10];
	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		char* error = (char*) WSAGetLastError();
		mensajeLog((char*)"Failed. Error Code : %d", error);
		fflush(stdout);
		return -1;
	}

	printf("Initialised.\n");
	mensajeLog((char*)"Initialised.\n", NULL);

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		char* error = (char*) WSAGetLastError();
		mensajeLog((char*)"Could not create socket", error);
		fflush(stdout);
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");
	mensajeLog((char*)"Socket created.\n", NULL);
	fflush(stdout);

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		char* error = (char*) WSAGetLastError();
		mensajeLog((char*)"Bind failed with error code: %d", error);
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		char* error = (char*) WSAGetLastError();
		mensajeLog((char*)"Listen failed with error code: %d", error);
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	mensajeLog((char*)"Waiting for incoming connections...\n", NULL);
	fflush(stdout);
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		char* error = (char*) WSAGetLastError();
		mensajeLog((char*)"accept failed with error code : %d", error);
		fflush(stdout);
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));
	mensajeLog((char*)"Incomming connection...\n", NULL);
	fflush(stdout);

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	//SEND and RECEIVE data (CLIENT/SERVER PROTOCOL)
	printf("Waiting for incoming commands from client... \n");
	mensajeLog((char*)"Waiting for incoming commands from client... \n", NULL);
	fflush(stdout);
	fflush(stdout);

	iniciarBD();

	do
	{
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

		printf("Command received: %s \n", recvBuff);
		mensajeLog((char*)"Command received\n", NULL);
		fflush(stdout);

		if (strcmp(recvBuff, "ComprobarCliente") == 0)
		{
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
			mensajeLog((char*)"Response sent\n", NULL);
			fflush(stdout);
		}

		if (strcmp(recvBuff, "VisualizarCiudades") == 0)
		{
			int tamanyo = getNCiudades();
			Ciudad** ciudades = new Ciudad*[tamanyo];

			ciudades = getCiudades();

			int i;
			for(i = 0; i < tamanyo; i++)
			{
				sprintf(sendBuff, "%d", ciudades[i]->codigo);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, ciudades[i]->nombre);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			if(tamanyo==0){
				strcpy(sendBuff, "TAM0");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("Response sent: %d ciudades \n", tamanyo);
				mensajeLog((char*)"Response sent\n", NULL);
			}else{
				strcpy(sendBuff, "FIN");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("Response sent: %d ciudades \n", tamanyo);
				mensajeLog((char*)"Response sent\n", NULL);

			}

			fflush(stdout);
		}

		if (strcmp(recvBuff, "VisualizarActividades") == 0)
		{
			int tamanyo = getNActividades();
			Actividad** actividades = new Actividad*[tamanyo];

			actividades = getActividades();

			int i;
			for(i = 0; i < tamanyo; i++)
			{
				sprintf(sendBuff, "%d", actividades[i]->codigo);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, actividades[i]->nombre);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, actividades[i]->dificultad);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->limitePerMin);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->limitePerMax);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->edadMin);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			if(tamanyo==0){
				strcpy(sendBuff, "TAM0");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("Response sent: %d actividades \n", tamanyo);
				mensajeLog((char*)"Response sent\n", NULL);
			}else{
				strcpy(sendBuff, "FIN");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("Response sent: %d actividades \n", tamanyo);
				mensajeLog((char*)"Response sent\n", NULL);

			}

			fflush(stdout);
		}

		if (strcmp(recvBuff, "VisualizarActividadesPorCiudad") == 0)
		{
			char ciudad[20];

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(ciudad, recvBuff);

			int tamanyo = getNActividadesPorCiudad(ciudad);


			Actividad** actividades = new Actividad*[tamanyo];

			actividades = getActividadesPorCiudad(ciudad);

			int i;
			for(i = 0; i < tamanyo; i++)
			{
				sprintf(sendBuff, "%d", actividades[i]->codigo);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, actividades[i]->nombre);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, actividades[i]->dificultad);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->limitePerMin);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->limitePerMax);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->edadMin);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			if(tamanyo==0){
					strcpy(sendBuff, "TAM0");
					send(comm_socket, sendBuff, sizeof(sendBuff), 0);

				}
			if(tamanyo!=0)
						{
				strcpy(sendBuff, "FIN");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("Response sent: %d actividades \n", tamanyo);
				mensajeLog((char*)"Response sent\n", NULL);
						}



		}

		if (strcmp(recvBuff, "VisualizarActividadesPorDificultad") == 0)
		{
			char dificultad[20];

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(dificultad, recvBuff);

			int tamanyo = getNActividadesPorDificultad(dificultad);
			Actividad** actividades = new Actividad*[tamanyo];

			actividades = getActividadesPorDificultad(dificultad);

			int i;
			for(i = 0; i < tamanyo; i++)
			{
				sprintf(sendBuff, "%d", actividades[i]->codigo);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, actividades[i]->nombre);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				strcpy(sendBuff, actividades[i]->dificultad);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->limitePerMin);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->limitePerMax);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				sprintf(sendBuff, "%d", actividades[i]->edadMin);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			}
			if(tamanyo==0){
				strcpy(sendBuff, "TAM0");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			}
			if(tamanyo!=0)
			{
				strcpy(sendBuff, "FIN");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("Response sent: %d actividades \n", tamanyo);
				mensajeLog((char*)"Response sent\n", NULL);

			}

			fflush(stdout);
		}

		if (strcmp(recvBuff, "VisualizarReservas") == 0)
			{
				int tamanyo = getNReservas(dni);
				Reserva** reservas = new Reserva*[tamanyo];

				reservas = getReservasDNI(dni);

				int i;
				for(i = 0; i < tamanyo; i++)
				{
					strcpy(sendBuff, reservas[i]->fecha);
					send(comm_socket, sendBuff, sizeof(sendBuff), 0);

					sprintf(sendBuff, "%d", reservas[i]->cantPersonas);
					send(comm_socket, sendBuff, sizeof(sendBuff), 0);
					sprintf(sendBuff, "%d", reservas[i]->codActividad);
					send(comm_socket, sendBuff, sizeof(sendBuff), 0);
					sprintf(sendBuff, "%d", reservas[i]->codCliente);
					send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				}

				if(tamanyo==0){
					strcpy(sendBuff, "TAM0");
					send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				}else{
					strcpy(sendBuff, "FIN");
					send(comm_socket, sendBuff, sizeof(sendBuff), 0);
					printf("Response sent: %d actividades \n", tamanyo);
					mensajeLog((char*)"Response sent", NULL);
					fflush(stdout);
				}

			}

		if (strcmp(recvBuff, "R8") == 0)
		{
			int codCliente = codigoCliente(dni);
			int codActividad;
			char fecha[30];
			int cantPersonas;

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			codActividad = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(fecha, recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			cantPersonas = atoi(recvBuff);

			crearReserva(codCliente, codActividad, fecha, cantPersonas);

			strcpy(sendBuff, "R");
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: R \n");
			mensajeLog((char*)"Response sent", NULL);
			fflush(stdout);

		}


		if (strcmp(recvBuff, "RD") == 0)
		{
			int codCliente = codigoCliente(dni);
			int codActividad;
			char fecha[30];

			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			codActividad = atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(fecha, recvBuff);



			borrarReserva(codCliente, codActividad, fecha);

			strcpy(sendBuff, "RF");
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: RF \n");
			mensajeLog((char*)"Response sent", NULL);
			fflush(stdout);

		}
		if (strcmp(recvBuff, "REG") == 0)
				{
			char dni[10], nombre[20],apellido[20],correo[20],contra[20];
			int tlf, cod_ciu;
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(dni, recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(nombre, recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(apellido, recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			tlf= atoi(recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(correo, recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			strcpy(contra, recvBuff);
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			cod_ciu= atoi(recvBuff);


				registrarse(dni,nombre,apellido,correo,contra,tlf,cod_ciu);

				strcpy(sendBuff, "REGE");
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("Response sent: RF \n");
				mensajeLog((char*)"Response sent", NULL);
				fflush(stdout);

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
