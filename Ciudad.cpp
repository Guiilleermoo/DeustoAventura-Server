#include "Ciudad.h"
#include "string.h"

Ciudad::Ciudad()
{
	this->codigo = 0;
	this->nombre = NULL;
}

Ciudad::Ciudad(int codigo, char* nombre)
{
	this->codigo = codigo;
	this->nombre = new char[strlen(nombre) + 1];
	this->nombre = nombre;
}

Ciudad::Ciudad(const Ciudad &c)
{
	this->codigo = c.codigo;
	this->nombre = new char[strlen(c.nombre) + 1];
	this->nombre = c.nombre;
}

Ciudad::~Ciudad()
{
	delete [] nombre;
}

