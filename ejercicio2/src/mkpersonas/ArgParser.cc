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
		case 's':
			argParser->_session = arg;
			break;
		case 'p':
		{
			unsigned long personas = strtoul (arg, NULL, 10);
			if (personas == 0) {
				argp_failure (state, 1, 0, "the PERSONS_COUNT argument must be a number greater than 0");
			} else {
				argParser->_personas = personas;
			}
			break;
		}
		case 1000:
		{
			unsigned long maxWait = strtoul (arg, NULL, 10);
			if (maxWait == 0) {
				argp_failure (state, 1, 0, "the WAIT_MSEC argument must be a number greater than 0");
			} else {
				argParser->_maxWaitBatch = maxWait;
			}
			break;
		}
		case 1001:
		{
			unsigned long maxWait = strtoul (arg, NULL, 10);
			if (maxWait == 0) {
				argp_failure (state, 1, 0, "the WAIT_MSEC argument must be a number greater than 0");
			} else {
				argParser->_maxWaitPerson = maxWait;
			}
			break;
		}
		case 'b':
		{
			unsigned long maxBatchSize = strtoul (arg, NULL, 10);
			if (maxBatchSize == 0) {
				argp_failure (state, 1, 0, "the BATCH_SIZE argument must be a number greater than 0");
			} else {
				argParser->_maxBatchSize = maxBatchSize;
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
	{"config", 'c', "CONFIG_FILE", 0, "Use CONFIG_FILE as the configuration file", 0},
	{"session", 's', "SESSION_FILE", 0, "Use SESSION_FILE as the session file", 0},
	{"persons", 'p', "PERSONS_COUNT", 0, "Generate up to PERSONS_COUNT persons", 0},
	{"max-wait-batch", 1000, "WAIT_MSEC", 0, "Wait WAIT_MSEC milli-seconds between batchs", 0},
	{"max-wait-person", 1001, "WAIT_MSEC", 0, "Wait WAIT_MSEC milli-seconds between batchs", 0},
	{"max-batch-size", 'b', "BATCH_SIZE", 0, "Generate up to BATCH_SIZE persons per batch", 0},
	{0, 0, 0, 0, 0, 0}
};

static struct argp optionParser = {options, parserFunc, 0, 0, 0, 0, 0};

ArgParser::ArgParser ()
	: _config ("config.yml")
	, _session ("session.yml")
	, _personas (1)
	, _maxWaitBatch (400)
	, _maxWaitPerson (400)
	, _maxBatchSize (20)
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
