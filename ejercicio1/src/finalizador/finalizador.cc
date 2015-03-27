#include <ArgParser.h>
#include <logging/LoggerRegistry.h>
#include <signal.h>
#include <system/System.h>
#include <utils/yaml-converters.h>
#include <yaml-cpp/yaml.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.applyConfig (args.logConf ());
	registry.application ("finalizador");

	Logger& logger = LoggerRegistry::getLogger ("main");

	logger << Level::INFO
			<< "Obteniendo datos de la sesión desde "
			<< '\'' << args.session () << '\''
			<< Logger::endl;

	YAML::Node session = YAML::LoadFile (args.session ());

	YAML::Node pids = session["pids"];
	if (pids) {
		logger << Level::INFO
				<< "Finalizando procesos..."
				<< Logger::endl;

		for (size_t i = 0; i < pids.size (); i++) {
			pid_t pid = pids[i].as<pid_t> ();
			kill (pid, SIGKILL);
			logger << Level::INFO
					<< "Enviada señal SIGKILL al proceso " << pid
					<< Logger::endl;
		}
	}

	YAML::Node shms = session["shms"];
	if (shms) {
		logger << Level::INFO
				<< "Eliminando segmentos de memoria compartida..."
				<< Logger::endl;

		for (size_t i = 0; i < shms.size (); i++) {
			IPCName name = shms[i].as<IPCName> ();
			try {
				System::shmrm (name);
			} catch (SystemErrorException& e) {
				logger << Level::ERROR
						<< "Error al eliminar memoria compartida "
						<< '[' << name.path << ':' << name.index << ']'
						<< ". " << e.what ()
						<< Logger::endl;
			}
		}
	}

	YAML::Node sems = session["sems"];
	if (sems) {
		logger << Level::INFO
				<< "Eliminando semáforos..."
				<< Logger::endl;

		for (size_t i = 0; i < sems.size (); i++) {
			IPCName name = sems[i].as<IPCName> ();
			try {
				System::semrm (name);
			} catch (SystemErrorException& e) {
				logger << Level::ERROR
						<< "Error al eliminar semáforo "
						<< '[' << name.path << ':' << name.index << ']'
						<< ". " << e.what ()
						<< Logger::endl;
			}
		}
	}

	return 0;
}
