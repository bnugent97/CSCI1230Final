#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

// Code largely ported over form Ray Intersect project, which in turn was ported from Lab 5

// The traverseDFS function from your lab to populate RenderData based on a scene graph traversal
static void traverseDFS(SceneNode* node, glm::mat4 currentCTM, RenderData &renderData) {
    for (auto& transform : node->transformations) {
        switch (transform->type) {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            currentCTM = glm::translate(currentCTM, transform->translate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            currentCTM = glm::scale(currentCTM, transform->scale);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            currentCTM = glm::rotate(currentCTM, transform->angle, transform->rotate);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            currentCTM = currentCTM * transform->matrix;
            break;
        }
    }

    for (auto& primitive : node->primitives) {
        RenderShapeData shapeData;
        shapeData.primitive = *primitive;
        shapeData.ctm = currentCTM;
        renderData.shapes.push_back(shapeData);
    }

    for (auto& light : node->lights) {
        SceneLightData lightData;
        lightData.id = light->id;
        lightData.type = light->type;
        lightData.color = light->color;
        lightData.function = light->function;
        lightData.pos = currentCTM * glm::vec4(0, 0, 0, 1); // Assuming light's local position is the origin
        lightData.dir = glm::normalize(currentCTM * light->dir);
        lightData.penumbra = light->penumbra;
        lightData.angle = light->angle;
        lightData.width = light->width;
        lightData.height = light->height;
        renderData.lights.push_back(lightData);
    }

    for (auto& child : node->children) {
        traverseDFS(child, currentCTM, renderData);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();

    renderData.shapes.clear();
    renderData.lights.clear();

    SceneNode* rootNode = fileReader.getRootNode();
    traverseDFS(rootNode, glm::mat4(1.0f), renderData);

    return true;
}
