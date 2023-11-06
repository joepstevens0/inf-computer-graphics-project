#include "model.h"

#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <string>
#include <QFileInfo>

struct VertexData
{
    float position[3];
    float normal[3];
    float texCoord[2];
};

Model::Model()
    : arrayBuf(QOpenGLBuffer::Type::VertexBuffer)
{
    initializeOpenGLFunctions();
    m_mat = material();
    init();
}

Model::~Model()
{
    arrayBuf.destroy();

    for (unsigned int i = 0; i < m_submodels.size();++i){
        delete m_submodels[i];
    }
}

void Model::init()
{
    arrayBuf.create();
}

void Model::draw(QOpenGLShaderProgram *program)
{

    bind(program);

    glDrawArrays(GL_TRIANGLES, 0, m_nvertices);

    for (unsigned int i = 0; i < m_submodels.size();++i){
        m_submodels[i]->draw(program);
    }
}

void Model::setMaterial(Model::material mat)
{
    m_mat = mat;
}

void Model::bind(QOpenGLShaderProgram *program)
{

    program->bind();

    program->setUniformValue("mat.ambient", m_mat.ambient);
    program->setUniformValue("mat.diffuse", m_mat.diffuse);
    program->setUniformValue("mat.specular", m_mat.specular);
    program->setUniformValue("mat.specExp", m_mat.specExp);

    if (m_mat.diffuseTex != nullptr){
        m_mat.diffuseTex->bind(0);
        program->setUniformValue("mat.texture_diffuse", 0);
        program->setUniformValue("mat.hasTex", true);
    } else
        program->setUniformValue("mat.hasTex", false);

    arrayBuf.bind();



    int offset = 0;

    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += 3*sizeof(float);

    program->enableAttributeArray(1);
    program->setAttributeBuffer(1, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += 3*sizeof(float);

    program->enableAttributeArray(2);
    program->setAttributeBuffer(2, GL_FLOAT, offset, 2, sizeof(VertexData));
}


Model* Model::load(std::string path)
{
    QFile f{QString(path.c_str())};
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return nullptr;

    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> texcoords;
    std::vector<QVector3D> indices;
    std::map<QString, Model::material> materials;
    material* currentmat{nullptr};
    std::vector<Model*> submodels;

    while(!f.atEnd()){
        QString line = QString(f.readLine()).trimmed();

        if (line.size() == 0)
            continue;
        QString p = line.left(line.indexOf(' '));
        QString pars = line.mid(line.indexOf(' ') + 1);

        if(p == "v"){
            QStringList par = pars.split(' ');
            vertices.push_back(QVector3D(par[0].toFloat(),par[1].toFloat(), par[2].toFloat()));

        }else if(p == "vt"){
            QStringList par = pars.split(' ');
            texcoords.push_back(QVector2D(par[0].toFloat(),par[1].toFloat()));

        }else if(p == "vn"){
            QStringList par = pars.split(' ');
            normals.push_back(QVector3D(par[0].toFloat(),par[1].toFloat(), par[2].toFloat()));

        }else if(p == "f"){
            QStringList par = pars.split(' ');
            QStringList ind = par[0].split('/');
            indices.push_back(QVector3D(ind[0].toUShort()-1, ind[1].toUShort()-1, ind[2].toUShort()-1));
            ind = par[1].split('/');
            indices.push_back(QVector3D(ind[0].toUShort()-1, ind[1].toUShort()-1, ind[2].toUShort()-1));
            ind = par[2].split('/');
            indices.push_back(QVector3D(ind[0].toUShort()-1, ind[1].toUShort()-1, ind[2].toUShort()-1));
        } else if(p == "usemtl"){
            if(currentmat != nullptr){
                submodels.push_back(createModel(vertices, normals, texcoords, indices, *currentmat));
                indices.clear();
            }
            currentmat = &materials[pars];

        } else if(p == "mtllib"){
            QString croped_fileName = QString::fromStdString(path).split(".",QString::SkipEmptyParts).at(0);

            materials = loadMaterial((croped_fileName + ".mtl").toStdString());
        }
    }

    Model* newmodel;
    if( currentmat == nullptr){
        currentmat = new material();
        newmodel = createModel(vertices, normals, texcoords, indices, *currentmat);
        delete currentmat;
    } else newmodel = createModel(vertices, normals, texcoords, indices, *currentmat);

    newmodel->m_submodels = submodels;

    return newmodel;
}

std::map<QString,Model::material> Model::loadMaterial(std::string path)
{
    std::map<QString,material> map;
    QFile f{QString(path.c_str())};
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return map;

    float specExp = 0;
    QVector3D diffuse;
    QVector3D ambient;
    QVector3D specular;
    QString name = "";
    QOpenGLTexture* texture = nullptr;

    while(!f.atEnd()){
        QString line = QString(f.readLine()).trimmed();

        if (line.size() == 0)
            continue;

        QString p = line.left(line.indexOf(' '));
        QString pars = line.mid(line.indexOf(' ') + 1);

        if(p == "Ns"){
            specExp = pars.toFloat();
        }else if(p == "Ka"){
            QStringList par = pars.split(' ');
            ambient = QVector3D(par[0].toFloat(),par[1].toFloat(), par[2].toFloat());
        }else if(p == "Kd"){
            QStringList par = pars.split(' ');
            diffuse = QVector3D(par[0].toFloat(),par[1].toFloat(), par[2].toFloat());
        }else if(p == "Ks"){
            QStringList par = pars.split(' ');
            specular = QVector3D(par[0].toFloat(),par[1].toFloat(), par[2].toFloat());
        }else if(p == "Te"){
            QImage image;

            if(!image.load(":/" + pars))
                throw "could not load texture";

            texture = new QOpenGLTexture(image, QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
            texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture->setMagnificationFilter(QOpenGLTexture::Linear);
            texture->setMipLevels(4);
        }else if(p == "newmtl"){
            if (name != "")
                map[name] = Model::material(diffuse, ambient, specular, specExp, texture);
            texture = nullptr;
            name = pars;
        }
    }
    if(name != "")
        map[name] = Model::material(diffuse, ambient, specular, specExp, texture);

    return map;
}

Model* Model::createModel(std::vector<QVector3D> vertices,std::vector<QVector3D> normals,
                          std::vector<QVector2D> texcoords,std::vector<QVector3D> indices, Model::material mat)
{
    Model* newmodel = new Model();
    std::vector<float> arraydata;
    for (unsigned int i = 0; i < indices.size();++i){
        QVector3D vertex = vertices[indices[i][0]];
        QVector3D normal = normals[indices[i][2]];
        QVector2D texcoord = texcoords[indices[i][1]];
        arraydata.push_back(vertex[0]);
        arraydata.push_back(vertex[1]);
        arraydata.push_back(vertex[2]);
        arraydata.push_back(normal[0]);
        arraydata.push_back(normal[1]);
        arraydata.push_back(normal[2]);
        arraydata.push_back(texcoord[0]);
        arraydata.push_back(texcoord[1]);
    }

    newmodel->setMaterial(mat);
    newmodel->arrayBuf.bind();
    newmodel->arrayBuf.allocate(arraydata.data(), arraydata.size()*sizeof(float));
    newmodel->m_nvertices = arraydata.size();
    return newmodel;
}



Model::material::material()
{

}

Model::material::material(QVector3D diffuse, QVector3D ambient, QVector3D specular, float specExp, QOpenGLTexture* diffuseTex)
{
    this->diffuse = diffuse;
    this->ambient = ambient;
    this->specular = specular;
    this->specExp = specExp;
    this->diffuseTex = diffuseTex;
}
