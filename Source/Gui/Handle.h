#pragma once

#include <QPen>

namespace FovCalculator
{
    class Handle
    {
      public:
        Handle() = default;

        bool Contains(float x, float y);
        bool Contains(const QPointF& point);

        void Draw(QPaintDevice* device);

        void SetPressed(bool newPressed);
        bool GetPressed() const;

        void SetHovered(bool newHovered);
        bool GetHovered() const;

        void SetPen(const QPen& newPen);
        void SetBrush(const QBrush& newBrush);
        void SetHoveredBrush(const QBrush& newHoveredBrush);
        void SetPressedBrush(const QBrush& newPressedBrush);

        void SetCenter(float x, float y);
        void SetCenter(const QPointF& center);

        void SetSize(float w, float h);

        QPointF GetCenter();
        QPointF GetCenter(float leftOffset, float topOffset);

      private:
        QRect mRectangle;
        QBrush mBrush;
        QBrush mPressedBrush;
        QBrush mHoveredBrush;
        QPen mPen;

        bool mPressed{ false };
        bool mHovered{ false };
    };
}