#include "Handle.h"

#include <QMouseEvent>
#include <QPainter>

Handle::Handle()
    : mPressed(false)
    , mHovered(false)
{}

bool Handle::contains(float x, float y)
{
    return mRectangle.contains(x, y);
}

bool Handle::contains(const QPoint &point)
{
    return mRectangle.contains(point);
}
void Handle::setCenter(float x, float y)
{
    mRectangle = QRect(x - 0.5f * mRectangle.width(), y - 0.5f * mRectangle.height(), mRectangle.width(), mRectangle.height());
}

void Handle::setCenter(const QPointF &center)
{
    setCenter(center.x(), center.y());
}

void Handle::setSize(float w, float h)
{
    mRectangle = QRect(mRectangle.x(), mRectangle.y(), w, h);
}

QPointF Handle::getCenter()
{
    return mRectangle.center();
}

QPointF Handle::getCenter(float leftOffset, float topOffset)
{
    QPointF center = mRectangle.center();
    return QPointF(center.x() + leftOffset, center.y() + topOffset);
}

void Handle::draw(QPaintDevice *device)
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

bool Handle::pressed() const
{
    return mPressed;
}

void Handle::setPressed(bool newPressed)
{
    mPressed = newPressed;
}

bool Handle::hovered() const
{
    return mHovered;
}

void Handle::setHovered(bool newHovered)
{
    mHovered = newHovered;
}
void Handle::setPen(const QPen &newPen)
{
    mPen = newPen;
}

void Handle::setBrush(const QBrush &newBrush)
{
    mBrush = newBrush;
}

void Handle::setHoveredBrush(const QBrush &newHoveredBrush)
{
    mHoveredBrush = newHoveredBrush;
}

void Handle::setPressedBrush(const QBrush &newPressedBrush)
{
    mPressedBrush = newPressedBrush;
}
