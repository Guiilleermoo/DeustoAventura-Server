#ifndef FUNCIONESBD_H_
#define FUNCIONESBD_H_

#include "sqlite3.h"
#include "../Actividad.h"
#include "../Reserva.h"

void mensajeLog(char msg[],char* error);
char* load_config(char* filename, char* buscar);
void iniciarBD();
void cerrarBD();
char* comprobarCliente(char* dni, char* contra);
Actividad** getActividades();
int getNActividades();
int getNActividadesPorCiudad(char ciudad[]);
Actividad** getActividadesPorCiudad(char ciudad[]);
Actividad** getActividadesPorDificultad(char dificultad[]);
int getNActividadesPorDificultad(char dificultad[]);
Reserva** getReservasDNI(char* dni);
int getNReservas(char* dni);
void crearReserva(int codCliente, int codActividad, char fecha[], int cantPersonas);
int codigoCliente(char* dni);
void borrarReserva(int codCliente,int  codActividad, char*  fecha);
void registrarse(char* dni, char* nombre,char* apellido,char* correo,char* contra,int tlf,int cod_ciu);

#endif
