#include "Material.h"

Material::Material()
    : mAmbient(0.25f)
    , mDiffuse(0.75f)
    , mSpecular(0.25f)
    , mShininess(32.0f)
{}

float Material::ambient() const
{
    return mAmbient;
}

void Material::setAmbient(float newAmbient)
{
    mAmbient = newAmbient;
}

float Material::diffuse() const
{
    return mDiffuse;
}

void Material::setDiffuse(float newDiffuse)
{
    mDiffuse = newDiffuse;
}

float Material::specular() const
{
    return mSpecular;
}

void Material::setSpecular(float newSpecular)
{
    mSpecular = newSpecular;
}

float Material::shininess() const
{
    return mShininess;
}

void Material::setShininess(float newShininess)
{
    mShininess = newShininess;
}
