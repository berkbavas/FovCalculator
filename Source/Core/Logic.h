#pragma once

#include "Core/Constants.h"
#include "Core/Structs.h"
#include "Util/Macros.h"

#include <Dense>

namespace FovCalculator
{
    class Logic
    {
      public:
        Logic() = default;

        void Calculate();

        float CalculateHorizontalFovForGivenFovWidth(float fovWidth);

        Eigen::Vector3f GetFrustumTopVertex(int index) const { return mTopVertices[index]; }
        Eigen::Vector3f GetFrustumBottomVertex(int index) const { return mBottomVertices[index]; }
        Eigen::Vector3f GetBisector(int index) const { return mBisectors[index]; }
        Eigen::Vector3f GetEdgeDirection(Edge edge) const { return mEdgeDirections[edge]; }
        Eigen::Vector3f GetTargetIntersection(Edge edge) const { return mTargetIntersections[edge]; }
        Eigen::Vector3f GetLowerBoundaryIntersection(Edge edge) const { return mLowerBoundaryIntersections[edge]; }
        Region GetRegion(int index) const { return mRegions[index]; }

        float ValidateCameraHeight(float newCameraHeight);
        float ValidateTargetDistance(float newTargetDistance);
        float ValidateTargetHeight(float newTargetHeight);

        QVector<Eigen::Vector3f> FindIntersection(const Region& region, const Eigen::Hyperplane<float, 3>& plane);

      private:
        void CalculateFrustum();
        void CalculateTargetParameters();
        void CalculateLowerBoundaryParameters();
        void CalculateRegions();

        QVector<Eigen::Vector3f> FindIntersection(const Eigen::Vector3f& start, const Eigen::Vector3f& end, const Eigen::Hyperplane<float, 3>& plane);
        QVector<Eigen::Vector3f> SortClockwiseOrder(const QVector<Eigen::Vector3f>& points, const Eigen::Vector3f& planeNormal);
        Eigen::Vector3f FindMeanCenter(const QVector<Eigen::Vector3f>& points);
        QVector<Eigen::Vector3f> Translate(const QVector<Eigen::Vector3f>& points, const Eigen::Vector3f& translation);

        // Frustum
        Eigen::Vector3f mTopVertices[4];
        Eigen::Vector3f mBottomVertices[4];
        Eigen::Vector3f mBisectors[2];
        Eigen::Vector3f mEdgeDirections[4];

        // Camera
        DEFINE_MEMBER(float, TiltAngle);
        DEFINE_MEMBER(float, CameraHeight, INITIAL_CAMERA_HEIGHT);
        DEFINE_MEMBER(float, HorizontalFov, INITIAL_HORIZONTAL_FOV);
        DEFINE_MEMBER(float, VerticalFov);
        DEFINE_MEMBER(float, ZNear, INITIAL_Z_NEAR);
        DEFINE_MEMBER(float, ZFar, INITIAL_Z_FAR);
        DEFINE_MEMBER(float, SensorWidth, INITIAL_CAMERA_SENSOR_WIDTH);
        DEFINE_MEMBER(float, SensorHeight, INITIAL_CAMERA_SENSOR_HEIGHT);
        DEFINE_MEMBER(float, AspectRatio);
        DEFINE_MEMBER(Eigen::Vector3f, CameraPosition);

        // Target
        DEFINE_MEMBER(float, TargetHeight, INITIAL_TARGET_HEIGHT);
        DEFINE_MEMBER(float, TargetDistance, INITIAL_TARGET_DISTANCE);
        DEFINE_MEMBER(float, FovWidth);

        Eigen::Vector3f mTargetIntersections[4];

        // Lower Boundary
        DEFINE_MEMBER(float, LowerBoundaryHeight, INITIAL_LOWER_BOUNDARY_HEIGHT);
        DEFINE_MEMBER(float, LowerBoundaryDistance);

        Eigen::Vector3f mLowerBoundaryIntersections[4];

        // Regions
        Region mRegions[7];

        // Ephemeral
        float mHalfHorizontalFovRadians;
        float mHalfVerticalFovRadians;
        float mTiltAngleRadians;
        float mZFarNorm;
        float mZNearNorm;

        inline float f(double x, double c, double k, float aspectRatio)
        {
            return k * cos(x - c) + k * tan(x) * sin(x - c) - aspectRatio * tan(x);
        }

        static const Eigen::Hyperplane<float, 3> GROUND;
    };
}