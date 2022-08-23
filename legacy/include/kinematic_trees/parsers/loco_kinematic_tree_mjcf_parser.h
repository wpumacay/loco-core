#pragma once

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

    const std::string MJCF_WORLDBODY_TAG = "worldbody";
    const std::string MJCF_BODY_TAG = "body";
    const std::string MJCF_GEOM_TAG = "geom";
    const std::string MJCF_JOINT_TAG = "joint";

    class TKinematicTreeMjcfParser
    {
    public :

        TKinematicTreeMjcfParser( TKinematicTree* kintree_ref ) : m_KintreeRef( kintree_ref ) {}

        TKinematicTreeMjcfParser( const TKinematicTreeMjcfParser& other ) = delete;

        TKinematicTreeMjcfParser& operator=( const TKinematicTreeMjcfParser& other ) = delete;

        ~TKinematicTreeMjcfParser() = default;

        void parse_kintree( const std::string& filepath_mjcf );

        void parse_kintree( const parsing::TElement* model_mjcf );

        std::pair<std::unique_ptr<TKinematicTreeBody>, TMat4> parse_body( const parsing::TElement* body_elm );

        std::pair<std::unique_ptr<TKinematicTreeCollider>, TMat4> parse_primitive_collider( const parsing::TElement* collider_elm );

        std::unique_ptr<TKinematicTreeCollider> parse_compound_collider( std::vector<const parsing::TElement*> colliders_elms, const std::string& collider_name );

        std::pair<std::unique_ptr<visualizer::TDrawable>, TMat4> parse_primitive_drawable( const parsing::TElement* drawable_elm );

        std::unique_ptr<visualizer::TDrawable> parse_compound_drawable( std::vector<const parsing::TElement*> drawables_elms, const std::string& drawable_name );

        std::pair<std::unique_ptr<TKinematicTreeJoint>, TMat4> parse_joint( const parsing::TElement* joint_elm );

        TInertialData parse_inertia( const parsing::TElement* inertial_elm );

    private :

        void parse_kintree();

        void collect_settings( const parsing::TElement* model_elm );

        void collect_assets( const parsing::TElement* assets_elm );

        void collect_defaults_with_class( const parsing::TElement* defaults_elm );

        void collect_defaults_with_no_class( const parsing::TElement* defaults_elm );

        std::string get_string( const parsing::TElement* elm, const std::string& attrib_id, const std::string& default_value ) const;

        ssize_t get_int( const parsing::TElement* elm, const std::string& attrib_id, const ssize_t& default_value ) const;

        float get_float( const parsing::TElement* elm, const std::string& attrib_id, const float& default_value ) const;

        TVec2 get_vec2( const parsing::TElement* elm, const std::string& attrib_id, const TVec2& default_value ) const;

        TVec3 get_vec3( const parsing::TElement* elm, const std::string& attrib_id, const TVec3& default_value ) const;

        TVec4 get_vec4( const parsing::TElement* elm, const std::string& attrib_id, const TVec4& default_value ) const;

        TSizei get_array_int( const parsing::TElement* elm, const std::string& attrib_id, const TSizei& default_value ) const;

        TSizef get_array_float( const parsing::TElement* elm, const std::string& attrib_id, const TSizef& default_value ) const;

        TMat4 get_transform( const parsing::TElement* elm ) const;

        TVec3 get_standard_size( const parsing::TElement* elm ) const;

        std::string get_class( const parsing::TElement* elm, const std::string& attrib_id ) const;

        bool has_default_no_class( const std::string& elm_type, const std::string& attrib_id ) const;

        bool has_default_from_class( const std::string& elm_type, const std::string& attrib_id, const std::string& elm_class ) const;

        TMat4 convert_global_to_local_transform( const TMat4& parent_world_tf, const TMat4& this_world_tf );

        bool is_planar_group( const std::vector<const parsing::TElement*>& joints_elms );

        std::unique_ptr<TKinematicTreeJoint> make_planar_joint( const std::vector<const parsing::TElement*>& joints_elms,
                                                                const std::string& joint_name );

    private :

        /// Reference to the kinematic-tree being constructed from the loaded mjcf-model
        TKinematicTree* m_KintreeRef = nullptr;
        /// Model data used to construct the kinematic-tree
        std::unique_ptr<parsing::TElement> m_MjcfModel = nullptr;
        /// Container for defaults of the model that are not related to specific class id
        std::unordered_map<std::string, TGenericParams> m_DefaultsNoClass;
        /// Container for defaults of the model that are related to a specific class-id
        std::unordered_map<std::string, std::unordered_map<std::string, TGenericParams>> m_DefaultsPerClass;
        /// Container for mesh-assets found in the mjcf model being parsed
        std::unordered_map<std::string, TGenericParams> m_AssetsMeshes;
        /// Container for material-assets found in the mjcf model being parse
        std::unordered_map<std::string, TGenericParams> m_AssetsMaterials;
        /// File path used to load the current model
        std::string m_CurrentFilepath = "";
        /// Folder path from where the model being parsed is loaded
        std::string m_CurrentFolderpath = "";
        /// Flag used to indicate whether or not we are using local coordinates
        bool m_OptUseLocalCoordinates = true;
        /// Flag used to indicate whether or not to use degrees or radians for angles
        bool m_OptUseDegrees = true;

    private :

        static ssize_t NUM_UNNAMED_DRAWABLES;
        static ssize_t NUM_UNNAMED_COLLIDERS;
        static ssize_t NUM_UNNAMED_JOINTS;
        static ssize_t NUM_UNNAMED_BODIES;
    };
}}