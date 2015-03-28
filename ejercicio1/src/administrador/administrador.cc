#include <ArgParser.h>
#include <logging/LoggerRegistry.h>
#include <museo/constantes.h>
#include <museo/Museo.h>
#include <system/Semaphore.h>
#include <system/SharedVariable.h>
#include <system/System.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.applyConfig (args.logConf ());
	registry.application ("administrador");

	Logger& logger = LoggerRegistry::getLogger ("main");
	
	logger << Level::INFO
			<< "La operación a realizar es "
			<< '\'' << args.operacion () << '\''
			<< Logger::endl;

	logger << Level::DEBUG
			<< "Accediendo a IPCs..."
			<< Logger::endl;

	SharedVariable<Museo> svMuseo (
		IPCName(constantes::PATH_NAME, constantes::AREA_MUSEO),
		0666);
	svMuseo.persist ();

	Semaphore mutex(
		IPCName(constantes::PATH_NAME, constantes::SEM_MUTEX),
		1, 0666);
	mutex.persist ();
	
	if (args.operacion () == "abrir") {
		svMuseo.get ().abierto (true);
		mutex.signal ();
	} else if (args.operacion () == "cerrar") {
		mutex.wait ();
		svMuseo.get ().abierto (false);
		mutex.signal ();
	} else {
		return 1;
	}

	return 0;
}
