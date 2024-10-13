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
        void OnUserRequestsTargetDistanceChange(const QPointF& delta);
        void OnUserRequestsFovWidthChange(float delta);
        void OnUserRequestsTargetHeightChange(const QPointF& delta);
        void OnUserRequestsCameraHeightChange(const QPointF& delta);
        void OnUserRequestsLowerBoundaryHeightChange(const QPointF& delta);

        void Update();
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