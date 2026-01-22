#include "SideViewWidget.h"

#include "Core/Constants.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

FovCalculator::SideViewWidget::SideViewWidget(QWidget* parent)
    : QWidget(parent)
{
    mDashedPen.setWidthF(1.5f);
    mDashedPen.setDashPattern(QVector<qreal>{ 2, 2 });
    mDashedPen.setStyle(Qt::DashLine);

    mSolidPen.setWidthF(1.5f);

    mLabelFont = QFont();
    mLabelFont.setPixelSize(11);

    mLabelColor = PRIMARY_COLOR;

    mAxisPen.setColor(PRIMARY_COLOR);
    mAxisPen.setWidthF(1.5f);

    mTargetHeightHandle.SetBrush(QColor(127, 0, 0));
    mTargetHeightHandle.SetHoveredBrush(QColor(255, 255, 255));
    mTargetHeightHandle.SetPressedBrush(QColor(0, 225, 0));
    mTargetHeightHandle.SetSize(HANDLE_SIZE, HANDLE_SIZE);

    mTargetDistanceHandle.SetBrush(QColor(185, 95, 0));
    mTargetDistanceHandle.SetHoveredBrush(QColor(255, 255, 255));
    mTargetDistanceHandle.SetPressedBrush(QColor(0, 255, 0));
    mTargetDistanceHandle.SetSize(HANDLE_SIZE, HANDLE_SIZE);

    mCameraHeightHandle.SetBrush(QColor(63, 150, 157));
    mCameraHeightHandle.SetHoveredBrush(QColor(255, 255, 255));
    mCameraHeightHandle.SetPressedBrush(QColor(0, 255, 0));
    mCameraHeightHandle.SetSize(HANDLE_SIZE, HANDLE_SIZE);

    mLowerBoundaryHandle.SetBrush(QColor(127, 0, 0));
    mLowerBoundaryHandle.SetHoveredBrush(QColor(255, 255, 255));
    mLowerBoundaryHandle.SetPressedBrush(QColor(0, 255, 0));
    mLowerBoundaryHandle.SetSize(HANDLE_SIZE, HANDLE_SIZE);

    setMouseTracking(true);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void FovCalculator::SideViewWidget::paintEvent(QPaintEvent* event)
{
    UpdateHandles();
    DrawHorizontalAxis();
    DrawVerticalAxis();
    DrawTickmarks();
    DrawRegions();
    DrawTargetStuff();
    DrawTiltAngleStuff();
    DrawHandlers();
}

void FovCalculator::SideViewWidget::DrawTargetStuff()
{
    // Height line
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    QPen pen(SECONDARY_COLOR);
    pen.setWidthF(2.0f);
    pen.setCapStyle(Qt::FlatCap);
    painter.setPen(pen);
    painter.drawLine(mTargetDistanceHandle.GetCenter(1, 0), mTargetHeightHandle.GetCenter(1, 0));

    painter.setRenderHint(QPainter::Antialiasing, true);

    // Height label
    painter.setPen(SECONDARY_COLOR);
    painter.setFont(mLabelFont);
    QPointF point = QPointF(mTargetHeightHandle.GetCenter().x() + 8, (mTargetDistanceHandle.GetCenter().y() + mTargetHeightHandle.GetCenter().y() + mLabelFont.pixelSize()) / 2);
    painter.drawText(point, QString::number(mTargetHeight / mMeterToPixelRatio, 'f', 2) + " m");

    // Bisectors
    mDashedPen.setColor(SECONDARY_COLOR);
    painter.setPen(mDashedPen);
    painter.drawLine(mCameraHeightHandle.GetCenter(), mBisectorIntersection);
    painter.drawLine(mCameraHeightHandle.GetCenter(), mOppositeBisectorIntersection);

    mSolidPen.setColor(SECONDARY_COLOR);
    painter.setPen(mSolidPen);
    painter.drawLine(mCameraHeightHandle.GetCenter(), mTargetHeightHandle.GetCenter());

    // Ground intersections
    mDashedPen.setColor(PRIMARY_COLOR);
    painter.setPen(mDashedPen);
    painter.drawLine(mTargetHeightHandle.GetCenter(), mGroundIntersections[0]);

    mSolidPen.setColor(SECONDARY_COLOR);
    painter.setPen(mSolidPen);
    painter.drawLine(mCameraHeightHandle.GetCenter(), mGroundIntersections[1]);
}

void FovCalculator::SideViewWidget::DrawTiltAngleStuff()
{
    QPainter painter(this);

    // Draw tilt angle reference line
    {
        painter.setRenderHint(QPainter::Antialiasing, false);
        mDashedPen.setColor(PRIMARY_COLOR);
        painter.setPen(mDashedPen);
        painter.drawLine(mCameraHeightHandle.GetCenter(), QPoint(0, mCameraHeightHandle.GetCenter().y()));
    }

    // Tilt angle
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPainterPath path;
        path.moveTo(mCameraHeightHandle.GetCenter().x(), mCameraHeightHandle.GetCenter().y());
        path.arcTo(mCameraHeightHandle.GetCenter().x() - 50, mCameraHeightHandle.GetCenter().y() - 50, 100, 100, -180, -mTiltAngle);
        path.closeSubpath();
        QBrush brush;
        brush.setStyle(Qt::BrushStyle::Dense6Pattern);
        brush.setColor(SECONDARY_COLOR);
        painter.fillPath(path, brush);
    }

    // Tilt angle label
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setFont(mLabelFont);
        painter.setPen(mLabelColor);
        QString label = QString::number(mTiltAngle, 'f', 2) + " º";
        QRectF boundingBox;
        if (mTiltAngle > 0)
            boundingBox = QRectF(mCameraHeightHandle.GetCenter().x() - 75, mCameraHeightHandle.GetCenter().y(), 50, 2 * mLabelFont.pixelSize());
        else
            boundingBox = QRectF(mCameraHeightHandle.GetCenter().x() - 75,
                                 mCameraHeightHandle.GetCenter().y() - 2 * mLabelFont.pixelSize(),
                                 50,
                                 2 * mLabelFont.pixelSize());
        painter.drawText(boundingBox, Qt::AlignCenter, label);
    }
}

void FovCalculator::SideViewWidget::UpdateHandles()
{
    mCameraHeightHandle.SetCenter(mOrigin.x(), mOrigin.y() - mCameraHeight);
    mTargetHeightHandle.SetCenter(mOrigin.x() + mTargetDistance, mOrigin.y() - mTargetHeight);
    mTargetDistanceHandle.SetCenter(mOrigin.x() + mTargetDistance, mOrigin.y());
    mLowerBoundaryHandle.SetCenter(mOrigin.x() + mLowerBoundaryDistance, mOrigin.y() - mLowerBoundaryHeight);
}

void FovCalculator::SideViewWidget::DrawHorizontalAxis()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.setPen(mAxisPen);
    QLine line = QLine(0, mOrigin.y(), width(), mOrigin.y());
    painter.drawLine(line);
}

void FovCalculator::SideViewWidget::DrawVerticalAxis()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QLine line = QLine(mOrigin.x(), 0, mOrigin.x(), height());
    painter.setPen(mAxisPen);
    painter.drawLine(line);
}

void FovCalculator::SideViewWidget::DrawTickmarks()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(mAxisPen);
    painter.setFont(mLabelFont);

    constexpr int TICK_LENGTH = 5;
    constexpr int LABEL_OFFSET = 3;

    // Calculate tick interval based on zoom level (mMeterToPixelRatio)
    float tickIntervalMeters = 1.0f;
    if (mMeterToPixelRatio < 5.0f)
        tickIntervalMeters = 10.0f;
    else if (mMeterToPixelRatio < 15.0f)
        tickIntervalMeters = 5.0f;
    else if (mMeterToPixelRatio < 40.0f)
        tickIntervalMeters = 2.0f;
    else if (mMeterToPixelRatio > 80.0f)
        tickIntervalMeters = 0.5f;

    float tickIntervalPixels = tickIntervalMeters * mMeterToPixelRatio;

    // Horizontal axis tickmarks (distance)
    // Positive direction (right of origin)
    for (float x = mOrigin.x() + tickIntervalPixels; x < width(); x += tickIntervalPixels)
    {
        painter.drawLine(QPointF(x, mOrigin.y() - TICK_LENGTH), QPointF(x, mOrigin.y() + TICK_LENGTH));
        float meters = (x - mOrigin.x()) / mMeterToPixelRatio;
        QString label = QString::number(meters, 'f', tickIntervalMeters < 1.0f ? 1 : 0);
        QRectF textRect(x - 20, mOrigin.y() + TICK_LENGTH + LABEL_OFFSET, 40, mLabelFont.pixelSize() + 2);
        painter.drawText(textRect, Qt::AlignCenter, label);
    }

    // Negative direction (left of origin)
    for (float x = mOrigin.x() - tickIntervalPixels; x > 0; x -= tickIntervalPixels)
    {
        painter.drawLine(QPointF(x, mOrigin.y() - TICK_LENGTH), QPointF(x, mOrigin.y() + TICK_LENGTH));
        float meters = (x - mOrigin.x()) / mMeterToPixelRatio;
        QString label = QString::number(meters, 'f', tickIntervalMeters < 1.0f ? 1 : 0);
        QRectF textRect(x - 20, mOrigin.y() + TICK_LENGTH + LABEL_OFFSET, 40, mLabelFont.pixelSize() + 2);
        painter.drawText(textRect, Qt::AlignCenter, label);
    }

    // Vertical axis tickmarks (height)
    // Positive direction (above origin - going up means smaller y)
    for (float y = mOrigin.y() - tickIntervalPixels; y > 0; y -= tickIntervalPixels)
    {
        painter.drawLine(QPointF(mOrigin.x() - TICK_LENGTH, y), QPointF(mOrigin.x() + TICK_LENGTH, y));
        float meters = (mOrigin.y() - y) / mMeterToPixelRatio;
        QString label = QString::number(meters, 'f', tickIntervalMeters < 1.0f ? 1 : 0);
        QRectF textRect(mOrigin.x() - 35 - LABEL_OFFSET, y - mLabelFont.pixelSize() / 2, 30, mLabelFont.pixelSize() + 2);
        painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, label);
    }

    // Negative direction (below origin)
    for (float y = mOrigin.y() + tickIntervalPixels; y < height(); y += tickIntervalPixels)
    {
        painter.drawLine(QPointF(mOrigin.x() - TICK_LENGTH, y), QPointF(mOrigin.x() + TICK_LENGTH, y));
        float meters = (mOrigin.y() - y) / mMeterToPixelRatio;
        QString label = QString::number(meters, 'f', tickIntervalMeters < 1.0f ? 1 : 0);
        QRectF textRect(mOrigin.x() - 35 - LABEL_OFFSET, y - mLabelFont.pixelSize() / 2, 30, mLabelFont.pixelSize() + 2);
        painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, label);
    }
}

void FovCalculator::SideViewWidget::DrawRegions()
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

void FovCalculator::SideViewWidget::DrawHandlers()
{
    mCameraHeightHandle.Draw(this);
    mTargetHeightHandle.Draw(this);
    mTargetDistanceHandle.Draw(this);
    mLowerBoundaryHandle.Draw(this);
}

void FovCalculator::SideViewWidget::mousePressEvent(QMouseEvent* event)
{
    QPointF mousePosition = event->position();

    if (mLowerBoundaryHandle.Contains(mousePosition))
    {
        mLowerBoundaryHandle.SetPressed(true);
    }
    else if (mTargetHeightHandle.Contains(mousePosition))
    {
        mTargetHeightHandle.SetPressed(true);
    }
    else if (mTargetDistanceHandle.Contains(mousePosition))
    {
        mTargetDistanceHandle.SetPressed(true);
    }
    else if (mCameraHeightHandle.Contains(mousePosition))
    {
        mCameraHeightHandle.SetPressed(true);
    }
    else
    {
        mUserRequestsPan = true;
    }

    mPreviousMousePosition = mousePosition;
    UpdateCursor();
    update();
}

void FovCalculator::SideViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPointF mousePosition = event->position();

    mTargetHeightHandle.SetHovered(mTargetHeightHandle.Contains(mousePosition));
    mTargetDistanceHandle.SetHovered(mTargetDistanceHandle.Contains(mousePosition));
    mCameraHeightHandle.SetHovered(mCameraHeightHandle.Contains(mousePosition));
    mLowerBoundaryHandle.SetHovered(mLowerBoundaryHandle.Contains(mousePosition));

    QPointF delta = mousePosition - mPreviousMousePosition;

    if (mTargetHeightHandle.GetPressed())
    {
        emit UserRequestsTargetHeightDeltaChange(delta);
    }
    else if (mTargetDistanceHandle.GetPressed())
    {
        emit UserRequestsTargetDistanceDeltaChange(delta);
    }
    else if (mCameraHeightHandle.GetPressed())
    {
        emit UserRequestsCameraHeightDeltaChange(delta);
    }
    else if (mLowerBoundaryHandle.GetPressed())
    {
        emit UserRequestsLowerBoundaryHeightDeltaChange(delta);
    }
    else if (mUserRequestsPan)
    {
        emit UserRequestsPan(delta);
    }

    mPreviousMousePosition = mousePosition;
    UpdateCursor();
    update();

    emit MouseMoved(mousePosition);
}

void FovCalculator::SideViewWidget::mouseReleaseEvent(QMouseEvent*)
{
    mTargetHeightHandle.SetPressed(false);
    mTargetDistanceHandle.SetPressed(false);
    mCameraHeightHandle.SetPressed(false);
    mLowerBoundaryHandle.SetPressed(false);
    mUserRequestsPan = false;

    UpdateCursor();
    update();
}

void FovCalculator::SideViewWidget::wheelEvent(QWheelEvent* event)
{
    emit WheelMoved(event);
}

void FovCalculator::SideViewWidget::UpdateCursor()
{
    bool verticalCursor = mTargetHeightHandle.GetHovered() || mTargetHeightHandle.GetPressed();
    verticalCursor = verticalCursor || mCameraHeightHandle.GetHovered() || mCameraHeightHandle.GetPressed();
    verticalCursor = verticalCursor || mLowerBoundaryHandle.GetHovered() || mLowerBoundaryHandle.GetPressed();
    bool horizontalCursor = mTargetDistanceHandle.GetHovered() || mTargetDistanceHandle.GetPressed();

    if (verticalCursor)
        setCursor(Qt::SizeVerCursor);
    else if (horizontalCursor)
        setCursor(Qt::SizeHorCursor);
    else
        setCursor(Qt::ArrowCursor);
}

void FovCalculator::SideViewWidget::SetGroundIntersection(int index, const QPointF& point)
{
    mGroundIntersections[index] = point;
}

void FovCalculator::SideViewWidget::SetTargetIntersection(int index, const QPointF& point)
{
    mTargetIntersections[index] = point;
}

void FovCalculator::SideViewWidget::SetLowerBoundaryIntersection(int index, const QPointF& point)
{
    mLowerBoundaryIntersections[index] = point;
}

void FovCalculator::SideViewWidget::SetRegion(int index, const QPolygonF& region)
{
    mRegions[index] = region;
}
