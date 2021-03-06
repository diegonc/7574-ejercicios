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
		case 'c':
			argParser->_config = arg;
			break;
		case ARGP_KEY_END:
			if (state->arg_num > 0) {
				argp_failure (state, 1, 0, "too many arguments");
			}
			break;
	}
	return 0;
}

static struct argp_option options[] = {
	{"config", 'c', "CONFIG_FILE", 0, "Use CONFIG_FILE as the configuration file", 0},
	{0, 0, 0, 0, 0, 0}
};

static struct argp optionParser = {options, parserFunc, 0, 0, 0, 0, 0};

ArgParser::ArgParser () : _config ("config.yml")
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
