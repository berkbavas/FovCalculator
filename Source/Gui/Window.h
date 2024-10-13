#pragma once

#include "Gui/InfoWidget.h"
#include "Gui/SideViewWidget.h"
#include "Gui/TopViewWidget.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QWidget>

namespace FovCalculator
{
    class Window : public QWidget
    {
      public:
        explicit Window(QWidget* parent = nullptr);

        TopViewWidget* GetTopViewWidget() { return mTopViewWidget; }
        InfoWidget* GetInfoWidget() { return mInfoWidget; }
        SideViewWidget* GetSideViewWidget() { return mSideViewWidget; }

      private:
        QGridLayout* mMainLayout;

        InfoWidget* mInfoWidget;
        TopViewWidget* mTopViewWidget;
        SideViewWidget* mSideViewWidget;
    };
}