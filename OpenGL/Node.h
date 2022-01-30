#ifndef NODE_H
#define NODE_H

#include <QMatrix4x4>
#include <QObject>
#include <QQuaternion>
#include <QVector3D>

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = nullptr);
    virtual ~Node();

    int id() const;
    void setId(int newId);

    virtual const QMatrix4x4 &transformation();

    virtual QVector3D position() const;
    virtual void setPosition(const QVector3D &newPosition);

    virtual QQuaternion rotation() const;
    virtual void setRotation(const QQuaternion &newRotation);

    virtual QVector3D scale() const;
    virtual void setScale(const QVector3D &newScale);

protected:
    int mId;
    QMatrix4x4 mTransformation;
    QVector3D mPosition;
    QQuaternion mRotation;
    QVector3D mScale;
};

#endif // NODE_H
