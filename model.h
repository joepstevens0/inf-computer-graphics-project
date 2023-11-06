#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class Texture;

class Model : protected QOpenGLFunctions
{
public:
    Model();
    virtual ~Model();

    void draw(QOpenGLShaderProgram *program);

    struct material{
        material();
        material(QVector3D diffuse, QVector3D ambient, QVector3D specular, float specExp, QOpenGLTexture* diffuseTex);

        QVector3D diffuse = QVector3D(1,1,1);
        QVector3D ambient = QVector3D(0.10f,0.10f,0.10f);
        QVector3D specular = QVector3D(0.5,0.5,0.5);
        float specExp = 300;

        QOpenGLTexture* diffuseTex{nullptr};
    };

    void setMaterial(material mat);
    material getMaterial() const { return m_mat;}

    static Model* load(std::string path);
    static std::map<QString,material> loadMaterial(std::string path);
private:

    void init();
    void bind(QOpenGLShaderProgram *program);

    static Model* createModel(std::vector<QVector3D> vertices,std::vector<QVector3D> normals,
                              std::vector<QVector2D> texcoords,std::vector<QVector3D> indices, Model::material mat);

    std::vector<Model*> m_submodels;
    unsigned int m_nvertices;
    QOpenGLBuffer arrayBuf;

    material m_mat;
};

#endif // MODEL_H
