#include "light.h"
#include <QOpenGLShaderProgram>
#include <string>

unsigned int Light::activeLights{0};

Light::Light()
{

}

void Light::loadLight(QOpenGLShaderProgram* shader) const
{
    shader->bind();
    std::string location = std::string("lights[") + std::to_string(activeLights) + "]";
    std::string locationpos = location + ".pos";
    std::string locationcolor = location + ".color";
    std::string locationspot = location + ".isSpot";

    shader->setUniformValue(locationpos.c_str(), m_pos);
    shader->setUniformValue(locationcolor.c_str(), m_color);
    shader->setUniformValue(locationspot.c_str(), m_spot);

    if (m_spot){
        std::string locationdir = location + ".direction";
        std::string locationinner = location + ".innercutoff";
        std::string locationouter = location + ".outercutoff";
        shader->setUniformValue(locationdir.c_str(), m_direction);
        shader->setUniformValue(locationinner.c_str(), m_innercutoff);
        shader->setUniformValue(locationouter.c_str(), m_outercutoff);
    }

    shader->setUniformValue("nrlights", ++activeLights);

}
