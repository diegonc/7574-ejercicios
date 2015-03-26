#include <logging/LoggerRegistry.h>
#include <sstream>
#include <stdexcept>
#include <yaml-cpp/yaml.h>

LoggerRegistry::LoggerRegistry ()
	: _quiet (true)
	, _defaultLevel (LEVEL_INFO)
{
}

LoggerRegistry::~LoggerRegistry ()
{
	std::map<std::string, Logger*>::iterator it = registry.begin ();
	while (it != registry.end ()) {
		delete it->second;
		it++;
	}
}

LoggerRegistry& LoggerRegistry::getInstance ()
{
	static LoggerRegistry instance;
	return instance;
}

void LoggerRegistry::applyConfig (const std::string& path)
{
	YAML::Node config = YAML::LoadFile (path);

	_filename = config["log-file"].as<std::string> ();
	_defaultLevel = config["default"].as<LevelId> ();

	YAML::Node loggers = config["loggers"];
	if (loggers) {
		YAML::const_iterator it = loggers.begin ();
		while (it != loggers.end ()) {
			std::string logger = it->first.as<std::string> ();
			std::string levelName = it->second.as<std::string> ();
			LevelId levelId = level_id (levelName);
			levels[logger] = levelId;
			it++;
		}
	}
}

LevelId LoggerRegistry::getLoggerLevel (const std::string& name)
{
	std::map<std::string, LevelId>::iterator it
		= levels.find (name);
	if (it != levels.end ())
		return it->second;
	else
		return _defaultLevel;
}

Logger& LoggerRegistry::getLogger (const std::string& name)
{
	LoggerRegistry& registry = LoggerRegistry::getInstance ();

	if (registry._filename.size () == 0) {
		throw std::logic_error ("LoggerRegistry is not initialized: missing filename parameter"); 
	}

	if (registry._application.size () == 0) {
		throw std::logic_error ("LoggerRegistry is not initialized: missing application parameter"); 
	}

	std::ostringstream loggerName;
	loggerName << registry._application << " - " << name;

	try {
		return *registry.registry.at (loggerName.str ());
	} catch (std::out_of_range& e) {
		LevelId loggerLevel = registry.getLoggerLevel (name);
		Logger* logger = new Logger (
			  registry._filename
			, loggerName.str ()
			, registry._quiet
			, loggerLevel);
		registry.registry[loggerName.str ()] = logger;
		return *logger;
	}
}
