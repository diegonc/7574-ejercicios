#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <boost/algorithm/string/trim.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utils/NonCopyable.h>
#include <yaml-cpp/yaml.h>

enum LevelId
{
	LEVEL_ERROR = 0,
	LEVEL_WARN  = 1,
	LEVEL_INFO  = 2,
	LEVEL_DEBUG = 3,
	LEVEL_TRACE = 4,

	LEVEL_UNSET = 9,
	LEVEL_DEFLT = LEVEL_INFO
};

const char* level_name(LevelId level);
LevelId level_id(const std::string& name);

class Logger : private NonCopyable
{
	int fd;
	std::string filename;
	std::string module;
	// indica el nivel a partir del que el Logger genera mensajes
	// de log.
	LevelId nivelLogger;

	// buffer donde se recolectan los resultados intermedios del
	// operador << hasta que recibe el manipulador endl y se escribe
	// a disco.
	std::ostringstream buffer;
	// indica si se grabó el nombre del módulo en el buffer.
	bool printedModule;
	// indica el nivel del mensaje de log que se esta construyendo.
	LevelId nivelMensaje;

	// abre el archivo de log
	void open ();

	// bloquea el archivo de log para escribir en el en forma
	// atómica
	void lock () const;
	// desbloquea el archivo para que otros procesos puedan
	// escribir en el log
	void unlock () const;

	// reinicializa el estado interno para hacer que el Logger
	// este listo para generar un nuevo mensaje
	void reset ();

	// clase que implementa el patron RAII para bloquear
	// y desbloquear un Logger.
	friend class Locker;

	// clase que provee manipuladores para cada nivel de log.
	friend class Level;

	public:
		Logger (const std::string& filename, const std::string& module);
		Logger (const std::string& filename, const std::string& module, LevelId nivel);
		~Logger ();

		// operador << para poder escribir en el log
		template<typename T>
		friend Logger& operator<< (Logger& logger, const T& o);

		// soporte para manipuladores
		friend Logger& operator<< (Logger& logger, Logger& (*pf)(Logger&));
		// manipulador Logger::endl, similar a std::endl, permite
		// concluir una linea de log y grabar en el archivo el
		// contenido del buffer en forma atómica
		static Logger& endl (Logger& logger);
};

class Level
{
	public:
		static Logger& TRACE (Logger& logger);
		static Logger& DEBUG (Logger& logger);
		static Logger& INFO  (Logger& logger);
		static Logger& WARN  (Logger& logger);
		static Logger& ERROR (Logger& logger);
};

template<typename T>
Logger& operator<< (Logger& logger, const T& o)
{
	if (logger.nivelMensaje == LEVEL_UNSET) {
		logger.nivelMensaje = LEVEL_DEFLT;
	}

	if (logger.nivelMensaje > logger.nivelLogger) {
		return logger;
	}

	if (!logger.printedModule) {
		logger.buffer << level_name(logger.nivelMensaje) << ' ';
		logger.buffer << "[" << logger.module << "] ";
		logger.printedModule = true;
	}
	logger.buffer << o;
	return logger;
}

#endif // LOGGIGNG_LOGGER_H
