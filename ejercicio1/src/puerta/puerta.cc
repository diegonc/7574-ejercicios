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
    // mensaje con nivel por defecto
    logger << "Mensaje de prueba" << Logger::endl;
    // mensaje con nivel de error
    logger << Level::ERROR
            << "Error: mensaje de prueba"
            << Logger::endl;

    try {
        // mensaje con nivel ubicado en lugar inválido
        logger << "Mensaje" << Level::DEBUG << "Otro mensaje"
                << Logger::endl;
        logger << Level::ERROR
                << "El logger debe arrojar excepción"
                << Logger::endl;
    } catch (std::logic_error& e) {
        logger << Level::INFO
                << "El logger funciona correctamente"
                << Logger::endl;
    }

    return 0;
}
