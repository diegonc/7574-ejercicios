#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <argp.h>
#include <string>
#include <utils/NonCopyable.h>

class ArgParser : private NonCopyable
{
	private:
		std::string _config;
		std::string _session;
		unsigned long _personas;
		unsigned long _maxWaitBatch;
		unsigned long _maxWaitPerson;
		unsigned long _maxBatchSize;

		ArgParser();
		~ArgParser();

		friend int parserFunc (int key, char *arg, struct argp_state *state);

	public:
		static ArgParser& getInstance();

		void parse (int argc, char **argv);

		const std::string& config () const { return _config; }
		const std::string& session () const { return _session; }
		unsigned long personas () const { return _personas; }
		unsigned long maxWaitBatch () const { return _maxWaitBatch; }
		unsigned long maxWaitPerson () const { return _maxWaitPerson; }
		unsigned long maxBatchSize () const { return _maxBatchSize; }
};

#endif
