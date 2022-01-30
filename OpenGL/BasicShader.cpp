#include "BasicShader.h"
#include "Helper.h"

BasicShader::BasicShader(QObject *parent)
    : QObject{parent}
    , mProgram(nullptr)
{}

BasicShader::~BasicShader()
{
    if (mProgram)
        mProgram->deleteLater();

    mProgram = nullptr;
}

bool BasicShader::init()
{
    initializeOpenGLFunctions();

    mProgram = new QOpenGLShaderProgram;
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, Helper::getBytes(":/Resources/Shaders/Basic/VertexShader.vert"));
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, Helper::getBytes(":/Resources/Shaders/Basic/FragmentShader.frag"));

    mProgram->link();
    mProgram->bind();

    mLocations.insert("light.color", mProgram->uniformLocation("light.color"));
    mLocations.insert("light.position", mProgram->uniformLocation("light.position"));
    mLocations.insert("light.ambient", mProgram->uniformLocation("light.ambient"));
    mLocations.insert("light.diffuse", mProgram->uniformLocation("light.diffuse"));
    mLocations.insert("light.specular", mProgram->uniformLocation("light.specular"));

    mLocations.insert("node.color", mProgram->uniformLocation("node.color"));
    mLocations.insert("node.ambient", mProgram->uniformLocation("node.ambient"));
    mLocations.insert("node.diffuse", mProgram->uniformLocation("node.diffuse"));
    mLocations.insert("node.specular", mProgram->uniformLocation("node.specular"));
    mLocations.insert("node.shininess", mProgram->uniformLocation("node.shininess"));

    mLocations.insert("cameraPosition", mProgram->uniformLocation("cameraPosition"));
    mLocations.insert("viewMatrix", mProgram->uniformLocation("viewMatrix"));
    mLocations.insert("nodeMatrix", mProgram->uniformLocation("nodeMatrix"));
    mLocations.insert("projectionMatrix", mProgram->uniformLocation("projectionMatrix"));

    mProgram->bindAttributeLocation("vPosition", 0);
    mProgram->bindAttributeLocation("vNormal", 1);

    mProgram->release();

    qDebug() << Q_FUNC_INFO << "Uniform locations:" << mLocations;

    return true;
}

bool BasicShader::bind()
{
    return mProgram->bind();
}

void BasicShader::release()
{
    mProgram->release();
}

void BasicShader::setUniformValue(const QString &name, float value)
{
    mProgram->setUniformValue(mLocations[name], value);
}

void BasicShader::setUniformValue(const QString &name, const QVector3D &value)
{
    mProgram->setUniformValue(mLocations[name], value);
}

void BasicShader::setUniformValue(const QString &name, const QVector4D &value)
{
    mProgram->setUniformValue(mLocations[name], value);
}

void BasicShader::setUniformValue(const QString &name, const QMatrix4x4 &value)
{
    mProgram->setUniformValue(mLocations[name], value);
}
