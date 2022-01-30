#ifndef SIMPLEMODEL_H
#define SIMPLEMODEL_H

#include "Material.h"
#include "Node.h"

#include <QOpenGLTexture>

class SimpleModel : public Node
{
public:
    enum Type { //
        Camera,
        Capsule,
        Cone,
        Cube,
        Cylinder,
        Dome,
        Plane,
        Pyramid,
        Sphere,
        Suzanne,
        Tetrahedron,
        Torus,
        TorusKnot
    };

    explicit SimpleModel(QObject *parent = nullptr);
    virtual ~SimpleModel();

    Type type() const;
    void setType(Type newType);

    Material &material();
    void setMaterial(const Material &newMaterial);

protected:
    Type mType;
    Material mMaterial;
};

class ColoredSimpleModel : public SimpleModel
{
public:
    explicit ColoredSimpleModel(QObject *parent = nullptr);
    virtual ~ColoredSimpleModel();

    const QVector4D &color() const;
    void setColor(const QVector4D &newColor);

protected:
    QVector4D mColor;
};

class TexturedSimpleModel : public SimpleModel
{
public:
    explicit TexturedSimpleModel(QObject *parent = nullptr);
    virtual ~TexturedSimpleModel();

    QOpenGLTexture *texture() const;
    void setTexture(QOpenGLTexture *newTexture);

protected:
    QOpenGLTexture *mTexture;
};

#endif // SIMPLEMODEL_H
