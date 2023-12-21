#pragma once

#include <memory>

#include "./common_meshcat.hpp"

#include <loco/core/visualizer/impl/visualizer_impl.hpp>

namespace loco {
namespace meshcat {

class VisualizerImplMeshcat : public ::loco::core::VisualizerImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(VisualizerImplMeshcat)

    DEFINE_SMART_POINTERS(VisualizerImplMeshcat)

 public:
    /// \brief Creates a visualizer that integrates with Meshcat
    ///
    /// \param[in] scenario The scenario from which to collect drawables
    explicit VisualizerImplMeshcat(::loco::core::Scenario::ptr scenario);

    ~VisualizerImplMeshcat() override = default;

    auto Init() -> void override;

    auto Reset() -> void override;

    auto Update() -> void override;

 private:
    /// The handle to the Meshcat instance
    std::shared_ptr<MeshcatCpp::Meshcat> m_MeshcatInstance = nullptr;
};

}  // namespace meshcat
}  // namespace loco
