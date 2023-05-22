/*
 * reserva.cpp
 *
 *  Created on: 22 may 2023
 *      Author: borja
 */

#include "Reserva.h"
#include <string.h>
Reserva::Reserva() {
	// TODO Auto-generated constructor stub
this->cantPersonas=0;
this->codActividad=0;
this->codCliente=0;
this->fecha=NULL;

}
Reserva::Reserva(int codC,int codA,char* f,int cantP){
	this->cantPersonas=cantP;
	this->codActividad=codA;
	this->codCliente=codC;
	this->fecha=new char[strlen(f)+1];
	strcpy(this->fecha,f);
}

Reserva::~Reserva() {
	// TODO Auto-generated destructor stub
	delete[] this->fecha;
}


Reserva::Reserva(const Reserva &r){
    	this->cantPersonas=r.cantPersonas;
		this->codActividad=r.codActividad;
		this->codCliente=r.codCliente;
		this->fecha=new char[strlen(r.fecha)+1];
		strcpy(this->fecha,r.fecha);
}
