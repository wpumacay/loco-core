
#include <tysoc_logger.h>

namespace tysoc
{

    std::shared_ptr<spdlog::logger> TLogger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> TLogger::s_ClientLogger;

    void TLogger::Init()
    {
        spdlog::set_pattern( "%^[%T] %n: %v%$" );

        s_CoreLogger = spdlog::stdout_color_mt( "TYSOC" );
        s_CoreLogger->set_level( spdlog::level::trace );

        s_ClientLogger = spdlog::stdout_color_mt( "USER" );
        s_ClientLogger->set_level( spdlog::level::trace );
    }

}