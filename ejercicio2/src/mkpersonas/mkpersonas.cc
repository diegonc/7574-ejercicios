#include <algorithm>
#include <ArgParser.h>
#include <cassert>
#include <config/ConfigParser.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <limits>
#include <logging/LoggerRegistry.h>
#include <sstream>
#include <system/System.h>
#include <vector>
#include <utils/yaml-converters.h>
#include <yaml-cpp/yaml.h>

void millisleep (unsigned long millis)
{
	struct timespec per;
	per.tv_sec = millis / 1000;
	per.tv_nsec = (millis % 1000) * 1000000;
	nanosleep (&per, NULL);
}

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
		<< "Generando " << args.personas ()
		<< " personas" << Logger::endl;
	logger << Level::INFO
		<< "Esperando hasta " << args.maxWaitBatch ()
		<< " milisegundos entre batches"
		<< Logger::endl;
	logger << Level::INFO
		<< "Esperando hasta " << args.maxWaitPerson ()
		<< " milisegundos entre personas"
		<< Logger::endl;
	logger << Level::INFO
		<< "Generando batches de hasta " << args.maxBatchSize ()
		<< " personas" << Logger::endl;

	unsigned int seed = static_cast<unsigned int> (time (NULL));
	logger << Level::DEBUG
		<< "Inicializando generador de números al azar con semilla: "
		<< seed << Logger::endl;
	srand (seed);

	unsigned long personas = args.personas ();
	do {
		unsigned long batch = 1 + rand () % args.maxBatchSize ();
		batch = std::min (personas, batch);

		logger << Level::INFO
			<< "Iniciando batch de " << batch
			<< " personas" << Logger::endl;

		while (batch > 0) {
			assert (puertas.size () <= static_cast<unsigned int> (std::numeric_limits<int>::max ()));
			int idxPuerta = rand () % static_cast<int> (puertas.size ());
			std::string numPuerta = puertas[idxPuerta].as<std::string> ();

			logger << Level::INFO
				<< "Lanzando persona por puerta "
				<< numPuerta << Logger::endl;

			std::vector<const char*> params;
			params.push_back (config.modPersona ().c_str ());
			params.push_back (numPuerta.c_str ());
			params.push_back (NULL);

			System::spawn (config.modPersona ().c_str (), params);

			if (batch > 1) {
				unsigned long millis = 100 + rand () % args.maxWaitPerson ();
				logger << Level::INFO
					<< "Esperando " << millis
					<< " milisegundos hasta próximo lanzamiento"
					<< Logger::endl;
				millisleep (millis);
			}

			batch--;
			personas--;
		}

		if (personas > 0) {
			unsigned long millis = 100 + rand () % args.maxWaitBatch ();
			logger << Level::INFO
				<< "Esperando " << millis
				<< " milisegundos hasta próximo batch"
				<< Logger::endl;
			millisleep (millis);
		}
	} while (personas > 0);

	return 0;
}
