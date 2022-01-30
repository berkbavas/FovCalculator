#include "CentralWidget.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{}

void CentralWidget::init()
{
    // Camera
    {
        mCameraGroupBox = new QGroupBox("Camera");
        QGridLayout *layout = new QGridLayout;

        mLineEditCameraHeight = new QLineEdit;
        mLineEditCameraHeight->setValidator(new QDoubleValidator(0.1, 1000, 2));
        layout->addWidget(new QLabel("Height (m):"), 0, 0);
        layout->addWidget(mLineEditCameraHeight, 0, 1);
        connect(mLineEditCameraHeight, &QLineEdit::editingFinished, this, [this]() {
            mParameters->camera.height = mLineEditCameraHeight->text().toFloat();
            emit dirty();
        });

        mLineEditTiltAngle = new QLineEdit;
        mLineEditTiltAngle->setEnabled(false);
        layout->addWidget(new QLabel("Tilt Angle (°):"), 1, 0);
        layout->addWidget(mLineEditTiltAngle, 1, 1);

        mLineEditSensorWidth = new QLineEdit;
        mLineEditSensorWidth->setValidator(new QIntValidator(1, 10000));
        layout->addWidget(new QLabel("Sensor Width (px):"), 2, 0);
        layout->addWidget(mLineEditSensorWidth, 2, 1);
        connect(mLineEditSensorWidth, &QLineEdit::editingFinished, this, [this]() {
            mParameters->camera.sensor.width = mLineEditSensorWidth->text().toInt();
            emit dirty();
        });

        mLineEditSensorHeight = new QLineEdit;
        mLineEditSensorHeight->setValidator(new QIntValidator(1, 10000));
        layout->addWidget(new QLabel("Sensor Height (px):"), 3, 0);
        layout->addWidget(mLineEditSensorHeight, 3, 1);
        connect(mLineEditSensorHeight, &QLineEdit::editingFinished, this, [this]() {
            mParameters->camera.sensor.height = mLineEditSensorHeight->text().toInt();
            emit dirty();
        });

        mLineEditAspectRatio = new QLineEdit;
        mLineEditAspectRatio->setEnabled(false);
        layout->addWidget(new QLabel("Aspect Ratio:"), 4, 0);
        layout->addWidget(mLineEditAspectRatio, 4, 1);

        mLineEditHorizontalFov = new QLineEdit;
        mLineEditHorizontalFov->setValidator(new QDoubleValidator(0.1, 179, 2));
        layout->addWidget(new QLabel("Horizontal Fov (°):"), 5, 0);
        layout->addWidget(mLineEditHorizontalFov, 5, 1);
        connect(mLineEditHorizontalFov, &QLineEdit::editingFinished, this, [this]() {
            mParameters->frustum.horizontalFov = mLineEditHorizontalFov->text().toFloat();
            emit dirty();
        });

        mLineEditVerticalFov = new QLineEdit;
        mLineEditVerticalFov->setEnabled(false);
        layout->addWidget(new QLabel("Vertical Fov (°):"), 6, 0);
        layout->addWidget(mLineEditVerticalFov, 6, 1);

        mLineEditZNear = new QLineEdit;
        mLineEditZNear->setValidator(new QDoubleValidator(0.1, 1000, 1));
        layout->addWidget(new QLabel("Z Near (m):"), 7, 0);
        layout->addWidget(mLineEditZNear, 7, 1);
        connect(mLineEditZNear, &QLineEdit::editingFinished, this, [this]() {
            mParameters->frustum.zNear = mLineEditZNear->text().toFloat();
            emit dirty();
        });

        mLineEditZFar = new QLineEdit;
        mLineEditZFar->setValidator(new QDoubleValidator(0.1, 10000, 1));
        layout->addWidget(new QLabel("Z Far (m):"), 8, 0);
        layout->addWidget(mLineEditZFar, 8, 1);
        connect(mLineEditZFar, &QLineEdit::editingFinished, this, [this]() {
            mParameters->frustum.zFar = mLineEditZFar->text().toFloat();
            emit dirty();
        });

        mCameraGroupBox->setLayout(layout);
    }

    // Target
    {
        mTargetGroupBox = new QGroupBox("Target");
        QGridLayout *layout = new QGridLayout;

        mLineEditTargetHeight = new QLineEdit;
        mLineEditTargetHeight->setValidator(new QDoubleValidator(0.1, 10000, 1));
        layout->addWidget(new QLabel("Height (m):"), 1, 0);
        layout->addWidget(mLineEditTargetHeight, 1, 1);
        connect(mLineEditTargetHeight, &QLineEdit::editingFinished, this, [this]() {
            mParameters->target.height = mLineEditTargetHeight->text().toFloat();
            emit dirty();
        });

        mLineEditTargetDistance = new QLineEdit;
        mLineEditTargetDistance->setValidator(new QDoubleValidator(0.1, 10000, 1));
        layout->addWidget(new QLabel("Distance (m):"), 2, 0);
        layout->addWidget(mLineEditTargetDistance, 2, 1);
        connect(mLineEditTargetDistance, &QLineEdit::editingFinished, this, [this]() {
            mParameters->target.distance = mLineEditTargetDistance->text().toFloat();
            emit dirty();
        });

        mLineEditFovWidth = new QLineEdit;
        mLineEditFovWidth->setValidator(new QDoubleValidator(0.1, 10000, 1));
        layout->addWidget(new QLabel("Fov Width (m):"), 3, 0);
        layout->addWidget(mLineEditFovWidth, 3, 1);
        connect(mLineEditFovWidth, &QLineEdit::editingFinished, this, [this]() {
            mParameters->target.fovWidth = mLineEditFovWidth->text().toFloat();
            emit dirty();
        });

        mTargetGroupBox->setLayout(layout);
    }

    // Lower Boundary
    {
        mLowerBoundaryGroupBox = new QGroupBox("Lower Boundary");
        QGridLayout *layout = new QGridLayout;

        mLineEditLowerBoundaryHeight = new QLineEdit;
        mLineEditLowerBoundaryHeight->setValidator(new QDoubleValidator(0.01, 10000, 2));
        layout->addWidget(new QLabel("Height (m):"), 1, 0);
        layout->addWidget(mLineEditLowerBoundaryHeight, 1, 1);
        connect(mLineEditLowerBoundaryHeight, &QLineEdit::editingFinished, this, [this]() {
            mParameters->lowerBoundary.height = mLineEditLowerBoundaryHeight->text().toFloat();
            emit dirty();
        });

        mLineEditLowerBoundaryDistance = new QLineEdit;
        mLineEditLowerBoundaryDistance->setEnabled(false);
        mLineEditLowerBoundaryDistance->setValidator(new QDoubleValidator(0.01, 10000, 2));
        layout->addWidget(new QLabel("Distance (m):"), 2, 0);
        layout->addWidget(mLineEditLowerBoundaryDistance, 2, 1);

        mLowerBoundaryGroupBox->setLayout(layout);
    }

    // Cursor Positon
    {
        mCursorPositionGroupBox = new QGroupBox("Cursor Position");
        QGridLayout *layout = new QGridLayout;

        mLabelCursorPosition = new QLabel;
        layout->addWidget(new QLabel("Cursor Position (x, y):"), 0, 0);
        layout->addWidget(mLabelCursorPosition, 0, 1);

        mCursorPositionGroupBox->setLayout(layout);
    }

    // Regions
    {
        mRegionInfoGroupBox = new QGroupBox("Regions");
        QGridLayout *layout = new QGridLayout;

        QStringList names;
        names << "Strong Identification"
              << "Identification"
              << "Recognition"
              << "Observation"
              << "Detection"
              << "Monitoring"
              << "Dead Zone";

        for (int i = 0; i < names.length(); ++i) {
            mRectangles << new class Rectangle;
            mRectangles[i]->setColor(REGION_COLORS[i]);
            mRectangles[i]->setFixedSize(20, 20);

            mLimitLabels << new QLabel;
            mLimitLabels[i]->setAlignment(Qt::AlignRight);

            QLabel *label = new QLabel(QString("%1 ppm").arg(REGION_PPMS[i]));
            label->setAlignment(Qt::AlignRight);

            layout->addWidget(mRectangles[i], i, 0);
            layout->addWidget(new QLabel(names[i]), i, 1);
            layout->addWidget(label, i, 2);
            layout->addWidget(mLimitLabels[i], i, 3);
        }

        mRegionInfoGroupBox->setLayout(layout);
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 5);
    mainLayout->addWidget(mSideViewWidget, 0, 1, 2, 1);
    mainLayout->addWidget(mTopViewWidget, 2, 1, 4, 1);
    mainLayout->addWidget(mCameraGroupBox, 0, 0);
    mainLayout->addWidget(mTargetGroupBox, 1, 0);
    mainLayout->addWidget(mLowerBoundaryGroupBox, 2, 0);
    mainLayout->addWidget(mRegionInfoGroupBox, 3, 0);
    mainLayout->addWidget(mCursorPositionGroupBox, 4, 0);
    mainLayout->addItem(new QSpacerItem(-1, -1, QSizePolicy::Ignored, QSizePolicy::Expanding), 5, 0);
    setLayout(mainLayout);
}

void CentralWidget::refresh()
{
    mLineEditCameraHeight->setText(QString("%1").arg(mParameters->camera.height, 0, 'f', 2));
    mLineEditTiltAngle->setText(QString("%1").arg(mParameters->camera.tiltAngle, 0, 'f', 2));
    mLineEditSensorWidth->setText(QString("%1").arg(mParameters->camera.sensor.width, 0, 'f', 0));
    mLineEditSensorHeight->setText(QString("%1").arg(mParameters->camera.sensor.height, 0, 'f', 0));
    mLineEditAspectRatio->setText(QString("%1").arg(mParameters->camera.sensor.aspectRatio, 0, 'f', 2));
    mLineEditHorizontalFov->setText(QString("%1").arg(mParameters->frustum.horizontalFov, 0, 'f', 2));
    mLineEditVerticalFov->setText(QString("%1").arg(mParameters->frustum.verticalFov, 0, 'f', 2));
    mLineEditZNear->setText(QString("%1").arg(mParameters->frustum.zNear, 0, 'f', 2));
    mLineEditZFar->setText(QString("%1").arg(mParameters->frustum.zFar, 0, 'f', 2));
    mLineEditTargetHeight->setText(QString("%1").arg(mParameters->target.height, 0, 'f', 2));
    mLineEditTargetDistance->setText(QString("%1").arg(mParameters->target.distance, 0, 'f', 2));
    mLineEditFovWidth->setText(QString("%1").arg(mParameters->target.fovWidth, 0, 'f', 2));
    mLineEditLowerBoundaryHeight->setText(QString("%1").arg(mParameters->lowerBoundary.height, 0, 'f', 2));
    mLineEditLowerBoundaryDistance->setText(QString("%1").arg(mParameters->lowerBoundary.distance, 0, 'f', 2));
    mLabelCursorPosition->setText(QString("(%1, %2)").arg(mCursorPosition.x(), 0, 'f', 3).arg(mCursorPosition.y(), 0, 'f', 3));
    for (int i = 0; i < mLimitLabels.size(); ++i)
        mLimitLabels[i]->setText(QString("%1 m").arg(mParameters->regions[i].limit, -1, 'f', 2));
}

void CentralWidget::onCursorPositionChanged(QPointF position)
{
    mCursorPosition = position;
    mLabelCursorPosition->setText(QString("(%1, %2)").arg(mCursorPosition.x(), 0, 'f', 3).arg(mCursorPosition.y(), 0, 'f', 3));
}

void CentralWidget::setTopViewWidget(TopViewWidget *newTopViewWidget)
{
    mTopViewWidget = newTopViewWidget;
}

void CentralWidget::setSideViewWidget(SideViewWidget *newSideViewWidget)
{
    mSideViewWidget = newSideViewWidget;
}

void CentralWidget::setParameters(Logic::Parameters *newParameters)
{
    mParameters = newParameters;
}
