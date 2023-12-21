#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <loco/core/common.hpp>
#include <loco/core/single_body/single_body_t.hpp>
#include <loco/core/visualizer/drawable_t.hpp>

namespace loco {
namespace core {

/// \brief Representation for the main container of simulated objects
class Scenario {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Scenario)

    DEFINE_SMART_POINTERS(Scenario)

 public:
    /// Max number of free drawables allowed in a single scene
    static constexpr size_t MAX_DRAWABLES = 1024;

    /// Creates a scenario with a default dummy runtime and backend
    Scenario();

    /// Releases/Frees all allocated resources of this scenario
    ~Scenario();

    /// \brief Adds a given free drawable to the scenario with the given name
    ///
    /// \param[in] drawable The drawable we want to add to the scenario
    /// \param[in] name The name of the drawable in the scenario
    auto AddDrawable(Drawable::ptr drawable) -> void;

    /// \brief Returns the drawable at given index
    ///
    /// \param[in] index The index of the drawable we want to retrieve
    auto GetDrawableByIndex(size_t index) -> Drawable::ptr;

    /// \brief Returns the drawable with given name
    ///
    /// \param[in] name The name of the drawable we want to retrieve
    auto GetDrawableByName(const std::string& name) -> Drawable::ptr;

    /// Returns the current number of free drawables in this scenario
    auto num_drawables() const -> size_t;

    /// Returns a string representation of this scenario
    auto ToString() const -> std::string;

 protected:
    /// The list of free drawables hold by this scenario
    std::vector<Drawable::ptr> m_Drawables;

    /// The keympa used to link drawables by name to its location in the list
    std::unordered_map<std::string, size_t> m_DrawablesKeymap;
};

}  // namespace core
}  // namespace loco
