#include <ArgParser.h>
#include <config/ConfigParser.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <logging/LoggerRegistry.h>
#include <sstream>
#include <system/System.h>
#include <vector>
#include <utils/yaml-converters.h>
#include <yaml-cpp/yaml.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	ConfigParser& config = ConfigParser::getInstance ();
	config.parse (args.config ());

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.applyConfig (config.logConf ());
	registry.application ("mkpersonas");

	Logger& logger = LoggerRegistry::getLogger ("main");

	logger << Level::INFO
		<< "Cargando archivo de sesión: "
		<< args.session () << Logger::endl;

	YAML::Node session = YAML::LoadFile (args.session ());
	YAML::Node puertas = session["puertas"];

	logger << Level::INFO
		<< "Lanzando persona..." << Logger::endl;

	std::string numPuerta = puertas[0].as<std::string> ();

	std::vector<const char*> params;
	params.push_back (config.modPuerta ().c_str ());
	params.push_back (numPuerta.c_str ());
	params.push_back (NULL);

	System::spawn (config.modPuerta ().c_str (), params);

	return 0;
}
