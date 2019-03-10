
#pragma once

#include <pytysoc_common.h>
#include <pytysoc_tests.h>
#include <pytysoc_agent_core.h>
#include <pytysoc_sensors.h>
#include <pytysoc_terrainGen.h>
#include <pytysoc_scenario.h>
#include <pytysoc_runtime.h>

namespace pytysoc
{
    void initializeMjcfSchemaPath( const std::string& schemaPath );

    void initializeLoader( const std::string& pathTemplatesMjcf,
                           const std::string& pathTemplatesUrdf,
                           const std::string& pathTemplatesRlsim );

}