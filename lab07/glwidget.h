#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLVertexArrayObject m_terrainVao;
    QOpenGLBuffer m_terrainVbo;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;
};

#endif // GLWIDGET_H
