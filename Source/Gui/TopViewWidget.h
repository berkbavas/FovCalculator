#pragma once

#include "Gui/Handle.h"
#include "Util/Macros.h"

#include <QPen>
#include <QWidget>

namespace FovCalculator
{
    class TopViewWidget : public QWidget
    {
        Q_OBJECT
      public:
        explicit TopViewWidget(QWidget* parent = nullptr);

        void SetGroundIntersection(int index, const QPointF& point);
        void SetTargetIntersection(int index, const QPointF& point);
        void SetRegion(int index, const QPolygonF& region);

      signals:
        void
        UserRequestsPan(const QPointF& delta);
        void WheelMoved(QWheelEvent* event);
        void UserRequestsTargetDistanceChange(const QPointF& delta);
        void UserRequestsFovWidthChange(float delta);

      private:
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent*) override;
        void mouseMoveEvent(QMouseEvent*) override;
        void mouseReleaseEvent(QMouseEvent*) override;
        void wheelEvent(QWheelEvent* event) override;

        void UpdateHandles();
        void DrawCrossPattern();
        void DrawRegions();
        void DrawGroundIntersections();
        void DrawTargetIntersections();
        void DrawHandles();
        void DrawLabels();

        void UpdateCursor();

        QBrush mCrossPatternBursh;
        QPen mDashedPen;
        QPen mSolidPen;

        QPointF mGroundIntersections[4];
        QPointF mTargetIntersections[4];

        DEFINE_MEMBER(QPointF, Origin);
        DEFINE_MEMBER(float, HorizontalFov);
        DEFINE_MEMBER(float, FovWidth);

        QPointF mPreviousMousePosition;
        bool mUserRequestsPan{ false };

        Handle mTargetHandle;
        Handle mCameraHandle;
        Handle mFovWidthHandleTop;
        Handle mFovWidthHandleBottom;

        QFont mLabelFont;
        QColor mLabelColor;

        QPolygonF mRegions[7];
    };
}
