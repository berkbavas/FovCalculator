#include "CustomNode.h"

CustomNode::CustomNode(QObject *parent)
    : Node{parent}
    , mPrimitive(Triangles)
{}

const QString &CustomNode::name() const
{
    return mName;
}

void CustomNode::setName(const QString &newName)
{
    mName = newName;
}

const QVector4D &CustomNode::color() const
{
    return mColor;
}

void CustomNode::setColor(const QVector4D &newColor)
{
    mColor = newColor;
}

Material &CustomNode::material()
{
    return mMaterial;
}

void CustomNode::setMaterial(const Material &newMaterial)
{
    mMaterial = newMaterial;
}

CustomNode::Primitive CustomNode::primitive() const
{
    return mPrimitive;
}

void CustomNode::setPrimitive(Primitive newPrimitive)
{
    mPrimitive = newPrimitive;
}
