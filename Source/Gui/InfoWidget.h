#pragma once

#include "Util/Macros.h"

#include <QInputEvent>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

#include <QtImGui.h>

namespace FovCalculator
{
    class Logic;

    class InfoWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
    {
        Q_OBJECT
      public:
        InfoWidget(QWidget* parent);

      signals:
        void CameraHeightChanged(float val);
        void SensorWidthChanged(float val);
        void SensorHeightChanged(float val);
        void HorizontalFovChanged(float val);
        void ZNearChanged(float val);
        void ZFarChanged(float val);
        void TargetHeightChanged(float val);
        void TargetDistanceChanged(float val);
        void LowerBoundaryHeightChanged(float val);

      private:
        void initializeGL() override;
        void paintGL() override;

        void DrawCameraInputs();
        void DrawTargetInputs();
        void DrawLowerBoundaryInputs();
        void DrawRegionLegend();
        void DrawMousePosition();

        DEFINE_MEMBER(float, CameraHeight);
        DEFINE_MEMBER(float, TiltAngle);
        DEFINE_MEMBER(float, SensorWidth);
        DEFINE_MEMBER(float, SensorHeight);
        DEFINE_MEMBER(float, AspectRatio);
        DEFINE_MEMBER(float, HorizontalFov);
        DEFINE_MEMBER(float, VerticalFov);
        DEFINE_MEMBER(float, ZNear);
        DEFINE_MEMBER(float, ZFar);

        DEFINE_MEMBER(float, TargetHeight);
        DEFINE_MEMBER(float, TargetDistance);

        DEFINE_MEMBER(float, FovWidth);
        DEFINE_MEMBER(float, LowerBoundaryHeight);
        DEFINE_MEMBER(float, LowerBoundaryDistance);

        DEFINE_MEMBER(QPointF, MousePosition);

        QtImGui::RenderRef mRenderRef;
    };
}
