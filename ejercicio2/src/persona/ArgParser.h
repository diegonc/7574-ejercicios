#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <argp.h>
#include <string>
#include <utils/NonCopyable.h>

class ArgParser : private NonCopyable
{
	private:
		std::string _logConf;
		long _puerta;

		ArgParser();
		~ArgParser();

		friend int parserFunc (int key, char *arg, struct argp_state *state);

	public:
		static ArgParser& getInstance();

		void parse (int argc, char **argv);

		const std::string& logConf () const { return _logConf; }
		long puerta () const { return _puerta; }
};

#endif
