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

    mMinorTickmarkCount = 0;
    mAxisPen.setColor(PRIMARY_COLOR);
    mAxisPen.setWidthF(1.5f);
    mTickmarkSize = QSizeF(1.5, 8);
    mTickmarkColor = PRIMARY_COLOR;

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
    mTickmarkPixelStep = FindSuitableTickmarkPixelStep(mMeterToPixelRatio);

    UpdateHandles();
    DrawHorizontalAxis();
    DrawVerticalAxis();
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
    painter.drawText(point, QString::number(mTargetHeight, 'f', 2) + " m");

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

    QRectF tickmark = QRectF(0, 0, mTickmarkSize.width(), mTickmarkSize.height());

    painter.setFont(mLabelFont);
    painter.setPen(mLabelColor);

    const QRectF originBoundingBox(mOrigin.x() - 8, mOrigin.y() - 8, 16, 16);

    int start = mOrigin.x() - mTickmarkPixelStep * floor(mOrigin.x() / mTickmarkPixelStep);
    int end = width();

    // Main loop for tickmarks, labels, and minor tickmarks
    for (int x = start; x < end; x += mTickmarkPixelStep)
    {
        tickmark.moveCenter(QPointF(x, mOrigin.y()));

        if (originBoundingBox.intersects(tickmark))
            continue;

        painter.fillRect(tickmark, mTickmarkColor);
        tickmark.moveCenter(QPointF(x, mOrigin.y()));
        float value = (x - mOrigin.x()) / mMeterToPixelRatio;
        float integral = (int) value;
        QString label;
        if (qFuzzyCompare(integral, value))
            label = QString::number(value, 'f', 0);
        else
            label = QString::number(value, 'f', 1);
        QRectF boundingBox(x - 50, tickmark.y() + 12.5f, 100, mLabelFont.pixelSize());
        painter.drawText(boundingBox, Qt::AlignCenter, label);
    }
}

void FovCalculator::SideViewWidget::DrawVerticalAxis()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QLine line = QLine(mOrigin.x(), 0, mOrigin.x(), height());
    painter.setPen(mAxisPen);
    painter.drawLine(line);

    QRectF tickmark = QRectF(0, 0, mTickmarkSize.height(), mTickmarkSize.width());
    painter.setFont(mLabelFont);
    painter.setPen(mLabelColor);

    const QRectF originBoundingBox(mOrigin.x() - 8, mOrigin.y() - 8, 16, 16);

    int start = mOrigin.y() - mTickmarkPixelStep * floor(mOrigin.y() / mTickmarkPixelStep);
    int end = height();

    // Main loop for tickmarks, labels, and minor tickmarks
    for (int y = start; y < end; y += mTickmarkPixelStep)
    {
        tickmark.moveCenter(QPointF(mOrigin.x(), y));

        if (originBoundingBox.intersects(tickmark))
            continue;

        painter.fillRect(tickmark, mTickmarkColor);
        tickmark.moveCenter(QPointF(mOrigin.x(), y));

        float value = (mOrigin.y() - y) / mMeterToPixelRatio;
        float integral = (int) value;

        QString label;
        if (qFuzzyCompare(integral, value))
            label = QString::number(value, 'f', 0);
        else
            label = QString::number(value, 'f', 1);

        QRectF boundingBox(mOrigin.x() - 110, tickmark.y() - 0.6 * mLabelFont.pixelSize(), 100, 1.125 * mLabelFont.pixelSize());
        painter.drawText(boundingBox, Qt::AlignRight, label);
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

int FovCalculator::SideViewWidget::FindSuitableTickmarkPixelStep(float meterToPixelRatio)
{
    float tickmarkMeterStep = 36 / meterToPixelRatio;

    int powers = 0;

    while (tickmarkMeterStep > 1)
    {
        tickmarkMeterStep = tickmarkMeterStep / 10.0f;
        powers++;
    }

    if (tickmarkMeterStep <= 0.25)
        return round(0.25f * pow(10, powers) * meterToPixelRatio);
    else if (tickmarkMeterStep <= 0.5)
        return round(0.5f * pow(10, powers) * meterToPixelRatio);
    else if (tickmarkMeterStep <= 0.75)
        return round(0.75f * pow(10, powers) * meterToPixelRatio);
    else
        return round(pow(10, powers) * meterToPixelRatio);
}

void FovCalculator::SideViewWidget::mousePressEvent(QMouseEvent* event)
{
    if (mLowerBoundaryHandle.Contains(event->pos()))
    {
        mLowerBoundaryHandle.SetPressed(true);
    }
    else if (mTargetHeightHandle.Contains(event->pos()))
    {
        mTargetHeightHandle.SetPressed(true);
    }
    else if (mTargetDistanceHandle.Contains(event->pos()))
    {
        mTargetDistanceHandle.SetPressed(true);
    }
    else if (mCameraHeightHandle.Contains(event->pos()))
    {
        mCameraHeightHandle.SetPressed(true);
    }
    else
    {
        mUserRequestsPan = true;
    }

    mPreviousMousePosition = event->pos();
    UpdateCursor();
    update();
}

void FovCalculator::SideViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    mTargetHeightHandle.SetHovered(mTargetHeightHandle.Contains(event->pos()));
    mTargetDistanceHandle.SetHovered(mTargetDistanceHandle.Contains(event->pos()));
    mCameraHeightHandle.SetHovered(mCameraHeightHandle.Contains(event->pos()));
    mLowerBoundaryHandle.SetHovered(mLowerBoundaryHandle.Contains(event->pos()));

    QPointF delta = event->position() - mPreviousMousePosition;

    if (mTargetHeightHandle.GetPressed())
    {
        emit UserRequestsTargetHeightChange(delta);
    }
    else if (mTargetDistanceHandle.GetPressed())
    {
        emit UserRequestsTargetDistanceChange(delta);
    }
    else if (mCameraHeightHandle.GetPressed())
    {
        emit UserRequestsCameraHeightChange(delta);
    }
    else if (mLowerBoundaryHandle.GetPressed())
    {
        emit UserRequestsLowerBoundaryHeightChange(delta);
    }
    else if (mUserRequestsPan)
    {
        emit UserRequestsPan(delta);
    }

    mPreviousMousePosition = event->pos();
    UpdateCursor();
    update();
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
