#include "TopViewWidget.h"

#include "Core/Constants.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

TopViewWidget::TopViewWidget(QWidget *parent)
    : QWidget(parent)
    , mMousePressedOnCanvas(false)
{
    // General GUI
    {
        QVector<qreal> dashes;
        dashes << 2 << 2;
        mDashedPen.setDashPattern(dashes);
        mDashedPen.setStyle(Qt::DashLine);

        mLabelFont = QFont();
        mLabelFont.setPixelSize(10);
        mLabelColor = QColor(100, 100, 100);

        mCrossedPatternBursh.setStyle(Qt::BrushStyle::CrossPattern);
        mCrossedPatternBursh.setColor(QColor(210, 210, 210));
    }

    // Target Handle
    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mTargetHandle.setPen(pen);
        mTargetHandle.setBrush(QColor(255, 128, 0));
        mTargetHandle.setHoveredBrush(QColor(255, 255, 255));
        mTargetHandle.setPressedBrush(QColor(0, 255, 0));
        mTargetHandle.setSize(10, 10);
    }

    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mFovWidthHandleTop.setPen(pen);
        mFovWidthHandleTop.setBrush(QColor(255, 0, 0));
        mFovWidthHandleTop.setHoveredBrush(QColor(255, 255, 255));
        mFovWidthHandleTop.setPressedBrush(QColor(0, 255, 0));
        mFovWidthHandleTop.setSize(10, 10);
    }

    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mFovWidthHandleBottom.setPen(pen);
        mFovWidthHandleBottom.setBrush(QColor(255, 0, 0));
        mFovWidthHandleBottom.setHoveredBrush(QColor(255, 255, 255));
        mFovWidthHandleBottom.setPressedBrush(QColor(0, 255, 0));
        mFovWidthHandleBottom.setSize(10, 10);
    }

    {
        QPen pen = QColor(0, 0, 0);
        pen.setWidth(1);
        pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
        mCameraHandle.setPen(pen);
        mCameraHandle.setBrush(QColor(63, 150, 157));
        mCameraHandle.setSize(10, 10);
    }

    setMouseTracking(true);
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void TopViewWidget::setParameters(Controller::TopViewWidgetParameters *newParameters)
{
    mParameters = newParameters;
}

void TopViewWidget::refresh()
{
    updateHandles();
    update();
}

QPointF TopViewWidget::mapFrom3d(float x, float y)
{
    return QPointF(mOrigin.x() + x * mMeterToPixelRatio, mOrigin.y() - y * mMeterToPixelRatio);
}

QPointF TopViewWidget::mapFrom3d(Eigen::Vector3f vector)
{
    return mapFrom3d(vector.x(), vector.y());
}

Eigen::Vector3f TopViewWidget::mapFrom2d(QPointF point)
{
    return mapFrom2d(point.x(), point.y());
}

Eigen::Vector3f TopViewWidget::mapFrom2d(float x, float y)
{
    Eigen::Vector3f vector;

    vector[0] = (x - mOrigin.x()) / mMeterToPixelRatio;
    vector[1] = (mOrigin.y() - y) / mMeterToPixelRatio;
    vector[2] = 0;
    return vector;
}

void TopViewWidget::updateHandles()
{
    mTargetHandle.setCenter(mapFrom3d(mParameters->target.distance, 0));
    mFovWidthHandleTop.setCenter(mapFrom3d(mParameters->target.distance, 0.5 * mParameters->target.fovWidth));
    mFovWidthHandleBottom.setCenter(mapFrom3d(mParameters->target.distance, -0.5 * mParameters->target.fovWidth));
    mCameraHandle.setCenter(mOrigin.x(), mOrigin.y());
}

void TopViewWidget::updateCursor()
{
    bool horizontalCursor = mTargetHandle.hovered() || mTargetHandle.pressed();
    bool verticalCursor = mFovWidthHandleTop.hovered() || mFovWidthHandleTop.pressed();
    verticalCursor = verticalCursor || mFovWidthHandleBottom.hovered() || mFovWidthHandleBottom.pressed();

    if (verticalCursor)
        this->setCursor(Qt::SizeVerCursor);
    else if (horizontalCursor)
        this->setCursor(Qt::SizeHorCursor);
    else
        this->setCursor(Qt::ArrowCursor);
}

void TopViewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // Draw crossed pattern
    painter.fillRect(0, 0, width(), height(), mCrossedPatternBursh);

    // Regions
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        for (int i = 0; i < 7; i++) {
            if (mParameters->regions[i].visible) {
                QPainterPath path;
                path.addPolygon(mParameters->regions[i].region);

                QBrush brush;
                brush.setStyle(Qt::BrushStyle::SolidPattern);
                brush.setColor(REGION_COLORS[i]);

                painter.fillPath(path, brush);
            }
        }
    }

    {
        // Draw ground and frustum intersection
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(QColor(128, 128, 128));
        pen.setWidthF(1.0f);
        pen.setCapStyle(Qt::FlatCap);
        painter.setPen(pen);

        for (int i = 0; i < 4; i++)
            painter.drawLine(mParameters->ground.intersections[i], mParameters->ground.intersections[(i + 1) % 4]);
    }

    // Draw frustum and target intersection
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPen pen;
        pen.setColor(QColor(0, 102, 213));
        pen.setWidth(1.0f);
        pen.setCapStyle(Qt::FlatCap);
        painter.setPen(pen);
        painter.drawLine(mOrigin, mParameters->ground.intersections[1]);
        painter.drawLine(mOrigin, mParameters->ground.intersections[2]);
        painter.drawLine(mOrigin, mParameters->target.intersections[0]);
        painter.drawLine(mOrigin, mParameters->target.intersections[3]);

        mDashedPen.setColor(QColor(128, 128, 128));
        painter.setPen(mDashedPen);
        painter.drawLine(mParameters->target.intersections[0], mParameters->ground.intersections[0]);
        painter.drawLine(mParameters->target.intersections[3], mParameters->ground.intersections[3]);
    }

    // Draw fov width line
    {
        painter.setRenderHint(QPainter::Antialiasing, false);
        QPen pen;
        pen.setColor(QColor(0, 128, 0));
        pen.setWidthF(3.0f);
        pen.setCapStyle(Qt::FlatCap);
        painter.setPen(pen);
        painter.drawLine(mFovWidthHandleBottom.getCenter(1, 0), mFovWidthHandleTop.getCenter(1, 0));
    }

    // Draw fov width label
    {
        painter.setPen(QColor(0, 128, 0));
        painter.setFont(mLabelFont);
        QPointF point = QPointF(mTargetHandle.getCenter().x() + 12, mTargetHandle.getCenter().y() + mLabelFont.pixelSize() / 2);
        painter.drawText(point, QString::number(mParameters->target.fovWidth, 'f', 2) + " m");
    }

    // Fill horizontal fov label
    {
        painter.setPen(QColor(0, 0, 0));
        painter.setFont(mLabelFont);
        QPointF point(mCameraHandle.getCenter().x() - 50, mCameraHandle.getCenter().y() + mLabelFont.pixelSize() / 2);
        painter.drawText(point, QString::number(mParameters->camera.horizontalFov, 'f', 2) + " °");
    }

    mCameraHandle.draw(this);
    mTargetHandle.draw(this);
    mFovWidthHandleTop.draw(this);
    mFovWidthHandleBottom.draw(this);
}

void TopViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (mTargetHandle.contains(event->pos())) {
        mTargetHandle.setPressed(true);
    } else if (mFovWidthHandleTop.contains(event->pos())) {
        mFovWidthHandleTop.setPressed(true);
    } else if (mFovWidthHandleBottom.contains(event->pos())) {
        mFovWidthHandleBottom.setPressed(true);
    } else {
        mMousePressedOnCanvas = true;
    }

    mOldMousePosition = event->pos();
    update();
}

void TopViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    mTargetHandle.setHovered(mTargetHandle.contains(event->pos()));
    mFovWidthHandleTop.setHovered(mFovWidthHandleTop.contains(event->pos()));
    mFovWidthHandleBottom.setHovered(mFovWidthHandleBottom.contains(event->pos()));

    bool isDirty = false;

    if (mTargetHandle.pressed()) {
        isDirty = true;
        mParameters->target.distance += (event->pos() - mOldMousePosition).x() / mMeterToPixelRatio;
    }

    if (mFovWidthHandleTop.pressed() || mFovWidthHandleBottom.pressed()) {
        isDirty = true;
        mParameters->target.fovWidth += 2 * (mOldMousePosition - event->pos()).y() / mMeterToPixelRatio;
    }

    if (isDirty)
        emit dirty();

    else if (mMousePressedOnCanvas)
        emit pan((event->pos() - mOldMousePosition).x(), (event->pos() - mOldMousePosition).y());

    if (!isDirty) // dirty signal will call update anyway
        update();

    mOldMousePosition = event->pos();
    updateCursor();
    Eigen::Vector3f position = mapFrom2d(event->pos());
    emit cursorPositionChanged(QPointF(position.x(), position.y()));
}

void TopViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mTargetHandle.setPressed(false);
    mFovWidthHandleTop.setPressed(false);
    mFovWidthHandleBottom.setPressed(false);
    mMousePressedOnCanvas = false;

    mOldMousePosition = event->pos();
    update();
}

void TopViewWidget::wheelEvent(QWheelEvent *event)
{
    emit zoom(event->angleDelta().y());
}

void TopViewWidget::setMeterToPixelRatio(float newMeterToPixelRatio)
{
    mMeterToPixelRatio = newMeterToPixelRatio;
}

void TopViewWidget::setOrigin(QPointF newOrigin)
{
    mOrigin = newOrigin;
}
