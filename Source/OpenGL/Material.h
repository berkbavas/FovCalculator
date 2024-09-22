#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector4D>

class Material
{
public:
    Material();

    float ambient() const;
    void setAmbient(float newAmbient);

    float diffuse() const;
    void setDiffuse(float newDiffuse);

    float specular() const;
    void setSpecular(float newSpecular);

    float shininess() const;
    void setShininess(float newShininess);

private:
    float mAmbient;
    float mDiffuse;
    float mSpecular;
    float mShininess;
};

#endif // MATERIAL_H
