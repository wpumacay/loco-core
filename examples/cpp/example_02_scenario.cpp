// Disable warnings generated by the Bullet codebase
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif

#include <loco/core/scenario_t.hpp>
#include <loco/core/simulation_t.hpp>
#include <loco/core/visualizer_t.hpp>

#if defined(LOCO_BULLET_ENABLED)
#include <loco/backends/bullet/simulation_impl_bullet.hpp>
#endif

#if defined(LOCO_MUJOCO_ENABLED)
#include <loco/backends/mujoco/simulation_impl_mujoco.hpp>
#endif

#if defined(LOCO_DART_ENABLED)
#include <loco/backends/dart/simulation_impl_dart.hpp>
#endif

#include <cassert>

auto main() -> int {
    auto scenario = std::make_shared<loco::core::Scenario>();
    auto sim = std::make_shared<loco::core::Simulation>(
        scenario, loco::eBackendType::DART);
    auto viz = std::make_shared<loco::core::Visualizer>(
        scenario, loco::eVisualizerType::NONE);

    sim->Init();
    viz->Init();

    sim->SetTimeStep(ToScalar(0.01));
    sim->SetGravity({ToScalar(0.0), ToScalar(0.0), ToScalar(-1.62)});

    sim->Step(ToScalar(1. / 60.));
    viz->Update();

    // Grab the internal pimpl and play around with it (just for completeness)
    if (sim->backend_type() == loco::eBackendType::BULLET) {
#if defined(LOCO_BULLET_ENABLED)
        auto& pimpl =
            dynamic_cast<loco::bullet::SimulationImplBullet&>(sim->impl());

        LOG_INFO("We're using Bullet as physics backend");

        auto& bt_world = pimpl.bullet_world();

        auto bt_world_type = bt_world.getWorldType();

        if (bt_world_type == BT_DISCRETE_DYNAMICS_WORLD) {
            LOG_TRACE("Using btDiscreteDynamicsWorld as btWorld");
        } else if (bt_world_type == BT_DEFORMABLE_MULTIBODY_DYNAMICS_WORLD) {
            LOG_TRACE("Using btMultibodyDynamicsWorld as btWorld");
        }
#endif
    } else if (sim->backend_type() == loco::eBackendType::MUJOCO) {
#if defined(LOCO_MUJOCO_ENABLED)
        // NOLINTNEXTLINE
        auto& pimpl =
            dynamic_cast<loco::mujoco::SimulationImplMujoco&>(sim->impl());

        LOG_INFO("We're using MuJoCo as physics backend");

        //// auto* mjc_model = pimpl.mujoco_model();
        //// auto* mjc_data = pimpl.mujoco_data();
        //// assert(mjc_model != nullptr);
        //// assert(mjc_data != nullptr);

        //// LOG_TRACE("Number of generalized coordinates {0}", mjc_model->nq);
        //// LOG_TRACE("Number of degrees of freedom", mjc_model->nv);
        //// LOG_TRACE("Number of constraints", mjc_data->nefc);
        //// LOG_TRACE("Number of detected contacts", mjc_data->ncon);
#endif
    } else if (sim->backend_type() == loco::eBackendType::DART) {
#if defined(LOCO_DART_ENABLED)
        // NOLINTNEXTLINE
        auto& pimpl =
            dynamic_cast<loco::dart::SimulationImplDart&>(sim->impl());

        LOG_INFO("We're using DART as physics backend");
#endif
    } else {
        LOG_INFO("We're using a dummy backend");
    }

    sim->Reset();

    return 0;
}

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif
