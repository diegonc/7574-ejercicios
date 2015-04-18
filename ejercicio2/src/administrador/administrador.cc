#include <ArgParser.h>
#include <logging/LoggerRegistry.h>
#include <museo/constantes.h>
#include <museo/Museo.h>
#include <museo/OpPuerta.h>
#include <set>
#include <stdexcept>
#include <system/MessageQueue.h>
#include <system/Semaphore.h>
#include <system/SharedVariable.h>
#include <system/System.h>
#include <yaml-cpp/yaml.h>

void notificarCierre (
			MessageQueue<OpPuerta>& mqPuertas,
			const std::set<long> puertas)
{
	Logger& logger = LoggerRegistry::getLogger ("notificarCierre");

	logger << Level::INFO
		<< "Notificando el cierre a todas las puertas"
		<< Logger::endl;

	OpPuerta op;
	op.causa = PUERTA_CAUSA_CIERRE;
	op.persona = 0;
	for (std::set<long>::const_iterator it = puertas.begin ();
				it != puertas.end (); ++it) {
		logger << Level::DEBUG
			<< "Notificando a puerta " << *it << Logger::endl;

		op.mtype = *it;
		mqPuertas.send (op);
	}
}

std::set<long> cargarPuertasEnSesion (const std::string& sesion)
{
	YAML::Node root = YAML::LoadFile (sesion);
	YAML::Node puertas = root["puertas"];

	if (!puertas)
		throw std::runtime_error (
			"El archivo de sesión no tiene la sección"
			" requerida 'puertas'");

	std::set<long> idPuertas;
	YAML::const_iterator it = puertas.begin ();
	while (it != puertas.end ()) {
		long id = it->as<long> ();
		idPuertas.insert (id);
		++it;
	}
	return idPuertas;
}

int main (int argc, char** argv)
{
	ArgParser& args = ArgParser::getInstance ();
	args.parse (argc, argv);

	LoggerRegistry& registry = LoggerRegistry::getInstance ();
	registry.applyConfig (args.logConf ());
	registry.application ("administrador");

	Logger& logger = LoggerRegistry::getLogger ("main");
	
	logger << Level::INFO
			<< "La operación a realizar es "
			<< '\'' << args.operacion () << '\''
			<< Logger::endl;

	logger << Level::DEBUG
			<< "Accediendo a IPCs..."
			<< Logger::endl;

	SharedVariable<Museo> svMuseo (
		IPCName(constantes::PATH_NAME, constantes::AREA_MUSEO),
		0666);
	svMuseo.persist ();

	Semaphore mutex(
		IPCName(constantes::PATH_NAME, constantes::SEM_MUTEX),
		1, 0666);
	mutex.persist ();

	if (args.operacion () == "abrir") {
		mutex.wait ();
		svMuseo.get ().abierto (true);
		mutex.signal ();
	} else if (args.operacion () == "cerrar") {
		MessageQueue<OpPuerta> mqPuertas (
			IPCName (constantes::PATH_NAME, constantes::MQ_PUERTAS),
			0666);
		mqPuertas.persist ();

		std::set<long> puertas = cargarPuertasEnSesion (args.sesion ());

		mutex.wait ();
		svMuseo.get ().abierto (false);
		mutex.signal ();

		notificarCierre (mqPuertas, puertas);
	} else {
		return 1;
	}

	return 0;
}
