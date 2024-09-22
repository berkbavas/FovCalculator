#include "NodeManager.h"

NodeManager::NodeManager(QObject *parent)
    : QObject{parent}
    , mBasicShader(nullptr)

    , mCamera(nullptr)
    , mLight(nullptr)
{}

NodeManager::~NodeManager()
{
    if (mBasicShader)
        mBasicShader->deleteLater();

    mBasicShader = nullptr;

    // Clean Up Simple Model Data
    {
        const QList<SimpleModel::Type> types = mSimpleModelData.keys();
        for (const auto type : types) {
            SimpleModelData *data = mSimpleModelData[type];
            if (data)
                data->deleteLater();
        }
    }

    // Clean Up Colored Simple Models
    {
        const QList<SimpleModel::Type> types = mColoredSimpleModels.keys();

        for (auto type : types) {
            const QList<ColoredSimpleModel *> models = mColoredSimpleModels.values(type);

            for (auto model : models) {
                if (model)
                    model->deleteLater();
            }
        }
    }

    // Clean Up Custom Node Data
    {
        const QList<QString> names = mCustomNodeData.keys();
        for (const auto &name : names) {
            CustomNodeData *data = mCustomNodeData[name];
            if (data)
                data->deleteLater();
        }
    }

    // Clean Up Colored Simple Models
    {
        const QList<QString> names = mCustomNodes.keys();

        for (const auto &name : names) {
            const QList<CustomNode *> models = mCustomNodes.values(name);

            for (auto model : models) {
                if (model)
                    model->deleteLater();
            }
        }
    }

    mSimpleModelData.clear();
    mColoredSimpleModels.clear();
    mCustomNodeData.clear();
    mCustomNodes.clear();
}

void NodeManager::init()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glLineWidth(1.5f);

    mSimpleModelData = ModelLoader::loadSimpleModelData();

    const QList<SimpleModel::Type> types = mSimpleModelData.keys();

    for (const auto type : types) {
        SimpleModelData *data = mSimpleModelData[type];
        if (data)
            data->create();
    }

    const QList<QString> names = mCustomNodeData.keys();

    for (const auto &name : names) {
        CustomNodeData *data = mCustomNodeData[name];
        if (data)
            data->create();
    }

    mBasicShader = new BasicShader;
    mBasicShader->init();
}

void NodeManager::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mBasicShader->bind();

    // Projection
    {
        mBasicShader->setUniformValue("projectionMatrix", mProjection);
    }

    // Camera
    {
        if (mCamera) {
            mBasicShader->setUniformValue("viewMatrix", mCamera->transformation());
            mBasicShader->setUniformValue("cameraPosition", mCamera->position());
        }
    }

    // Light
    {
        if (mLight) {
            mBasicShader->setUniformValue("light.position", mLight->position());
            mBasicShader->setUniformValue("light.color", mLight->color());
            mBasicShader->setUniformValue("light.ambient", mLight->ambient());
            mBasicShader->setUniformValue("light.diffuse", mLight->diffuse());
            mBasicShader->setUniformValue("light.specular", mLight->specular());
        }
    }

    // Colored Simple Models
    {
        const QList<SimpleModel::Type> types = mColoredSimpleModels.keys();

        for (auto type : types) {
            SimpleModelData *data = mSimpleModelData.value(type);
            const QList<ColoredSimpleModel *> models = mColoredSimpleModels.values(type);
            if (data) {
                data->bind();

                for (auto model : models) {
                    mBasicShader->setUniformValue("nodeMatrix", model->transformation());
                    mBasicShader->setUniformValue("node.color", model->color());
                    mBasicShader->setUniformValue("node.ambient", model->material().ambient());
                    mBasicShader->setUniformValue("node.diffuse", model->material().diffuse());
                    mBasicShader->setUniformValue("node.specular", model->material().specular());
                    mBasicShader->setUniformValue("node.shininess", model->material().shininess());

                    glDrawArrays(GL_TRIANGLES, 0, data->count());
                }

                data->release();
            }
        }
    }

    // Custom Nodes
    {
        const QList<QString> names = mCustomNodes.keys();

        for (const auto &name : names) {
            CustomNodeData *data = mCustomNodeData.value(name);
            const QList<CustomNode *> nodes = mCustomNodes.values(name);
            if (data) {
                data->bind();

                for (auto node : nodes) {
                    mBasicShader->setUniformValue("nodeMatrix", node->transformation());
                    mBasicShader->setUniformValue("node.color", node->color());
                    mBasicShader->setUniformValue("node.ambient", node->material().ambient());
                    mBasicShader->setUniformValue("node.diffuse", node->material().diffuse());
                    mBasicShader->setUniformValue("node.specular", node->material().specular());
                    mBasicShader->setUniformValue("node.shininess", node->material().shininess());

                    if (node->color().w() < 1.0f) { // TODO: Draw transparent nodes
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        glDrawArrays(node->primitive(), 0, data->count());
                        glDisable(GL_BLEND);
                    } else
                        glDrawArrays(node->primitive(), 0, data->count());
                }

                data->release();
            }
        }
    }

    mBasicShader->release();
}

void NodeManager::addCustomNodeData(const QString &name, CustomNodeData *data)
{
    mCustomNodeData.insert(name, data);
}

void NodeManager::addCustomNode(CustomNode *node)
{
    mCustomNodes.insert(node->name(), node);
}

void NodeManager::addColoredSimpleModel(ColoredSimpleModel *model)
{
    mColoredSimpleModels.insert(model->type(), model);
}

Camera *NodeManager::camera() const
{
    return mCamera;
}

void NodeManager::setCamera(Camera *newCamera)
{
    mCamera = newCamera;
}

Light *NodeManager::light() const
{
    return mLight;
}

void NodeManager::setLight(Light *newLight)
{
    mLight = newLight;
}

const QMatrix4x4 &NodeManager::projection() const
{
    return mProjection;
}

void NodeManager::setProjection(const QMatrix4x4 &newProjection)
{
    mProjection = newProjection;
}
