#ifndef CONFIGPARSER_H
#define	CONFIGPARSER_H

#include <string>
#include <utils/NonCopyable.h>

class ConfigParser : private NonCopyable
{
	private:
		std::string _modPuerta;
		std::string _logConf;
		int _puertas;
		int _capacidad;

		ConfigParser ();
		~ConfigParser ();

	public:
		static ConfigParser& getInstance ();

		void parse (const std::string& file);

		const std::string& modPuerta () const { return _modPuerta; }
		const std::string& logConf () const { return _logConf; }
		int puertas () const { return _puertas; }
		int capacidad () const { return _capacidad; }
};

#endif	/* CONFIGPARSER_H */
