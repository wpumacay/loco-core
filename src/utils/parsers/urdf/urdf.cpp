
#include <utils/parsers/urdf/urdf.h>


namespace tysoc {
namespace urdf {

    UrdfModel* loadGenericModel( const std::string& modelfile )
    {
        tinyxml2::XMLDocument _doc;
        _doc.LoadFile( modelfile.c_str() );

        if ( _doc.Error() )
        {
            _logError( _doc.ErrorStr() );
            _doc.ClearError();

            return NULL;
        }

        XMLElement* _robotXml = _doc.FirstChildElement( "robot" );
        if ( !_robotXml )
        {
            _logError( "expected a robot element" );
            return NULL;
        }

        // Get robot name
        const char* name = _robotXml->Attribute( "name" );
        if (!name)
        {
            _logError( "Expected a name for robot" );
            return NULL;
        }

        auto _urdfModel = new UrdfModel();

        _urdfModel->name = name;

        // Get all Material elements
        for (XMLElement* material_xml = _robotXml->FirstChildElement("material"); material_xml; material_xml = material_xml->NextSiblingElement("material"))
        {
            UrdfMaterial* material = new UrdfMaterial();

            parseMaterial(*material, material_xml);

            UrdfMaterial** mat = urdf2Model.materials.find(material->name.c_str());
            if (mat)
            {
                delete material;
                _logWarning("Duplicate material");
            }
            else
            {
                urdf2Model.materials.insert(material->name.c_str(), material);
            }
        }

        //  char msg[1024];
        //  sprintf(msg,"Num materials=%d", model.materials.size());

        for (XMLElement* link_xml = _robotXml->FirstChildElement("link"); link_xml; link_xml = link_xml->NextSiblingElement("link"))
        {
            UrdfLink* link = new UrdfLink;

            if (parseLink(urdf2Model, *link, link_xml))
            {
                if (urdf2Model.links.find(link->name.c_str()))
                {
                    _logError("Link name is not unique, link names in the same model have to be unique");
                    _logError(link->name.c_str());
                    delete link;
                    return false;
                }
                else
                {
                    //copy model material into link material, if link has no local material
                    for (int i = 0; i < link->visualArray.size(); i++)
                    {
                        UrdfVisual& vis = link->visualArray.at(i);
                        if (!vis.geometry.hasLocalMaterial && vis.materialName.c_str())
                        {
                            UrdfMaterial** mat = urdf2Model.materials.find(vis.materialName.c_str());
                            if (mat && *mat)
                            {
                                vis.geometry.localMaterial = **mat;
                            }
                            else
                            {
                                //_logError("Cannot find material with name:");
                                //_logError(vis.materialName.c_str());
                            }
                        }
                    }

                    urdf2Model.links.insert(link->name.c_str(), link);
                }
            }
            else
            {
                _logError("failed to parse link");
                delete link;
                return false;
            }
        }
        if (urdf2Model.links.size() == 0)
        {
            _logWarning("No links found in URDF file");
            return false;
        }

        // Get all Joint elements
        for (XMLElement* joint_xml = _robotXml->FirstChildElement("joint"); joint_xml; joint_xml = joint_xml->NextSiblingElement("joint"))
        {
            UrdfJoint* joint = new UrdfJoint;

            if (parseJoint(*joint, joint_xml))
            {
                if (urdf2Model.joints.find(joint->name.c_str()))
                {
                    _logError("joint '%s' is not unique.");
                    _logError(joint->name.c_str());
                    delete joint;
                    return false;
                }
                else
                {
                    urdf2Model.joints.insert(joint->name.c_str(), joint);
                }
            }
            else
            {
                _logError("joint xml is not initialized correctly");
                delete joint;
                return false;
            }
        }

        if (urdf2Model.links.size() == 0)
        {
            _logWarning("No links found in URDF file");
            return false;
        }

        bool ok(initTreeAndRoot(urdf2Model));
        if (!ok)
        {
            return false;
        }

        return true;
    }

    void _logError( const std::string& errorMsg )
    {
        std::cout << "ERROR> " << errorMsg << std::endl;
    }

    void _logWarning( const std::string& warningMsg )
    {
        std::cout << "WARNING> " << warningMsg << std::endl;
    }

    bool _parseMaterial( UrdfMaterial& material, XMLElement* config )
    {
        if (!config->Attribute("name"))
        {
            _logError("Material must contain a name attribute");
            return false;
        }

        material.name = config->Attribute("name");

        // texture
        XMLElement* t = config->FirstChildElement("texture");
        if (t)
        {
            if (t->Attribute("filename"))
            {
                material.textureFilename = t->Attribute("filename");
            }
        }

        if (material.textureFilename.length() == 0)
        {
            //_logWarning("material has no texture file name");
        }

        // color
        {
            XMLElement* c = config->FirstChildElement("color");
            if (c)
            {
                if (c->Attribute("rgba"))
                {
                    if (!parseVector4(material.rgbaColor, c->Attribute("rgba")))
                    {
                        std::string msg = material.name + " has no rgba";
                        _logWarning(msg.c_str());
                    }
                }
            }
        }

        {
            // specular (non-standard)
            XMLElement* s = config->FirstChildElement("specular");
            if (s)
            {
                if (s->Attribute("rgb"))
                {
                    if (!parseVector3(material.specularColor, s->Attribute("rgb")))
                    {
                    }
                }
            }
        }
        return true;
    }

    bool _parseTransform( btTransform& tr, XMLElement* xml )
    {
        tr.setIdentity();

        btVector3 vec(0, 0, 0);

        const char* xyz_str = xml->Attribute("xyz");

        if (xyz_str)
        {
            parseVector3(vec, std::string(xyz_str));
        }

        tr.setOrigin(vec);


        const char* rpy_str = xml->Attribute("rpy");
        if (rpy_str != NULL)
        {
            btVector3 rpy;
            if (parseVector3(rpy, std::string(rpy_str)))
            {
                double phi, the, psi;
                double roll = rpy[0];
                double pitch = rpy[1];
                double yaw = rpy[2];

                phi = roll / 2.0;
                the = pitch / 2.0;
                psi = yaw / 2.0;

                btQuaternion orn(
                    sin(phi) * cos(the) * cos(psi) - cos(phi) * sin(the) * sin(psi),
                    cos(phi) * sin(the) * cos(psi) + sin(phi) * cos(the) * sin(psi),
                    cos(phi) * cos(the) * sin(psi) - sin(phi) * sin(the) * cos(psi),
                    cos(phi) * cos(the) * cos(psi) + sin(phi) * sin(the) * sin(psi));

                orn.normalize();
                tr.setRotation(orn);
            }
        }

        return true;
    }

    bool _parseInertia(UrdfInertia& inertia, XMLElement* config)
    {
        inertia.linkLocalFrame.setIdentity();
        inertia.mass = 0.f;

        // Origin
        XMLElement* o = config->FirstChildElement("origin");
        if (o)
        {
            if (!parseTransform(inertia.linkLocalFrame, o))
            {
                return false;
            }
        }

        XMLElement* mass_xml = config->FirstChildElement("mass");
        if (!mass_xml)
        {
            _logError("Inertial element must have a mass element");
            return false;
        }

        if (!mass_xml->Attribute("value"))
        {
            _logError("Inertial: mass element must have value attribute");
            return false;
        }

        inertia.mass = urdfLexicalCast<double>(mass_xml->Attribute("value"));

        XMLElement* inertia_xml = config->FirstChildElement("inertia");
        if (!inertia_xml)
        {
            _logError("Inertial element must have inertia element");
            return false;
        }

        if (!(inertia_xml->Attribute("ixx") && inertia_xml->Attribute("ixy") && inertia_xml->Attribute("ixz") &&
              inertia_xml->Attribute("iyy") && inertia_xml->Attribute("iyz") &&
              inertia_xml->Attribute("izz")))
        {
            if ((inertia_xml->Attribute("ixx") && inertia_xml->Attribute("iyy") &&
                 inertia_xml->Attribute("izz")))
            {
                inertia.ixx = urdfLexicalCast<double>(inertia_xml->Attribute("ixx"));
                inertia.ixy = 0;
                inertia.ixz = 0;
                inertia.iyy = urdfLexicalCast<double>(inertia_xml->Attribute("iyy"));
                inertia.iyz = 0;
                inertia.izz = urdfLexicalCast<double>(inertia_xml->Attribute("izz"));
            }
            else
            {
                _logError("Inertial: inertia element must have ixx,ixy,ixz,iyy,iyz,izz attributes");
                return false;
            }
        }
        else
        {
            inertia.ixx = urdfLexicalCast<double>(inertia_xml->Attribute("ixx"));
            inertia.ixy = urdfLexicalCast<double>(inertia_xml->Attribute("ixy"));
            inertia.ixz = urdfLexicalCast<double>(inertia_xml->Attribute("ixz"));
            inertia.iyy = urdfLexicalCast<double>(inertia_xml->Attribute("iyy"));
            inertia.iyz = urdfLexicalCast<double>(inertia_xml->Attribute("iyz"));
            inertia.izz = urdfLexicalCast<double>(inertia_xml->Attribute("izz"));
        }

        return true;
    }

    bool _parseGeometry(UrdfGeometry& geom, XMLElement* g)
    {
        //  btAssert(g);
        if (g == 0)
            return false;

        XMLElement* shape = g->FirstChildElement();
        if (!shape)
        {
            _logError("Geometry tag contains no child element.");
            return false;
        }

        //const std::string type_name = shape->ValueTStr().c_str();
        const std::string type_name = shape->Value();
        if (type_name == "sphere")
        {
            geom.type = "sphere";
            
            if (!shape->Attribute("radius"))
            {
                _logError("Sphere shape must have a radius attribute");
                return false;
            }
            else
            {
                geom.sphereRadius = urdfLexicalCast<double>(shape->Attribute("radius"));
            }
        }
        else if ( type_name == "box" )
        {
            geom.type = "box";
            
            if (!shape->Attribute("size"))
            {
                _logError("box requires a size attribute");
                return false;
            }
            else
            {
                parseVector3(geom.boxSize, shape->Attribute("size"));
            }
        }
        else if (type_name == "cylinder")
        {
            geom.type = "cylinder";
            geom.hasFromTo = false;
            geom.capsuleRadius = 0.1;
            geom.capsuleHeight = 0.1;


            if (!shape->Attribute("length") || !shape->Attribute("radius"))
            {
                _logError("Cylinder shape must have both length and radius attributes");
                return false;
            }
            geom.capsuleRadius = urdfLexicalCast<double>(shape->Attribute("radius"));
            geom.capsuleHeight = urdfLexicalCast<double>(shape->Attribute("length"));
        }
        else if (type_name == "capsule")
        {
            geom.type = "capsule";
            geom.hasFromTo = false;

            if (!shape->Attribute("length") || !shape->Attribute("radius"))
            {
                _logError("Capsule shape must have both length and radius attributes");
                return false;
            }
            geom.capsuleRadius = urdfLexicalCast<double>(shape->Attribute("radius"));
            geom.capsuleHeight = urdfLexicalCast<double>(shape->Attribute("length"));
        }
        else if ((type_name == "mesh") || (type_name == "cdf"))
        {
            if ((type_name == "cdf"))
            {
                geom.type = "cdf";
            }
            else
            {
                geom.type = "mesh";
            }
            geom.meshScale.setValue(1, 1, 1);
            std::string fn;

            // URDF
            if (shape->Attribute("filename"))
            {
                fn = shape->Attribute("filename");
            }
            if (shape->Attribute("scale"))
            {
                if (!parseVector3(geom.meshScale, shape->Attribute("scale")))
                {
                    _logWarning("Scale should be a vector3, not single scalar. Workaround activated.\n");
                    std::string scalar_str = shape->Attribute("scale");
                    double scaleFactor = urdfLexicalCast<double>(scalar_str.c_str());
                    if (scaleFactor)
                    {
                        geom.meshScale.setValue(scaleFactor, scaleFactor, scaleFactor);
                    }
                }
            }

            if (fn.empty())
            {
                _logError("Mesh filename is empty");
                return false;
            }

            geom.meshFileName = fn;
        }
        else
        {
            if (type_name == "plane")
            {
                geom.type = "plane";
                
                if (!shape->Attribute("normal"))
                {
                    _logError("plane requires a normal attribute");
                    return false;
                }
                else
                {
                    parseVector3(geom.planeNormal, shape->Attribute("normal"));
                }
            }
            else
            {
                _logError("Unknown geometry type:");
                _logError(type_name.c_str());
                return false;
            }
        }

        return true;
    }

    bool _parseCollision(UrdfCollision& collision, XMLElement* config)
    {
        collision.linkLocalFrame.setIdentity();

        // Origin
        XMLElement* o = config->FirstChildElement("origin");
        if (o)
        {
            if (!parseTransform(collision.linkLocalFrame, o))
                return false;
        }
        // Geometry
        XMLElement* geom = config->FirstChildElement("geometry");
        if (!parseGeometry(collision.geometry, geom))
        {
            return false;
        }

        {
            const char* group_char = config->Attribute("group");
            if (group_char)
            {
                collision.flags |= URDF_HAS_COLLISION_GROUP;
                collision.collisionGroup = urdfLexicalCast<int>(group_char);
            }
        }

        {
            const char* mask_char = config->Attribute("mask");
            if (mask_char)
            {
                collision.flags |= URDF_HAS_COLLISION_MASK;
                collision.collisionMask = urdfLexicalCast<int>(mask_char);
            }
        }

        const char* name_char = config->Attribute("name");
        if (name_char)
            collision.name = name_char;

        const char* concave_char = config->Attribute("concave");
        if (concave_char)
            collision.flags |= URDF_FORCE_CONCAVE_TRIMESH;

        return true;
    }

    bool _parseVisual(UrdfModel& model, UrdfVisual& visual, XMLElement* config)
    {
        visual.linkLocalFrame.setIdentity();

        // Origin
        XMLElement* o = config->FirstChildElement("origin");
        if (o)
        {
            if (!parseTransform(visual.linkLocalFrame, o))
                return false;
        }
        // Geometry
        XMLElement* geom = config->FirstChildElement("geometry");
        if (!parseGeometry(visual.geometry, geom))
        {
            return false;
        }

        const char* name_char = config->Attribute("name");
        if (name_char)
            visual.name = name_char;

        visual.geometry.hasLocalMaterial = false;

        // Material
        XMLElement* mat = config->FirstChildElement("material");
    
        if (mat)
        {

            // get material name
            if (!mat->Attribute("name"))
            {
                _logError("Visual material must contain a name attribute");
                return false;
            }
            visual.materialName = mat->Attribute("name");

            // try to parse material element in place

            XMLElement* t = mat->FirstChildElement("texture");
            XMLElement* c = mat->FirstChildElement("color");
            XMLElement* s = mat->FirstChildElement("specular");
            if (t || c || s)
            {
                if (parseMaterial(visual.geometry.localMaterial, mat))
                {
                    UrdfMaterial* matPtr = new UrdfMaterial(visual.geometry.localMaterial);

                    UrdfMaterial** oldMatPtrPtr = model.materials[matPtr->name.c_str()];
                    if (oldMatPtrPtr)
                    {
                        UrdfMaterial* oldMatPtr = *oldMatPtrPtr;
                        model.materials.remove(matPtr->name.c_str());
                        if (oldMatPtr)
                            delete oldMatPtr;
                    }
                    model.materials.insert(matPtr->name.c_str(), matPtr);
                    visual.geometry.hasLocalMaterial = true;
                }
            }
        }

        return true;
    }

    bool _parseLink(UrdfModel& model, UrdfLink& link, XMLElement* config)
    {
        const char* linkName = config->Attribute("name");
        if (!linkName)
        {
            _logError("Link with no name");
            return false;
        }
        link.name = linkName;

        {
            //optional 'contact' parameters
            XMLElement* ci = config->FirstChildElement("contact");
            if (ci)
            {
                XMLElement* damping_xml = ci->FirstChildElement("inertia_scaling");
                if (damping_xml)
                {
                        if (!damping_xml->Attribute("value"))
                        {
                            _logError("Link/contact: damping element must have value attribute");
                            return false;
                        }

                        link.contactInfo.inertiaScaling = urdfLexicalCast<double>(damping_xml->Attribute("value"));
                        link.contactInfo.flags |= URDF_CONTACT_HAS_INERTIA_SCALING;
                }
                {
                    XMLElement* friction_xml = ci->FirstChildElement("lateral_friction");
                    if (friction_xml)
                    {
                        if (!friction_xml->Attribute("value"))
                        {
                            _logError("Link/contact: lateral_friction element must have value attribute");
                            return false;
                        }

                        link.contactInfo.lateralFriction = urdfLexicalCast<double>(friction_xml->Attribute("value"));
                    }
                }

                {
                    XMLElement* rolling_xml = ci->FirstChildElement("rolling_friction");
                    if (rolling_xml)
                    {
                        if (!rolling_xml->Attribute("value"))
                        {
                            _logError("Link/contact: rolling friction element must have value attribute");
                            return false;
                        }

                        link.contactInfo.rollingFriction = urdfLexicalCast<double>(rolling_xml->Attribute("value"));
                        link.contactInfo.flags |= URDF_CONTACT_HAS_ROLLING_FRICTION;
                    }
                }

                {
                    XMLElement* restitution_xml = ci->FirstChildElement("restitution");
                    if (restitution_xml)
                    {
                        if (!restitution_xml->Attribute("value"))
                        {
                            _logError("Link/contact: restitution element must have value attribute");
                            return false;
                        }

                        link.contactInfo.restitution = urdfLexicalCast<double>(restitution_xml->Attribute("value"));
                        link.contactInfo.flags |= URDF_CONTACT_HAS_RESTITUTION;
                    }
                }

                {
                    XMLElement* spinning_xml = ci->FirstChildElement("spinning_friction");
                    if (spinning_xml)
                    {
                        if (!spinning_xml->Attribute("value"))
                        {
                            _logError("Link/contact: spinning friction element must have value attribute");
                            return false;
                        }

                        link.contactInfo.spinningFriction = urdfLexicalCast<double>(spinning_xml->Attribute("value"));
                        link.contactInfo.flags |= URDF_CONTACT_HAS_SPINNING_FRICTION;
                    }
                }
                {
                    XMLElement* friction_anchor = ci->FirstChildElement("friction_anchor");
                    if (friction_anchor)
                    {
                        link.contactInfo.flags |= URDF_CONTACT_HAS_FRICTION_ANCHOR;
                    }
                }
                {
                    XMLElement* stiffness_xml = ci->FirstChildElement("stiffness");
                    if (stiffness_xml)
                    {
                        if (!stiffness_xml->Attribute("value"))
                        {
                            _logError("Link/contact: stiffness element must have value attribute");
                            return false;
                        }

                        link.contactInfo.contactStiffness = urdfLexicalCast<double>(stiffness_xml->Attribute("value"));
                        link.contactInfo.flags |= URDF_CONTACT_HAS_STIFFNESS_DAMPING;
                    }
                }
                {
                    XMLElement* damping_xml = ci->FirstChildElement("damping");
                    if (damping_xml)
                    {
                        if (!damping_xml->Attribute("value"))
                        {
                            _logError("Link/contact: damping element must have value attribute");
                            return false;
                        }

                        link.contactInfo.contactDamping = urdfLexicalCast<double>(damping_xml->Attribute("value"));
                        link.contactInfo.flags |= URDF_CONTACT_HAS_STIFFNESS_DAMPING;
                    }
                }
            }
        }

        // Inertial (optional)
        XMLElement* i = config->FirstChildElement("inertial");
        if (i)
        {
            if (!parseInertia(link.inertia, i))
            {
                _logError("Could not parse inertial element for Link:");
                _logError(link.name.c_str());
                return false;
            }
        }
        else
        {
            if ((strlen(linkName) == 5) && (strncmp(linkName, "world", 5)) == 0)
            {
                link.inertia.mass = 0.f;
                link.inertia.linkLocalFrame.setIdentity();
                link.inertia.ixx = 0.f;
                link.inertia.iyy = 0.f;
                link.inertia.izz = 0.f;
            }
            else
            {
                _logWarning("No inertial data for link, using mass=1, localinertiadiagonal = 1,1,1, identity local inertial frame");
                link.inertia.mass = 1.f;
                link.inertia.linkLocalFrame.setIdentity();
                link.inertia.ixx = 1.f;
                link.inertia.iyy = 1.f;
                link.inertia.izz = 1.f;
                _logWarning(link.name.c_str());
            }
        }

        // Multiple Visuals (optional)
        for ( XMLElement* vis_xml = config->FirstChildElement("visual"); vis_xml; vis_xml = vis_xml->NextSiblingElement("visual") )
        {
            UrdfVisual visual;

            if (parseVisual(model, visual, vis_xml))
            {
                link.visualArray.push_back(visual);
            }
            else
            {
                _logError("Could not parse visual element for Link:");
                _logError(link.name.c_str());
                return false;
            }
        }

        // Multiple Collisions (optional)
        for (XMLElement* col_xml = config->FirstChildElement("collision"); col_xml; col_xml = col_xml->NextSiblingElement("collision"))
        {
            UrdfCollision col;

            if (parseCollision(col, col_xml))
            {
                link.collisionArray.push_back(col);
            }
            else
            {
                _logError("Could not parse collision element for Link:");
                _logError(link.name.c_str());
                return false;
            }
        }
        return true;
    }

    bool _parseJointLimits(UrdfJoint& joint, XMLElement* config)
    {
        joint.lowerLimit = 0.f;
        joint.upperLimit = -1.f;
        joint.effortLimit = 0.f;
        joint.velocityLimit = 0.f;
        joint.jointDamping = 0.f;
        joint.jointFriction = 0.f;

        const char* lower_str = config->Attribute("lower");
        if (lower_str)
        {
            joint.lowerLimit = urdfLexicalCast<double>(lower_str);
        }

        const char* upper_str = config->Attribute("upper");
        if (upper_str)
        {
            joint.upperLimit = urdfLexicalCast<double>(upper_str);
        }

        // Get joint effort limit
        const char* effort_str = config->Attribute("effort");
        if (effort_str)
        {
            joint.effortLimit = urdfLexicalCast<double>(effort_str);
        }

        // Get joint velocity limit
        const char* velocity_str = config->Attribute("velocity");
        if (velocity_str)
        {
            joint.velocityLimit = urdfLexicalCast<double>(velocity_str);
        }

        return true;
    }

    bool _parseJointDynamics(UrdfJoint& joint, XMLElement* config)
    {
        joint.jointDamping = 0;
        joint.jointFriction = 0;

        // Get joint damping
        const char* damping_str = config->Attribute("damping");
        if (damping_str)
        {
            joint.jointDamping = urdfLexicalCast<double>(damping_str);
        }

        // Get joint friction
        const char* friction_str = config->Attribute("friction");
        if (friction_str)
        {
            joint.jointFriction = urdfLexicalCast<double>(friction_str);
        }

        if (damping_str == NULL && friction_str == NULL)
        {
            _logError("joint dynamics element specified with no damping and no friction");
            return false;
        }

        return true;
    }

    bool _parseJoint(UrdfJoint& joint, XMLElement* config)
    {
        // Get Joint Name
        const char* name = config->Attribute("name");
        if (!name)
        {
            _logError("unnamed joint found");
            return false;
        }
        joint.name = name;
        joint.parentLinkToJointTransform.setIdentity();

        // Get transform from Parent Link to Joint Frame
        XMLElement* origin_xml = config->FirstChildElement("origin");
        if (origin_xml)
        {
            if (!parseTransform(joint.parentLinkToJointTransform, origin_xml))
            {
                _logError("Malformed parent origin element for joint:");
                _logError(joint.name.c_str());
                return false;
            }
        }

        // Get Parent Link
        XMLElement* parent_xml = config->FirstChildElement("parent");
        if (parent_xml)
        {
            const char* pname = parent_xml->Attribute("link");
            if (!pname)
            {
                _logError("no parent link name specified for Joint link. this might be the root?");
                _logError(joint.name.c_str());
                return false;
            }
            else
            {
                joint.parentLinkName = std::string(pname);
            }
        }

        // Get Child Link
        XMLElement* child_xml = config->FirstChildElement("child");
        if (child_xml)
        {
            const char* pname = child_xml->Attribute("link");
            if (!pname)
            {
                _logError("no child link name specified for Joint link [%s].");
                _logError(joint.name.c_str());
                return false;
            }
            else
            {
                joint.childLinkName = std::string(pname);
            }
        }

        // Get Joint type
        const char* _typeChar = config->Attribute( "type" );
        if ( !_typeChar )
        {
            std::string _msg;
            _msg += "Joint ";
            _msg += joint.name + " ";
            _msg += "has no type, check to see if it's a reference";

            _logError( _msg );

            return false;
        }

        std::string _typeStr = _typeChar;
        if ( _typeStr == "spherical" || _typeStr == "planar" || 
             _typeStr == "floating" || _typeStr == "revolute" ||
             _typeStr == "continuous" || _typeStr == "prismatic" ||
             _typeStr == "fixed" )
        {
            joint.type = _typeStr;
        }
        else
        {
            std::string _msg = "Joint ";
            _msg += joint.name + " ";
            _msg += "has unknown type: "
            _msg += _typeStr;

            _logError( _msg );

            return false;
        }

        // Get Joint Axis
        if ( joint.type != "floating" && joint.type != "fixed" )
        {
            // axis
            XMLElement* _axisXml = config->FirstChildElement( "axis" );
            if (!_axisXml)
            {
                _logWarning("urdfdom: no axis elemement for Joint, defaulting to (1,0,0) axis");
                _logWarning(joint.name.c_str());
                joint.localJointAxis.setValue(1, 0, 0);
            }
            else
            {
                if (_axisXml->Attribute("xyz"))
                {
                    if (!parseVector3(joint.localJointAxis, _axisXml->Attribute("xyz")))
                    {
                        _logError("Malformed axis element:");
                        _logError(joint.name.c_str());
                        _logError(" for joint:");
                        _logError(_axisXml->Attribute("xyz"));
                        return false;
                    }
                }
            }
        }

        // Get limit
        XMLElement* limit_xml = config->FirstChildElement("limit");
        if (limit_xml)
        {
            if (!parseJointLimits(joint, limit_xml))
            {
                _logError("Could not parse limit element for joint:");
                _logError(joint.name.c_str());
                return false;
            }
        }
        else if (joint.type == URDFRevoluteJoint)
        {
            _logError("Joint is of type REVOLUTE but it does not specify limits");
            _logError(joint.name.c_str());
            return false;
        }
        else if (joint.type == URDFPrismaticJoint)
        {
            _logError("Joint is of type PRISMATIC without limits");
            _logError(joint.name.c_str());
            return false;
        }

        joint.jointDamping = 0;
        joint.jointFriction = 0;

        // Get Dynamics
        XMLElement* prop_xml = config->FirstChildElement("dynamics");
        if (prop_xml)
        {
            // Get joint damping
            const char* damping_str = prop_xml->Attribute("damping");
            if (damping_str)
            {
                joint.jointDamping = urdfLexicalCast<double>(damping_str);
            }

            // Get joint friction
            const char* friction_str = prop_xml->Attribute("friction");
            if (friction_str)
            {
                joint.jointFriction = urdfLexicalCast<double>(friction_str);
            }

            if (damping_str == NULL && friction_str == NULL)
            {
                _logError("joint dynamics element specified with no damping and no friction");
                return false;
            }
        }

        return true;
    }

    bool _initTreeAndRoot(UrdfModel& model)
    {
        // every link has children links and joints, but no parents, so we create a
        // local convenience data structure for keeping child->parent relations
        btHashMap<btHashString, btHashString> parentLinkTree;

        // loop through all joints, for every link, assign children links and children joints
        for (int i = 0; i < model.joints.size(); i++)
        {
            UrdfJoint** jointPtr = model.joints.getAtIndex(i);
            if (jointPtr)
            {
                UrdfJoint* joint = *jointPtr;
                std::string parent_link_name = joint->parentLinkName;
                std::string child_link_name = joint->childLinkName;
                if (parent_link_name.empty() || child_link_name.empty())
                {
                    _logError("parent link or child link is empty for joint");
                    _logError(joint->name.c_str());
                    return false;
                }

                UrdfLink** childLinkPtr = model.links.find(joint->childLinkName.c_str());
                if (!childLinkPtr)
                {
                    _logError("Cannot find child link for joint ");
                    _logError(joint->name.c_str());

                    return false;
                }
                UrdfLink* childLink = *childLinkPtr;

                UrdfLink** parentLinkPtr = model.links.find(joint->parentLinkName.c_str());
                if (!parentLinkPtr)
                {
                    _logError("Cannot find parent link for a joint");
                    _logError(joint->name.c_str());
                    return false;
                }
                UrdfLink* parentLink = *parentLinkPtr;

                childLink->parentLink = parentLink;

                childLink->parentJoint = joint;
                parentLink->childJoints.push_back(joint);
                parentLink->childLinks.push_back(childLink);
                parentLinkTree.insert(childLink->name.c_str(), parentLink->name.c_str());
            }
        }

        //search for children that have no parent, those are 'root'
        for (int i = 0; i < model.links.size(); i++)
        {
            UrdfLink** linkPtr = model.links.getAtIndex(i);
            btAssert(linkPtr);
            if (linkPtr)
            {
                UrdfLink* link = *linkPtr;
                link->linkIndex = i;

                if (!link->parentLink)
                {
                    model.rootLinks.push_back(link);
                }
            }
        }

        if (model.rootLinks.size() > 1)
        {
            _logWarning("URDF file with multiple root links found");
        }

        if (model.rootLinks.size() == 0)
        {
            _logError("URDF without root link found");
            return false;
        }
        return true;
    }

}}