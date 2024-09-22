#include "Window3D.h"

#include "Core/Constants.h"

#include <QKeyEvent>
#include <QMouseEvent>

Window3D::Window3D(QWindow *parent)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
    , mNodeManager(nullptr)
    , mLight(nullptr)
    , mCamera(nullptr)
    , mRefresh(false)

{
    mNodeManager = new NodeManager;

    mCamera = new Camera;
    mCamera->setPosition(QVector3D(2, 2, 15));
    mNodeManager->setCamera(mCamera);

    mLight = new Light;
    mLight->setPosition(QVector3D(2, 50, 2));
    mNodeManager->setLight(mLight);

    // Plane
    mPlane = new ColoredSimpleModel;
    mPlane->setType(SimpleModel::Plane);
    mPlane->setPosition(QVector3D(0, 0, 0));
    mNodeManager->addColoredSimpleModel(mPlane);

    // Camera Model
    mCameraModel = new ColoredSimpleModel;
    mCameraModel->setType(SimpleModel::Camera);
    mCameraModel->setPosition(QVector3D(-10, 0, 0));
    mNodeManager->addColoredSimpleModel(mCameraModel);

    // Regions
    QStringList names = {"STRONG_IDENTIFICATION", "IDENTIFICATION", "RECOGNITION", "OBSERVATION", "DETECTION", "MONITORING", "DEAD_ZONE"};

    for (int i = 0; i < names.length(); i++) {
        mCustomNodeData[i] = new CustomNodeData;
        mNodeManager->addCustomNodeData(names[i], mCustomNodeData[i]);

        mRegions[i] = new CustomNode;
        mRegions[i]->setName(names[i]);
        mRegions[i]->material().setAmbient(1.0f);
        mRegions[i]->material().setDiffuse(1.0f);
        mRegions[i]->setColor(QVector4D(REGION_COLORS[i].red() / 255.0f,   //
                                        REGION_COLORS[i].green() / 255.0f, //
                                        REGION_COLORS[i].blue() / 255.0f,  //
                                        0.5f));
        mNodeManager->addCustomNode(mRegions[i]);
    }

    // Frustum edges
    mFrustumEdgesData = new CustomNodeData;
    mNodeManager->addCustomNodeData("FRUSTUM_EDGES", mFrustumEdgesData);

    mFrustumEdges = new CustomNode;
    mFrustumEdges->setPrimitive(CustomNode::Lines);
    mFrustumEdges->material().setAmbient(1.0);
    mFrustumEdges->material().setDiffuse(0.0f);
    mFrustumEdges->material().setSpecular(0.0f);
    mFrustumEdges->material().setShininess(1.0f);
    mFrustumEdges->setColor(QVector4D(0.5, 0.5, 0.5, 1));
    mFrustumEdges->setName("FRUSTUM_EDGES");
    mNodeManager->addCustomNode(mFrustumEdges);

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

Window3D::~Window3D()
{
    makeCurrent();

    if (mNodeManager)
        mNodeManager->deleteLater();

    if (mLight)
        mLight->deleteLater();

    if (mCamera)
        mCamera->deleteLater();

    doneCurrent();
}

void Window3D::initializeGL()
{
    initializeOpenGLFunctions();

    mNodeManager->init();
}

void Window3D::paintGL()
{
    if (mRefresh) {
        mRefresh = false;

        for (int i = 0; i < 7; i++)
            mCustomNodeData[i]->write(mParameters->regions[i].vertices);

        mFrustumEdgesData->write(mParameters->frustumEdges);
    }

    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(45, (float) width() / height(), 0.1, 1000);
    mNodeManager->setProjection(mProjectionMatrix);
    mNodeManager->render();
}

void Window3D::refresh()
{
    mRefresh = true;

    QQuaternion rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, -1), mParameters->tiltAngle);
    mCameraModel->setRotation(rotation);
    QVector3D cameraForward = rotation * QVector3D(1, 0, 0);
    QVector3D position = QVector3D(0, mParameters->cameraHeight, 0) - 0.5 * cameraForward;
    mCameraModel->setPosition(position);
}

void Window3D::keyPressEvent(QKeyEvent *event)
{
    mCamera->keyPressed(event);
}

void Window3D::keyReleaseEvent(QKeyEvent *event)
{
    mCamera->keyReleased(event);
}

void Window3D::mousePressEvent(QMouseEvent *event)
{
    mCamera->mousePressed(event);
}

void Window3D::mouseReleaseEvent(QMouseEvent *event)
{
    mCamera->mouseReleased(event);
}

void Window3D::mouseMoveEvent(QMouseEvent *event)
{
    mCamera->mouseMoved(event);
}

void Window3D::setParameters(Controller::Window3DParameters *newParameters)
{
    mParameters = newParameters;
}
