#pragma once

#include "Core/Constants.h"
#include "Util/Macros.h"

#include <Dense>
#include <QObject>
#include <QPointF>

namespace FovCalculator
{
    class Converter : public QObject
    {
        Q_OBJECT
      public:
        explicit Converter(QObject* parent);

        QPointF MapFromWorldToTopView(const Eigen::Vector3f& point);
        QPointF MapFromWorldToTopView(float x, float y);
        Eigen::Vector3f MapFromTopViewToWorld(const QPointF& point);

        QPointF MapFromWorldToSideView(const Eigen::Vector3f& point);
        QPointF MapFromWorldToSideView(float x, float z);
        Eigen::Vector3f MapFromSideViewToWorld(const QPointF& point);

        float ConvertDistanceFromWorldToGui(float distance);
        float ConvertDistanceFromGuiToWorld(float distance);

        void Pan(const QPointF& delta);
        void ZoomIn();
        void ZoomOut();

        void SetOrigin(const QPointF& newOrigin);
        void SetMeterToPixelRatio(float newMeterToPixelRatio);

      signals:
        void GuiNeedsUpdate();

      private:
        DEFINE_MEMBER_CONST(float, MeterToPixelRatio, INITIAL_METER_TO_PIXEL_RATIO);
        DEFINE_MEMBER_CONST(QPointF, Origin, { 350, 350 });
    };
}