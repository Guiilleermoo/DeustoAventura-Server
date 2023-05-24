#include "Ciudad.h"
#include "string.h"

#include <iostream>
using namespace std;

Ciudad::Ciudad()
{
	this->codigo = 0;
	this->nombre = NULL;
}

Ciudad::Ciudad(int codigo, char* nombre)
{
	this->codigo = codigo;
	this->nombre = new char[strlen(nombre) + 1];
	strcpy(this->nombre,nombre);

}

Ciudad::Ciudad(const Ciudad &c)
{
	this->codigo = c.codigo;
	this->nombre = new char[strlen(c.nombre) + 1];
	strcpy(this->nombre,nombre);
}

Ciudad::~Ciudad()
{
	delete [] nombre;
}

void Ciudad::imprimir()
{
	cout << "Ciudad " << this->codigo << ": " <<this->nombre << endl;;
}
