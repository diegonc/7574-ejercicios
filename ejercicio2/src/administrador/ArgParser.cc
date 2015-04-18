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
		case 's':
			argParser->_sesion = arg;
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num == 0) {
				std::string op = arg;
				if (op != "abrir" && op != "cerrar") {
					argp_failure (state, 1, 0,
						"invalid value '%s' for argument OPERACION", arg);
				}
				argParser->_operacion = op;
			}
			break;
		case ARGP_KEY_END:
			if (state->arg_num > 1) {
				argp_failure (state, 1, 0, "too many arguments");
			}
			if (state->arg_num == 0) {
				argp_failure (state, 1, 0, "too few arguments");
			}
			break;
	}
	return 0;
}

static struct argp_option options[] = {
	{"logconf", 'l', "LOG_CONF", 0, "Use LOG_CONF as the logging configuration file", 0},
	{"sesion", 's', "SESION", 0, "Use SESION as the session configuration file", 0},
	{0, 0, 0, 0, 0, 0}
};

static const char* doc =
	""
	"\v"
	"The argument OPERACION only accepts the values 'abrir' or 'cerrar'.";

static struct argp optionParser = {options, parserFunc, "OPERACION", doc, 0, 0, 0};

ArgParser::ArgParser ()
	: _logConf ("logging.yml")
	, _sesion ("session.yml")
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
