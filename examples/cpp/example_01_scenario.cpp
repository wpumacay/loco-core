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
#include <loco/backends/bullet/simulation_impl_bullet.hpp>
#include <loco/backends/mujoco/simulation_impl_mujoco.hpp>

#include <cassert>

auto main() -> int {
    auto scenario = std::make_shared<loco::core::Scenario>();
    auto sim = std::make_shared<loco::core::Simulation>(
        scenario, loco::eBackendType::BULLET);

    sim->Init();

    sim->SetTimeStep(ToScalar(0.01));
    sim->SetGravity({ToScalar(0.0), ToScalar(0.0), ToScalar(-1.62)});

    // Grab the internal pimpl and play around with it (just for completeness)
    if (sim->backend_type() == loco::eBackendType::BULLET) {
        auto& pimpl =
            dynamic_cast<loco::bullet::SimulationImplBullet&>(sim->impl());

        assert(pimpl != nullptr);

        LOG_INFO("We're using Bullet as physics backend");

        auto* bt_world = pimpl.bullet_world();
        assert(bt_world != nullptr);

        auto bt_world_type = bt_world->getWorldType();

        if (bt_world_type == BT_DISCRETE_DYNAMICS_WORLD) {
            LOG_TRACE("Using btDiscreteDynamicsWorld as btWorld");
        } else if (bt_world_type == BT_DEFORMABLE_MULTIBODY_DYNAMICS_WORLD) {
            LOG_TRACE("Using btMultibodyDynamicsWorld as btWorld");
        }
    } else if (sim->backend_type() == loco::eBackendType::MUJOCO) {
        // NOLINTNEXTLINE
        auto& pimpl =
            dynamic_cast<loco::mujoco::SimulationImplMujoco&>(sim->impl());

        assert(pimpl != nullptr);

        LOG_INFO("We're using MuJoCo as physics backend");

        //// auto* mjc_model = pimpl.mujoco_model();
        //// auto* mjc_data = pimpl.mujoco_data();
        //// assert(mjc_model != nullptr);
        //// assert(mjc_data != nullptr);

        //// LOG_TRACE("Number of generalized coordinates {0}", mjc_model->nq);
        //// LOG_TRACE("Number of degrees of freedom", mjc_model->nv);
        //// LOG_TRACE("Number of constraints", mjc_data->nefc);
        //// LOG_TRACE("Number of detected contacts", mjc_data->ncon);

    } else {
        LOG_INFO("We're using a dummy backend");
    }

    sim->Reset();

    return 0;
}

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif
