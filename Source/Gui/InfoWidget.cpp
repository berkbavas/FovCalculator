#include "InfoWidget.h"

#include "Core/Logic.h"
#include "Util/Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>
#include <QtImGui.h>
#include <imgui.h>

FovCalculator::InfoWidget::InfoWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    connect(this, &QOpenGLWidget::frameSwapped, [=]()
            { update(); });

    setFocusPolicy(Qt::ClickFocus);
}

void FovCalculator::InfoWidget::initializeGL()
{
    initializeOpenGLFunctions();

    QtImGui::initialize(this);
}

void FovCalculator::InfoWidget::paintGL()
{
    glClearColor(0.054, 0.054, 0.054, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    QtImGui::newFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoBackground           //
                                       | ImGuiWindowFlags_NoMove           //
                                       | ImGuiWindowFlags_NoDecoration     //
                                       | ImGuiWindowFlags_NoCollapse       //
                                       | ImGuiWindowFlags_NoTitleBar       //
                                       | ImGuiWindowFlags_AlwaysAutoResize //
                                       | ImGuiWindowFlags_HorizontalScrollbar);

    DrawCameraInputs();
    ImGui::Dummy(ImVec2(1, 16));

    DrawTargetInputs();
    ImGui::Dummy(ImVec2(1, 16));

    DrawLowerBoundaryInputs();
    ImGui::Dummy(ImVec2(1, 16));

    DrawRegionLegend();
    ImGui::Dummy(ImVec2(1, 16));

    DrawMousePosition();

    ImGui::End();

    ImGui::Render();
    QtImGui::render();
}

void FovCalculator::InfoWidget::DrawCameraInputs()
{
    ImGui::Text("Camera");

    if (ImGui::InputFloat("Camera Height (m)", &mCameraHeight, 0.1, 10, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit CameraHeightChanged(mCameraHeight);

    ImGui::BeginDisabled();
    ImGui::InputFloat("Tilt Angle (°) ", &mTiltAngle);
    ImGui::EndDisabled();

    if (ImGui::InputFloat("Sensor Width (px)", &mSensorWidth, 0.1, 10, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit SensorWidthChanged(mSensorWidth);

    if (ImGui::InputFloat("Sensor Height (px)", &mSensorHeight, 0.1, 10, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit SensorHeightChanged(mSensorHeight);

    ImGui::BeginDisabled();
    ImGui::InputFloat("Aspect Ratio", &mAspectRatio);
    ImGui::EndDisabled();

    if (ImGui::InputFloat("Horizontal Fov (°)", &mHorizontalFov, 0.1, 1, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit HorizontalFovChanged(mHorizontalFov);

    ImGui::BeginDisabled();
    ImGui::InputFloat("Vertical Fov (°)", &mVerticalFov);
    ImGui::EndDisabled();

    if (ImGui::InputFloat("Z Near (m)", &mZNear, 0.01, 1, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit ZNearChanged(mZNear);

    if (ImGui::InputFloat("Z Far (m)", &mZFar, 1, 10, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit ZFarChanged(mZFar);
}

void FovCalculator::InfoWidget::DrawTargetInputs()
{
    ImGui::Text("Target");
    if (ImGui::InputFloat("Target Height (m)", &mTargetHeight, 0.1, 10, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit TargetHeightChanged(mTargetHeight);

    if (ImGui::InputFloat("Target Distance (m)", &mTargetDistance, 0.1, 10, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit TargetDistanceChanged(mTargetDistance);

    ImGui::BeginDisabled();
    ImGui::InputFloat("Fov Width (m)", &mFovWidth);
    ImGui::EndDisabled();
}

void FovCalculator::InfoWidget::DrawLowerBoundaryInputs()
{
    ImGui::Text("Lower Boundary");
    if (ImGui::InputFloat("Height (m)##LowerBoundary", &mLowerBoundaryHeight, 0.1, 10, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        emit LowerBoundaryHeightChanged(mLowerBoundaryHeight);

    ImGui::BeginDisabled();
    ImGui::InputFloat("Distance (m)##LowerBoundary", &mLowerBoundaryDistance);
    ImGui::EndDisabled();
}

void FovCalculator::InfoWidget::DrawRegionLegend()
{
    ImGui::Text("Regions");
    ImGui::ColorButton("Strong Identification", ImVec4(104 / 255.0f, 51 / 255.0f, 78 / 255.0f, 1.0f), ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine();
    ImGui::Text("Strong Identification");

    ImGui::ColorButton("Identification", ImVec4(100 / 255.0f, 65 / 255.0f, 67 / 255.0f, 1.0f), ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine();
    ImGui::Text("Identification");

    ImGui::ColorButton("Recognition", ImVec4(104 / 255.0f, 101 / 255.0f, 47 / 255.0f, 1.0f), ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine();
    ImGui::Text("Recognition");

    ImGui::ColorButton("Observation", ImVec4(72 / 255.0f, 89 / 255.0f, 68 / 255.0f, 1.0f), ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine();
    ImGui::Text("Observation");

    ImGui::ColorButton("Detection", ImVec4(73 / 255.0f, 105 / 255.0f, 73 / 255.0f, 1.0f), ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine();
    ImGui::Text("Detection");

    ImGui::ColorButton("Monitoring", ImVec4(65 / 255.0f, 73 / 255.0f, 90 / 255.0f, 1.0f), ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine();
    ImGui::Text("Monitoring");

    ImGui::ColorButton("Dead Zone", ImVec4(87 / 255.0f, 87 / 255.0f, 87 / 255.0f, 1.0f), ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine();
    ImGui::Text("Dead Zone");
}

void FovCalculator::InfoWidget::DrawMousePosition()
{
    ImGui::Text("Mouse Position: (%.2f, %.2f)", mMousePosition.x(), mMousePosition.y());
}
