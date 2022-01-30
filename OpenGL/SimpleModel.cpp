#include "SimpleModel.h"

SimpleModel::SimpleModel(QObject *parent)
    : Node{parent}
{}

SimpleModel::~SimpleModel() {}

SimpleModel::Type SimpleModel::type() const
{
    return mType;
}

void SimpleModel::setType(Type newType)
{
    mType = newType;
}

Material &SimpleModel::material()
{
    return mMaterial;
}

void SimpleModel::setMaterial(const Material &newMaterial)
{
    mMaterial = newMaterial;
}

ColoredSimpleModel::ColoredSimpleModel(QObject *parent)
    : SimpleModel(parent)
    , mColor(1, 1, 1, 1)
{}

ColoredSimpleModel::~ColoredSimpleModel() {}

const QVector4D &ColoredSimpleModel::color() const
{
    return mColor;
}

void ColoredSimpleModel::setColor(const QVector4D &newColor)
{
    mColor = newColor;
}

TexturedSimpleModel::TexturedSimpleModel(QObject *parent)
    : SimpleModel(parent)
    , mTexture(nullptr)
{}

TexturedSimpleModel::~TexturedSimpleModel()
{
    if (mTexture)
        mTexture->destroy();

    mTexture = nullptr;
}

QOpenGLTexture *TexturedSimpleModel::texture() const
{
    return mTexture;
}

void TexturedSimpleModel::setTexture(QOpenGLTexture *newTexture)
{
    mTexture = newTexture;
}
