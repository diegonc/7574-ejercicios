#ifndef CTRL_PERSONA_H
#define CTRL_PERSONA_H

#include <museo/OpCierre.h>
#include <museo/OpPuerta.h>
#include <museo/OpPersona.h>
#include <system/MessageQueue.h>
#include <utils/NonCopyable.h>

class CtrlPersona : private NonCopyable
{
	private:
		MessageQueue<OpPuerta> mqPuertas;
		MessageQueue<OpPersona> mqPersonas;
		MessageQueue<OpCierre> mqCierre;
		long puerta;
		long idPersona;

		OpPersona entrarAlMuseo ();
		void recorrerMuseo ();
		void aguardarCierre (long mtype);
		void salirDelMuseo ();

	public:
		CtrlPersona (long puerta, long idPersona);
		~CtrlPersona ();

		int run ();
};

#endif /* CTRL_PERSONA_H */
