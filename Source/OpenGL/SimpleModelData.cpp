#include "SimpleModelData.h"

#include <QVector2D>

SimpleModelData::SimpleModelData(QObject *parent)
    : QObject{parent}
{}

SimpleModelData::~SimpleModelData()
{
    mVertexArray.destroy();
    mVertexBuffer.destroy();
    mNormalBuffer.destroy();
    mTextureCoordsBuffer.destroy();
}

bool SimpleModelData::create()
{
    initializeOpenGLFunctions();

    qInfo() << Q_FUNC_INFO << "Creating model" << (int) mType;

    mVertexArray.create();
    mVertexArray.bind();

    // Vertices
    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mVertexBuffer.allocate(mVertices.constData(), sizeof(QVector3D) * mVertices.size());
    glVertexAttribPointer(0,
                          3,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );
    glEnableVertexAttribArray(0);
    mVertexBuffer.release();

    // Normals
    mNormalBuffer.create();
    mNormalBuffer.bind();
    mNormalBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mNormalBuffer.allocate(mNormals.constData(), sizeof(QVector3D) * mNormals.size());

    glVertexAttribPointer(1,
                          3,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );
    glEnableVertexAttribArray(1);
    mNormalBuffer.release();

    // TextureCoords
    mTextureCoordsBuffer.create();
    mTextureCoordsBuffer.bind();
    mTextureCoordsBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mTextureCoordsBuffer.allocate(mTextureCoords.constData(), sizeof(QVector2D) * mTextureCoords.size());
    glVertexAttribPointer(2,
                          2,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized
                          sizeof(QVector2D), // Stride
                          nullptr            // Offset
    );
    glEnableVertexAttribArray(2);
    mTextureCoordsBuffer.release();

    mVertexArray.release();

    return true;
}

void SimpleModelData::bind()
{
    mVertexArray.bind();
}

void SimpleModelData::release()
{
    mVertexArray.release();
}

int SimpleModelData::count()
{
    return mVertices.count();
}

void SimpleModelData::addVertex(const QVector3D &vertex)
{
    mVertices << vertex;
}

void SimpleModelData::addNormal(const QVector3D &normal)
{
    mNormals << normal;
}

void SimpleModelData::addTextureCoord(const QVector2D &uv)
{
    mTextureCoords << uv;
}

void SimpleModelData::setType(SimpleModel::Type newType)
{
    mType = newType;
}

SimpleModel::Type SimpleModelData::type() const
{
    return mType;
}
