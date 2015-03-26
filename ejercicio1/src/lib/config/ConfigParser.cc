#include <config/ConfigParser.h>
#include <yaml-cpp/yaml.h>

ConfigParser::ConfigParser ()
	: _modPuerta ("puerta")
	, _logConf ("logging.yml")
	, _puertas (1)
{
}

ConfigParser::~ConfigParser ()
{
}

ConfigParser& ConfigParser::getInstance ()
{
	static ConfigParser instance;
	return instance;
}

void ConfigParser::parse (const std::string& file)
{
	YAML::Node config = YAML::LoadFile (file);

	YAML::Node mods = config["modulos"];
	if (mods) {
		if (mods["puerta"])
			_modPuerta = mods["puerta"].as<std::string> ();
	}

	YAML::Node logging = config["logging"];
	if (logging) {
		_logConf = logging["config-file"].as<std::string> ();
	}

	_puertas = config["museo"]["puertas"].as<int> ();
}
