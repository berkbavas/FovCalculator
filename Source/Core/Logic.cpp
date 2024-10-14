#include "Logic.h"

#include "Core/Constants.h"

#include <QtMath>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

void FovCalculator::Logic::Calculate()
{
    mCameraPosition = Eigen::Vector3f(0, 0, mCameraHeight);
    mAspectRatio = mSensorWidth / mSensorHeight;
    mHalfHorizontalFovRadians = 0.5 * mHorizontalFov * DEG_TO_RAD;
    mHalfVerticalFovRadians = std::atan(std::tan(mHalfHorizontalFovRadians) / mAspectRatio);
    mVerticalFov = 2 * mHalfVerticalFovRadians * RAD_TO_DEG;

    // Clamp
    mLowerBoundaryHeight = qBound(0.0f, mLowerBoundaryHeight, qMin(mCameraHeight, mTargetHeight) - 0.01f);

    mTiltAngleRadians = mHalfVerticalFovRadians - std::atan2(mTargetHeight - mCameraHeight, mTargetDistance);
    if (mTiltAngleRadians > M_PI_2)
    {
        mTiltAngleRadians = M_PI_2;
    }

    mTiltAngle = mTiltAngleRadians * RAD_TO_DEG;

    CalculateFrustum();
    CalculateTargetParameters();
    CalculateLowerBoundaryParameters();
    CalculateRegions();
}

void FovCalculator::Logic::CalculateFrustum()
{
    const Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-mTiltAngleRadians, Eigen::Vector3f(0, -1, 0));
    mZNearNorm = Eigen::Vector3f(mZNear, mZNear * std::tan(mHalfHorizontalFovRadians), mZNear * std::tan(mHalfVerticalFovRadians)).norm();
    mZFarNorm = Eigen::Vector3f(mZFar, mZFar * std::tan(mHalfHorizontalFovRadians), mZFar * std::tan(mHalfVerticalFovRadians)).norm();

    const float x = 1;
    const float y = x * std::tan(mHalfHorizontalFovRadians);
    const float z = x * std::tan(mHalfVerticalFovRadians);

    mEdgeDirections[E0] = Eigen::Vector3f(x, y, z);
    mEdgeDirections[E1] = Eigen::Vector3f(x, y, -z);
    mEdgeDirections[E2] = Eigen::Vector3f(x, -y, -z);
    mEdgeDirections[E3] = Eigen::Vector3f(x, -y, z);

    for (Edge edge : { E0, E1, E2, E3 })
    {
        mEdgeDirections[edge].normalize();
        mEdgeDirections[edge] = rotation * mEdgeDirections[edge];
        mTopVertices[edge] = mCameraPosition + mZNearNorm * mEdgeDirections[edge];
        Eigen::ParametrizedLine<float, 3> ray = Eigen::ParametrizedLine<float, 3>(mCameraPosition, mEdgeDirections[edge]);
        float t = ray.intersectionParameter(GROUND);
        if (!std::isnan(t) && !std::isinf(t) && 0 < t)
        {
            mBottomVertices[edge] = ray.pointAt(t);
        }
        else
        {
            mBottomVertices[edge] = mCameraPosition + mZFarNorm * mEdgeDirections[edge];
        }
    }

    mBisectors[0] = Eigen::Vector3f(1, 0, 0);
    mBisectors[1] = Eigen::Vector3f(-1, 0, 0);

    for (int i = 0; i < 2; ++i)
    {
        mBisectors[i].normalize();
        mBisectors[i] = rotation * mBisectors[i];

        Eigen::ParametrizedLine<float, 3> ray = Eigen::ParametrizedLine<float, 3>(mCameraPosition, mBisectors[i]);
        float t = ray.intersectionParameter(GROUND);
        if (!std::isnan(t) && !std::isinf(t) && 0 < t)
        {
            mBisectors[i] = ray.pointAt(t);
        }
        else
        {
            mBisectors[i] = mCameraPosition + mZFar * mBisectors[i];
        }
    }
}

void FovCalculator::Logic::CalculateTargetParameters()
{
    for (Edge edge : { E0, E1, E2, E3 })
    {
        Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(mCameraPosition, mEdgeDirections[edge]);
        float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), -mTargetHeight)); // above z axis means negative offset
        if (!std::isnan(t) && !std::isinf(t) && 0 < t)
        {
            mTargetIntersections[edge] = line.pointAt(t);
        }
        else
        {
            mTargetIntersections[edge] = mCameraPosition + mZFarNorm * mEdgeDirections[edge];
        }
    }

    mFovWidth = 2 * std::abs(mTargetDistance) * std::tan(std::atan2(mEdgeDirections[E0].y(), mEdgeDirections[E0].x()));
}

void FovCalculator::Logic::CalculateLowerBoundaryParameters()
{
    for (Edge edge : { E0, E1, E2, E3 })
    {
        Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(mCameraPosition, mEdgeDirections[edge]);
        float t = line.intersectionParameter(Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), -mLowerBoundaryHeight)); // above z axis means negative offset
        if (!std::isnan(t) && !std::isinf(t) && 0 < t)
        {
            mLowerBoundaryIntersections[edge] = line.pointAt(t);
        }
        else
        {
            mLowerBoundaryIntersections[edge] = mCameraPosition + mZFarNorm * mEdgeDirections[edge];
        }
    }

    mLowerBoundaryDistance = std::min(mTargetDistance, mLowerBoundaryIntersections[1].x());
}

void FovCalculator::Logic::CalculateRegions()
{
    const Eigen::AngleAxis<float> rotation = Eigen::AngleAxis<float>(-mTiltAngleRadians, Eigen::Vector3f(0, -1, 0));
    const Eigen::Vector3f normalAfterRotation = rotation * Eigen::Vector3f(-1, 0, 0);

    for (int i = 0; i < 7; i++)
    {
        float limit = 0.5 * (mSensorWidth / REGION_PPMS[i]) / std::tan(mHalfHorizontalFovRadians);

        if (limit < mZNear)
            limit = mZNear;

        if (limit > mZFar)
            limit = mZFar;

        mRegions[i].limit = limit;

        for (Edge edge : { E0, E1, E2, E3 })
        {
            if (i == STRONG_IDENTIFICATION)
            {
                mRegions[STRONG_IDENTIFICATION].topVertices[edge] = mTopVertices[edge];
            }
            else
            {
                mRegions[i].topVertices[edge] = mRegions[i - 1].bottomVertices[edge]; // Top vertices is the same as previous region's bottom vertices
            }
        }

        const Eigen::Vector3f limitPointAfterRotation = mCameraPosition + rotation * Eigen::Vector3f(limit, 0, 0);

        for (Edge edge : { E0, E1, E2, E3 })
        {
            Eigen::ParametrizedLine<float, 3> line = Eigen::ParametrizedLine<float, 3>(mCameraPosition, mEdgeDirections[edge]);
            Eigen::Hyperplane<float, 3> regionLimitPlaneAfterRotation(normalAfterRotation, limitPointAfterRotation);
            mRegions[i].bottomVertices[edge] = line.intersectionPoint(regionLimitPlaneAfterRotation);
        }
    }
}

float FovCalculator::Logic::ValidateCameraHeight(float newCameraHeight)
{
    float tiltAngleRadians = mHalfVerticalFovRadians - std::atan2(mTargetHeight - newCameraHeight, mTargetDistance);

    if (tiltAngleRadians > M_PI_2)
        return mTargetHeight + mTargetDistance * std::tan(M_PI_2 - mHalfVerticalFovRadians);
    else
        return newCameraHeight;
}

float FovCalculator::Logic::ValidateTargetDistance(float newTargetDistance)
{
    float tiltAngleRadians = mHalfVerticalFovRadians - std::atan2(mTargetHeight - mCameraHeight, newTargetDistance);

    if (tiltAngleRadians > M_PI_2)
        return (mCameraHeight - mTargetHeight) / std::tan(M_PI_2 - mHalfVerticalFovRadians);
    else
        return newTargetDistance;
}

float FovCalculator::Logic::ValidateTargetHeight(float newTargetHeight)
{
    float tiltAngleRadians = mHalfVerticalFovRadians - std::atan2(newTargetHeight - mCameraHeight, mTargetDistance);

    if (tiltAngleRadians > M_PI_2)
        return mCameraHeight - mTargetDistance * std::tan(M_PI_2 - mHalfVerticalFovRadians);
    else
        return newTargetHeight;
}

QVector<Eigen::Vector3f> FovCalculator::Logic::FindIntersection(const Region& region, const Eigen::Hyperplane<float, 3>& plane)
{
    QVector<Eigen::Vector3f> intersections;

    for (int i = 0; i < 4; i++)
        intersections << FindIntersection(region.bottomVertices[i], region.bottomVertices[(i + 1) % 4], plane);

    for (int i = 0; i < 4; i++)
        intersections << FindIntersection(region.topVertices[i], region.topVertices[(i + 1) % 4], plane);

    intersections << FindIntersection(region.topVertices[0], region.bottomVertices[0], plane);
    intersections << FindIntersection(region.topVertices[1], region.bottomVertices[1], plane);
    intersections << FindIntersection(region.topVertices[2], region.bottomVertices[2], plane);
    intersections << FindIntersection(region.topVertices[3], region.bottomVertices[3], plane);

    return SortClockwiseOrder(intersections, plane.normal());
}

QVector<Eigen::Vector3f> FovCalculator::Logic::FindIntersection(const Eigen::Vector3f& start, const Eigen::Vector3f& end, const Eigen::Hyperplane<float, 3>& plane)
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

QVector<Eigen::Vector3f> FovCalculator::Logic::SortClockwiseOrder(const QVector<Eigen::Vector3f>& points, const Eigen::Vector3f& planeNormal)
{
    // Find mean center
    Eigen::Vector3f meanCenter = FindMeanCenter(points);
    // Translate to the origin
    QVector<Eigen::Vector3f> translatedPoints = Translate(points, -meanCenter);
    // Rotate
    Eigen::Quaternionf rotation = Eigen::Quaternionf::FromTwoVectors(planeNormal, Eigen::Vector3f(0, 0, 1));

    QVector<Eigen::Vector3f> rotatedPoints;
    for (int i = 0; i < translatedPoints.size(); ++i)
    {
        rotatedPoints << rotation * translatedPoints[i];
    }

    // Now we can sort
    std::sort(rotatedPoints.begin(), rotatedPoints.end(), [=](const Eigen::Vector3f& p1, const Eigen::Vector3f& p2) -> bool
              {
        float theta1 = atan2(p1.y(), p1.x());
        if (theta1 < 0)
            theta1 += 2 * M_PI;

        float theta2 = atan2(p2.y(), p2.x());

        if (theta2 < 0)
            theta2 += 2 * M_PI;

        if (qFuzzyCompare(theta1, theta2))
            return p1.norm() < p2.norm();
        else
            return theta1 < theta2; });

    // Rotate back to the original rotations
    QVector<Eigen::Vector3f> sortedPoints;
    for (int i = 0; i < rotatedPoints.size(); ++i)
    {
        sortedPoints << rotation.inverse() * rotatedPoints[i];
    }

    // Move them back to the their original position
    return Translate(sortedPoints, meanCenter);
}

Eigen::Vector3f FovCalculator::Logic::FindMeanCenter(const QVector<Eigen::Vector3f>& points)
{
    float x = 0;
    float y = 0;
    float z = 0;
    for (const auto& point : points)
    {
        x += point.x();
        y += point.y();
        z += point.z();
    }

    return Eigen::Vector3f(x / points.size(), y / points.size(), z / points.size());
}

QVector<Eigen::Vector3f> FovCalculator::Logic::Translate(const QVector<Eigen::Vector3f>& points, const Eigen::Vector3f& translation)
{
    QVector<Eigen::Vector3f> result;
    for (const auto& point : points)
    {
        result << (point + translation);
    }

    return result;
}

float FovCalculator::Logic::CalculateHorizontalFovForGivenFovWidth(float fovWidth)
{

    double c = atan2(mTargetHeight - mCameraHeight, mTargetDistance);
    double k = fovWidth / (2 * mTargetDistance);
    double x0 = 0.0f, x1 = 0.0f, x2 = 0.0f;
    bool hasSolution = false;

    for (int i = 0; i < 9000; ++i)
    {
        if (f(x0, c, k, mAspectRatio) * f(i * 0.000174532925, c, k, mAspectRatio) < 0)
        {
            x1 = i * 0.000174532925;
            hasSolution = true;
            break;
        }
    }

    if (hasSolution == false)
    {
        double tiltAngle = atan(tan(qDegreesToRadians(0.5 * 179.0)) / mAspectRatio) - c;
        if (tiltAngle > M_PI_2)
        {
            return 2 * qRadiansToDegrees(atan(tan(M_PI_2 + c) * mAspectRatio));
        }
        else
            return 179.0;
    }

    int i = 0;
    while ((abs(x1 - x0) > 0.000001) & (i < 10000))
    {
        x2 = x1 - (x1 - x0) * f(x1, c, k, mAspectRatio) / (f(x1, c, k, mAspectRatio) - f(x0, c, k, mAspectRatio));
        x0 = x1;
        x1 = x2;
        ++i;
    }

    double horizontalFov = 2 * qRadiansToDegrees(atan(mAspectRatio * tan(x1)));
    double tiltAngle = atan(tan(0.5 * qDegreesToRadians(horizontalFov)) / mAspectRatio) - c;
    if (tiltAngle > M_PI_2)
        return 2 * qRadiansToDegrees(atan(tan(M_PI_2 + c) * mAspectRatio));
    else
        return qMax(1.0, qMin(horizontalFov, 179.0));
}

const Eigen::Hyperplane<float, 3> FovCalculator::Logic::GROUND = Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), 0);