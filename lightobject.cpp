#include "lightobject.h"
#include <QOpenGLShaderProgram>

LightObject::LightObject(Model* model, QVector3D pos)
    :m_object(model, pos)
{
    m_light.setPos(pos);
    m_onColor = m_light.color();
}

void LightObject::loadLight(QOpenGLShaderProgram* program)
{
    if (m_isOn){
        m_light.setColor(m_onColor);
    }
    else{
        m_light.setColor(QVector3D(0,0,0));
    }

    m_light.loadLight(program);
}
void LightObject::draw(QOpenGLShaderProgram *program)
{
    program->setUniformValue("lightOn", m_isOn);
    m_object.draw(program);
}
