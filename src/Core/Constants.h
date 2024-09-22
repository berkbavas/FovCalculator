#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QColor>

constexpr float REGION_PPMS[] = {1000.0f, 250.0f, 125.0f, 62.0f, 26.0f, 12.0f, 0.1f}; // Pixel per meter
constexpr QColor REGION_COLORS[] = {QColor(253, 111, 182, 168),
                                    QColor(243, 150, 155, 168),
                                    QColor(252, 245, 102, 168),
                                    QColor(167, 212, 156, 168),
                                    QColor(170, 255, 170, 168),
                                    QColor(148, 170, 214, 168),
                                    QColor(208, 208, 208, 168)};

#endif // CONSTANTS_H
