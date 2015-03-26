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
	registry.application (oss.str ());
	registry.filename ("ejercicio1.log");
	registry.quiet (!args.debug ());

	SharedVariable<Museo> svMuseo (
			IPCName(constantes::PATH_NAME, constantes::AREA_MUSEO),
			0666);
	svMuseo.persist ();

	Semaphore mutex(
			IPCName(constantes::PATH_NAME, constantes::SEM_MUTEX),
			1, 0666);
	mutex.persist ();

	unsigned int seed = static_cast<unsigned int> (time (NULL));
	srand (seed);
	Museo& museo = svMuseo.get ();
	while (true) {
		mutex.wait ();
		if (!museo.abierto ()) {
			int cant = museo.personas ();
			while (cant > 0) {
				museo.sacar ();
				cant = museo.personas ();
			}
		} else {
			mutex.signal ();

			int entra = rand () % 2;
			if (entra) {
				mutex.wait ();
				int personas = museo.personas ();
				int cap = museo.capacidad ();
				if (personas < cap && museo.abierto ()) {
					museo.agregar ();
				}
				mutex.signal ();
			} else {
				mutex.wait ();
				if (museo.personas () > 0) {
					museo.sacar ();
				}
				mutex.signal ();
			}
		}
	}

	return 0;
}
