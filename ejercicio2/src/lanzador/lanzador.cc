#include <ArgParser.h>
#include <config/ConfigParser.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <logging/LoggerRegistry.h>
#include <museo/constantes.h>
#include <museo/Museo.h>
#include <museo/OpCierre.h>
#include <museo/OpPersona.h>
#include <museo/OpPuerta.h>
#include <set>
#include <signal.h>
#include <sstream>
#include <system/MessageQueue.h>
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
	YAML::Node puertas;
	YAML::Node shms;
	YAML::Node sems;
	YAML::Node msgq;
	std::set<pid_t> pids;
	try {
		logger << Level::INFO
				<< "Creando IPCs..."
				<< Logger::endl;

		SharedVariable<Museo> svMuseo (
			IPCName (constantes::PATH_NAME, constantes::AREA_MUSEO),
			0666 | IPC_CREAT | IPC_EXCL);
		Semaphore mutex(
			IPCName (constantes::PATH_NAME, constantes::SEM_MUTEX),
			1, 0666 | IPC_CREAT | IPC_EXCL);

		MessageQueue<OpCierre> mqCierre (
			IPCName (constantes::PATH_NAME, constantes::MQ_CIERRE),
			0666 | IPC_CREAT | IPC_EXCL);
		MessageQueue<OpPuerta> mqPuertas (
			IPCName (constantes::PATH_NAME, constantes::MQ_PUERTAS),
			0666 | IPC_CREAT | IPC_EXCL);
		MessageQueue<OpPersona> mqPersonas (
			IPCName (constantes::PATH_NAME, constantes::MQ_PERSONAS),
			0666 | IPC_CREAT | IPC_EXCL);

		logger << Level::INFO
			<< "Inicializando IPCs..."
			<< Logger::endl;

		Museo& museo = svMuseo.get ();
		museo.abierto (false);
		museo.capacidad (config.capacidad ());
		museo.personas (0);
		mutex.initialize ();
		mutex.signal ();

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
			puertas[i] = i + 1;
			pids.insert (child);
		}

		shms[0] = IPCName (constantes::PATH_NAME, constantes::AREA_MUSEO);
		sems[0] = IPCName (constantes::PATH_NAME, constantes::SEM_MUTEX);
		msgq[0] = IPCName (constantes::PATH_NAME, constantes::MQ_CIERRE);
		msgq[1] = IPCName (constantes::PATH_NAME, constantes::MQ_PUERTAS);
		msgq[2] = IPCName (constantes::PATH_NAME, constantes::MQ_PERSONAS);


		state["puertas"] = puertas;
		state["shms"] = shms;
		state["sems"] = sems;
		state["msgq"] = msgq;
		std::ofstream stateFile ("session.yml");
		stateFile << state;

		svMuseo.persist ();
		mutex.persist ();
		mqCierre.persist ();
		mqPuertas.persist ();
		mqPersonas.persist ();

		return 0;
	} catch (std::exception& e) {
		logger << Level::ERROR
				<< "Error lanzando procesos puerta: "
				<< e.what ()
				<< Logger::endl;
		logger << Level::INFO
				<< "Matando procesos lanzados hasta el momento..."
				<< Logger::endl;

		std::set<pid_t>::iterator it = pids.begin ();
		while (it != pids.end ()) {
			pid_t child = *it;
			kill (child, SIGKILL);
			++it;
		}
		return 1;
	}
}
