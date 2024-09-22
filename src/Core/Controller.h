#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Logic.h"

#include "Eigen/src/Geometry/Hyperplane.h"

#include <QObject>
#include <QPolygonF>
#include <QVector2D>
#include <QVector3D>

class CentralWidget;
class Window3D;
class SideViewWidget;
class TopViewWidget;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    struct Camera
    {
        float height;
        float tiltAngle;
        float horizontalFov;
        float verticalFov;
        QPointF position;
    };

    struct Target
    {
        float fovWidth;
        float height;
        float distance;
        QPointF position;
        QPointF intersections[4];
    };

    struct LowerBoundary
    {
        float height;
        float distance;
        QPointF position;
        QPointF intersections[4];
    };

    struct Ground
    {
        QPointF intersections[4];
    };

    struct Region
    {
        QPolygonF region;
        bool visible;
    };

    struct SideViewWidgetParameters
    {
        Camera camera;
        Target target;
        Ground ground;
        LowerBoundary lowerBoundary;
        Region regions[7];
        QPointF bisectorIntersection;
        QPointF oppositeBisectorIntersection;
    };

    struct TopViewWidgetParameters
    {
        Camera camera;
        Target target;
        Ground ground;
        LowerBoundary lowerBoundary;
        Region regions[7];
    };

    struct Region3D
    {
        QVector<QVector3D> vertices;
        bool intersectsGround;
    };

    struct Window3DParameters
    {
        Region3D regions[7];
        QVector<QVector3D> frustumEdges;
        float cameraHeight;
        float tiltAngle;
    };

public slots:
    void init();
    void onDirty();
    void onZoom(int);
    void onPan(int x, int y);

private:
    void update();
    bool intersectsGround(const Region3D &region);
    QVector<QVector3D> convert(const QVector<Eigen::Vector3f> &vectors);
    QVector<QVector3D> createFrustumEdges(const Logic::Frustum &frustum);
    QVector<QVector3D> createVertices(const Logic::Region &region);
    QVector<QVector3D> createNormals(const QVector<QVector3D> &vertices);

    void setMeterToPixelRatio(float newMeterToPixelRatio);
    void setOrigin(QPointF newOrigin);

    Logic *mLogic;

    Logic::Parameters *mLogicParameters;
    SideViewWidgetParameters *mSideViewWidgetParameters;
    TopViewWidgetParameters *mTopViewWidgetParameters;
    Window3DParameters *mWindow3DParameters;

    CentralWidget *mCentralWidget;
    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;
    Window3D *mWindow3D;

    const float mZoomStepSize;

    float mMeterToPixelRatio;
    QPointF mOrigin;
    Eigen::Hyperplane<float, 3> mGround;
};

#endif // CONTROLLER_H
