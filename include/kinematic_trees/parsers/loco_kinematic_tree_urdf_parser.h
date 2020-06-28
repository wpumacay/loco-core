#pragma once

#include <tuple>
#include <utils/loco_parsing_element.h>
#include <kinematic_trees/loco_kinematic_tree.h>

namespace loco {
namespace kintree {
    class TKinematicTree;
    class TKinematicTreeBody;
    class TKinematicTreeCollider;
    class TKinematicTreeJoint;
}}

namespace loco {
namespace kintree {

    class TKinematicTreeUrdfParser
    {
    public :

        TKinematicTreeUrdfParser( TKinematicTree* kintree_ref ) : m_KintreeRef( kintree_ref ) {}

        TKinematicTreeUrdfParser( const TKinematicTreeUrdfParser& other ) = delete;

        TKinematicTreeUrdfParser& operator=( const TKinematicTreeUrdfParser& other ) = delete;

        ~TKinematicTreeUrdfParser() = default;

        void parse_kintree( const std::string& filepath_urdf );

        void parse_kintree( const parsing::TElement* model_urdf );

        std::unique_ptr<TKinematicTreeBody> parse_body( const parsing::TElement* body_elm );

        std::unique_ptr<TKinematicTreeJoint> parse_joint( const parsing::TElement* joint_elm );

        std::pair<std::unique_ptr<TKinematicTreeCollider>, TMat4> parse_collider( const parsing::TElement* collider_elm, const std::string& body_parent_name );

        std::pair<std::unique_ptr<visualizer::TDrawable>, TMat4> parse_drawable( const parsing::TElement* drawable_elm, const std::string& body_parent_name );

        TInertialData parse_inertia( const parsing::TElement* inertial_elm );

    private :

        void parse_kintree();

        void collect_materials( const parsing::TElement* robot_elm );

        void collect_bodies( const parsing::TElement* robot_elm );

        void collect_joints( const parsing::TElement* robot_elm );

        void connect_kintree();

        void reorder_treelike( std::vector<std::tuple<std::string, std::string, std::string, TMat4>>& connections );

        TMat4 get_transform( const parsing::TElement* origin_elm );

        void get_shape_information( const parsing::TElement* geometry_elm, TShapeData& dst_shape_data );

    private :

        /// Reference to the kinematic-tree being constructed from the loaded mjcf-model
        TKinematicTree* m_KintreeRef = nullptr;
        /// Model data used to construct the kinematic-tree
        std::unique_ptr<parsing::TElement> m_UrdfModel = nullptr;
        /// Temporal storage for the bodies created during parsing of the model
        std::unordered_map<std::string, std::unique_ptr<TKinematicTreeBody>> m_TempStorageBodies;
        /// Temporal storage for the joints created during parsing of the model
        std::unordered_map<std::string, std::unique_ptr<TKinematicTreeJoint>> m_TempStorageJoints;
        /// Connections that the joints make between links <joint-name,parent-link, child-link, relative-transform>
        std::vector<std::tuple<std::string, std::string, std::string, TMat4>> m_JointLinkConnections;
        /// Container for materials found in the urdf model being parse
        std::unordered_map<std::string, TGenericParams> m_Materials;
        /// File path used to load the current model
        std::string m_CurrentFilepath = "";
        /// Folder path from where the model being parsed is loaded
        std::string m_CurrentFolderpath = "";

    private :

        static ssize_t NUM_UNNAMED_DRAWABLES;
        static ssize_t NUM_UNNAMED_COLLIDERS;
        static ssize_t NUM_UNNAMED_JOINTS;
        static ssize_t NUM_UNNAMED_BODIES;
    };

}}