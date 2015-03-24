#include <ArgParser.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>

int main(int argc, char** argv)
{
    ArgParser& args = ArgParser::getInstance ();
    args.parse (argc, argv);

    LoggerRegistry& registry = LoggerRegistry::getInstance ();
    registry.application ("puerta");
    registry.filename ("ejercicio1.log");
    registry.quiet (!args.debug ());

    Logger& logger = LoggerRegistry::getLogger ("main");
    logger << "Mensaje de prueba" << Logger::endl;

    return 0;
}
