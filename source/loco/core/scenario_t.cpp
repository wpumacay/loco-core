#include <stdexcept>

#include <loco/core/scenario_t.hpp>

#include <spdlog/fmt/bundled/format.h>

namespace loco {
namespace core {

size_t Scenario::s_bodies_so_far = 0;     // NOLINT
size_t Scenario::s_drawables_so_far = 0;  // NOLINT

Scenario::Scenario() {
    m_SingleBodies.reserve(Scenario::MAX_BODIES);
    m_FreeDrawables.reserve(Scenario::MAX_FREE_DRAWABLES);
}

Scenario::~Scenario() {
    m_SingleBodies.clear();
    m_FreeDrawables.clear();
}

auto Scenario::AddSingleBody(SingleBody::ptr body, const std::string& name)
    -> void {
    auto body_name =
        (name.empty())
            ? "single_body_" + std::to_string(Scenario::s_bodies_so_far)
            : name;
    m_SingleBodies.push_back(std::move(body));
    m_SingleBodiesKeymap[body_name] =
        static_cast<int>(m_SingleBodies.size()) - 1;
}

auto Scenario::AddFreeDrawable(Drawable::ptr drawable, const std::string& name)
    -> void {
    auto drawable_name =
        (name.empty())
            ? "free_drawable_" + std::to_string(Scenario::s_drawables_so_far)
            : name;
    m_FreeDrawables.push_back(std::move(drawable));
    m_FreeDrawablesKeymap[drawable_name] =
        static_cast<int>(m_FreeDrawables.size()) - 1;
}

auto Scenario::num_single_bodies() const -> size_t {
    return m_SingleBodies.size();
}

auto Scenario::num_free_drawables() const -> size_t {
    return m_FreeDrawables.size();
}

auto Scenario::empty() const -> bool {
    return m_SingleBodies.empty() && m_FreeDrawables.empty();
}

auto Scenario::toString() const -> std::string {
    return fmt::format(
        "<Scenario\n"
        "  num_single_bodies={}\n"
        "  num_free_drawables={}\n"
        "  num_single_bodies_so_far={}\n"
        "  num_free_drawables_so_far={}\n"
        "  max_single_bodies={}\n"
        "  max_free_drawables={}\n"
        ">",
        m_SingleBodies.size(), m_FreeDrawables.size(),
        Scenario::s_bodies_so_far, Scenario::s_drawables_so_far,
        Scenario::MAX_BODIES, Scenario::MAX_FREE_DRAWABLES);
}

}  // namespace core
}  // namespace loco
