#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Camera;
class Scene;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

protected slots:
    void update();

private:
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_vertexObject;
    void drawForPicking();

    bool m_blockMovement = true;

    QBasicTimer timer;

    std::vector<Scene*> m_scenes;
    unsigned int m_currentscene;

    Camera* m_camera{nullptr};
    QMatrix4x4 m_projectionMatrix;

};

#endif // GLWIDGET_H
