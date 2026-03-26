#include <pixelforge/core/camera.hpp>
#include <pixelforge/geometry/meshes/mesh.hpp>
#include <pixelforge/graphics/texturing/material.hpp>

#include <pixelforge/scene/nodes/scene.hpp>
#include <pixelforge/scene/nodes/model.hpp>
#include <pixelforge/scene/nodes/render_unit.hpp>
#include <pixelforge/scene/nodes/light_unit.hpp>

#include <pixelforge/scene/lighting/ambient_light.hpp>
#include <pixelforge/scene/lighting/directional_light.hpp>
#include <pixelforge/scene/lighting/positional_light.hpp>
#include <pixelforge/scene/lighting/spot_light.hpp>

#include "core/renderer.hpp"
#include "graphics/pipeline/vertex_array.hpp"
#include "graphics/pipeline/shader.hpp"


namespace pixelforge::core {

    namespace {

        /// Generates a string representation of a uniform array element.
        /// @param uniform The name of the array uniform.
        /// @param index The index of the array element.
        /// @param attribute The attribute name (if any).
        /// @return A string representing the array element.
        const char* getUniformArray(const char* uniform, const size_t index, const char* attribute = nullptr) {
            static char buffer[1024];
            if (attribute == nullptr) {
                snprintf(buffer, sizeof(buffer), "%s[%lu]", uniform, index);
            } else {
                snprintf(buffer, sizeof(buffer), "%s[%lu].%s", uniform, index, attribute);
            }
            return buffer;
        }

    } // namespace


    using namespace scene::lighting;
    using std::unordered_map;
    using std::vector;
    using std::string;
    using std::shared_ptr;
    using std::make_shared;
    using std::dynamic_pointer_cast;


    unordered_map<string, shared_ptr<graphics::Shader>> Renderer::shaders_;
    shared_ptr<graphics::Shader> Renderer::currentShader_ = nullptr;
    shared_ptr<graphics::Material> Renderer::fallbackMaterial_ = nullptr;
    vector<shared_ptr<scene::nodes::LightUnit>> Renderer::fallbackLightUnits_;
    bool Renderer::useFallbackLights_ = true;


    void Renderer::initialize() {
        initializeShader("white");
        initializeShader("emissive");
        initializeShader("lighting");
        fallbackMaterial_ = make_shared<graphics::Material>(graphics::WhiteRubber());
        auto ambientUnit = make_shared<scene::nodes::LightUnit>("FallBack1");
            ambientUnit->setLight(make_shared<AmbientLight>());
        const auto directionalLight = make_shared<DirectionalLight>();
            directionalLight->setDirection({1.0f, -2.0f, -2.0f});
        auto directionalUnit = make_shared<scene::nodes::LightUnit>("FallBack2");
            directionalUnit->setLight(directionalLight);
        fallbackLightUnits_ = {
            ambientUnit,
            directionalUnit
        };
    }


    void Renderer::initializeShader(const string& name) {
        const auto program = make_shared<graphics::Shader>();
        const string vertexShader = name + ".vert";
        const string fragmentShader = name + ".frag";
        program->addModule(GL_VERTEX_SHADER, vertexShader.c_str());
        program->addModule(GL_FRAGMENT_SHADER, fragmentShader.c_str());
        program->activateShader();
        shaders_[name] = program;
    }


    void Renderer::prepareFrame(const RenderMode mode, const glm::vec4 clearColor) {
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, mode == WIREFRAME ? GL_LINE : GL_FILL);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }


    void Renderer::setShader(const string& name) { // NOLINT
        if (shaders_.contains(name) && currentShader_ != shaders_.at(name)) {
            graphics::Shader::unbindShader();
            currentShader_ = shaders_.at(name);
        }
    }


    void Renderer::setCameraUniforms() { // NOLINT
        currentShader_->setUniform("viewMatrix", Camera::getViewMatrix());
        currentShader_->setUniform("projectionMatrix", Camera::getProjectionMatrix());
        currentShader_->setUniform("cameraPosition", Camera::getPosition());
    }


    void Renderer::setLightingUniforms(const vector<shared_ptr<scene::nodes::LightUnit>>& lightNodes) { // NOLINT
        if (currentShader_ != shaders_.at("lighting")) setShader("lighting");
        currentShader_->setUniform("lightCount", static_cast<int>(lightNodes.size()));
        for (size_t i = 0; i < lightNodes.size(); i++) {
            auto light = lightNodes.at(i)->getLight();
            currentShader_->setUniform(getUniformArray("lights", i, "type"), static_cast<int>(light->getType()));
            currentShader_->setUniform(getUniformArray("lights", i, "ambient.color"), light->getAmbient().color);
            currentShader_->setUniform(getUniformArray("lights", i, "ambient.intensity"), light->getAmbient().intensity);
            currentShader_->setUniform(getUniformArray("lights", i, "diffuse.color"), light->getDiffuse().color);
            currentShader_->setUniform(getUniformArray("lights", i, "diffuse.intensity"), light->getDiffuse().intensity);
            currentShader_->setUniform(getUniformArray("lights", i, "specular.color"), light->getSpecular().color);
            currentShader_->setUniform(getUniformArray("lights", i, "specular.intensity"), light->getSpecular().intensity);
            switch (light->getType()) {
                case Light::AMBIENT: break;
                case Light::DIRECTIONAL: {
                    const auto directionalLight = dynamic_pointer_cast<DirectionalLight>(light);
                    currentShader_->setUniform(getUniformArray("lights", i, "direction"), lightNodes.at(i)->getCurrentDirection());
                    break;
                }
                case Light::POSITIONAL: {
                    const auto positionalLight = dynamic_pointer_cast<PositionalLight>(light);
                    currentShader_->setUniform(getUniformArray("lights", i, "position"), lightNodes.at(i)->getCurrentPosition());
                    currentShader_->setUniform(getUniformArray("lights", i, "attenuation.constant"), positionalLight->getAttenuation().constant);
                    currentShader_->setUniform(getUniformArray("lights", i, "attenuation.linear"), positionalLight->getAttenuation().linear);
                    currentShader_->setUniform(getUniformArray("lights", i, "attenuation.quadratic"), positionalLight->getAttenuation().quadratic);
                    break;
                }
                case Light::SPOT: {
                    const auto spotLight = dynamic_pointer_cast<SpotLight>(light);
                    currentShader_->setUniform(getUniformArray("lights", i, "direction"), lightNodes.at(i)->getCurrentDirection());
                    currentShader_->setUniform(getUniformArray("lights", i, "position"), lightNodes.at(i)->getCurrentPosition());
                    currentShader_->setUniform(getUniformArray("lights", i, "attenuation.constant"), spotLight->getAttenuation().constant);
                    currentShader_->setUniform(getUniformArray("lights", i, "attenuation.linear"), spotLight->getAttenuation().linear);
                    currentShader_->setUniform(getUniformArray("lights", i, "attenuation.quadratic"), spotLight->getAttenuation().quadratic);
                    currentShader_->setUniform(getUniformArray("lights", i, "innerCutoff"), spotLight->getInnerCutoff());
                    currentShader_->setUniform(getUniformArray("lights", i, "outerCutoff"), spotLight->getOuterCutoff());
                    break;
                }
            }
        }
    }


    void Renderer::setMaterialUniforms(const shared_ptr<graphics::Material>& material) { // NOLINT
        if (currentShader_ != shaders_.at("lighting")) setShader("lighting");
        if (material->getDiffuseMap()) {
            currentShader_->setUniform("material.useDiffuseMap", true);
            currentShader_->setUniform("material.diffuseMap", material->getDiffuseMap()->bindTexture());
        } else {
            currentShader_->setUniform("material.useDiffuseMap", false);
        }
        if (material->getSpecularMap()) {
            currentShader_->setUniform("material.useSpecularMap", true);
            currentShader_->setUniform("material.specularMap", material->getSpecularMap()->bindTexture());
        } else {
            currentShader_->setUniform("material.useSpecularMap", false);
        }
        currentShader_->setUniform("material.ambient", material->getAmbient());
        currentShader_->setUniform("material.diffuse", material->getDiffuse());
        currentShader_->setUniform("material.specular", material->getSpecular());
        currentShader_->setUniform("material.shininess", material->getShininess());
    }


    void Renderer::drawMesh(const shared_ptr<geometry::Mesh>& mesh) { // NOLINT
        setShader("white");
        setCameraUniforms();
        currentShader_->setUniform("modelMatrix", glm::mat4(1.0f));
        draw(mesh);
    }


    void Renderer::drawLightUnit(const shared_ptr<scene::nodes::LightUnit>& lightUnit) { // NOLINT
        if (lightUnit->getMesh()) {
            setShader("emissive");
            currentShader_->setUniform("modelMatrix", lightUnit->getModelMatrix());
            currentShader_->setUniform("viewMatrix", Camera::getViewMatrix());
            currentShader_->setUniform("projectionMatrix", Camera::getProjectionMatrix());
            currentShader_->setUniform("emissive.color", lightUnit->getLight()->getAmbient().color);
            currentShader_->setUniform("emissive.intensity", lightUnit->getLight()->getAmbient().intensity);
            draw(lightUnit->getMesh());
        }
    }


    void Renderer::drawRenderUnit(const shared_ptr<scene::nodes::RenderUnit>& renderUnit) { // NOLINT
        setShader("Lighting");
        setCameraUniforms();
        currentShader_->setUniform("modelMatrix", renderUnit->getModelMatrix());
        if (useFallbackLights_) setLightingUniforms(fallbackLightUnits_);
        setMaterialUniforms(renderUnit->getMaterial() ? renderUnit->getMaterial() : fallbackMaterial_);
        draw(renderUnit->getMesh());
    }


    void Renderer::drawModel(const shared_ptr<scene::nodes::Model>& model) { // NOLINT
        for (const auto& lightUnit : model->getLightUnits()) {
            drawLightUnit(lightUnit);
        }
        useFallbackLights_ = model->getLightUnits().empty();
        setLightingUniforms(useFallbackLights_ ? fallbackLightUnits_ : model->getLightUnits());
        for (const auto& renderUnit : model->getRenderUnits()) {
            drawRenderUnit(renderUnit);
        }
    }



    void Renderer::drawScene(const shared_ptr<scene::nodes::Scene>& scene) { // NOLINT
        for (const auto& lightUnit : scene->getAllLightUnits()) {
            drawLightUnit(lightUnit);
        }
        useFallbackLights_ = scene->getAllLightUnits().empty();
        setLightingUniforms(useFallbackLights_ ? fallbackLightUnits_ : scene->getAllLightUnits());
        for (const auto& model : scene->getModels()) {
            for (const auto& renderUnit : model->getRenderUnits()) {
                drawRenderUnit(renderUnit);
            }
        }
    }


    void Renderer::draw(const shared_ptr<geometry::Mesh>& mesh) { // NOLINT
        currentShader_->bindShader();
        mesh->uploadToGPU();
        if (!mesh->getVAO()) return;
        mesh->getVAO()->bindVAO();
        if (mesh->getIndices().empty()) {
            glDrawArrays(GL_TRIANGLES, 0, mesh->getVertexCount());
        } else {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->getIndices().size()), GL_UNSIGNED_INT, nullptr);
        }
        graphics::VertexArray::unbindVAO();
    }

} // namespace pixelforge::core
