/*
 * reserva.h
 *
 *  Created on: 22 may 2023
 *      Author: borja
 */

#ifndef RESERVA_H_
#define RESERVA_H_

class Reserva {
public:
	int codCliente;
	int codActividad;
	char *fecha;
	int cantPersonas;

	Reserva();
	Reserva(int codC,int codA,char* f,int cantP);
	virtual ~Reserva();
	 Reserva(const Reserva &r);
};

#endif /* RESERVA_H_ */
