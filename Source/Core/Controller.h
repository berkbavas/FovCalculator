#pragma once

#include "Core/Constants.h"
#include "Core/Converter.h"
#include "Core/Logic.h"
#include "Gui/Window.h"

#include <QObject>
#include <QOpenGLExtraFunctions>

namespace FovCalculator
{
    class Controller : public QObject
    {
      public:
        explicit Controller(QObject* parent = nullptr);
        void Run();

      private:
        void OnUserRequestsPan(const QPointF& delta);
        void OnWheelMoved(QWheelEvent* event);
        void OnUserRequestsTargetDistanceDeltaChange(const QPointF& delta);
        void OnUserRequestsFovWidthDeltaChange(float delta);
        void OnUserRequestsTargetHeightDeltaChange(const QPointF& delta);
        void OnUserRequestsCameraHeightDeltaChange(const QPointF& delta);
        void OnUserRequestsLowerBoundaryHeightDeltaChange(const QPointF& delta);
        void OnMouseMoved(const QPointF& position);

        void OnUserRequestsCameraHeightChange(float newHeight);
        void OnUserRequestsTargetHeightChange(float newHeight);
        void OnUserRequestsTargetDistanceChange(float newDistance);
        void OnUserRequestsLowerBoundaryHeightChange(float newHeight);

        void Update();
        void UpdateInfoWidget();
        void UpdateSideViewWidget();
        void UpdateTopViewWidget();

        Logic* mLogic;
        Converter* mConverter;
        Window* mWindow;
        TopViewWidget* mTopViewWidget;
        InfoWidget* mInfoWidget;
        SideViewWidget* mSideViewWidget;
    };
}