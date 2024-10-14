#include "Window.h"

#include "InfoWidget.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QLabel>
#include <QtImGui.h>
#include <imgui.h>

FovCalculator::Window::Window(QWidget* parent)
    : QWidget(parent)
{
    mInfoWidget = new InfoWidget(this);
    mTopViewWidget = new TopViewWidget(this);
    mSideViewWidget = new SideViewWidget(this);

    mMainLayout = new QGridLayout;
    mMainLayout->setColumnStretch(0, 25);
    mMainLayout->setColumnStretch(1, 100);
    mMainLayout->addWidget(mInfoWidget, 0, 0, 2, 1);
    mMainLayout->addWidget(mSideViewWidget, 0, 1);
    mMainLayout->addWidget(mTopViewWidget, 1, 1);
    setLayout(mMainLayout);

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(14, 14, 14));
    setAutoFillBackground(true);
    setPalette(pal);

    setMinimumSize(640, 480);
}
