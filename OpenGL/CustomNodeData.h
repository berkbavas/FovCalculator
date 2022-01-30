#ifndef CUSTOMNODEDATA_H
#define CUSTOMNODEDATA_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

class CustomNodeData : public QObject, protected QOpenGLFunctions
{
public:
    CustomNodeData(QObject *parent = nullptr);
    ~CustomNodeData();

    bool create();
    void write(const QVector<QVector3D> &vertices);
    void bind();
    void release();
    int count();

    int maxNumberOfVertices() const;
    void setMaxNumberOfVertices(int newMaxNumberOfVertices);

private:
    QOpenGLVertexArrayObject mVertexArray;
    QOpenGLBuffer mVertexBuffer;
    QVector<QVector3D> mVertices;
    int mMaxNumberOfVertices;
};

#endif // CUSTOMNODEDATA_H
