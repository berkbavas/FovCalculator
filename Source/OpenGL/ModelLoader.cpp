#include "ModelLoader.h"

#include <QFile>
#include <QVector2D>

ModelLoader::ModelLoader(QObject *parent)
    : QObject{parent}
{}

QMap<SimpleModel::Type, SimpleModelData *> ModelLoader::loadSimpleModelData()
{
    const QList<SimpleModel::Type> types = SIMPLE_MODELS_PATHS.keys();
    QMap<SimpleModel::Type, SimpleModelData *> simpleModelData;

    for (const auto type : types) {
        simpleModelData.insert(type, loadSimpleModelData(type));
    }

    return simpleModelData;
}

SimpleModelData *ModelLoader::loadSimpleModelData(SimpleModel::Type type)
{
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;

    QFile file(SIMPLE_MODELS_PATHS[type]);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        SimpleModelData *data = new SimpleModelData;
        data->setType(type);
        QTextStream fileText(&file);
        while (!fileText.atEnd()) {
            QString fileLine = fileText.readLine();
            if (fileLine.startsWith("vn ")) {
                QStringList lineList = fileLine.split(" ");
                normals << QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
            } else if (fileLine.startsWith("vt ")) {
                QStringList lineList = fileLine.split(" ");
                textureCoords << QVector2D(lineList[1].toFloat(), lineList[2].toFloat());
            } else if (fileLine.startsWith("v ")) {
                QStringList lineList = fileLine.split(" ");
                vertices << QVector3D(lineList[1].toFloat(), lineList[2].toFloat(), lineList[3].toFloat());
            } else if (fileLine.startsWith("f ")) {
                QStringList lineList = fileLine.split(" ");
                for (int i = 1; i <= 3; i++) {
                    QStringList arg = lineList[i].split("/");
                    if (arg.size() == 2) {
                        if (arg[0].toInt() - 1 < vertices.size())
                            data->addVertex(vertices[arg[0].toInt() - 1]);

                        if (arg[1].toInt() - 1 < normals.size())
                            data->addNormal(normals[arg[1].toInt() - 1]);
                    } else if (arg.size() == 3) {
                        if (arg[0].toInt() - 1 < vertices.size())
                            data->addVertex(vertices[arg[0].toInt() - 1]);

                        if (arg[1].toInt() - 1 < textureCoords.size())
                            data->addTextureCoord(textureCoords[arg[1].toInt() - 1]);

                        if (arg[2].toInt() - 1 < normals.size())
                            data->addNormal(normals[arg[2].toInt() - 1]);
                    }
                }

            } else if (fileLine.startsWith("mtllib ")) {
            }
        }
        file.close();

        return data;
    }

    qWarning() << QString("Could not open file '%1'.").arg(file.fileName());
    return nullptr;
}

const QString ModelLoader::MODELS_PATH = ":/Resources/Models/SimpleModels/";

const QMap<SimpleModel::Type, QString> ModelLoader::SIMPLE_MODELS_PATHS = { //
    {SimpleModel::Camera, MODELS_PATH + "Camera.obj"},
    {SimpleModel::Capsule, MODELS_PATH + "Capsule.obj"},
    {SimpleModel::Cone, MODELS_PATH + "Cone.obj"},
    {SimpleModel::Cube, MODELS_PATH + "Cube.obj"},
    {SimpleModel::Cylinder, MODELS_PATH + "Cylinder.obj"},
    {SimpleModel::Dome, MODELS_PATH + "Dome.obj"},
    {SimpleModel::Plane, MODELS_PATH + "Plane.obj"},
    {SimpleModel::Pyramid, MODELS_PATH + "Pyramid.obj"},
    {SimpleModel::Sphere, MODELS_PATH + "Sphere.obj"},
    {SimpleModel::Suzanne, MODELS_PATH + "Suzanne.obj"},
    {SimpleModel::Tetrahedron, MODELS_PATH + "Tetrahedron.obj"},
    {SimpleModel::Torus, MODELS_PATH + "Torus.obj"},
    {SimpleModel::TorusKnot, MODELS_PATH + "TorusKnot.obj"}

};
