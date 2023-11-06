#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class QOpenGLShaderProgram;
class QKeyEvent;


class Camera
{
public:
    static const QVector3D LocalForward;
    static const QVector3D LocalUp;
    static const QVector3D LocalRight;

    Camera();

    void translate(const QVector3D &dt);
    void translate(float dx, float dy, float dz);
    void rotate(const QQuaternion &dr);
    void rotate(float angle, const QVector3D &axis);
    void rotate(float angle, float ax, float ay, float az);

    void setTranslation(const QVector3D &t);
    void setTranslation(float x, float y, float z);
    void setRotation(const QQuaternion &r);
    void setRotation(float angle, const QVector3D &axis);
    void setRotation(float angle, float ax, float ay, float az);

    const QVector3D& translation() const;
    const QQuaternion& rotation() const;
    const QMatrix4x4& toMatrix();

    QVector3D forward() const;
    QVector3D right() const;
    QVector3D up() const;

    void setView(QOpenGLShaderProgram *program);

    void keyPressEvent(QKeyEvent *event);

    void FrameUpdate();
private:
    bool m_dirty;
    QVector3D m_translation;
    QQuaternion m_rotation;
    QMatrix4x4 m_viewMatrix;
};

inline Camera::Camera() : m_dirty(true) {}

inline void Camera::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void Camera::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

inline void Camera::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void Camera::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

inline const QVector3D& Camera::translation() const { return m_translation; }
inline const QQuaternion& Camera::rotation() const { return m_rotation; }



#endif // CAMERA_H
