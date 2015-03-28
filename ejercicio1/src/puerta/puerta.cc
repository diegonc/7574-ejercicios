#include <ArgParser.h>
#include <cstdlib>
#include <ctime>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <museo/constantes.h>
#include <museo/Museo.h>
#include <sstream>
#include <system/IPCName.h>
#include <system/Semaphore.h>
#include <system/SharedVariable.h>

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	std::ostringstream oss;
	oss << "puerta " << args.id ();

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.applyConfig (args.logConf ());
	registry.application (oss.str ());

	Logger& logger = LoggerRegistry::getLogger ("main");

	logger << Level::DEBUG
			<< "Accediendo a mecanismos de IPC"
			<< Logger::endl;

	SharedVariable<Museo> svMuseo (
			IPCName(constantes::PATH_NAME, constantes::AREA_MUSEO),
			0666);
	svMuseo.persist ();

	Semaphore mutex(
			IPCName(constantes::PATH_NAME, constantes::SEM_MUTEX),
			1, 0666);
	mutex.persist ();

	unsigned int seed = static_cast<unsigned int> (time (NULL));
	logger << Level::INFO
			<< "Inicializando generador de números al azar con semilla: "
			<< seed
			<< Logger::endl;
	srand (seed);
	Museo& museo = svMuseo.get ();
	while (true) {
		mutex.wait ();
		if (!museo.abierto ()) {
			logger << Level::INFO
					<< "El museo cerró. Sacando personas..."
					<< Logger::endl;
			int cant = museo.personas ();
			while (cant > 0) {
				museo.sacar ();
				cant = museo.personas ();
			}
		} else {
			mutex.signal ();

			int entra = rand () % 2;
			if (entra) {
				logger << Level::INFO
						<< "Haciendo entrar una persona..."
						<< Logger::endl;

				mutex.wait ();
				int personas = museo.personas ();
				int cap = museo.capacidad ();
				if (personas < cap && museo.abierto ()) {
					museo.agregar ();
				}
				mutex.signal ();
			} else {
				logger << Level::INFO
						<< "Haciendo salir una persona..."
						<< Logger::endl;

				mutex.wait ();
				if (museo.personas () > 0) {
					museo.sacar ();
				}
				mutex.signal ();
			}

			int tprox = rand () % 10 + 1;
			logger << Level::INFO
					<< "Esperando " << tprox << " segundos antes del próximo"
					<< " movimiento..."
					<< Logger::endl;
			sleep (tprox);
		}
	}

	return 0;
}
