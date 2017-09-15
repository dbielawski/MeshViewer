#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QGLWidget>
#include <QGLShaderProgram>

#include "pgm3d.h"
#include "mesh.h"

class Scene;

class GLWidget : public QGLWidget
{
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    QGLShaderProgram*   m_shaderProgram;
    Scene*              m_scene;


    Mesh*               m_mesh; //TODO: remove
};

#endif // GLVIEWER_H
