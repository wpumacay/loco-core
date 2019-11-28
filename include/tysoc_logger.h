#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace tysoc
{
    class TLogger
    {
    public :
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private :
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

#define TYSOC_CORE_TRACE(...)                               \
    if ( !TLogger::GetCoreLogger() )                        \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetCoreLogger()->trace(__VA_ARGS__)

#define TYSOC_CORE_INFO(...)                                \
    if ( !TLogger::GetCoreLogger() )                        \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetCoreLogger()->info(__VA_ARGS__)

#define TYSOC_CORE_WARN(...)                                \
    if ( !TLogger::GetCoreLogger() )                        \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetCoreLogger()->warn(__VA_ARGS__)

#define TYSOC_CORE_ERROR(...)                               \
    if ( !TLogger::GetCoreLogger() )                        \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetCoreLogger()->error(__VA_ARGS__)

#define TYSOC_CORE_CRITICAL(...)                            \
    if ( !TLogger::GetCoreLogger() )                        \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetCoreLogger()->critical(__VA_ARGS__)

#define TYSOC_CORE_ASSERT(x, ...) \
    {                                                                               \
        if( !x )                                                                    \
        {                                                                           \
            TYSOC_CORE_CRITICAL( "Failed due to assertion: {0}", __VA_ARGS__ );    \
            exit( EXIT_FAILURE );                                                   \
        }                                                                           \
    }

#define TYSOC_TRACE(...)                                    \
    if ( !TLogger::GetClientLogger() )                      \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetClientLogger()->trace(__VA_ARGS__)

#define TYSOC_INFO(...)                                     \
    if ( !TLogger::GetClientLogger() )                      \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetClientLogger()->info(__VA_ARGS__)

#define TYSOC_WARN(...)                                     \
    if ( !TLogger::GetClientLogger() )                      \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetClientLogger()->warn(__VA_ARGS__)

#define TYSOC_ERROR(...)                                    \
    if ( !TLogger::GetClientLogger() )                      \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetClientLogger()->error(__VA_ARGS__)

#define TYSOC_CRITICAL(...)                                 \
    if ( !TLogger::GetClientLogger() )                      \
        TLogger::Init();                                    \
    ::tysoc::TLogger::GetClientLogger()->critical(__VA_ARGS__)

#define TYSOC_ASSERT(x, ...) \
    {                                                                       \
        if( !x )                                                            \
        {                                                                   \
            TYSOC_CRITICAL( "Failed due to assertion: {0}", __VA_ARGS__ ); \
            exit( EXIT_FAILURE );                                           \
        }                                                                   \
    }
