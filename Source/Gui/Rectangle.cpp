#include "Rectangle.h"

#include <QPainter>

Rectangle::Rectangle(QWidget *parent)
    : QWidget(parent)
{}

void Rectangle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), mColor);
}

const QColor &Rectangle::color() const
{
    return mColor;
}

void Rectangle::setColor(const QColor &newColor)
{
    mColor = newColor;
}
