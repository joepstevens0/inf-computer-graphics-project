#include "glwidget.h"
#include <QKeyEvent>

#include "scene.h"
#include "camera.h"
#include "input.h"
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{
    delete m_camera;

    for(Scene* s : m_scenes)
        delete s;
}

void GLWidget::timerEvent(QTimerEvent *)
{
    update();
    m_camera->FrameUpdate();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);

    Scene* s = new Scene();
    s->load(":/scene2.txt");
    m_scenes.push_back(s);
    s = new Scene();
    s->load(":/scene1.txt");
    m_scenes.push_back(s);
    m_currentscene = 0;

    // create camera
    m_camera = new Camera();

    timer.start(12, this);
}

void GLWidget::update()
{
    // Update input
    QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
    Input::update(m_blockMovement, glob);

    static const float transSpeed = 0.3f;
    static const float rotSpeed   = 0.3f;
    if (!m_blockMovement){
        // Handle rotations
        m_camera->rotate(-rotSpeed * Input::mouseDelta().x(), Camera::LocalUp);
        m_camera->rotate(-rotSpeed * Input::mouseDelta().y(), m_camera->right());



        // Handle translations
        QVector3D translation;
        if (Input::keyPressed(Qt::Key_W))
        {
          translation += m_camera->forward();
        }
        if (Input::keyPressed(Qt::Key_S))
        {
          translation -= m_camera->forward();
        }
        if (Input::keyPressed(Qt::Key_A))
        {
          translation -= m_camera->right();
        }
        if (Input::keyPressed(Qt::Key_D))
        {
          translation += m_camera->right();
        }
        if (Input::keyPressed(Qt::Key_Space))
        {
          translation += m_camera->up();
        }
        if (Input::keyPressed(Qt::Key_Shift))
        {
          translation -= m_camera->up();
        }
        m_camera->translate(transSpeed * translation);
    }
     // Schedule a redraw
     QOpenGLWidget::update();
}

void GLWidget::resizeGL(int w, int h)
{
    float aspect = float(w) / float(h ? h : 1);

    const float zNear = 0.2f, zFar = 100.0, fov = 45.0;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(fov, aspect, zNear, zFar);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_scenes[m_currentscene]->draw(m_camera, m_projectionMatrix);

}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    if (event->key() == Qt::Key_Escape)
    {
        if (!m_blockMovement){
            m_blockMovement = true;
            QCursor cursor(Qt::ArrowCursor);
            GLWidget::setCursor(cursor);
        }
        else{
            m_blockMovement = false;
            QCursor cursor(Qt::CrossCursor);
            GLWidget::setCursor(cursor);
            QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
            QCursor::setPos(glob);
        }
    } else if (event->key() == Qt::Key_F){
        m_scenes[m_currentscene]->switchFlashlight();
    } else if (event->key() == Qt::Key_K){
        ++m_currentscene;
        if (m_currentscene >= m_scenes.size())
            m_currentscene = 0;
    }
    Input::registerKeyPress(event->key());
  }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
  {
    event->ignore();
  }
  else
  {
    Input::registerKeyRelease(event->key());
  }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
  Input::registerMousePress(event->button());

  // picking
  drawForPicking();

  unsigned char res[4];
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);
  glReadPixels(event->x(), viewport[3] - event->y(), 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);
  m_scenes[m_currentscene]->itemPress(res[0]);
  paintGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  Input::registerMouseRelease(event->button());
}

void GLWidget::drawForPicking()
{
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_scenes[m_currentscene]->drawForPicking(m_camera, m_projectionMatrix);
}

