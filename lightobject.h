#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include "light.h"
#include "object.h"

class LightObject
{
public:
    LightObject(Model* model, QVector3D pos);

    void setScale(float value){m_object.setScale(value);}

    void loadLight(QOpenGLShaderProgram* program);
    void draw(QOpenGLShaderProgram *program);

    int getId() const { return m_object.getId();}

    void switchLight(bool isOn){m_isOn = isOn;}
    bool isOn()const { return m_isOn;}

    static void resetLights(){Light::resetLights();}
     void setColor(float r, float g, float b){m_light.setColor(QVector3D(r,g,b)); m_onColor = QVector3D(r,g,b);}
private:
    bool m_isOn = true;
    QVector3D m_onColor;
    Light m_light;
    Object m_object;
};

#endif // LIGHTOBJECT_H
