#pragma once

#include "Gui/Handle.h"
#include "Util/Macros.h"

#include <QWidget>

namespace FovCalculator
{
    class SideViewWidget : public QWidget
    {
        Q_OBJECT
      public:
        explicit SideViewWidget(QWidget* parent = nullptr);

        void SetGroundIntersection(int index, const QPointF& point);
        void SetTargetIntersection(int index, const QPointF& point);
        void SetLowerBoundaryIntersection(int index, const QPointF& point);
        void SetRegion(int index, const QPolygonF& region);

      signals:
        void UserRequestsPan(const QPointF& delta);
        void WheelMoved(QWheelEvent* event);
        void UserRequestsTargetHeightDeltaChange(const QPointF& delta);
        void UserRequestsTargetDistanceDeltaChange(const QPointF& delta);
        void UserRequestsCameraHeightDeltaChange(const QPointF& delta);
        void UserRequestsLowerBoundaryHeightDeltaChange(const QPointF& delta);
        void MouseMoved(const QPointF& position);

      private:
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;
        void wheelEvent(QWheelEvent* event) override;

        void UpdateHandles();
        void DrawHorizontalAxis();
        void DrawVerticalAxis();
        void DrawTickmarks();
        void DrawRegions();
        void DrawTargetStuff();
        void DrawTiltAngleStuff();
        void DrawHandlers();

        void UpdateCursor();

        DEFINE_MEMBER(QPointF, Origin);
        DEFINE_MEMBER(float, MeterToPixelRatio);
        DEFINE_MEMBER(float, CameraHeight);
        DEFINE_MEMBER(float, TargetDistance);
        DEFINE_MEMBER(float, TargetHeight);
        DEFINE_MEMBER(float, LowerBoundaryDistance);
        DEFINE_MEMBER(float, LowerBoundaryHeight);
        DEFINE_MEMBER(float, TiltAngle);
        DEFINE_MEMBER(QPointF, BisectorIntersection);
        DEFINE_MEMBER(QPointF, OppositeBisectorIntersection);

        QPointF mGroundIntersections[4];
        QPointF mTargetIntersections[4];
        QPointF mLowerBoundaryIntersections[4];

        Handle mTargetHeightHandle;
        Handle mTargetDistanceHandle;
        Handle mCameraHeightHandle;
        Handle mLowerBoundaryHandle;

        QPointF mPreviousMousePosition;
        bool mUserRequestsPan{ false };

        QPen mDashedPen;
        QPen mSolidPen;
        QFont mLabelFont;
        QColor mLabelColor;

        QPen mAxisPen;

        QPolygonF mRegions[7];
    };
}