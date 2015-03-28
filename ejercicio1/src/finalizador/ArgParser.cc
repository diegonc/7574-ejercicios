#include <argp.h>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

#include <ArgParser.h>

int parserFunc (int key, char *arg, struct argp_state *state)
{
	ArgParser* argParser = static_cast<ArgParser*> (state->input);
	switch (key) {
		case 'l':
			argParser->_logConf = arg;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				argParser->_session = arg;
			}
			break;
		case ARGP_KEY_END:
			if (state->arg_num > 1) {
				argp_failure (state, 1, 0, "too many arguments");
			}
			break;
	}
	return 0;
}

static struct argp_option options[] = {
	{"logconf", 'l', "LOG_CONF", 0, "Use LOG_CONF as the logging configuration file", 0},
	{0, 0, 0, 0, 0, 0}
};

static struct argp optionParser = {options, parserFunc, "[SESSION_FILE]", 0, 0, 0, 0};

ArgParser::ArgParser ()
	: _session ("session.yml")
	, _logConf ("logging.yml")
{
}

ArgParser::~ArgParser ()
{
}

ArgParser& ArgParser::getInstance ()
{
	static ArgParser parser;
	return parser;
}

void ArgParser::parse (int argc, char** argv)
{
	argp_parse (&optionParser, argc, argv, 0, 0, this);
}
