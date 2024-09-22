#include "Logic.h"

#include "Core/Constants.h"
#include "Core/Enums.h"

#include "Eigen/src/Geometry/AngleAxis.h"

#include <QtDebug>
#include <QtMath>

Logic::Logic()
    : mGround(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), 0))
{}

void Logic::setParameters(Logic::Parameters *newParameters)
{
    mParameters = newParameters;
}

void Logic::calculate()
{
    mParameters->camera.height = validateCameraHeight(mParameters->camera.height);

    //mParameters->frustum.horizontalFov = calculateHorizontalFovForGivenFovWidth(mParameters->target.fovWidth);

    mParameters->camera.sensor.aspectRatio = mParameters->camera.sensor.width / mParameters->camera.sensor.height;

    const float lowerBoundaryHeight = qMax(0.0f,
                                           qMin(mParameters->lowerBoundary.height - 0.0001f,
                                                qMin(mParameters->target.height - 0.0001f, mParameters->camera.height - 0.0001f)));
    const Eigen::Vector3f cameraPosition = Eigen::Vector3f(0, 0, mParameters->camera.height);

    // Vertical Fov
    const float halfHorizontalFovRadians = 0.5 * qDegreesToRadians(mParameters->frustum.horizontalFov);
    const float halfVerticalFovRadians = atan(tan(halfHorizontalFovRadians) / mParameters->camera.sensor.aspectRatio);

    // Tilt angle (angle between negative x-axis and bisector of the furstum measured clockwise)
    float tiltAngleRadians = halfVerticalFovRadians - atan2(mParameters->target.height - mParameters->camera.height, mParameters->target.distance);
    if (tiltAngleRadians > M_PI_2)
        tiltAngleRadians = M_PI_2;

    const float tiltAngle = qRadiansToDegrees(tiltAngleRadians);

    const float zNear = mParameters->frustum.zNear;
    const float zFar = mParameters->frustum.zFar;
    const float sensorWidth = mParameters->camera.sensor.width;

    // Construct frustum
    Frustum frustum;
    {
        const Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));
        const float zNearNorm = Eigen::Vector3f(zNear, zNear * tan(halfHorizontalFovRadians), zNear * tan(halfVerticalFovRadians)).norm();
        const float zFarNorm = Eigen::Vector3f(zFar, zFar * tan(halfHorizontalFovRadians), zFar * tan(halfVerticalFovRadians)).norm();

        const float x = 1;
        const float y = x * tan(halfHorizontalFovRadians);
        const float z = x * tan(halfVerticalFovRadians);

        frustum.edgeDirections[E0] = Eigen::Vector3f(x, y, z);
        frustum.edgeDirections[E1] = Eigen::Vector3f(x, y, -z);
        frustum.edgeDirections[E2] = Eigen::Vector3f(x, -y, -z);
        frustum.edgeDirections[E3] = Eigen::Vector3f(x, -y, z);

        for (EdgeNames name : {E0, E1, E2, E3}) {
            frustum.edgeDirections[name].normalize();
            frustum.edgeDirections[name] = rotation * frustum.edgeDirections[name];
            frustum.topVertices[name] = cameraPosition + zNearNorm * frustum.edgeDirections[name];
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[name]);
            float t = line.intersectionParameter(mGround);
            if (!std::isnan(t) && !std::isinf(t) && 0 < t)
                frustum.bottomVertices[name] = line.pointAt(t);
            else
                frustum.bottomVertices[name] = cameraPosition + zFarNorm * frustum.edgeDirections[name];
        }

        Eigen::Vector3f bisectors[2];
        bisectors[0] = Eigen::Vector3f(1, 0, 0);
        bisectors[1] = Eigen::Vector3f(-1, 0, 0);

        for (int i = 0; i < 2; ++i) {
            bisectors[i].normalize();
            bisectors[i] = rotation * bisectors[i];

            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, bisectors[i]);
            float t = line.intersectionParameter(mGround);
            if (!std::isnan(t) && !std::isinf(t) && 0 < t)
                bisectors[i] = line.pointAt(t);
            else
                bisectors[i] = cameraPosition + zFar * bisectors[i];
        }

        frustum.bisectorRay = bisectors[0];
        frustum.oppositeBisectorRay = bisectors[1];
        frustum.zNear = zNear;
        frustum.zFar = zFar;
        frustum.zNearNorm = zNearNorm;
        frustum.zFarNorm = zFarNorm;
        frustum.horizontalFov = 2 * qRadiansToDegrees(halfHorizontalFovRadians);
        frustum.verticalFov = 2 * qRadiansToDegrees(halfVerticalFovRadians);
    }

    // Find target intersections
    Entity target;
    {
        for (EdgeNames name : {E0, E1, E2, E3}) {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[name]);
            float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1),
                                                                             -mParameters->target.height)); // above z axis means negative offset
            if (!std::isnan(t) && !std::isinf(t) && 0 < t)
                target.intersections[name] = line.pointAt(t);
            else
                target.intersections[name] = cameraPosition + frustum.zFarNorm * frustum.edgeDirections[name];
        }

        target.distance = mParameters->target.distance;
        target.height = mParameters->target.height;
        target.fovWidth = 2 * abs(target.distance) * tan(atan2(frustum.edgeDirections[E0].y(), frustum.edgeDirections[E0].x()));
    }

    // Find lower boundary intersections
    Entity lowerBoundary;
    {
        for (EdgeNames name : {E0, E1, E2, E3}) {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[name]);
            float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1),
                                                                             -lowerBoundaryHeight)); // above z axis means negative offset
            if (!std::isnan(t) && !std::isinf(t) && 0 < t)
                lowerBoundary.intersections[name] = line.pointAt(t);
            else
                lowerBoundary.intersections[name] = cameraPosition + frustum.zFarNorm * frustum.edgeDirections[name];
        }

        lowerBoundary.distance = qMin(mParameters->target.distance, lowerBoundary.intersections[1].x());
        lowerBoundary.height = lowerBoundaryHeight;
    }

    Region regions[7];
    const Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-tiltAngleRadians, Eigen::Vector3f(0, -1, 0));
    const Eigen::Vector3f normalAfterRotation = rotation * Eigen::Vector3f(-1, 0, 0);

    for (int i = 0; i < 7; i++) {
        float limit = 0.5 * (sensorWidth / REGION_PPMS[i]) / tan(halfHorizontalFovRadians);

        if (limit < zNear)
            limit = zNear;

        if (limit > zFar)
            limit = zFar;

        regions[i].limit = limit;

        for (EdgeNames edgeName : {E0, E1, E2, E3}) {
            if (i == STRONG_IDENTIFICATION) {
                regions[STRONG_IDENTIFICATION].topVertices[edgeName] = frustum.topVertices[edgeName];
            } else {
                regions[i].topVertices[edgeName] = regions[i - 1]
                                                       .bottomVertices[edgeName]; // Top vertices is the same as previous region's bottom vertices
            }
        }

        const Eigen::Vector3f limitPointAfterRotation = Eigen::Vector3f(0, 0, mParameters->camera.height) + rotation * Eigen::Vector3f(limit, 0, 0);

        for (EdgeNames edgeName : {E0, E1, E2, E3}) {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(cameraPosition, frustum.edgeDirections[edgeName]);
            Eigen::Hyperplane<float, 3> regionLimitPlaneAfterRotation(normalAfterRotation, limitPointAfterRotation);
            regions[i].bottomVertices[edgeName] = line.intersectionPoint(regionLimitPlaneAfterRotation);
        }
    }

    mParameters->frustum = frustum;
    mParameters->target = target;
    mParameters->lowerBoundary = lowerBoundary;
    mParameters->camera.tiltAngle = tiltAngle;

    std::copy(regions, regions + 7, mParameters->regions);
}

QVector<Eigen::Vector3f> Logic::findIntersection(const Region &region, const Eigen::Hyperplane<float, 3> &plane)
{
    QVector<Eigen::Vector3f> intersections;

    for (int i = 0; i < 4; i++)
        intersections << findIntersection(region.bottomVertices[i], region.bottomVertices[(i + 1) % 4], plane);

    for (int i = 0; i < 4; i++)
        intersections << findIntersection(region.topVertices[i], region.topVertices[(i + 1) % 4], plane);

    intersections << findIntersection(region.topVertices[0], region.bottomVertices[0], plane);
    intersections << findIntersection(region.topVertices[1], region.bottomVertices[1], plane);
    intersections << findIntersection(region.topVertices[2], region.bottomVertices[2], plane);
    intersections << findIntersection(region.topVertices[3], region.bottomVertices[3], plane);

    return sortClockwiseOrder(intersections, plane.normal());
}

float Logic::calculateHorizontalFovForGivenFovWidth(float fovWidth)
{
    const float &targetHeight = mParameters->target.height;
    const float &targetDistance = mParameters->target.distance;
    const float &cameraHeight = mParameters->camera.height;
    const float &aspectRatio = mParameters->camera.sensor.aspectRatio;

    double c = atan2(targetHeight - cameraHeight, targetDistance);
    double k = fovWidth / (2 * targetDistance);
    double x0 = 0.0f, x1 = 0.0f, x2 = 0.0f;
    bool hasSolution = false;

    for (int i = 0; i < 9000; ++i) {
        if (f(x0, c, k, aspectRatio) * f(i * 0.000174532925, c, k, aspectRatio) < 0) {
            x1 = i * 0.000174532925;
            hasSolution = true;
            break;
        }
    }

    if (hasSolution == false) {
        double tiltAngle = atan(tan(qDegreesToRadians(0.5 * 179.0)) / aspectRatio) - c;
        if (tiltAngle > M_PI_2) {
            return 2 * qRadiansToDegrees(atan(tan(M_PI_2 + c) * aspectRatio));
        } else
            return 179.0;
    }

    int i = 0;
    while ((abs(x1 - x0) > 0.000001) & (i < 10000)) {
        x2 = x1 - (x1 - x0) * f(x1, c, k, aspectRatio) / (f(x1, c, k, aspectRatio) - f(x0, c, k, aspectRatio));
        x0 = x1;
        x1 = x2;
        ++i;
    }

    double horizontalFov = 2 * qRadiansToDegrees(atan(aspectRatio * tan(x1)));
    double tiltAngle = atan(tan(0.5 * qDegreesToRadians(horizontalFov)) / aspectRatio) - c;
    if (tiltAngle > M_PI_2)
        return 2 * qRadiansToDegrees(atan(tan(M_PI_2 + c) * aspectRatio));
    else
        return qMax(1.0, qMin(horizontalFov, 179.0));
}

float Logic::validateTargetDistance(float newTargetDistance)
{
    float halfVerticalFovRadians = atan(tan(qDegreesToRadians(0.5 * mParameters->frustum.horizontalFov)) / mParameters->camera.sensor.aspectRatio);
    float tiltAngleRadians = halfVerticalFovRadians - atan2(mParameters->target.height - mParameters->camera.height, newTargetDistance);

    if (tiltAngleRadians > M_PI_2)
        return (mParameters->camera.height - mParameters->target.height) / tan(M_PI_2 - halfVerticalFovRadians);
    else
        return newTargetDistance;
}

float Logic::validateTargetHeight(float newTargetHeight)
{
    float halfVerticalFovRadians = atan(tan(qDegreesToRadians(0.5 * mParameters->frustum.horizontalFov)) / mParameters->camera.sensor.aspectRatio);
    float tiltAngleRadians = halfVerticalFovRadians - atan2(newTargetHeight - mParameters->camera.height, mParameters->target.distance);

    if (tiltAngleRadians > M_PI_2)
        return mParameters->camera.height - mParameters->target.distance * tan(M_PI_2 - halfVerticalFovRadians);
    else
        return newTargetHeight;
}

float Logic::validateCameraHeight(float newCameraHeight)
{
    float halfVerticalFovRadians = atan(tan(qDegreesToRadians(0.5 * mParameters->frustum.horizontalFov)) / mParameters->camera.sensor.aspectRatio);
    float tiltAngleRadians = halfVerticalFovRadians - atan2(mParameters->target.height - newCameraHeight, mParameters->target.distance);

    if (tiltAngleRadians > M_PI_2)
        return mParameters->target.height + mParameters->target.distance * tan(M_PI_2 - halfVerticalFovRadians);
    else
        return newCameraHeight;
}

QVector<Eigen::Vector3f> Logic::findIntersection(const Eigen::Vector3f &start, const Eigen::Vector3f &end, const Eigen::Hyperplane<float, 3> &plane)
{
    float distance = (end - start).norm();
    Eigen::Vector3f direction = (end - start).normalized();
    Eigen::ParametrizedLine<float, 3> line(start, direction);
    float t = line.intersectionParameter(plane);

    if (std::isnan(t) || std::isinf(t))
        return QVector<Eigen::Vector3f>();

    Eigen::Vector3f point = line.pointAt(t);
    if (0 < t && (start - point).norm() < distance)
        return QVector<Eigen::Vector3f>(1, point);

    return QVector<Eigen::Vector3f>();
}
// Points are assume to be on the same plane whose normal is planeNormal
QVector<Eigen::Vector3f> Logic::sortClockwiseOrder(const QVector<Eigen::Vector3f> &points, const Eigen::Vector3f &planeNormal)
{
    // Find mean center
    Eigen::Vector3f meanCenter = findMeanCenter(points);
    // Translate to the origin
    QVector<Eigen::Vector3f> translatedPoints = translate(points, -meanCenter);
    // Rotate
    Eigen::Quaternionf rotation = Eigen::Quaternionf::FromTwoVectors(planeNormal, Eigen::Vector3f(0, 0, 1));

    QVector<Eigen::Vector3f> rotatedPoints;
    for (int i = 0; i < translatedPoints.size(); ++i) {
        rotatedPoints << rotation * translatedPoints[i];
    }

    // Now we can sort
    std::sort(rotatedPoints.begin(), rotatedPoints.end(), [=](const Eigen::Vector3f &p1, const Eigen::Vector3f &p2) -> bool {
        float theta1 = atan2(p1.y(), p1.x());
        if (theta1 < 0)
            theta1 += 2 * M_PI;

        float theta2 = atan2(p2.y(), p2.x());

        if (theta2 < 0)
            theta2 += 2 * M_PI;

        if (qFuzzyCompare(theta1, theta2))
            return p1.norm() < p2.norm();
        else
            return theta1 < theta2;
    });

    // Rotate back to the original rotations
    QVector<Eigen::Vector3f> sortedPoints;
    for (int i = 0; i < rotatedPoints.size(); ++i) {
        sortedPoints << rotation.inverse() * rotatedPoints[i];
    }

    // Move them back to the their original position
    return translate(sortedPoints, meanCenter);
}

Eigen::Vector3f Logic::findMeanCenter(const QVector<Eigen::Vector3f> &points)
{
    float x = 0;
    float y = 0;
    float z = 0;
    for (const auto &point : points) {
        x += point.x();
        y += point.y();
        z += point.z();
    }

    return Eigen::Vector3f(x / points.size(), y / points.size(), z / points.size());
}

QVector<Eigen::Vector3f> Logic::translate(const QVector<Eigen::Vector3f> &points, const Eigen::Vector3f &translation)
{
    QVector<Eigen::Vector3f> result;
    for (const auto &point : points)
        result << (point + translation);
    return result;
}
