#include <ArgParser.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <museo/constantes.h>
#include <museo/Museo.h>
#include <museo/OpCierre.h>
#include <museo/OpPersona.h>
#include <museo/OpPuerta.h>
#include <set>
#include <sstream>
#include <system/IPCName.h>
#include <system/MessageQueue.h>
#include <system/Semaphore.h>
#include <system/SharedVariable.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	std::ostringstream oss;
	oss << "puerta " << args.id ();

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.applyConfig (args.logConf ());
	registry.application (oss.str ());

	Logger& logger = LoggerRegistry::getLogger ("main");

	logger << Level::DEBUG
			<< "Accediendo a mecanismos de IPC"
			<< Logger::endl;

	SharedVariable<Museo> svMuseo (
		IPCName (constantes::PATH_NAME, constantes::AREA_MUSEO),
		0666);
	svMuseo.persist ();

	Semaphore mutex (
		IPCName (constantes::PATH_NAME, constantes::SEM_MUTEX),
		1, 0666);
	mutex.persist ();

	MessageQueue<OpPuerta> mqPuertas (
		IPCName (constantes::PATH_NAME, constantes::MQ_PUERTAS),
		0666);
	mqPuertas.persist ();

	MessageQueue<OpPersona> mqPersonas (
		IPCName (constantes::PATH_NAME, constantes::MQ_PERSONAS),
		0666);
	mqPersonas.persist ();

	MessageQueue<OpCierre> mqCierre (
		IPCName (constantes::PATH_NAME, constantes::MQ_CIERRE),
		0666);
	mqCierre.persist ();

	Museo& museo = svMuseo.get ();
	std::set<long> personasAdentro;
	bool cerro = false;
	while (!cerro || !personasAdentro.empty ()) {
		OpPuerta op = mqPuertas.receive (args.id ());

		logger << Level::DEBUG
			<< "OpPuerta { "
			<< op.mtype << ", "
			<< op.causa << ", "
			<< op.persona << "}" << Logger::endl;

		switch (op.causa) {
		case PUERTA_CAUSA_ENTRADA:
		{
			logger << Level::INFO
				<< "Persona " << op.persona
				<< " solicita entrar al museo."
				<< Logger::endl;

			OpPersona pers;
			pers.mtype = op.persona;

			mutex.wait ();
			bool abierto = museo.abierto ();
			if (abierto) {
				bool entro = museo.entrar ();
				mutex.signal ();

				logger << "Museo abierto" << Logger::endl;

				if (entro) {
					personasAdentro.insert (op.persona);
					pers.causa = PERSONA_CAUSA_NOTIF_ENTRADA;
					logger << "La persona " << op.persona
						<< " entró al museo"
						<< Logger::endl;
				} else {
					pers.causa = PERSONA_CAUSA_NOTIF_LLENO;
					logger << "Museo lleno" << Logger::endl;
				}
			} else {
				mutex.signal ();
				pers.causa = PERSONA_CAUSA_NOTIF_CERRADO;

				logger << Level::INFO
					<< "Museo cerrado" << Logger::endl;
			}

			logger << Level::INFO
				<< "Notificando Persona: OpPersona {"
				<< pers.mtype << ", "
				<< pers.causa << "}" << Logger::endl;

			mqPersonas.send (pers);
			break;
		}
		case PUERTA_CAUSA_SALIDA:
		{
			logger << Level::INFO
				<< "Persona " << op.persona
				<< " solicita salir del museo."
				<< Logger::endl;

			mutex.wait ();
			museo.salir ();
			mutex.signal ();

			personasAdentro.erase (op.persona);

			logger << Level::INFO
				<< "Se retiró a la persona del museo. "
				<< "Enviando confirmación"
				<< Logger::endl;

			OpPersona pers;
			pers.mtype = op.persona;
			pers.causa = PERSONA_CAUSA_CONF_SALIDA;
			mqPersonas.send (pers);
			break;
		}
		case PUERTA_CAUSA_CIERRE:
			logger << Level::INFO
				<< "Se recibió la notificación de cierre."
				<< Logger::endl;

			logger << Level::INFO
				<< "Hay " << personasAdentro.size ()
				<< " personas en el museo que entraron por aquí"
				<< Logger::endl;

			cerro = true;
			for (std::set<long>::iterator it = personasAdentro.begin ();
					it != personasAdentro.end (); ++it) {
				OpCierre op;
				op.mtype = *it;
				mqCierre.send (op);

				logger << Level::DEBUG
					<< "Persona " << *it << " notificada"
					<< Logger::endl;
			}
			break;
		}
	}

	logger << Level::INFO
		<< "El museo cerró y todas las personas salieron. "
		<< "Finalizado"
		<< Logger::endl;

	return 0;
}
