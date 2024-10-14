#include "Handle.h"

#include <QMouseEvent>
#include <QPainter>

bool FovCalculator::Handle::Contains(float x, float y)
{
    return mRectangle.contains(x, y);
}

bool FovCalculator::Handle::Contains(const QPointF& point)
{
    return mRectangle.contains(point.toPoint());
}

void FovCalculator::Handle::SetCenter(float x, float y)
{
    mRectangle = QRect(x - 0.5f * mRectangle.width(), y - 0.5f * mRectangle.height(), mRectangle.width(), mRectangle.height());
}

void FovCalculator::Handle::SetCenter(const QPointF& center)
{
    SetCenter(center.x(), center.y());
}

void FovCalculator::Handle::SetSize(float w, float h)
{
    mRectangle = QRect(mRectangle.x(), mRectangle.y(), w, h);
}

QPointF FovCalculator::Handle::GetCenter()
{
    return mRectangle.center();
}

QPointF FovCalculator::Handle::GetCenter(float leftOffset, float topOffset)
{
    QPointF center = mRectangle.center();
    return QPointF(center.x() + leftOffset, center.y() + topOffset);
}

void FovCalculator::Handle::Draw(QPaintDevice* device)
{
    QPainter painter(device);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(mPen);

    if (mPressed)
        painter.setBrush(mPressedBrush);
    else if (mHovered)
        painter.setBrush(mHoveredBrush);
    else
        painter.setBrush(mBrush);

    painter.drawRect(mRectangle);
}

bool FovCalculator::Handle::GetPressed() const
{
    return mPressed;
}

void FovCalculator::Handle::SetPressed(bool newPressed)
{
    mPressed = newPressed;
}

bool FovCalculator::Handle::GetHovered() const
{
    return mHovered;
}

void FovCalculator::Handle::SetHovered(bool newHovered)
{
    mHovered = newHovered;
}

void FovCalculator::Handle::SetPen(const QPen& newPen)
{
    mPen = newPen;
}

void FovCalculator::Handle::SetBrush(const QBrush& newBrush)
{
    mBrush = newBrush;
}

void FovCalculator::Handle::SetHoveredBrush(const QBrush& newHoveredBrush)
{
    mHoveredBrush = newHoveredBrush;
}

void FovCalculator::Handle::SetPressedBrush(const QBrush& newPressedBrush)
{
    mPressedBrush = newPressedBrush;
}
