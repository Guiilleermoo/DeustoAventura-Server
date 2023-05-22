#include <stdio.h>
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#define __cplusplus
#include "sqlite3.h"

#include "../Actividad.h"

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

void getActividades(Actividad* actividades, int* tamanyo)
{
	*tamanyo = 0;
	char sql[] = "select * from ACTIVIDAD";

	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	do {
		result = sqlite3_step(stmt);
		tamanyo++;
		if (result == SQLITE_ROW)
		{
			char nombre[30];
			char dificultad[10];
			strcpy(nombre, (char*) sqlite3_column_text(stmt, 1));
			strcpy(dificultad, (char*) sqlite3_column_text(stmt, 2));
			int limitePerMin = sqlite3_column_int(stmt, 3);
			int limitePerMax = sqlite3_column_int(stmt, 4);
			int edadMin = sqlite3_column_int(stmt, 5);

			Actividad actividad(nombre, dificultad, limitePerMin, limitePerMax, edadMin);

			actividades[*tamanyo] = actividad;

		}
	} while (result == SQLITE_ROW);
}
