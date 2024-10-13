#include "InfoWidget.h"

#include "Core/Logic.h"
#include "Util/Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QKeyEvent>
#include <QtImGui.h>
#include <imgui.h>

FovCalculator::InfoWidget::InfoWidget()
    : QOpenGLWidget()
{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(8);
    setFormat(format);

    connect(this, &QOpenGLWidget::frameSwapped, [=]()
            { update(); });
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
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoMove                    //
                                       | ImGuiWindowFlags_NoCollapse          //
                                       | ImGuiWindowFlags_NoTitleBar          //
                                       | ImGuiWindowFlags_AlwaysAutoResize    //
                                       | ImGuiWindowFlags_HorizontalScrollbar //
    );

    ImGui::Text("Camera");
    if (ImGui::InputFloat("Camera Height (m)", &mLogic->GetCameraHeight_NonConst(), 0.1, 10))
        emit GuiNeedsUpdate();

    ImGui::BeginDisabled();
    ImGui::InputFloat("Tilt Angle (°) ", &mLogic->GetTiltAngle_NonConst());
    ImGui::EndDisabled();

    if (ImGui::InputFloat("Sensor Width (px)", &mLogic->GetSensorWidth_NonConst(), 0.1, 10))
        emit GuiNeedsUpdate();

    if (ImGui::InputFloat("Sensor Height (px)", &mLogic->GetSensorHeight_NonConst(), 0.1, 10))
        emit GuiNeedsUpdate();

    ImGui::BeginDisabled();
    ImGui::InputFloat("Aspect Ratio", &mLogic->GetAspectRatio_NonConst());
    ImGui::EndDisabled();

    if (ImGui::InputFloat("Horizontal Fov (°)", &mLogic->GetHorizontalFov_NonConst(), 0.1, 1))
        emit GuiNeedsUpdate();

    ImGui::BeginDisabled();
    ImGui::InputFloat("Vertical Fov (°)", &mLogic->GetVerticalFov_NonConst());
    ImGui::EndDisabled();

    if (ImGui::InputFloat("Z Near (m)", &mLogic->GetZNear_NonConst(), 1, 10))
        emit GuiNeedsUpdate();

    if (ImGui::InputFloat("Z Far (m)", &mLogic->GetZFar_NonConst(), 1, 10))
        emit GuiNeedsUpdate();

    ImGui::Text("Target");
    if (ImGui::InputFloat("Target Height (m):", &mLogic->GetTargetHeight_NonConst(), 0.1, 10))
        emit GuiNeedsUpdate();

    if (ImGui::InputFloat("Target Distance (m)", &mLogic->GetTargetDistance_NonConst(), 0.1, 10))
        emit GuiNeedsUpdate();

    ImGui::BeginDisabled();
    ImGui::InputFloat("Fov Width (m)", &mLogic->GetFovWidth_NonConst());
    ImGui::EndDisabled();

    ImGui::Text("Lower Boundary");
    if (ImGui::InputFloat("Height (m)##LowerBoundary", &mLogic->GetLowerBoundaryHeight_NonConst(), 0.1, 10))
        emit GuiNeedsUpdate();

    if (ImGui::InputFloat("Distance (m)##LowerBoundary", &mLogic->GetLowerBoundaryDistance_NonConst(), 0.1, 10))
        emit GuiNeedsUpdate();

    ImGui::End();

    ImGui::Render();
    QtImGui::render();
}
