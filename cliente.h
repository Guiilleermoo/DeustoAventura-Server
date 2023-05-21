#ifndef CLIENTE_CLIENTE_H_
#define CLIENTE_CLIENTE_H_

typedef struct{
	char* dni;
	char* nombre;
	char* apellido;
	int telefono;
	char* correo;
	char* contrasena;
	int cod_ciudad;
}Cliente;

void imprimirCliente(Cliente cliente);

#endif
