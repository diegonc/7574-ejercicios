#include <ArgParser.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <museo/constantes.h>
#include <museo/Museo.h>
#include <system/IPCName.h>
#include <system/Semaphore.h>
#include <system/SharedVariable.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.application ("puerta");
	registry.filename ("ejercicio1.log");
	registry.quiet (!args.debug ());

	SharedVariable<Museo> museo (
			IPCName(constantes::PATH_NAME, constantes::AREA_MUSEO),
			0666);
	Semaphore mutex(
			IPCName(constantes::PATH_NAME, constantes::SEM_MUTEX),
			1, 0666);

	return 0;
}
