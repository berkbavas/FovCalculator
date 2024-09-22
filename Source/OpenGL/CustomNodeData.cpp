#include "CustomNodeData.h"

#include <QVector3D>

CustomNodeData::CustomNodeData(QObject *parent)
    : QObject(parent)
    , mMaxNumberOfVertices(100)
{}

CustomNodeData::~CustomNodeData()
{
    mVertexArray.destroy();
    mVertexBuffer.destroy();
}

bool CustomNodeData::create()
{
    initializeOpenGLFunctions();

    mVertexArray.create();
    mVertexArray.bind();

    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    mVertexBuffer.allocate(mMaxNumberOfVertices * sizeof(QVector3D));
    glVertexAttribPointer(0,
                          3,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );
    glEnableVertexAttribArray(0);
    mVertexBuffer.release();

    mVertexArray.release();

    return true;
}

void CustomNodeData::write(const QVector<QVector3D> &vertices)
{
    mVertices = vertices;
    mVertexBuffer.bind();
    mVertexBuffer.write(0, mVertices.constData(), mVertices.count() * sizeof(QVector3D));
    mVertexBuffer.release();
}

int CustomNodeData::count()
{
    return mVertices.count();
}

int CustomNodeData::maxNumberOfVertices() const
{
    return mMaxNumberOfVertices;
}

void CustomNodeData::setMaxNumberOfVertices(int newMaxNumberOfVertices)
{
    mMaxNumberOfVertices = newMaxNumberOfVertices;
}

void CustomNodeData::bind()
{
    mVertexArray.bind();
}

void CustomNodeData::release()
{
    mVertexArray.release();
}
