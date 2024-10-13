#include "Converter.h"

FovCalculator::Converter::Converter(QObject* parent)
    : QObject(parent)
{
}

QPointF FovCalculator::Converter::MapFromWorldToTopView(const Eigen::Vector3f& point)
{
    float x = mOrigin.x() + point.x() * mMeterToPixelRatio;
    float y = mOrigin.y() - point.y() * mMeterToPixelRatio;
    return QPointF(x, y);
}

QPointF FovCalculator::Converter::MapFromWorldToTopView(float x, float y)
{
    return MapFromWorldToTopView(Eigen::Vector3f(x, y, 0));
}

Eigen::Vector3f FovCalculator::Converter::MapFromTopViewToWorld(const QPointF& point)
{
    Eigen::Vector3f vector;
    vector[0] = (point.x() - mOrigin.x()) / mMeterToPixelRatio;
    vector[1] = 0;
    vector[2] = (mOrigin.y() - point.y()) / mMeterToPixelRatio;
    return vector;
}

QPointF FovCalculator::Converter::MapFromWorldToSideView(const Eigen::Vector3f& point)
{
    float x = mOrigin.x() + point.x() * mMeterToPixelRatio;
    float y = mOrigin.y() - point.z() * mMeterToPixelRatio;
    return QPointF(x, y);
}

QPointF FovCalculator::Converter::MapFromWorldToSideView(float x, float z)
{
    return MapFromWorldToSideView(Eigen::Vector3f(x, 0, z));
}

Eigen::Vector3f FovCalculator::Converter::MapFromSideViewToWorld(const QPointF& point)
{
    Eigen::Vector3f vector;

    vector[0] = (point.x() - mOrigin.x()) / mMeterToPixelRatio;
    vector[1] = 0;
    vector[2] = (mOrigin.y() - point.y()) / mMeterToPixelRatio;
    return vector;
}

float FovCalculator::Converter::ConvertDistanceFromWorldToGui(float distance)
{
    return distance * mMeterToPixelRatio;
}

float FovCalculator::Converter::ConvertDistanceFromGuiToWorld(float distance)
{
    return distance / mMeterToPixelRatio;
}

void FovCalculator::Converter::Pan(const QPointF& delta)
{
    SetOrigin(mOrigin + delta);
}

void FovCalculator::Converter::ZoomIn()
{
    SetMeterToPixelRatio(2.0f * mMeterToPixelRatio);
}

void FovCalculator::Converter::ZoomOut()
{
    SetMeterToPixelRatio(mMeterToPixelRatio / 2.0f);
}

void FovCalculator::Converter::SetOrigin(const QPointF& newOrigin)
{
    if (mOrigin == newOrigin)
        return;
    mOrigin = newOrigin;
    emit GuiNeedsUpdate();
}

void FovCalculator::Converter::SetMeterToPixelRatio(float newMeterToPixelRatio)
{
    if (qFuzzyCompare(mMeterToPixelRatio, newMeterToPixelRatio))
        return;

    if (newMeterToPixelRatio < 2.0 || newMeterToPixelRatio > 512.0f)
    {
        return;
    }

    mMeterToPixelRatio = newMeterToPixelRatio;
    emit GuiNeedsUpdate();
}
