#include <argp.h>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

#include <ArgParser.h>

int parserFunc (int key, char *arg, struct argp_state *state)
{
	(void) arg;

	ArgParser* argParser = static_cast<ArgParser*> (state->input);
	switch (key) {
		case 'l':
			argParser->_logConf = arg;
			break;
		case 'i':
		{
			unsigned long id = strtoul(arg, NULL, 10);
			if (id == 0) {
				argp_failure (state, 1, 0, "the ID_PUERTA option value must be a number greater than 0");
			} else {
				argParser->_id = static_cast<int> (id);
			}
			break;
		}
		case ARGP_KEY_END:
			if (state->arg_num > 0) {
				argp_failure (state, 1, 0, "too many arguments");
			}
			break;
	}
	return 0;
}

static struct argp_option options[] = {
	{"logconf", 'l', "LOG_CONF", 0, "Use LOG_CONF as the logging configuration file", 0},
	{"id", 'i', "ID_PUERTA", 0, "Give an id to this door", 0},
	{0, 0, 0, 0, 0, 0}
};

static struct argp optionParser = {options, parserFunc, 0, 0, 0, 0, 0};

ArgParser::ArgParser ()
	: _logConf ("logging.yml")
	, _id (getpid ())
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
