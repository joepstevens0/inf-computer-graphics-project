#include "object.h"
#include <QOpenGLShaderProgram>
#include "model.h"

int Object::m_ids{0};

Object::Object(Model* model, QVector3D pos)
    :m_model{model}, m_pos{pos}
{

    m_id = ++m_ids;
}

void Object::draw(QOpenGLShaderProgram *program)
{
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_pos);
    m_modelMatrix.rotate(m_rotation);
    m_modelMatrix.scale(m_scale);

    program->setUniformValue("model", m_modelMatrix);

    m_model->draw(program);
}


