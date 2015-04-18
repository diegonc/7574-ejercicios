#include <CtrlPersona.h>
#include <cstdlib>
#include <ctime>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <museo/constantes.h>
#include <museo/OpCierre.h>
#include <museo/OpPuerta.h>
#include <museo/OpPersona.h>
#include <signal.h>
#include <sstream>
#include <sys/types.h>
#include <system/MessageQueue.h>
#include <system/System.h>
#include <unistd.h>

CtrlPersona::CtrlPersona (long puerta, long idPersona)
	: mqPuertas (
		IPCName (constantes::PATH_NAME, constantes::MQ_PUERTAS),
		0666)
	, mqPersonas (
		IPCName (constantes::PATH_NAME, constantes::MQ_PERSONAS),
		0666)
	, mqCierre (
		IPCName (constantes::PATH_NAME, constantes::MQ_CIERRE),
		0666)
	, puerta (puerta)
	, idPersona (idPersona)
{
	mqPuertas.persist ();
	mqPersonas.persist ();
	mqCierre.persist ();
}

CtrlPersona::~CtrlPersona ()
{
}

int CtrlPersona::run ()
{
	Logger& logger = LoggerRegistry::getLogger ("CtrlPersona::run");

	logger << Level::INFO
		<< "iniciando recorrida del museo por puerta "
		<< puerta << Logger::endl;

	OpPersona resp = entrarAlMuseo ();

	if (resp.causa == PERSONA_CAUSA_NOTIF_LLENO) {
		logger << Level::INFO
			<< "el museo esta lleno. Finalizado"
			<< Logger::endl;
		return 0;
	}
	if (resp.causa == PERSONA_CAUSA_NOTIF_CERRADO) {
		logger << Level::INFO
			<< "el museo esta cerrado. Finalizado"
			<< Logger::endl;
		return 0;
	}

	logger << Level::INFO
		<< "la puerta nos dejo pasar" << Logger::endl;

	int returnValue = 0;
	pid_t child = fork ();
	if (child == -1) {
		SystemErrorException e;
		logger << Level::ERROR
			<< "Error en fork: " << e.what ()
			<< Logger::endl;
		returnValue = 1;
	} else if (child != 0) {
		/* proceso que realiza el recorrido del museo */
		recorrerMuseo ();
		kill (child, SIGKILL);
	} else {
		/* proceso que espera el cierre del museo */
		pid_t parent = getppid ();

		/* se actualiza la descripción de la aplicación para
		 * distinguir los mensajes generados por este proceso.
		 */
		LoggerRegistry& registry = LoggerRegistry::getInstance ();
		std::ostringstream app;
		app << registry.application () << " [reaper]";
		registry.application (app.str ());

		aguardarCierre (parent);
		kill (parent, SIGKILL);
	}

	salirDelMuseo ();
	return returnValue;
}

OpPersona CtrlPersona::entrarAlMuseo ()
{
	Logger& logger = LoggerRegistry::getLogger ("CtrlPersona::entrarAlMuseo");
	logger << Level::INFO
		<< "solicitando entrada al museo" << Logger::endl;

	OpPuerta entrada;
	entrada.mtype = puerta;
	entrada.causa = PUERTA_CAUSA_ENTRADA;
	entrada.persona = idPersona;

	mqPuertas.send (entrada);

	logger << Level::INFO
		<< "aguardando respuesta de puerta" << Logger::endl;

	return mqPersonas.receive (idPersona);
}

void CtrlPersona::recorrerMuseo ()
{
	Logger& logger = LoggerRegistry::getLogger ("CtrlPersona::recorrerMuseo");

	srand (static_cast<unsigned> (time (NULL)));
	int tiempo = rand () % 10 + 1;

	logger << Level::INFO
		<< "recorriendo el museo por "
		<< tiempo << " segundos" << Logger::endl;

	sleep (tiempo);
}

void CtrlPersona::aguardarCierre (long mtype)
{
	Logger& logger = LoggerRegistry::getLogger ("CtrlPersona::aguardarCierre");

	logger << Level::DEBUG
		<< "Aguardando mensaje de cierre"
		<< Logger::endl;

	mqCierre.receive (mtype);
}

void CtrlPersona::salirDelMuseo ()
{
	Logger& logger = LoggerRegistry::getLogger ("CtrlPersona::salirDelMuseo");

	logger << Level::INFO
		<< "solicitando salida del museo" << Logger::endl;

	OpPuerta salida;
	salida.mtype = puerta;
	salida.causa = PUERTA_CAUSA_SALIDA;
	salida.persona = idPersona;

	mqPuertas.send (salida);

	logger << Level::INFO
		<< "aguardando respuesta" << Logger::endl;

	OpPersona resp = mqPersonas.receive (idPersona);
	if (resp.causa != PERSONA_CAUSA_CONF_SALIDA) {
		logger << Level::ERROR
			<< "la puerta respondió con una causa erronea: "
			<< resp.causa << Logger::endl;
	}
}
