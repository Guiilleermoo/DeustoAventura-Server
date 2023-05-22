#ifndef ACTIVIDAD_H_
#define ACTIVIDAD_H_

class Actividad
{
	public:
		char* nombre;
		char* dificultad;
		int limitePerMin;
		int limitePerMax;
		int edadMin;

		Actividad();
		Actividad(char* nombre, char* dificultad, int limitePerMin, int limitePerMax, int edadMin);
		Actividad(const Actividad &a);
		~Actividad();

		void imprimir();
};

#endif
