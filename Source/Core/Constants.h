#pragma once

#include <QColor>

namespace FovCalculator
{
    constexpr float REGION_PPMS[] = { 1000.0f, 250.0f, 125.0f, 62.0f, 26.0f, 12.0f, 0.1f }; // Pixel per meter

    constexpr float DEG_TO_RAD = 0.0174532925f;
    constexpr float RAD_TO_DEG = 1.0f / DEG_TO_RAD;

    constexpr float HANDLE_SIZE = 10.0f; // pixels
    constexpr QColor PRIMARY_COLOR = QColor(128, 128, 128);
    constexpr QColor SECONDARY_COLOR = QColor(0, 65, 135);

    constexpr float INITIAL_CAMERA_HEIGHT = 5.0f;           // meters
    constexpr float INITIAL_CAMERA_SENSOR_WIDTH = 1920.0f;  // px
    constexpr float INITIAL_CAMERA_SENSOR_HEIGHT = 1080.0f; // px
    constexpr float INITIAL_HORIZONTAL_FOV = 90.0f;         // degrees
    constexpr float INITIAL_Z_NEAR = 0.1f;                  // meters
    constexpr float INITIAL_Z_FAR = 1000.0f;                // meters
    constexpr float INITIAL_TARGET_HEIGHT = 4.0f;           // meters
    constexpr float INITIAL_TARGET_DISTANCE = 8.0f;         // meters
    constexpr float INITIAL_LOWER_BOUNDARY_HEIGHT = 0.0f;   // meters
    constexpr float INITIAL_METER_TO_PIXEL_RATIO = 8.0f;

    // constexpr QColor REGION_COLORS[] = { QColor(253, 111, 182, 168),
    //                                      QColor(243, 150, 155, 168),
    //                                      QColor(252, 245, 102, 168),
    //                                      QColor(167, 212, 156, 168),
    //                                      QColor(170, 255, 170, 168),
    //                                      QColor(148, 170, 214, 168),
    //                                      QColor(208, 208, 208, 168) };

    constexpr QColor REGION_COLORS[] = { QColor(172, 78, 125, 128),
                                         QColor(165, 104, 107, 128),
                                         QColor(171, 166, 72, 128),
                                         QColor(115, 145, 108, 128),
                                         QColor(92, 135, 92, 128),
                                         QColor(92, 92, 115, 128),
                                         QColor(128, 128, 128, 128) };

    enum Vertex
    {
        V0,
        V1,
        V2,
        V3
    };
    enum Edge
    {
        E0,
        E1,
        E2,
        E3
    };

    enum RegionType
    {
        STRONG_IDENTIFICATION,
        IDENTIFICATION,
        RECOGNITION,
        OBSERVATION,
        DETECTION,
        MONITORING,
        DEAD_ZONE
    };

}