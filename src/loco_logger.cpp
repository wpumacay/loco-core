
#include <loco_logger.h>

namespace loco
{

    std::shared_ptr<spdlog::logger> TLogger::s_CoreLogger = nullptr;
    std::shared_ptr<spdlog::logger> TLogger::s_ClientLogger = nullptr;

    bool TLogger::s_IsActive = false;

    void TLogger::Init()
    {
        if ( TLogger::s_IsActive )
            return;

        spdlog::set_pattern( "%^[%T] %n: %v%$" );
    #if defined( LOCO_CORE_USE_LOGS )
        s_CoreLogger = spdlog::stdout_color_mt( "TYSOC" );
        s_CoreLogger->set_level( spdlog::level::trace );

        s_ClientLogger = spdlog::stdout_color_mt( "USER" );
        s_ClientLogger->set_level( spdlog::level::trace );
    #endif

        TLogger::s_IsActive = true;
    }

    void TLogger::Release()
    {
        // Clean refs. If using only macros for logging then all other references should
        // have been deleted once the macro finishes.
        s_CoreLogger = nullptr;
        s_ClientLogger = nullptr;
        s_IsActive = false;
    }

    bool TLogger::IsActive()
    {
        return TLogger::s_IsActive;
    }

}