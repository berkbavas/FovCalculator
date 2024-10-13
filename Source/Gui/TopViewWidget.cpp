#include "TopViewWidget.h"

#include "Core/Constants.h"

#include <QInputEvent>
#include <QPainter>
#include <QPainterPath>

FovCalculator::TopViewWidget::TopViewWidget(QWidget* parent)
    : QWidget(parent)
{
    mCrossPatternBursh.setStyle(Qt::BrushStyle::CrossPattern);
    mCrossPatternBursh.setColor(QColor(32, 32, 32));

    mDashedPen.setDashPattern(QVector<qreal>{ 3, 3 });
    mDashedPen.setColor(PRIMARY_COLOR);
    mDashedPen.setStyle(Qt::DashLine);
    mDashedPen.setWidthF(1.5f);

    mSolidPen.setWidthF(1.5f);
    mSolidPen.setCapStyle(Qt::FlatCap);

    mCameraHandle.SetBrush(QColor(63, 150, 157));
    mCameraHandle.SetSize(HANDLE_SIZE, HANDLE_SIZE);

    mTargetHandle.SetBrush(QColor(185, 95, 0));
    mTargetHandle.SetHoveredBrush(QColor(255, 255, 255));
    mTargetHandle.SetPressedBrush(QColor(0, 255, 0));
    mTargetHandle.SetSize(HANDLE_SIZE, HANDLE_SIZE);

    mFovWidthHandleTop.SetBrush(QColor(128, 0, 0));
    mFovWidthHandleTop.SetHoveredBrush(QColor(255, 255, 255));
    mFovWidthHandleTop.SetPressedBrush(QColor(0, 255, 0));
    mFovWidthHandleTop.SetSize(HANDLE_SIZE, HANDLE_SIZE);

    mFovWidthHandleBottom.SetBrush(QColor(128, 0, 0));
    mFovWidthHandleBottom.SetHoveredBrush(QColor(255, 255, 255));
    mFovWidthHandleBottom.SetPressedBrush(QColor(0, 255, 0));
    mFovWidthHandleBottom.SetSize(HANDLE_SIZE, HANDLE_SIZE);

    mLabelFont = QFont();
    mLabelFont.setPixelSize(10);
    mLabelColor = PRIMARY_COLOR;

    setMouseTracking(true);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void FovCalculator::TopViewWidget::paintEvent(QPaintEvent* event)
{
    UpdateHandles();
    DrawCrossPattern();
    DrawRegions();
    DrawTargetIntersections();
    DrawGroundIntersections();
    DrawHandles();
    DrawLabels();
}

void FovCalculator::TopViewWidget::UpdateHandles()
{
    mCameraHandle.SetCenter(mOrigin.x(), mOrigin.y());
    mTargetHandle.SetCenter(mTargetIntersections[0].x(), mOrigin.y());
    mFovWidthHandleTop.SetCenter(mTargetIntersections[0]);
    mFovWidthHandleBottom.SetCenter(mTargetIntersections[3]);
}

void FovCalculator::TopViewWidget::DrawCrossPattern()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.fillRect(0, 0, width(), height(), mCrossPatternBursh);
}

void FovCalculator::TopViewWidget::DrawRegions()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < 7; i++)
    {
        if (mRegions[i].empty() == false)
        {
            QPainterPath path;
            path.addPolygon(mRegions[i]);

            QBrush brush;
            brush.setStyle(Qt::BrushStyle::SolidPattern);
            brush.setColor(REGION_COLORS[i]);
            painter.fillPath(path, brush);
        }
    }
}

void FovCalculator::TopViewWidget::DrawGroundIntersections()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    mSolidPen.setColor(PRIMARY_COLOR);
    painter.setPen(mSolidPen);

    for (int i = 0; i < 4; i++)
    {
        painter.drawLine(mGroundIntersections[i], mGroundIntersections[(i + 1) % 4]);
    }
}

void FovCalculator::TopViewWidget::DrawTargetIntersections()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    mSolidPen.setColor(SECONDARY_COLOR);
    painter.setPen(mSolidPen);
    painter.drawLine(mOrigin, mGroundIntersections[1]);
    painter.drawLine(mOrigin, mGroundIntersections[2]);
    painter.drawLine(mOrigin, mTargetIntersections[0]);
    painter.drawLine(mOrigin, mTargetIntersections[3]);

    painter.setPen(mDashedPen);
    painter.drawLine(mTargetIntersections[0], mGroundIntersections[0]);
    painter.drawLine(mTargetIntersections[3], mGroundIntersections[3]);
}

void FovCalculator::TopViewWidget::DrawHandles()
{
    // Fov Width Line
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    QPen pen;
    pen.setColor(SECONDARY_COLOR);
    pen.setWidthF(3.0f);
    pen.setCapStyle(Qt::FlatCap);
    painter.setPen(pen);
    painter.drawLine(mFovWidthHandleBottom.GetCenter(1, 0), mFovWidthHandleTop.GetCenter(1, 0));

    mCameraHandle.Draw(this);
    mTargetHandle.Draw(this);
    mFovWidthHandleTop.Draw(this);
    mFovWidthHandleBottom.Draw(this);
}

void FovCalculator::TopViewWidget::DrawLabels()
{
    QPainter painter(this);

    painter.setPen(QColor(0, 128, 0));
    painter.setFont(mLabelFont);
    QPointF point0(mTargetHandle.GetCenter().x() + 12, mTargetHandle.GetCenter().y() + mLabelFont.pixelSize() / 2);
    painter.drawText(point0, QString::number(mFovWidth, 'f', 2) + " m");

    painter.setPen(PRIMARY_COLOR);
    painter.setFont(mLabelFont);
    QPointF point1(mCameraHandle.GetCenter().x() - 50, mCameraHandle.GetCenter().y() + mLabelFont.pixelSize() / 2);
    painter.drawText(point1, QString::number(mHorizontalFov, 'f', 2) + " °");
}

void FovCalculator::TopViewWidget::UpdateCursor()
{
    bool horizontalCursor = mTargetHandle.GetHovered() || mTargetHandle.GetPressed();
    bool verticalCursor = mFovWidthHandleTop.GetHovered() || mFovWidthHandleTop.GetPressed();
    verticalCursor = verticalCursor || mFovWidthHandleBottom.GetHovered() || mFovWidthHandleBottom.GetPressed();

    if (verticalCursor)
        setCursor(Qt::SizeVerCursor);
    else if (horizontalCursor)
        setCursor(Qt::SizeHorCursor);
    else
        setCursor(Qt::ArrowCursor);
}

void FovCalculator::TopViewWidget::mousePressEvent(QMouseEvent* event)
{

    if (mTargetHandle.Contains(event->pos()))
    {
        mTargetHandle.SetPressed(true);
    }
    else if (mFovWidthHandleTop.Contains(event->pos()))
    {
        mFovWidthHandleTop.SetPressed(true);
    }
    else if (mFovWidthHandleBottom.Contains(event->pos()))
    {
        mFovWidthHandleBottom.SetPressed(true);
    }
    else
    {
        mUserRequestsPan = true;
    }

    mPreviousMousePosition = event->position();
    UpdateCursor();
    update();
}

void FovCalculator::TopViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    mTargetHandle.SetHovered(mTargetHandle.Contains(event->pos()));
    mFovWidthHandleTop.SetHovered(mFovWidthHandleTop.Contains(event->pos()));
    mFovWidthHandleBottom.SetHovered(mFovWidthHandleBottom.Contains(event->pos()));

    QPointF delta = event->position() - mPreviousMousePosition;

    if (mTargetHandle.GetPressed())
    {
        emit UserRequestsTargetDistanceChange(delta);
    }
    if (mFovWidthHandleTop.GetPressed() || mFovWidthHandleBottom.GetPressed())
    {
        emit UserRequestsFovWidthChange(-2 * delta.y());
    }
    if (mUserRequestsPan)
    {
        emit UserRequestsPan(delta);
    }

    mPreviousMousePosition = event->position();
    UpdateCursor();
    update();
}

void FovCalculator::TopViewWidget::mouseReleaseEvent(QMouseEvent*)
{
    mUserRequestsPan = false;
    mTargetHandle.SetPressed(false);
    mFovWidthHandleTop.SetPressed(false);
    mFovWidthHandleBottom.SetPressed(false);

    UpdateCursor();
    update();
}

void FovCalculator::TopViewWidget::wheelEvent(QWheelEvent* event)
{
    emit WheelMoved(event);
}

void FovCalculator::TopViewWidget::SetGroundIntersection(int index, const QPointF& point)
{
    mGroundIntersections[index] = point;
}

void FovCalculator::TopViewWidget::SetTargetIntersection(int index, const QPointF& point)
{
    mTargetIntersections[index] = point;
}

void FovCalculator::TopViewWidget::SetRegion(int index, const QPolygonF& region)
{
    mRegions[index] = region;
}
