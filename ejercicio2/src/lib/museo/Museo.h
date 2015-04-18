#ifndef MUSEO_H
#define	MUSEO_H

#include <cassert>

struct Museo
{
	bool _abierto;
	int _personas;
	int _capacidad;

	bool abierto () const { return _abierto; }
	void abierto (bool a) { _abierto = a; }

	bool entrar () {
		if (_personas < _capacidad) {
			_personas++;
			return true;
		}
		return false;
	}

	void salir () {
		assert (_personas > 0);
		_personas--;
	}

	int personas () const { return _personas; }
	void personas (int p) { _personas = p; }

	int capacidad () const { return _capacidad; }
	void capacidad (int c) { _capacidad = c; }
};

#endif	/* MUSEO_H */
