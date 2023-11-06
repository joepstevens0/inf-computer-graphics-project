#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLShaderProgram>

class LightObject;
class Model;
class Object;
class Camera;
class Light;

class Scene
{
public:
    Scene();
    ~Scene();

    void load(QString path);

    void draw(Camera* cam, QMatrix4x4 projectionmatrix);
    void drawForPicking(Camera* cam, QMatrix4x4 projectionmatrix);

    void itemPress(unsigned char id);

    void addObject(Object* ob){m_objects.push_back(ob);}
    void addLightObject(LightObject* lo){m_lights.push_back(lo);}

    void switchFlashlight() {m_flashlighton = !m_flashlighton;}
private:

    void initShaders();

    std::vector<LightObject*> m_lights;
    std::vector<Object*> m_objects;
    std::map<QString, Model*> m_models;

    QOpenGLShaderProgram m_objectShader;
    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_pickingShader;

    Object* m_wireframebutton{nullptr};
    Object* m_shadebutton{nullptr};

    Light* m_flashlight{nullptr};
    bool m_flashlighton = false;

    bool m_wireframe = false;
};

#endif // SCENE_H
