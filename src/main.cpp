#include <Engine/Engine.hpp>

#include <PluginSDK.hpp>

PLUGIN_FUNC void OnInit(obe::engine::Engine& engine)
{
    const obe::debug::Logger logger = engine.get_logger();
    logger->info("Hello, world!");
}
