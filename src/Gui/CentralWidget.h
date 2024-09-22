#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "Rectangle.h"
#include "SideViewWidget.h"
#include "TopViewWidget.h"
#include "Core/Controller.h"

#include <QDoubleValidator>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);

    void init();
    void setParameters(Logic::Parameters *newParameters);
    void setSideViewWidget(SideViewWidget *newSideViewWidget);
    void setTopViewWidget(TopViewWidget *newTopViewWidget);

public slots:
    void refresh();
    void onCursorPositionChanged(QPointF position);

signals:
    void dirty();

private:
    QGroupBox *mCameraGroupBox;
    QLineEdit *mLineEditCameraHeight;
    QLineEdit *mLineEditTiltAngle;
    QLineEdit *mLineEditSensorWidth;
    QLineEdit *mLineEditSensorHeight;
    QLineEdit *mLineEditAspectRatio;
    QLineEdit *mLineEditHorizontalFov;
    QLineEdit *mLineEditVerticalFov;
    QLineEdit *mLineEditZNear;
    QLineEdit *mLineEditZFar;

    QGroupBox *mTargetGroupBox;
    QLineEdit *mLineEditTargetHeight;
    QLineEdit *mLineEditTargetDistance;
    QLineEdit *mLineEditFovWidth;

    QGroupBox *mLowerBoundaryGroupBox;
    QLineEdit *mLineEditLowerBoundaryHeight;
    QLineEdit *mLineEditLowerBoundaryDistance;

    QGroupBox *mCursorPositionGroupBox;
    QLabel *mLabelCursorPosition;

    QGroupBox *mRegionInfoGroupBox;
    QVector<class Rectangle *> mRectangles;
    QVector<QLabel *> mLimitLabels;

    SideViewWidget *mSideViewWidget;
    TopViewWidget *mTopViewWidget;

    Logic::Parameters *mParameters;

    QPointF mCursorPosition;
};

#endif // CENTRALWIDGET_H
