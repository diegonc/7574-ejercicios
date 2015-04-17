#ifndef LOGGING_LOGGERREGISTRY_H
#define LOGGING_LOGGERREGISTRY_H

#include <logging/Logger.h>
#include <map>
#include <string>
#include <utils/NonCopyable.h>

class LoggerRegistry : private NonCopyable
{
	std::map<std::string, Logger*> registry;
	std::map<std::string, LevelId> levels;
	std::string _filename;
	std::string _application;
	LevelId _defaultLevel;

	LoggerRegistry ();
	~LoggerRegistry ();

	LevelId getLoggerLevel (const std::string& name);

	public:
		static LoggerRegistry& getInstance ();
		static Logger& getLogger (const std::string& name);

		void applyConfig (const std::string& path);

		void filename (const std::string& theFileName)
		{
			_filename = theFileName;
		}
		const std::string& filename () const
		{
			return _filename;
		}

		void application (const std::string& theApplication)
		{
			_application = theApplication;
		}

		const std::string& application () const
		{
			return _application;
		}

		void defaultLevel (LevelId level)
		{
			_defaultLevel = level;
		}

		LevelId defaultLevel () const
		{
			return _defaultLevel;
		}
};

#endif // LOGGING_LOGGERREGISTRY_H
