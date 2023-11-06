#include "scene.h"
#include "model.h"
#include "object.h"
#include "lightobject.h"
#include "camera.h"
#include <QFile>
#include <QOpenGLFunctions_4_0_Core>
#include "light.h"

Scene::Scene()
{

    initShaders();

    m_models["cube"] = Model::load(":/cube.obj");
    m_wireframebutton = new Object(m_models["cube"], QVector3D(15,0,0));
    m_wireframebutton->setScale(0.5f);

    m_objects.push_back(m_wireframebutton);

    m_flashlight = new Light();
    m_flashlight->setSpot(true);
}

Scene::~Scene()
{
    for(LightObject* l : m_lights){
        delete l;
    }
    for(Object* o: m_objects){
        delete o;
    }

    auto it = m_models.begin();
    while(it != m_models.end()){
        delete it->second;
        ++it;
    }

    delete m_flashlight;
}

void Scene::initShaders()
{
    // object shader
    if (!m_objectShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vert"))
        throw "error loading shader";

    if (!m_objectShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.frag"))
        throw "error loading shader";

    if (!m_objectShader.link())
        throw "error loading shader";

    if (!m_objectShader.bind())
        throw "error loading shader";

    // light object shader (const color)
    if (!m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lightshader.vert"))
       throw "error loading shader";

    if (!m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lightshader.frag"))
        throw "error loading shader";

    if (!m_lightShader.link())
        throw "error loading shader";

    if (!m_lightShader.bind())
        throw "error loading shader";


    // picking shader
    if (!m_pickingShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/pickingshader.vert"))
        throw "error loading shader";

    if (!m_pickingShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/pickingshader.frag"))
        throw "error loading shader";

    if (!m_pickingShader.link())
        throw "error loading shader";

    if (!m_pickingShader.bind())
        throw "error loading shader";
}

void Scene::draw(Camera* cam, QMatrix4x4 projectionmatrix)
{
    if (m_wireframe){
        QOpenGLFunctions_4_0_Core t;
        t.initializeOpenGLFunctions();
        t.glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }

    // draw objects
    cam->setView(&m_objectShader);
    m_objectShader.setUniformValue("projection", projectionmatrix);

    // set lights
    LightObject::resetLights();
    // set flashlight
    if(m_flashlighton){
        m_flashlight->setPos(cam->translation());
        m_flashlight->setDirection(cam->forward());
        m_flashlight->loadLight(&m_objectShader);
    }

    // set light objects
    for(LightObject* l: m_lights){
        l->loadLight(&m_objectShader);
    }

    for(Object* o: m_objects){
        o->draw(&m_objectShader);
    }

    // draw light objects
    cam->setView(&m_lightShader);
    m_lightShader.setUniformValue("projection", projectionmatrix);
    for(LightObject* o: m_lights){
        o->draw(&m_lightShader);
    }

    if (m_wireframe){
        QOpenGLFunctions_4_0_Core t;
        t.initializeOpenGLFunctions();
        t.glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

void Scene::drawForPicking(Camera* cam, QMatrix4x4 projectionmatrix)
{
    // set view, projection
    cam->setView(&m_pickingShader);
    m_pickingShader.setUniformValue("projection", projectionmatrix);

    // draw objects
    for(Object* o: m_objects){
        m_pickingShader.setUniformValue("objectid", o->getId());
        o->draw(&m_pickingShader);
    }

    // draw light objects
    for(LightObject* o: m_lights){
        m_pickingShader.setUniformValue("objectid", o->getId());
        o->draw(&m_pickingShader);
    }
}

void Scene::itemPress(unsigned char id)
{
    for(unsigned int i = 0; i < m_lights.size();++i){
        if (m_lights[i]->getId() == id){
            m_lights[i]->switchLight(!m_lights[i]->isOn());
        }
    }

    if (id == m_wireframebutton->getId()){
        m_wireframe = !m_wireframe;
    }
}

void Scene::load(QString path)
{
    QFile f{path};
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while(!f.atEnd()){
        QString line = QString(f.readLine()).trimmed();

        if (line.size() == 0)
            continue;
        QString p = line.left(line.indexOf(' '));
        QString pars = line.mid(line.indexOf(' ') + 1);

        if(p == "m"){
            QStringList s = pars.split(" ");
            m_models[s[1]] = Model::load(s[0].toStdString());
        } else if (p == "lo"){
            QStringList s = pars.split(" ");
            LightObject* o = new LightObject(m_models[s[0]], QVector3D(s[1].toFloat(),s[2].toFloat(),s[3].toFloat()));

            if (s.length() > 4){
                o->setScale(s[4].toFloat());
            }
            if (s.length() > 5){
                o->setColor(s[5].toFloat(),s[6].toFloat(),s[7].toFloat());
            }
            m_lights.push_back(o);

        } else if (p == "o"){
            QStringList s = pars.split(" ");
            Object* o = new Object(m_models[s[0]], QVector3D(s[1].toFloat(),s[2].toFloat(),s[3].toFloat()));
            if (s.length() > 4){
                o->setScale(s[4].toFloat());
            }
            m_objects.push_back(o);
        }
    }
}
