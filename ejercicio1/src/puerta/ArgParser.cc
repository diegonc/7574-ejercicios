#include <argp.h>
#include <cstdlib>

#include <ArgParser.h>

int parserFunc (int key, char *arg, struct argp_state *state)
{
	(void) arg;

	ArgParser* argParser = static_cast<ArgParser*> (state->input);
	switch (key) {
		case 'd':
			argParser->_debug = true;
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
	{"debug", 'd', 0, 0, "Enable debug mode", 0},
	{0, 0, 0, 0, 0, 0}
};

static struct argp optionParser = {options, parserFunc, 0, 0, 0, 0, 0};

ArgParser::ArgParser () : _debug (false)
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
