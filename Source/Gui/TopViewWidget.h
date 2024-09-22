#ifndef TOPVIEWWIDGET_H
#define TOPVIEWWIDGET_H

#include "Core/Controller.h"
#include "Gui/Handle.h"

#include <QWidget>

class TopViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopViewWidget(QWidget *parent = nullptr);

    void setParameters(Controller::TopViewWidgetParameters *newParameters);
    void setOrigin(QPointF newOrigin);
    void setMeterToPixelRatio(float newMeterToPixelRatio);

signals:
    void dirty();
    void pan(int x, int y);
    void zoom(int);
    void cursorPositionChanged(QPointF position);

public slots:
    void refresh();
    QPointF mapFrom3d(float x, float y);
    QPointF mapFrom3d(Eigen::Vector3f vector);
    Eigen::Vector3f mapFrom2d(QPointF point);
    Eigen::Vector3f mapFrom2d(float x, float y);

private:
    void updateHandles();
    void updateCursor();

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *event) override;

    Controller::TopViewWidgetParameters *mParameters;
    QPointF mOrigin;
    float mMeterToPixelRatio;

    Handle mFovWidthHandleTop;
    Handle mFovWidthHandleBottom;
    Handle mTargetHandle;
    Handle mCameraHandle;

    QBrush mCrossedPatternBursh;
    QPen mDashedPen;

    QPoint mOldMousePosition;

    QFont mLabelFont;
    QColor mLabelColor;

    bool mMousePressedOnCanvas;
};

#endif // TOPVIEWWIDGET_H
