#pragma once

#include "polyscope/render/opengl/gl_shaders.h"

namespace polyscope {
namespace render {
namespace backend_openGL3_glfw {

// High level pipeline
extern const ShaderStageSpecification FEEDBACK_GATHER_FLOAT3_VERT_SHADER;

// Rules for feedback shaders
// extern const ShaderReplacementRule SPHERE_PROPAGATE_VALUE;


} // namespace backend_openGL3_glfw
} // namespace render
} // namespace polyscope
