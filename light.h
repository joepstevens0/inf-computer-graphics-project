#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class QOpenGLShaderProgram;

class Light
{
public:
    Light();

    void loadLight(QOpenGLShaderProgram* shader) const;

    void setPos(QVector3D pos){m_pos = pos;}
    void setColor(QVector3D color){m_color = color;}

    QVector3D color() const { return m_color;}

    static void resetLights(){activeLights = 0;}

    void setDirection(QVector3D dir){m_direction = dir;}
    void setSpot(bool isSpot){m_spot = isSpot;}
private:
    static unsigned int activeLights;

    QVector3D m_pos{0,0,0};
    QVector3D m_color{1,1,1};

    bool m_spot = false;
    QVector3D m_direction{0,-1,0};
    float m_innercutoff{12};
    float m_outercutoff{17};
};

#endif // LIGHT_H
