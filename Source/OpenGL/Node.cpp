#include "Node.h"

Node::Node(QObject *parent)
    : QObject{parent}
    , mScale(1, 1, 1)
{}

Node::~Node() {}

const QMatrix4x4 &Node::transformation()
{
    mTransformation.setToIdentity();
    mTransformation.scale(mScale);
    mTransformation.rotate(mRotation);
    mTransformation.setColumn(3, QVector4D(mPosition, 1.0f));
    return mTransformation;
}

int Node::id() const
{
    return mId;
}

void Node::setId(int newId)
{
    mId = newId;
}

QVector3D Node::position() const
{
    return mPosition;
}

void Node::setPosition(const QVector3D &newPosition)
{
    mPosition = newPosition;
}

QQuaternion Node::rotation() const
{
    return mRotation;
}

void Node::setRotation(const QQuaternion &newRotation)
{
    mRotation = newRotation;
}

QVector3D Node::scale() const
{
    return mScale;
}

void Node::setScale(const QVector3D &newScale)
{
    mScale = newScale;
}
