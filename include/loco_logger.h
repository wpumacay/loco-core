#pragma once

#include <cstdlib>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace loco
{
    using string_view_t = fmt::basic_string_view<char>;

    class TLogger
    {
    public :
        static void Init();
        static void Release();
        static bool IsActive();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        template< typename... Args >
        static void CoreTrace( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_CoreLogger->trace( fmt, args... );
        }

        template< typename... Args >
        static void CoreInfo( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_CoreLogger->info( fmt, args... );
        }

        template< typename... Args >
        static void CoreWarn( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_CoreLogger->warn( fmt, args... );
        }

        template< typename... Args >
        static void CoreError( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_CoreLogger->error( fmt, args... );
        }

        template< typename... Args >
        static void CoreCritical( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_CoreLogger->critical( fmt, args... );
        }

        template< typename... Args >
        static void ClientTrace( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_ClientLogger->trace( fmt, args... );
        }

        template< typename... Args >
        static void ClientInfo( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_ClientLogger->info( fmt, args... );
        }

        template< typename... Args >
        static void ClientWarn( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_ClientLogger->warn( fmt, args... );
        }

        template< typename... Args >
        static void ClientError( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_ClientLogger->error( fmt, args... );
        }

        template< typename... Args >
        static void ClientCritical( string_view_t fmt, const Args &... args )
        {
            if ( !TLogger::s_IsActive )
                return;

            TLogger::s_ClientLogger->critical( fmt, args... );
        }

    private :
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;

        static bool s_IsActive;
    };
}

#if defined( LOCO_CORE_USE_LOGS )
    #define LOCO_CORE_TRACE(...)      ::loco::TLogger::CoreTrace(__VA_ARGS__)
    #define LOCO_CORE_INFO(...)       ::loco::TLogger::CoreInfo(__VA_ARGS__)
    #define LOCO_CORE_WARN(...)       ::loco::TLogger::CoreWarn(__VA_ARGS__)
    #define LOCO_CORE_ERROR(...)      ::loco::TLogger::CoreError(__VA_ARGS__)
    #define LOCO_CORE_CRITICAL(...)   ::loco::TLogger::CoreCritical(__VA_ARGS__)
    #define LOCO_CORE_ASSERT(x, ...) \
        {                                                                               \
            if( !(x) )                                                                  \
            {                                                                           \
                LOCO_CORE_CRITICAL( "Failed due to assertion: {0}", __VA_ARGS__ );      \
                exit( EXIT_FAILURE );                                                   \
            }                                                                           \
        }

    #define LOCO_TRACE(...)       ::loco::TLogger::ClientTrace(__VA_ARGS__)
    #define LOCO_INFO(...)        ::loco::TLogger::ClientInfo(__VA_ARGS__)
    #define LOCO_WARN(...)        ::loco::TLogger::ClientWarn(__VA_ARGS__)
    #define LOCO_ERROR(...)       ::loco::TLogger::ClientError(__VA_ARGS__)
    #define LOCO_CRITICAL(...)    ::loco::TLogger::ClientCritical(__VA_ARGS__)
    #define LOCO_ASSERT(x, ...) \
        {                                                                       \
            if( !(x) )                                                          \
            {                                                                   \
                LOCO_CRITICAL( "Failed due to assertion: {0}", __VA_ARGS__ );   \
                exit( EXIT_FAILURE );                                           \
            }                                                                   \
        }
#else
    #define LOCO_CORE_TRACE(...) ((void)0)
    #define LOCO_CORE_INFO(...) ((void)0)
    #define LOCO_CORE_WARN(...) ((void)0)
    #define LOCO_CORE_ERROR(...) ((void)0)
    #define LOCO_CORE_CRITICAL(...) ((void)0)
    #define LOCO_CORE_ASSERT(x, ...) \
        {                               \
            if( !(x) )                  \
            {                           \
                exit( EXIT_FAILURE );   \
            }                           \
        }

    #define LOCO_TRACE(...) ((void)0)
    #define LOCO_INFO(...) ((void)0)
    #define LOCO_WARN(...) ((void)0)
    #define LOCO_ERROR(...) ((void)0)
    #define LOCO_CRITICAL(...) ((void)0)
    #define LOCO_ASSERT(x, ...) \
        {                               \
            if( !(x) )                  \
            {                           \
                exit( EXIT_FAILURE );   \
            }                           \
        }
#endif
