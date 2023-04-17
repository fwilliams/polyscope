// Copyright 2017-2023, Nicholas Sharp and the Polyscope contributors. https://polyscope.run

#include "polyscope/render/opengl/shaders/feedback_shaders.h"


namespace polyscope {
namespace render {
namespace backend_openGL3_glfw {

// clang-format off

const ShaderStageSpecification FEEDBACK_GATHER_FLOAT3_VERT_SHADER = {

    ShaderStageType::Vertex,

    // uniforms
    {}, 

    // attributes
    { 
      {"a_val_in", RenderDataType::Vector3Float}, 
      {"a_val_out", RenderDataType::Vector3Float, ShaderAttributePurpose::FeedbackOutput}, 
    },

    {}, // textures

    // source
R"(
        ${ GLSL_VERSION }$

        in vec3 a_val_in;
        out vec3 a_val_out;

        void main() { 
          a_val_out = a_val_in; 
        }
)"
};

// clang-format on

} // namespace backend_openGL3_glfw
} // namespace render
} // namespace polyscope
