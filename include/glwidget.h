#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QGLWidget>
#include "scene.h"

class Scene;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    enum EDisplayMode
    {
        POINT = 0,
        LINE,
        FILL,
        DisplayModeCount
    };


    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    Scene* scene() const { return m_scene; }

    void setDrawMode(uint displayMode) { m_displayModeIndex = displayMode; }

    float pointSizeMin() const;
    float pointSizeMax() const;
    float pointSize() const;
    void pointSize(float size);

	void changeSceneColor(const Color4f&);

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
    Scene* m_scene;

    GLfloat m_pointSize;

    GLuint m_displayModeValues[EDisplayMode::DisplayModeCount] = {
        GL_POINT, GL_LINE, GL_FILL
    };

    uint m_displayModeIndex;

    // INPUTS
    bool m_wheelButtonPressed;
    bool m_leftButtonPressed;
    bool m_rightButtonPressed;

    QPoint m_previousMousePosition;

    float m_zoomStepValue;

    float m_dx;
    float m_dy;
};

#endif // GLVIEWER_H
