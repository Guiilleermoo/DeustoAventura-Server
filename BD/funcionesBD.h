#ifndef FUNCIONESBD_H_
#define FUNCIONESBD_H_

#include "sqlite3.h"
#include "../Actividad.h"
#include "../Reserva.h"
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

#endif
