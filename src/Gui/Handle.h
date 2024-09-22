#ifndef HANDLE_H
#define HANDLE_H

#include <QPen>
#include <QWidget>

class Handle
{
public:
    Handle();

    bool contains(float x, float y);
    bool contains(const QPoint &point);

    void draw(QPaintDevice *device);

    void setPressed(bool newPressed);
    bool pressed() const;

    void setHovered(bool newHovered);
    bool hovered() const;

    void setPen(const QPen &newPen);
    void setBrush(const QBrush &newBrush);
    void setHoveredBrush(const QBrush &newHoveredBrush);
    void setPressedBrush(const QBrush &newPressedBrush);

    void setCenter(float x, float y);
    void setCenter(const QPointF &center);

    void setSize(float w, float h);

    QPointF getCenter();
    QPointF getCenter(float leftOffset, float topOffset);

private:
    QRect mRectangle;
    QBrush mBrush, mPressedBrush, mHoveredBrush;
    QPen mPen;

    bool mPressed;
    bool mHovered;
};

#endif // HANDLE_H
