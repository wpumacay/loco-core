import math3d as m3d

import loco


def main() -> None:
    # Making the data for a box ------------------------
    box_collider = loco.ColliderData()
    box_collider.type = loco.ShapeType.BOX
    box_collider.size = m3d.Vector3f(1.0, 1.0, 1.0)

    box_drawable = loco.DrawableData()
    box_drawable.type = loco.ShapeType.BOX
    box_drawable.size = m3d.Vector3f(1.0, 1.0, 1.0)

    box_body = loco.BodyData()
    box_body.collider = box_collider
    box_body.drawable = box_drawable
    # --------------------------------------------------

    # Making a kind of bottle (compound shape) ---------
    base_collider = loco.ColliderData()
    base_collider.type = loco.ShapeType.CYLINDER
    base_collider.size = m3d.Vector3f(0.5, 1.0, 0.5)

    tip_collider = loco.ColliderData()
    tip_collider.type = loco.ShapeType.CYLINDER
    tip_collider.size = m3d.Vector3f(0.25, 0.4, 0.25)

    bottle_collider = loco.ColliderData()
    bottle_collider.type = loco.ShapeType.COMPOUND
    bottle_collider.children.append(base_collider)
    bottle_collider.children.append(tip_collider)

    base_drawable = loco.DrawableData()
    base_drawable.type = loco.ShapeType.CYLINDER
    base_drawable.size = m3d.Vector3f(0.5, 1.0, 0.5)

    tip_drawable = loco.DrawableData()
    tip_drawable.type = loco.ShapeType.CYLINDER
    tip_drawable.size = m3d.Vector3f(0.25, 0.4, 0.25)

    bottle_drawable = loco.DrawableData()
    bottle_drawable.type = loco.ShapeType.COMPOUND
    bottle_drawable.children.append(base_drawable)
    bottle_drawable.children.append(tip_drawable)

    bottle_body = loco.BodyData()
    bottle_body.collider = bottle_collider
    bottle_body.drawable = bottle_drawable
    # --------------------------------------------------


if __name__ == "__main__":
    main()
