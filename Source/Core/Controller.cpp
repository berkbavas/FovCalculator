#include "Controller.h"

#include <QGridLayout>
#include <QPolygonF>
#include <QStyle>
#include <QtMath>
#include <cmath>

FovCalculator::Controller::Controller(QObject* parent)
{
    mLogic = new Logic;
    mConverter = new Converter(this);
    mWindow = new Window;

    mInfoWidget = mWindow->GetInfoWidget();

    mTopViewWidget = mWindow->GetTopViewWidget();
    mSideViewWidget = mWindow->GetSideViewWidget();
    connect(mConverter, &Converter::GuiNeedsUpdate, this, &Controller::Update);

    // connect(mInfoWidget, &InfoWidget::GuiNeedsUpdate, this, &Controller::Update);
    connect(mTopViewWidget, &TopViewWidget::UserRequestsPan, this, &Controller::OnUserRequestsPan);
    connect(mTopViewWidget, &TopViewWidget::UserRequestsTargetDistanceDeltaChange, this, &Controller::OnUserRequestsTargetDistanceDeltaChange);
    connect(mTopViewWidget, &TopViewWidget::UserRequestsFovWidthDeltaChange, this, &Controller::OnUserRequestsFovWidthDeltaChange);
    connect(mTopViewWidget, &TopViewWidget::WheelMoved, this, &Controller::OnWheelMoved);
    connect(mTopViewWidget, &TopViewWidget::MouseMoved, this, &Controller::OnMouseMoved);

    connect(mSideViewWidget, &SideViewWidget::UserRequestsPan, this, &Controller::OnUserRequestsPan);
    connect(mSideViewWidget, &SideViewWidget::WheelMoved, this, &Controller::OnWheelMoved);
    connect(mSideViewWidget, &SideViewWidget::UserRequestsTargetDistanceDeltaChange, this, &Controller::OnUserRequestsTargetDistanceDeltaChange);
    connect(mSideViewWidget, &SideViewWidget::UserRequestsTargetHeightDeltaChange, this, &Controller::OnUserRequestsTargetHeightDeltaChange);
    connect(mSideViewWidget, &SideViewWidget::UserRequestsCameraHeightDeltaChange, this, &Controller::OnUserRequestsCameraHeightDeltaChange);
    connect(mSideViewWidget, &SideViewWidget::UserRequestsLowerBoundaryHeightDeltaChange, this, &Controller::OnUserRequestsLowerBoundaryHeightDeltaChange);
    connect(mSideViewWidget, &SideViewWidget::MouseMoved, this, &Controller::OnMouseMoved);

    connect(mInfoWidget, &InfoWidget::CameraHeightChanged, this, &Controller::OnUserRequestsCameraHeightChange);
    connect(mInfoWidget, &InfoWidget::TargetHeightChanged, this, &Controller::OnUserRequestsTargetHeightChange);
    connect(mInfoWidget, &InfoWidget::TargetDistanceChanged, this, &Controller::OnUserRequestsTargetDistanceChange);
    connect(mInfoWidget, &InfoWidget::LowerBoundaryHeightChanged, this, &Controller::OnUserRequestsLowerBoundaryHeightChange);

    connect(mInfoWidget, &InfoWidget::SensorWidthChanged, this, [this](float newValue)
            {
                mLogic->SetSensorWidth(newValue);
                Update(); });

    connect(mInfoWidget, &InfoWidget::SensorHeightChanged, this, [this](float newValue)
            {
                mLogic->SetSensorHeight(newValue);
                Update(); });

    connect(mInfoWidget, &InfoWidget::HorizontalFovChanged, this, [this](float newValue)
            {
                mLogic->SetHorizontalFov(newValue);
                Update(); });

    connect(mInfoWidget, &InfoWidget::ZNearChanged, this, [this](float newValue)
            {
                mLogic->SetZNear(newValue);
                Update(); });

    connect(mInfoWidget, &InfoWidget::ZFarChanged, this, [this](float newValue)
            {
                mLogic->SetZFar(newValue);
                Update(); });
}

void FovCalculator::Controller::Run()
{
    mWindow->showMaximized();
    Update();
}

void FovCalculator::Controller::Update()
{
    mLogic->Calculate();

    UpdateInfoWidget();
    UpdateSideViewWidget();
    UpdateTopViewWidget();
}

void FovCalculator::Controller::UpdateInfoWidget()
{
    mInfoWidget->SetCameraHeight(mLogic->GetCameraHeight());
    mInfoWidget->SetTiltAngle(mLogic->GetTiltAngle());
    mInfoWidget->SetSensorWidth(mLogic->GetSensorWidth());
    mInfoWidget->SetSensorHeight(mLogic->GetSensorHeight());
    mInfoWidget->SetAspectRatio(mLogic->GetAspectRatio());
    mInfoWidget->SetHorizontalFov(mLogic->GetHorizontalFov());
    mInfoWidget->SetVerticalFov(mLogic->GetVerticalFov());
    mInfoWidget->SetZNear(mLogic->GetZNear());
    mInfoWidget->SetZFar(mLogic->GetZFar());
    mInfoWidget->SetTargetHeight(mLogic->GetTargetHeight());
    mInfoWidget->SetTargetDistance(mLogic->GetTargetDistance());
    mInfoWidget->SetFovWidth(mLogic->GetFovWidth());
    mInfoWidget->SetLowerBoundaryHeight(mLogic->GetLowerBoundaryHeight());
    mInfoWidget->SetLowerBoundaryDistance(mLogic->GetLowerBoundaryDistance());
}

void FovCalculator::Controller::UpdateSideViewWidget()
{
    mSideViewWidget->SetMeterToPixelRatio(mConverter->GetMeterToPixelRatio());
    mSideViewWidget->SetOrigin(mConverter->GetOrigin());
    mSideViewWidget->SetCameraHeight(mConverter->ConvertDistanceFromWorldToGui(mLogic->GetCameraHeight()));
    mSideViewWidget->SetTargetDistance(mConverter->ConvertDistanceFromWorldToGui(mLogic->GetTargetDistance()));
    mSideViewWidget->SetTargetHeight(mConverter->ConvertDistanceFromWorldToGui(mLogic->GetTargetHeight()));
    mSideViewWidget->SetLowerBoundaryDistance(mConverter->ConvertDistanceFromWorldToGui(mLogic->GetLowerBoundaryDistance()));
    mSideViewWidget->SetLowerBoundaryHeight(mConverter->ConvertDistanceFromWorldToGui(mLogic->GetLowerBoundaryHeight()));
    mSideViewWidget->SetBisectorIntersection(mConverter->MapFromWorldToSideView(mLogic->GetBisector(0)));
    mSideViewWidget->SetOppositeBisectorIntersection(mConverter->MapFromWorldToSideView(mLogic->GetBisector(1)));
    mSideViewWidget->SetTiltAngle(mLogic->GetTiltAngle());

    for (int i = 0; i < 4; i++)
    {
        mSideViewWidget->SetGroundIntersection(i, mConverter->MapFromWorldToSideView(mLogic->GetFrustumBottomVertex(i)));
        mSideViewWidget->SetTargetIntersection(i, mConverter->MapFromWorldToSideView(mLogic->GetTargetIntersection(Edge(i))));
        mSideViewWidget->SetLowerBoundaryIntersection(i, mConverter->MapFromWorldToSideView(mLogic->GetLowerBoundaryIntersection(Edge(i))));
    }

    QPolygonF roi;

    roi.append(mConverter->MapFromWorldToSideView(mLogic->GetTargetDistance(), mLogic->GetTargetHeight()));
    roi.append(mConverter->MapFromWorldToSideView(mLogic->GetTargetDistance(), mLogic->GetLowerBoundaryHeight()));
    roi.append(mConverter->MapFromWorldToSideView(mLogic->GetLowerBoundaryDistance(), mLogic->GetLowerBoundaryHeight()));
    roi.append(mConverter->MapFromWorldToSideView(mLogic->GetLowerBoundaryDistance(), mLogic->GetTargetHeight()));

    for (int i = 0; i < 7; ++i)
    {
        QPolygonF region;

        region.append(mConverter->MapFromWorldToSideView(mLogic->GetRegion(i).bottomVertices[0]));
        region.append(mConverter->MapFromWorldToSideView(mLogic->GetRegion(i).topVertices[0]));
        region.append(mConverter->MapFromWorldToSideView(mLogic->GetRegion(i).topVertices[2]));
        region.append(mConverter->MapFromWorldToSideView(mLogic->GetRegion(i).bottomVertices[2]));

        region = region.intersected(roi);
        mSideViewWidget->SetRegion(i, region);
    }

    mSideViewWidget->update();
}

void FovCalculator::Controller::UpdateTopViewWidget()
{
    mTopViewWidget->SetOrigin(mConverter->GetOrigin());
    mTopViewWidget->SetFovWidth(mLogic->GetFovWidth());
    mTopViewWidget->SetHorizontalFov(mLogic->GetHorizontalFov());

    for (int i = 0; i < 4; i++)
    {
        mTopViewWidget->SetGroundIntersection(i, mConverter->MapFromWorldToTopView(mLogic->GetFrustumBottomVertex(i)));
        mTopViewWidget->SetTargetIntersection(i, mConverter->MapFromWorldToTopView(mLogic->GetTargetIntersection(Edge(i))));
    }

    QPolygonF targetRoi;

    if (std::abs(mLogic->GetCameraHeight() - mLogic->GetTargetHeight()) < 0.0001f)
    {
        targetRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetTargetDistance(), 0.5f * mLogic->GetFovWidth()));
        targetRoi.append(mConverter->MapFromWorldToTopView(0, 0));
        targetRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetTargetDistance(), -0.5f * mLogic->GetFovWidth()));
    }
    else if (mLogic->GetCameraHeight() < mLogic->GetTargetHeight())
    {
        targetRoi.append(mConverter->MapFromWorldToTopView(0, 0));
        targetRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetTargetIntersection(Edge(0))));
        targetRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetTargetIntersection(Edge(3))));
    }
    else
    {
        targetRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetTargetIntersection(Edge(0))));
        targetRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetTargetIntersection(Edge(1))));
        targetRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetTargetIntersection(Edge(2))));
        targetRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetTargetIntersection(Edge(3))));
    }

    QPolygonF lowerBoundaryRoi;
    lowerBoundaryRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetLowerBoundaryIntersection(Edge(0))));
    lowerBoundaryRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetLowerBoundaryIntersection(Edge(1))));
    lowerBoundaryRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetLowerBoundaryIntersection(Edge(2))));
    lowerBoundaryRoi.append(mConverter->MapFromWorldToTopView(mLogic->GetLowerBoundaryIntersection(Edge(3))));

    QPolygonF roi = lowerBoundaryRoi.intersected(targetRoi);

    for (int i = 0; i < 7; ++i)
    {
        Eigen::Hyperplane<float, 3> plane(Eigen::Vector3f(0, 0, 1).normalized(), -mLogic->GetLowerBoundaryHeight());
        QVector<Eigen::Vector3f> intersections = mLogic->FindIntersection(mLogic->GetRegion(RegionType(i)), plane);

        QPolygonF region;
        for (int i = 0; i < intersections.size(); ++i)
        {
            region.append(mConverter->MapFromWorldToTopView(intersections[i]));
        }

        region = region.intersected(roi);
        mTopViewWidget->SetRegion(i, region);
    }

    mTopViewWidget->update();
}

void FovCalculator::Controller::OnUserRequestsPan(const QPointF& delta)
{
    mConverter->Pan(delta);
}

void FovCalculator::Controller::OnWheelMoved(QWheelEvent* event)
{
    if (event->angleDelta().y() < 0)
    {
        mConverter->ZoomIn();
    }
    else
    {
        mConverter->ZoomOut();
    }
}

void FovCalculator::Controller::OnUserRequestsTargetDistanceDeltaChange(const QPointF& delta)
{
    float worldDelta = mConverter->ConvertDistanceFromGuiToWorld(delta.x());
    float newTargetDistance = mLogic->GetTargetDistance() + worldDelta;
    OnUserRequestsTargetDistanceChange(newTargetDistance);
}

void FovCalculator::Controller::OnUserRequestsFovWidthDeltaChange(float delta)
{
    float worldDelta = mConverter->ConvertDistanceFromGuiToWorld(delta);
    float newFovWidth = mLogic->GetFovWidth() + worldDelta;
    if (0.1f <= newFovWidth)
    {
        float hfov = mLogic->CalculateHorizontalFovForGivenFovWidth(newFovWidth);
        mLogic->SetHorizontalFov(hfov);
    }
    Update();
}

void FovCalculator::Controller::OnUserRequestsTargetHeightDeltaChange(const QPointF& delta)
{
    float worldDelta = mConverter->ConvertDistanceFromGuiToWorld(-delta.y());
    float newTargetHeight = mLogic->GetTargetHeight() + worldDelta;
    OnUserRequestsTargetHeightChange(newTargetHeight);
}

void FovCalculator::Controller::OnUserRequestsCameraHeightDeltaChange(const QPointF& delta)
{
    float worldDelta = mConverter->ConvertDistanceFromGuiToWorld(-delta.y());
    float newCameraHeight = mLogic->GetCameraHeight() + worldDelta;
    OnUserRequestsCameraHeightChange(newCameraHeight);
}

void FovCalculator::Controller::OnUserRequestsLowerBoundaryHeightDeltaChange(const QPointF& delta)
{
    float worldDelta = mConverter->ConvertDistanceFromGuiToWorld(-delta.y());
    float newLowerBoundaryHeight = mLogic->GetLowerBoundaryHeight() + worldDelta;
    OnUserRequestsLowerBoundaryHeightChange(newLowerBoundaryHeight);
}

void FovCalculator::Controller::OnUserRequestsCameraHeightChange(float newHeight)
{
    if (0.1f <= newHeight)
    {
        float validatedCameraHeight = mLogic->ValidateCameraHeight(newHeight);
        mLogic->SetCameraHeight(validatedCameraHeight);
    }

    Update();
}

void FovCalculator::Controller::OnUserRequestsTargetHeightChange(float newHeight)
{
    if (0.1f <= newHeight)
    {
        float validatedTargetHeight = mLogic->ValidateTargetHeight(newHeight);
        mLogic->SetTargetHeight(validatedTargetHeight);
    }

    Update();
}

void FovCalculator::Controller::OnUserRequestsTargetDistanceChange(float newDistance)
{
    if (0.1f < newDistance)
    {
        float validatedTargetDistance = mLogic->ValidateTargetDistance(newDistance);
        mLogic->SetTargetDistance(validatedTargetDistance);
    }

    Update();
}

void FovCalculator::Controller::OnUserRequestsLowerBoundaryHeightChange(float newHeight)
{
    float upperBound = qMin(mLogic->GetCameraHeight(), mLogic->GetTargetHeight()) - 0.01;
    newHeight = qBound(0.0f, newHeight, upperBound);

    mLogic->SetLowerBoundaryHeight(newHeight);
    Update();
}

void FovCalculator::Controller::OnMouseMoved(const QPointF& position)
{
    if (sender() == mTopViewWidget)
    {
        const auto worldPosition = mConverter->MapFromTopViewToWorld(position);
        mInfoWidget->SetMousePosition(QPointF(worldPosition.x(), worldPosition.z()));
    }
    else if (sender() == mSideViewWidget)
    {
        const auto worldPosition = mConverter->MapFromSideViewToWorld(position);
        mInfoWidget->SetMousePosition(QPointF(worldPosition.x(), worldPosition.z()));
    }
}
