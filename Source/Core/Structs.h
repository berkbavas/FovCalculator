#pragma once

#include <Dense>

namespace FovCalculator
{
    struct Region
    {
        Eigen::Vector3f topVertices[4];
        Eigen::Vector3f bottomVertices[4];
        float limit;
    };
}