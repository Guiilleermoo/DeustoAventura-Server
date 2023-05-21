#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../cliente.h"

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
	char*ruta=load_config("conf.txt","ruta");

	sqlite3_open(ruta, &db);
	free(ruta);

}

void cerrarBD()
{
	sqlite3_close(db);
}

void getClientes(sqlite3 *db, Cliente* clientes, int tamanyo)
{
	  char sql[] = "select * from CLIENTE";

	  int result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) ;
	  if(result != SQLITE_OK)
	  {
		  printf("Error preparando consulta SELECT");
		  printf("%s\n", sqlite3_errmsg(db));
	  }

	  do {
		  result = sqlite3_step(stmt) ;
		  if (result == SQLITE_ROW)
		  {

		  	char* dni = (char*) sqlite3_column_text(stmt, 1);
		  	char* nombre = (char*) sqlite3_column_text(stmt, 2);
		  	char* apellido = (char*) sqlite3_column_text(stmt, 3);
		  	int telefono = (int) sqlite3_column_text(stmt, 4);
		  	char* correo = (char*) sqlite3_column_text(stmt, 5);
		  	char* contrasena = (char*) sqlite3_column_text(stmt, 6);
		  	int cod_ciudad = (int) sqlite3_column_text(stmt, 7);

		  	int i;
		  	for(i = 0; i < tamanyo; i++)
		  	{
		  		clientes = malloc(sizeof(Cliente) * tamanyo);
		  		strcpy(clientes[i].dni, dni);
		  		strcpy(clientes[i].nombre, nombre);
		  		strcpy(clientes[i].apellido, apellido);
		  		clientes[i].telefono = telefono;
		  		strcpy(clientes[i].correo, correo);
		  		strcpy(clientes[i].contrasena, contrasena);
		  		clientes[i].cod_ciudad = cod_ciudad;
		  	}
		  }
	  } while (result == SQLITE_ROW);
	  printf("\n");

	  sqlite3_finalize(stmt);
}
