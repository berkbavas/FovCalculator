#ifndef SIMPLEMODELDATA_H
#define SIMPLEMODELDATA_H

#include "SimpleModel.h"

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

class SimpleModelData : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SimpleModelData(QObject *parent = nullptr);
    ~SimpleModelData();

    bool create();
    void bind();
    void release();
    int count();

    void setType(SimpleModel::Type newType);
    SimpleModel::Type type() const;

    void addVertex(const QVector3D &vertex);
    void addNormal(const QVector3D &normal);
    void addTextureCoord(const QVector2D &uv);

private:
    SimpleModel::Type mType;
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QOpenGLBuffer mNormalBuffer;
    QOpenGLBuffer mTextureCoordsBuffer;
    QVector<QVector3D> mVertices;
    QVector<QVector3D> mNormals;
    QVector<QVector2D> mTextureCoords;
};

#endif // SIMPLEMODELDATA_H
