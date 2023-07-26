// Copyright 2017-2023, Nicholas Sharp and the Polyscope contributors. https://polyscope.run


#include "polyscope/polyscope.h"

#include "polyscope/color_render_image_quantity.h"

#include "imgui.h"
#include "polyscope/render/engine.h"

namespace polyscope {


ColorRenderImageQuantity::ColorRenderImageQuantity(Structure& parent_, std::string name, size_t dimX, size_t dimY,
                                                   const std::vector<float>& depthData,
                                                   const std::vector<glm::vec3>& normalData,
                                                   const std::vector<glm::vec3>& colorsData_, ImageOrigin imageOrigin)
    : RenderImageQuantityBase(parent_, name, dimX, dimY, depthData, normalData, imageOrigin),
      colors("colors", colorsData), colorsData(colorsData_) {
  colors.setTextureSize(dimX, dimY);
}

void ColorRenderImageQuantity::draw() {}

void ColorRenderImageQuantity::drawDelayed() {
  if (!isEnabled()) return;

  if (!program) prepare();

  // set uniforms
  glm::mat4 P = view::getCameraPerspectiveMatrix();
  glm::mat4 Pinv = glm::inverse(P);

  program->setUniform("u_projMatrix", glm::value_ptr(P));
  program->setUniform("u_invProjMatrix", glm::value_ptr(Pinv));
  program->setUniform("u_viewport", render::engine->getCurrentViewport());
  program->setUniform("u_transparency", transparency.get());

  // make sure we have actual depth testing enabled
  render::engine->setDepthMode(DepthMode::LEqual);
  // render::engine->applyTransparencySettings();
  render::engine->setBlendMode(BlendMode::Over);

  // draw
  program->draw();
}

void ColorRenderImageQuantity::buildCustomUI() {
  ImGui::SameLine();

  // == Options popup
  if (ImGui::Button("Options")) {
    ImGui::OpenPopup("OptionsPopup");
  }
  if (ImGui::BeginPopup("OptionsPopup")) {

    RenderImageQuantityBase::addOptionsPopupEntries();

    ImGui::EndPopup();
  }
}


void ColorRenderImageQuantity::refresh() {
  program = nullptr;
  textureColor = nullptr;
  RenderImageQuantityBase::refresh();
}


void ColorRenderImageQuantity::prepare() {

  // Create the sourceProgram
  program = render::engine->requestShader("TEXTURE_DRAW_RENDERIMAGE_PLAIN",
                                          {getImageOriginRule(imageOrigin), "LIGHT_MATCAP", "TEXTURE_SHADE_COLOR"},
                                          render::ShaderReplacementDefaults::Process);

  program->setAttribute("a_position", render::engine->screenTrianglesCoords());
  program->setTextureFromBuffer("t_depth", depths.getRenderTextureBuffer().get());
  program->setTextureFromBuffer("t_normal", normals.getRenderTextureBuffer().get());
  program->setTextureFromBuffer("t_color", colors.getRenderTextureBuffer().get());
  render::engine->setMaterial(*program, material.get());
}


std::string ColorRenderImageQuantity::niceName() { return name + " (color render image)"; }

ColorRenderImageQuantity* ColorRenderImageQuantity::setEnabled(bool newEnabled) {
  enabled = newEnabled;
  requestRedraw();
  return this;
}


// Instantiate a construction helper which is used to avoid header dependencies. See forward declaration and note in
// structure.ipp.
ColorRenderImageQuantity* createColorRenderImage(Structure& parent, std::string name, size_t dimX, size_t dimY,
                                                 const std::vector<float>& depthData,
                                                 const std::vector<glm::vec3>& normalData,
                                                 const std::vector<glm::vec3>& colorData, ImageOrigin imageOrigin) {
  return new ColorRenderImageQuantity(parent, name, dimX, dimY, depthData, normalData, colorData, imageOrigin);
}

} // namespace polyscope
