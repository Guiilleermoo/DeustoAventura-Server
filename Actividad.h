#ifndef ACTIVIDAD_H_
#define ACTIVIDAD_H_

class Actividad
{
	public:
		int codigo;
		char* nombre;
		char* dificultad;
		int limitePerMin;
		int limitePerMax;
		int edadMin;

		Actividad();
		Actividad(int codigo, char* nombre, char* dificultad, int limitePerMin, int limitePerMax, int edadMin);
		Actividad(const Actividad &a);
		~Actividad();

		void imprimir();
};

#endif
