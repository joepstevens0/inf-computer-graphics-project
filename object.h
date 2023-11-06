#ifndef OBJECT_H
#define OBJECT_H

#include <QMatrix4x4>
#include <QQuaternion>
class Model;
class QOpenGLShaderProgram;

class Object
{
public:
    Object(Model* model, QVector3D pos);

    void setScale(float value){m_scale = value;}

    void draw(QOpenGLShaderProgram *program);

    int getId() const { return m_id;}

    Model* getModel()const {return m_model;}
private:

    static int m_ids;

    int m_id;
    Model* m_model;

    QVector3D m_pos;
    float m_scale{1};
    QQuaternion m_rotation;

    QMatrix4x4 m_modelMatrix;
};

#endif // OBJECT_H
