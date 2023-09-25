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
    /// Max number of bodies allowed in a single scene (used for array capacity)
    static constexpr size_t MAX_BODIES = 1024;
    /// Max number of free drawables allowed in a single scene
    static constexpr size_t MAX_FREE_DRAWABLES = 1024;

    /// The number of single bodies created/added so far to the scene
    static size_t s_bodies_so_far;  // NOLINT

    /// The number of free drawables created/added so far to the scene
    static size_t s_drawables_so_far;  // NOLINT

    /// Creates a scenario with a default dummy runtime and backend
    Scenario();

    /// Releases/Frees all allocated resources of this scenario
    ~Scenario();

    /// \brief Adds the given body to the scenario with the given name
    ///
    /// \param[in] body The body we want to add to the scenario
    /// \param[in] name The name of the body in the scenario
    auto AddSingleBody(SingleBody::ptr body, const std::string& name = "")
        -> void;

    /// \brief Adds a given free drawable to the scenario with the given name
    ///
    /// \param[in] drawable The drawable we want to add to the scenario
    /// \param[in] name The name of the drawable in the scenario
    auto AddFreeDrawable(Drawable::ptr drawable, const std::string& name = "")
        -> void;

    /// Returns the current number of bodies in this scenario
    auto num_single_bodies() const -> size_t;

    /// Returns the current number of free drawables in this scenario
    auto num_free_drawables() const -> size_t;

    /// Returns whether this scenario is empty or not
    auto empty() const -> bool;

    /// Returns a string representation of this scenario
    auto toString() const -> std::string;

 protected:
    /// The list of bodies currently hold by this scenario
    std::vector<SingleBody::ptr> m_SingleBodies;

    /// The list of free drawables hold by this scenario
    std::vector<Drawable::ptr> m_FreeDrawables;

    /// The keymap used to link bodies by name to its location in the list
    std::unordered_map<std::string, int> m_SingleBodiesKeymap;

    /// The keympa used to link drawables by name to its location in the list
    std::unordered_map<std::string, int> m_FreeDrawablesKeymap;
};

}  // namespace core
}  // namespace loco
