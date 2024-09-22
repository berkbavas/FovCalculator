#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "SimpleModel.h"
#include "SimpleModelData.h"

#include <QObject>

class ModelLoader : public QObject
{
    Q_OBJECT
public:
    explicit ModelLoader(QObject *parent = nullptr);

    static QMap<SimpleModel::Type, SimpleModelData *> loadSimpleModelData();

private:
    static SimpleModelData *loadSimpleModelData(SimpleModel::Type type);

    static const QMap<SimpleModel::Type, QString> SIMPLE_MODELS_PATHS;
    static const QString MODELS_PATH;
};

#endif // MODELLOADER_H
