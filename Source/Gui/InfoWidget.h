#pragma once

#include "Util/Macros.h"

#include <QInputEvent>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

namespace FovCalculator
{
    class Logic;

    class InfoWidget : public QOpenGLWidget, public QOpenGLExtraFunctions
    {
        Q_OBJECT
      public:
        InfoWidget();

        void SetLogic(Logic* logic) { mLogic = logic; }

      signals:
        void GuiNeedsUpdate();

      private:
        void initializeGL() override;
        void paintGL() override;

        Logic* mLogic;
    };
}
