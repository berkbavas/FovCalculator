#ifndef LOGIC_H
#define LOGIC_H

#include "Enums.h"
#include <Dependencies/Eigen/Dense>
#include <QVector>

class Logic
{
public:
    struct Region
    {
        Eigen::Vector3f topVertices[4];
        Eigen::Vector3f bottomVertices[4];
        float limit;
    };

    struct Frustum
    {
        Eigen::Vector3f topVertices[4];
        Eigen::Vector3f bottomVertices[4];
        Eigen::Vector3f bisectorRay;
        Eigen::Vector3f oppositeBisectorRay;
        Eigen::Vector3f edgeDirections[4];
        float horizontalFov;
        float verticalFov;
        float zNear;
        float zFar;
        float zNearNorm;
        float zFarNorm;
    };

    struct Sensor
    {
        float width;
        float height;
        float aspectRatio;
    };

    struct Camera
    {
        Sensor sensor;
        float height;
        float tiltAngle;
    };

    struct Entity
    {
        float height;
        float distance;
        Eigen::Vector3f intersections[4];
        float fovWidth;
    };

    struct Parameters
    {
        Frustum frustum;
        Camera camera;
        Entity target;
        Entity lowerBoundary;
        Region regions[7];
    };

    Logic();
    void calculate();
    QVector<Eigen::Vector3f> findIntersection(const Region &region, const Eigen::Hyperplane<float, 3> &plane);

    float calculateHorizontalFovForGivenFovWidth(float fovWidth);
    float validateTargetDistance(float newTargetDistance);
    float validateTargetHeight(float newTargetHeight);
    float validateCameraHeight(float newCameraHeight);

    void setParameters(Parameters *newParameters);

private:
    QVector<Eigen::Vector3f> findIntersection(const Eigen::Vector3f &start,
                                              const Eigen::Vector3f &end,
                                              const Eigen::Hyperplane<float, 3> &plane);
    QVector<Eigen::Vector3f> sortClockwiseOrder(const QVector<Eigen::Vector3f> &points,
                                                const Eigen::Vector3f &planeNormal);
    Eigen::Vector3f findMeanCenter(const QVector<Eigen::Vector3f> &points);
    QVector<Eigen::Vector3f> translate(const QVector<Eigen::Vector3f> &points, const Eigen::Vector3f &translation);

    const Eigen::Hyperplane<float, 3> mGround;
    Parameters *mParameters;

    inline float f(double x, double c, double k, float aspectRatio)
    {
        return k * cos(x - c) + k * tan(x) * sin(x - c) - aspectRatio * tan(x);
    }
};

#endif // LOGIC_H
