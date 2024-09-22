#ifndef WINDOW3D_H
#define WINDOW3D_H

#include "NodeManager.h"

#include "Core/Controller.h"

#include <QOpenGLFunctions>
#include <QOpenGLWindow>

class Window3D : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Window3D(QWindow *parent = nullptr);
    ~Window3D();

    void initializeGL() override;
    void paintGL() override;

    void refresh();
    void setParameters(Controller::Window3DParameters *newParameters);

protected:
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;

private:
    Controller::Window3DParameters *mParameters;

    NodeManager *mNodeManager;
    Light *mLight;
    Camera *mCamera;
    QMatrix4x4 mProjectionMatrix;
    ColoredSimpleModel *mCameraModel;
    ColoredSimpleModel *mPlane;

    CustomNodeData *mCustomNodeData[7];
    CustomNode *mRegions[7];
    CustomNodeData *mFrustumEdgesData;
    CustomNode *mFrustumEdges;

    bool mRefresh;
};

#endif // WINDOW3D_H
