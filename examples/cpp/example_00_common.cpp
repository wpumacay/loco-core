#include <loco/core/common.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

auto main() -> int {
    // Making the data for a box ------------------------
    ::loco::ColliderData box_collider;
    box_collider.type = ::loco::eShapeType::BOX;
    box_collider.size = {1.0, 1.0, 1.0};

    ::loco::DrawableData box_drawable;
    box_drawable.type = ::loco::eShapeType::BOX;
    box_drawable.size = {1.0, 1.0, 1.0};

    ::loco::BodyData box_body;
    box_body.collider = box_collider;
    box_body.drawable = box_drawable;
    // --------------------------------------------------

    // Making a kind of bottle (compound shape) ---------
    ::loco::ColliderData base_collider;
    base_collider.type = ::loco::eShapeType::CYLINDER;
    base_collider.size = {0.5, 1.0, 0.5};

    ::loco::ColliderData tip_collider;
    tip_collider.type = ::loco::eShapeType::CYLINDER;
    tip_collider.size = {0.25, 0.4, 0.25};
    tip_collider.local_tf.position = {0.0, 0.0, 0.5};

    ::loco::ColliderData bottle_collider;
    bottle_collider.type = ::loco::eShapeType::COMPOUND;
    bottle_collider.children.push_back(base_collider);
    bottle_collider.children.push_back(tip_collider);

    ::loco::DrawableData base_drawable;
    base_drawable.type = ::loco::eShapeType::CYLINDER;
    base_drawable.size = {0.5, 1.0};

    ::loco::DrawableData tip_drawable;
    tip_drawable.type = ::loco::eShapeType::CYLINDER;
    tip_drawable.size = {0.25, 0.4};
    tip_drawable.local_tf.position = {0.0, 0.0, 0.5};

    ::loco::DrawableData bottle_drawable;
    bottle_drawable.type = ::loco::eShapeType::COMPOUND;
    bottle_drawable.children.push_back(base_drawable);
    bottle_drawable.children.push_back(tip_drawable);

    ::loco::BodyData bottle_body;
    bottle_body.collider = bottle_collider;
    bottle_body.drawable = bottle_drawable;
    // --------------------------------------------------

    return 0;
}

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop  // NOLINT
#endif
