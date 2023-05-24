#ifndef INTERFAZ_CIUDAD_H_
#define INTERFAZ_CIUDAD_H_

class Ciudad
{
	public:
		int codigo;
		char* nombre;

		Ciudad();
		Ciudad(int codigo, char* nombre);
		Ciudad(const Ciudad &c);
		~Ciudad();

		void imprimir();
};

#endif
