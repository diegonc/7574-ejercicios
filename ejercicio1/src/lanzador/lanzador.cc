#include <ArgParser.h>
#include <config/ConfigParser.h>
#include <fstream>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <sstream>
#include <system/System.h>
#include <vector>
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
	try {
		logger << Level::INFO
				<< "Lanzando los procesos puerta..."
				<< Logger::endl;

		for (int i=0; i < config.puertas (); i++) {
			std::vector<const char*> args;
			std::ostringstream oss;
			oss << (i + 1);

			std::string idPuerta = oss.str ();

			args.push_back ("puerta");
			args.push_back ("-d");
			args.push_back ("-i");
			args.push_back (idPuerta.c_str ());
			args.push_back (NULL);

			pid_t child = System::spawn (config.modPuerta ().c_str (), args);
			System::check (child);
			pids[i] = child;
		}

		state["pids"] = pids;
		std::ofstream stateFile ("session.yml");
		stateFile << state;
		return 0;
	} catch (SystemErrorException& e) {
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
