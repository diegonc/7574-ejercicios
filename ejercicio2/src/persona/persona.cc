#include <ArgParser.h>
#include <CtrlPersona.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <sstream>

int main (int argc, char **argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	pid_t idPersona = getpid ();
	std::ostringstream oss;
	oss << "persona " << idPersona;

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.applyConfig (args.logConf ());
	registry.application (oss.str ());

	Logger& logger = LoggerRegistry::getLogger ("main");

	logger << Level::DEBUG
		<< "Creando CtrlPersona"
		<< Logger::endl;

	CtrlPersona ctrl (args.puerta (), idPersona);

	logger << Level::INFO
		<< "Iniciando Proceso de Persona"
		<< Logger::endl;

	return ctrl.run ();
}
