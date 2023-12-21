#include <loco/core/scenario_t.hpp>

#include <spdlog/fmt/bundled/format.h>

namespace loco {
namespace core {

Scenario::Scenario() { m_Drawables.reserve(Scenario::MAX_DRAWABLES); }

Scenario::~Scenario() { m_Drawables.clear(); }

auto Scenario::AddDrawable(Drawable::ptr drawable) -> void {
    auto drawable_name = drawable->name();
    m_Drawables.push_back(std::move(drawable));
    m_DrawablesKeymap[drawable_name] = m_Drawables.size() - 1;
}

auto Scenario::GetDrawableByIndex(size_t index) -> Drawable::ptr {
    if (index < 0 || index > m_Drawables.size() - 1) {
        return nullptr;
    }
    return m_Drawables[index];
}

auto Scenario::GetDrawableByName(const std::string& name) -> Drawable::ptr {
    if (m_DrawablesKeymap.find(name) == m_DrawablesKeymap.end()) {
        return nullptr;
    }
    return m_Drawables[m_DrawablesKeymap[name]];
}

auto Scenario::num_drawables() const -> size_t { return m_Drawables.size(); }

auto Scenario::ToString() const -> std::string {
    return fmt::format(
        "<Scenario\n"
        "  num_drawables={}\n"
        "  max_drawables={}\n"
        ">",
        m_Drawables.size(), Scenario::MAX_DRAWABLES);
}

}  // namespace core
}  // namespace loco
