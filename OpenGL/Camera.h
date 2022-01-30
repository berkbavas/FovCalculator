#ifndef CAMERA_H
#define CAMERA_H

#include "Node.h"

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QTimer>

class Camera : public Node
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);

    virtual QVector3D position() const override;
    virtual void setPosition(const QVector3D &newPosition) override;
    virtual QQuaternion rotation() const override;
    virtual void setRotation(const QQuaternion &newRotation) override;

    void keyPressed(QKeyEvent *event);
    void keyReleased(QKeyEvent *event);
    void mousePressed(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);

    const QMatrix4x4 &transformation() override;

private slots:
    void update();

private:
    QTimer mTimer;
    QMap<Qt::Key, bool> mPressedKeys;

    float mMovementSpeed;
    float mAngularSpeed;

    bool mMousePressed;
    float mMousePreviousX;
    float mMousePreviousY;
    float mMouseDeltaX;
    float mMouseDeltaY;

    static const QMap<Qt::Key, QVector3D> KEY_BINDINGS;
};

#endif // CAMERA_H
