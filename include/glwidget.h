#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QGLWidget>
#include <QGLShaderProgram>

#include <QWheelEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#include "pgm3d.h"
#include "mesh.h"

class Scene;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    Scene*              m_scene;


    // TODO: ameliorer ? ^^
    enum EDisplayMode
    {
        POINT = 0,
        LINE,
        FILL,
        DisplayModeCount
    };

    GLuint m_displayModeValues[EDisplayMode::DisplayModeCount] = {
        GL_POINT, GL_LINE, GL_FILL
    };
    unsigned int        m_displayModeIndex;


    // INPUTS
    bool m_wheelButtonPressed;
    QPoint m_previousMousePosition;

    float m_zoomValue;
};

#endif // GLVIEWER_H
