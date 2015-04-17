#include <ArgParser.h>
#include <config/ConfigParser.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <logging/LoggerRegistry.h>
#include <museo/constantes.h>
#include <museo/Museo.h>
#include <signal.h>
#include <sstream>
#include <system/Semaphore.h>
#include <system/SharedVariable.h>
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
	registry.application ("lanzador");

	Logger& logger = LoggerRegistry::getLogger ("main");

	YAML::Node state;
	YAML::Node pids;
	YAML::Node shms;
	YAML::Node sems;
	try {
		logger << Level::INFO
				<< "Creando IPCs..."
				<< Logger::endl;

		SharedVariable<Museo> svMuseo (
			IPCName(constantes::PATH_NAME, constantes::AREA_MUSEO),
			0666 | IPC_CREAT | IPC_EXCL);
		Semaphore mutex(
			IPCName(constantes::PATH_NAME, constantes::SEM_MUTEX),
			1, 0666 | IPC_CREAT | IPC_EXCL);

		Museo& museo = svMuseo.get ();
		museo.abierto (false);
		museo.capacidad (config.capacidad ());
		museo.personas (0);
		mutex.initialize ();

		unsigned int seed = static_cast<unsigned int> (time (NULL));
		logger << Level::INFO
				<< "Inicializando generador de numeros al azar con semilla "
				<< seed
				<< Logger::endl;
		srand (seed);

		logger << Level::INFO
				<< "Lanzando los procesos puerta..."
				<< Logger::endl;

		for (int i=0; i < config.puertas (); i++) {
			std::vector<const char*> args;
			std::ostringstream oss;
			oss << (i + 1);

			std::string idPuerta = oss.str ();

			args.push_back ("puerta");
			args.push_back ("-l");
			args.push_back (config.logConf ().c_str ());
			args.push_back ("-i");
			args.push_back (idPuerta.c_str ());
			args.push_back (NULL);

			pid_t child = System::spawn (config.modPuerta ().c_str (), args);
			System::check (child);
			pids[i] = child;

			int lapse = rand () % 5 + 1;
			logger << Level::DEBUG
					<< "Esperando " << lapse
					<< " segundos hasta el lanzamiento de la próxima puerta"
					<< Logger::endl;
			sleep (lapse);
		}

		shms[0] = IPCName (constantes::PATH_NAME, constantes::AREA_MUSEO);
		sems[0] = IPCName (constantes::PATH_NAME, constantes::SEM_MUTEX);

		state["pids"] = pids;
		state["shms"] = shms;
		state["sems"] = sems;
		std::ofstream stateFile ("session.yml");
		stateFile << state;

		svMuseo.persist ();
		mutex.persist ();

		return 0;
	} catch (std::exception& e) {
		logger << Level::ERROR
				<< "Error lanzando procesos puerta: "
				<< e.what ()
				<< Logger::endl;
		logger << Level::INFO
				<< "Matando procesos lanzados hasta el momento..."
				<< Logger::endl;
		for (size_t i = 0; i < pids.size (); i++) {
			pid_t child = pids[i].as<pid_t> ();
			kill (child, SIGKILL);
		}
		return 1;
	}
}
