#ifndef MUSEO_H
#define	MUSEO_H

struct Museo
{
	bool _abierto;
	int _personas;
	int _capacidad;

	bool abierto () const { return _abierto; }
	void abierto (bool a) { _abierto = a; }

	int personas () const { return _personas; }
	void personas (int p) { _personas = p; }
	void agregar () { _personas++; }
	void sacar () { _personas--; }

	int capacidad () const { return _capacidad; }
	void capacidad (int c) { _capacidad = c; }
};

#endif	/* MUSEO_H */
