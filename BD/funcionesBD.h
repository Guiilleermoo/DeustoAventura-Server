#ifndef FUNCIONESBD_H_
#define FUNCIONESBD_H_

char* load_config(char* filename, char* buscar);
void iniciarBD();
void cerrarBD();
void getClientes(sqlite3 *db, Cliente* clientes, int tamanyo);

#endif
