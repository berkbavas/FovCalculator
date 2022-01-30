#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include "BasicShader.h"
#include "Camera.h"
#include "CustomNode.h"
#include "CustomNodeData.h"
#include "Light.h"
#include "ModelLoader.h"

#include <QObject>

class NodeManager : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit NodeManager(QObject *parent = nullptr);
    ~NodeManager();

    void init();
    void render();

    void addColoredSimpleModel(ColoredSimpleModel *model);

    void addCustomNodeData(const QString &name, CustomNodeData *data);
    void addCustomNode(CustomNode *node);

    Camera *camera() const;
    void setCamera(Camera *newCamera);

    Light *light() const;
    void setLight(Light *newLight);

    const QMatrix4x4 &projection() const;
    void setProjection(const QMatrix4x4 &newProjection);

private:
    BasicShader *mBasicShader;

    Camera *mCamera;
    Light *mLight;

    QMap<SimpleModel::Type, SimpleModelData *> mSimpleModelData;
    QMultiMap<SimpleModel::Type, ColoredSimpleModel *> mColoredSimpleModels;

    QMap<QString, CustomNodeData *> mCustomNodeData;
    QMultiMap<QString, CustomNode *> mCustomNodes;

    QMatrix4x4 mProjection;
};

#endif // NODEMANAGER_H
