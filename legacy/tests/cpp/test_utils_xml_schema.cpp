
#include <loco.h>
#include <gtest/gtest.h>

TEST( TestLocoUtilsXmlSchema, TestXmlSchemaMjcf )
{
    loco::InitUtils();

    const std::string MJCF_SCHEMA_PATH = loco::PATH_RESOURCES + "xml/schema_mjcf.xml";
    auto schema_mjcf = std::make_unique<loco::parsing::TMjcfSchema>( MJCF_SCHEMA_PATH );
    // Add more checks as we test the functionality (and fails are discovered)
    EXPECT_TRUE( schema_mjcf->HasElement( "worldbody" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "worldbody", "geom" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "worldbody", "body" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "worldbody", "site" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "worldbody", "camera" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "worldbody", "light" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "body" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "body", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "body", "pos" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "body", "quat" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "body", "axisangle" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "body", "xyaxes" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "body", "zaxis" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "body", "euler" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "body", "childclass" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "body", "geom" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "body", "site" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "body", "joint" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "body", "freejoint" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "body", "inertial" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "geom" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "type" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "contype" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "conaffinity" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "condim" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "priority" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "size" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "material" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "rgba" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "friction" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "mass" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "density" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "solmix" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "solref" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "solimp" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "margin" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "gap" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "fromto" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "pos" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "quat" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "axisangle" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "xyaxes" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "zaxis" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "euler" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "hfield" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "mesh" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "fitscale" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "geom", "user" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "site" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "type" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "material" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "rgba" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "size" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "fromto" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "pos" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "quat" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "axisangle" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "xyaxes" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "zaxis" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "euler" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "site", "user" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "joint" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "type" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "pos" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "axis" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "springdamper" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "limited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "solreflimit" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "solimplimit" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "solreffriction" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "solimpfriction" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "stiffness" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "range" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "margin" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "ref" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "springref" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "armature" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "damping" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "frictionloss" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "joint", "user" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "freejoint" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "freejoint", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "freejoint", "group" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "inertial" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "pos" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "quat" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "axisangle" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "xyaxes" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "zaxis" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "euler" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "mass" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "diaginertia" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "inertial", "fullinertia" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "actuator" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "actuator", "general" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "actuator", "motor" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "actuator", "position" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "actuator", "velocity" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "actuator", "cylinder" ) );
    EXPECT_TRUE( schema_mjcf->HasChild( "actuator", "muscle" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "general" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "ctrllimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "forcelimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "ctrlrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "forcerange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "lengthrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "gear" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "cranklength" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "joint" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "jointinparent" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "site" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "tendon" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "cranksite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "slidersite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "user" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "dyntype" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "gaintype" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "biastype" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "dynprm" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "gainprm" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "general", "biasprm" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "motor" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "ctrllimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "forcelimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "ctrlrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "forcerange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "lengthrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "gear" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "cranklength" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "joint" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "jointinparent" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "tendon" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "cranksite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "slidersite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "site" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "motor", "user" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "position" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "ctrllimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "forcelimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "ctrlrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "forcerange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "lengthrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "gear" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "cranklength" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "joint" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "jointinparent" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "tendon" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "cranksite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "slidersite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "site" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "user" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "position", "kp" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "velocity" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "ctrllimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "forcelimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "ctrlrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "forcerange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "lengthrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "gear" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "cranklength" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "joint" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "jointinparent" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "tendon" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "cranksite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "slidersite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "site" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "user" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "velocity", "kv" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "cylinder" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "ctrllimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "forcelimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "ctrlrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "forcerange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "lengthrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "gear" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "cranklength" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "joint" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "jointinparent" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "tendon" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "cranksite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "slidersite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "site" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "user" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "timeconst" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "area" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "diameter" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "cylinder", "bias" ) );

    EXPECT_TRUE( schema_mjcf->HasElement( "muscle" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "name" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "class" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "group" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "ctrllimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "forcelimited" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "ctrlrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "forcerange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "lengthrange" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "gear" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "cranklength" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "joint" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "jointinparent" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "tendon" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "cranksite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "slidersite" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "site" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "user" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "timeconst" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "range" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "force" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "scale" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "lmin" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "lmax" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "vmax" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "fpmax" ) );
    EXPECT_TRUE( schema_mjcf->HasAttribute( "muscle", "fvmax" ) );

    LOCO_TRACE( "mjcf-schema:\n{0}", schema_mjcf->ToString() );
}

TEST( TestLocoUtilsXmlSchema, TestXmlSchemaUrdf )
{
    loco::InitUtils();

    const std::string URDF_SCHEMA_PATH = loco::PATH_RESOURCES + "xml/schema_urdf.xml";
    auto schema_urdf = std::make_unique<loco::parsing::TUrdfSchema>( URDF_SCHEMA_PATH );
    // Add more checks as we test the functionality (and fails are discovered)
    EXPECT_TRUE( schema_urdf->HasElement( "robot" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "robot", "name" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "robot", "version" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "robot", "link" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "robot", "joint" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "robot", "material" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "link" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "link", "name" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "link", "type" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "link", "visual" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "link", "collision" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "link", "inertial" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "visual" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "visual", "origin" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "visual", "geometry" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "visual", "material" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "origin" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "origin", "xyz" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "origin", "rpy" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "geometry" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "geometry", "box" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "geometry", "cylinder" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "geometry", "sphere" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "geometry", "mesh" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "box" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "box", "size" ) );
    EXPECT_TRUE( schema_urdf->HasElement( "cylinder" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "cylinder", "radius" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "cylinder", "length" ) );
    EXPECT_TRUE( schema_urdf->HasElement( "sphere" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "sphere", "radius" ) );
    EXPECT_TRUE( schema_urdf->HasElement( "mesh" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "mesh", "filename" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "mesh", "scale" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "material" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "material", "name" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "material", "color" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "material", "texture" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "collision" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "collision", "origin" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "collision", "geometry" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "inertial" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "inertial", "origin" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "inertial", "mass" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "inertial", "inertia" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "mass" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "mass", "value" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "inertia" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "inertia", "ixx" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "inertia", "ixy" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "inertia", "ixz" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "inertia", "iyy" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "inertia", "iyz" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "inertia", "izz" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "joint" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "joint", "name" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "joint", "type" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "joint", "origin" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "joint", "parent" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "joint", "child" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "joint", "axis" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "joint", "limit" ) );
    EXPECT_TRUE( schema_urdf->HasChild( "joint", "dynamics" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "parent" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "parent", "link" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "child" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "child", "link" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "axis" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "axis", "xyz" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "limit" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "limit", "lower" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "limit", "upper" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "limit", "effort" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "limit", "velocity" ) );

    EXPECT_TRUE( schema_urdf->HasElement( "dynamics" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "dynamics", "damping" ) );
    EXPECT_TRUE( schema_urdf->HasAttribute( "dynamics", "friction" ) );

    LOCO_TRACE( "urdf-schema:\n{0}", schema_urdf->ToString() );
}