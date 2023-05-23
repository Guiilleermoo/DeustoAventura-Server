#include <stdio.h>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#define __cplusplus
#include "sqlite3.h"

#include "../Actividad.h"
#include "../Reserva.h"
sqlite3 *db;
sqlite3_stmt *stmt;
int result;

char* load_config(char* filename, char* buscar)
{
    FILE* archivo;
    char linea[100];
    char* igual;
    char buscar2[20];

    archivo = fopen(filename, "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return NULL;
    }

    char* resultado = NULL;
    while (fgets(linea, 100, archivo)) {
        int i = 0;
        while (linea[i] != '=') {
            buscar2[i] = linea[i];
            i++;
        }
        buscar2[i] = '\0';

        if (strcmp(buscar, buscar2) == 0) {
            igual = strchr(linea, '=');
            if (igual != NULL) {
                int longitud = strlen(igual + 1);
                resultado = (char*) realloc(resultado, longitud * sizeof(char));
                if (resultado == NULL) {
                    printf("Error al asignar memoria.\n");
                    return NULL;
                }
                strncpy(resultado, igual + 1, longitud);
                resultado[longitud - 1] = '\0';
            }
        }
    }

    fclose(archivo);
    return resultado;
}

void iniciarBD()
{
	char* ruta = load_config("conf.txt","ruta");

	sqlite3_open(ruta, &db);
	free(ruta);

}

void cerrarBD()
{
	sqlite3_close(db);
}

char* comprobarCliente(char dni[], char contra[])
{

	char sql[] = "select * from CLIENTE where DNI = ? and CONTRASENA = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) ;

	if (result != SQLITE_OK)
	{
		printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
	 }


	sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, contra, strlen(contra), SQLITE_STATIC);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		return "Correcto";
		break;
	}

	sqlite3_finalize(stmt);

	return "Incorrecto";
}

int getNActividades()
{
	int resultado;
	char sql[] = "select count(*) from ACTIVIDAD";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
		{
			printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
		 }

	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW)
		{
			resultado = sqlite3_column_int(stmt, 0);
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);

	return resultado;
}

Actividad** getActividades()
{
	int tamanyo = getNActividades();
	Actividad** actividades = (Actividad**) malloc(sizeof(Actividad*) * tamanyo);

	char sql[] = "select * from ACTIVIDAD";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
		{
			printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
		 }

		int i;
		for(i = 0; i < tamanyo; i++)
		{
			result = sqlite3_step(stmt);
			if (result == SQLITE_ROW)
			{
				int codigo = sqlite3_column_int(stmt, 0);
				char nombre[30];
				char dificultad[10];
				strcpy(nombre, (char*) sqlite3_column_text(stmt, 1));
				strcpy(dificultad, (char*) sqlite3_column_text(stmt, 2));
				int limitePerMin = sqlite3_column_int(stmt, 3);
				int limitePerMax = sqlite3_column_int(stmt, 4);
				int edadMin = sqlite3_column_int(stmt, 5);

				Actividad* actividad = new Actividad(codigo, nombre, dificultad, limitePerMin, limitePerMax, edadMin);

				actividades[i] = actividad;
			}
		}

	sqlite3_finalize(stmt);

	return actividades;
}

int getNActividadesPorCiudad(char ciudad[])
{
	int resultado;
	char sql[] = "select count(*) from ACTIVIDAD A, PARQUE P, LUGAR L, OFERTA O where A.COD_ACT = O.COD_ACT and O.COD_PARK = P.COD_PARK and P.COD_CIU = L.COD_CIU and L.NOMBRE_CIU = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
		{
			printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
		 }

	sqlite3_bind_text(stmt, 1, ciudad, strlen(ciudad), SQLITE_STATIC);

	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW)
		{
			resultado = sqlite3_column_int(stmt, 0);
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);

	return resultado;
}

Actividad** getActividadesPorCiudad(char ciudad[])
{
	int tamanyo = getNActividades();
	Actividad** actividades = (Actividad**) malloc(sizeof(Actividad*) * tamanyo);

	char sql[] = "select A.COD_ACT, A.NOMBRE_ACT, A.DIFICULTAD, A.LIMITE_PER_MIN, A.LIMITE_PER_MAX, A.EDAD_MIN from ACTIVIDAD A, PARQUE P, LUGAR L, OFERTA O where A.COD_ACT = O.COD_ACT and O.COD_PARK = P.COD_PARK and P.COD_CIU = L.COD_CIU and L.NOMBRE_CIU = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
		{
			printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
		 }

	sqlite3_bind_text(stmt, 1, ciudad, strlen(ciudad), SQLITE_STATIC);

	int i;
	for(i = 0; i < tamanyo; i++)
	{
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW)
		{
			int codigo = sqlite3_column_int(stmt, 0);
			char nombre[30];
			char dificultad[10];
			strcpy(nombre, (char*) sqlite3_column_text(stmt, 1));
			strcpy(dificultad, (char*) sqlite3_column_text(stmt, 2));
			int limitePerMin = sqlite3_column_int(stmt, 3);
			int limitePerMax = sqlite3_column_int(stmt, 4);
			int edadMin = sqlite3_column_int(stmt, 5);

			Actividad* actividad = new Actividad(codigo, nombre, dificultad, limitePerMin, limitePerMax, edadMin);

			actividades[i] = actividad;
		}
	}

	sqlite3_finalize(stmt);

	return actividades;
}

Actividad** getActividadesPorDificultad(char dificultad[])
{
	int tamanyo = getNActividades();
	Actividad** actividades = (Actividad**) malloc(sizeof(Actividad*) * tamanyo);

	char sql[] = "select* from ACTIVIDAD where DIFICULTAD = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
		{
			printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
		 }

	sqlite3_bind_text(stmt, 1, dificultad, strlen(dificultad), SQLITE_STATIC);

	int i;
	for(i = 0; i < tamanyo; i++)
	{
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW)
		{
			int codigo = sqlite3_column_int(stmt, 0);
			char nombre[30];
			char dificultad[10];
			strcpy(nombre, (char*) sqlite3_column_text(stmt, 1));
			strcpy(dificultad, (char*) sqlite3_column_text(stmt, 2));
			int limitePerMin = sqlite3_column_int(stmt, 3);
			int limitePerMax = sqlite3_column_int(stmt, 4);
			int edadMin = sqlite3_column_int(stmt, 5);

			Actividad* actividad = new Actividad(codigo, nombre, dificultad, limitePerMin, limitePerMax, edadMin);

			actividades[i] = actividad;
		}
	}

	sqlite3_finalize(stmt);

	return actividades;
}

int getNActividadesPorDificultad(char dificultad[])
{
	int resultado;
	char sql[] = "select count(*) from ACTIVIDAD A where DIFICULTAD = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
		{
			printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
		 }

	sqlite3_bind_text(stmt, 1, dificultad, strlen(dificultad), SQLITE_STATIC);

	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW)
		{
			resultado = sqlite3_column_int(stmt, 0);
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);

	return resultado;
}
Reserva** getReservasDNI(char* dni){


	int tamanyo = getNActividades();
	Reserva** reservas = (Reserva**) malloc(sizeof(Reserva*) * tamanyo);

	char sql[] = "select * from RESERVA R, CLIENTE C where  C.COD_CLTE=R.COD_CLTE and C.DNI=?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
		{
			printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
		 }

	sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);

	int i;
	for(i = 0; i < tamanyo; i++)
	{
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW)
		{
			char fecha[30];
			int codC = sqlite3_column_int(stmt, 0);
			int codA = sqlite3_column_int(stmt, 1);
			strcpy(fecha, (char*) sqlite3_column_text(stmt, 2));

			int cantP = sqlite3_column_int(stmt, 3);

			Reserva* reserva = new Reserva(codC, codA, fecha, cantP);

			reservas[i] = reserva;
			}
		}

	sqlite3_finalize(stmt);

	return reservas;

}
int getNReservas(char* dni)
{
	int resultado;
	char sql[] = "select count(*) from RESERVA R, CLIENTE C where  C.COD_CLTE=R.COD_CLTE and C.DNI==?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);

	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW)
		{
			resultado = sqlite3_column_int(stmt, 0);
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);

	return resultado;
}

void crearReserva(int codCliente, int codActividad, char fecha[], int cantPersonas)
{
	char sql[] = "insert into RESERVA values(?, ?, ?, ?)";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_bind_int(stmt, 1, codCliente);
	sqlite3_bind_int(stmt, 2, codActividad);
	sqlite3_bind_text(stmt, 3, fecha, strlen(fecha), SQLITE_STATIC);
	sqlite3_bind_int(stmt, 4, cantPersonas);

	result = sqlite3_step(stmt);

	if (result != SQLITE_DONE) {
		printf("Error insertando la reserva\n");

		fprintf(stderr, "%s", sqlite3_errmsg(db));
		fflush(stdout);
	}else{
		printf("Reserva insertada ");
		fflush(stdout);
	}

	sqlite3_finalize(stmt);
}

int codigoCliente(char* dni)
{
	int resultado;

	char sql[] = "select COD_CLTE from CLIENTE where DNI = ?";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);

	do {
		result = sqlite3_step(stmt);
		if (result == SQLITE_ROW)
		{
			resultado = sqlite3_column_int(stmt, 0);
		}
	} while (result == SQLITE_ROW);

	sqlite3_finalize(stmt);

	return resultado;
}
void borrarReserva(int codCliente,int  codActividad, char*  fecha){


		char sql[] = "delete  from RESERVA where COD_ACT = ? and COD_CLTE=? and FECHA_RES=?";

		result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

		if (result != SQLITE_OK)
		{
			printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
		}

		sqlite3_bind_int(stmt, 1, codActividad);

		sqlite3_bind_int(stmt, 2,codCliente );

		sqlite3_bind_text(stmt, 3, fecha, strlen(fecha), SQLITE_STATIC);

		result = sqlite3_step(stmt);

		if (result != SQLITE_DONE) {
			printf("Error borrando reserva\n");

			fprintf(stderr, "%s", sqlite3_errmsg(db));
			fflush(stdout);
		}else{
			printf("Reserva borrada ");
			fflush(stdout);
		}

		sqlite3_finalize(stmt);

}

void registrarse(char* dni, char* nombre,char* apellido,char* correo,char* contra,int tlf,int cod_ciu){

	char sql[] = "insert  into CLIENTE values(NULL,?,?,?,?,?,?,?)";

	result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if (result != SQLITE_OK)
	{
		printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_bind_text(stmt, 1, dni, strlen(dni), SQLITE_STATIC);

	sqlite3_bind_text(stmt, 2, nombre, strlen(nombre), SQLITE_STATIC);

	sqlite3_bind_text(stmt, 3, apellido, strlen(apellido), SQLITE_STATIC);
	sqlite3_bind_int(stmt, 4, tlf);
	sqlite3_bind_text(stmt, 5, correo, strlen(correo), SQLITE_STATIC);

	sqlite3_bind_text(stmt, 6, contra, strlen(contra), SQLITE_STATIC);



	sqlite3_bind_int(stmt, 7,cod_ciu );



	result = sqlite3_step(stmt);

	if (result != SQLITE_DONE) {
		printf("Error borrando reserva\n");

		fprintf(stderr, "%s", sqlite3_errmsg(db));
		fflush(stdout);
	}else{
		printf("Reserva borrada ");
		fflush(stdout);
	}

	sqlite3_finalize(stmt);
}
