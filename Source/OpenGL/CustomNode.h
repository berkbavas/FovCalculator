#ifndef CUSTOMNODE_H
#define CUSTOMNODE_H

#include "Material.h"
#include "Node.h"
#include <qgl.h>

class CustomNode : public Node
{
public:
    enum Primitive { //
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineLoop = GL_LINE_LOOP,
        LineStrip = GL_LINE_STRIP,
        Triangles = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleFan = GL_TRIANGLE_FAN,
        Quads = GL_QUADS,
        QuadStrip = GL_QUAD_STRIP,
        Polygon = GL_POLYGON
    };

    explicit CustomNode(QObject *parent = nullptr);

    const QString &name() const;
    void setName(const QString &newName);

    const QVector4D &color() const;
    void setColor(const QVector4D &newColor);

    Material &material();
    void setMaterial(const Material &newMaterial);

    Primitive primitive() const;
    void setPrimitive(Primitive newPrimitive);

private:
    Primitive mPrimitive;
    QString mName;
    Material mMaterial;
    QVector4D mColor;
};

#endif // CUSTOMNODE_H
