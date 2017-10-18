#include "glwidget.h"

#include <QGLShader>
#include <QKeyEvent>
#include <QMouseEvent>

#include "scene.h"
#include "pgm3d.h"
#include "obj.h"
#include "mesh.h"
#include "camera.h"
#include "light.h"


GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    m_scene(new Scene),
    m_displayModeIndex(EDisplayMode::FILL)
{
    QGLFormat glFormat;
    glFormat.setVersion(3, 3);
    //    glFormat.setProfile(QGLFormat::CoreProfile); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers(true);

    this->setFormat(glFormat);
    setFocusPolicy( Qt::StrongFocus );


    m_wheelButtonPressed = false;
    m_leftButtonPressed = false;
    m_rightButtonPressed = false;

    m_zoomStepValue = 1.f;
    m_dx = 0;

    m_pointSize = 1.f;
}

GLWidget::~GLWidget()
{
    delete m_scene;
}

void GLWidget::changeSceneColor(const Color4f& c) {
	m_scene->setBackgroundColor(c);
	glClearColor(c.r, c.g, c.b, 1.0f);
	updateGL();
}

void GLWidget::initializeGL()
{
    glClearColor(m_scene->backgroundColor().r,
                 m_scene->backgroundColor().g,
                 m_scene->backgroundColor().b, 1.f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPointSize(m_pointSize);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //glEnable(GL_CULL_FACE);

    m_scene->init();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    m_scene->camera()->setSize(w, h);
    m_scene->camera()->setViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, m_displayModeValues[m_displayModeIndex]);

    m_scene->render();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_previousMousePosition.x();
    int dy = event->y() - m_previousMousePosition.y();

    if (m_leftButtonPressed)
    {
        if (dx > 0)
            m_dx = 1;
        else if (dx < 0)
            m_dx = -1;
        else
            m_dx = 0;

        if (dy > 0)
            m_dy = -1;
        else if (dy < 0)
            m_dy = 1;
        else
            m_dy = 0;

        // WARNING: magic numbers/ mouse sensitivity...
        m_scene->camera()->rotateAroundTarget(m_dx * 2, Vector3f(0.f, 1.f, 0.f));
        m_scene->camera()->rotateAroundTarget(m_dy * 2, Vector3f(1.f, 0.f, 0.f));
    }

    m_previousMousePosition = event->pos();

    updateGL();
    event->accept();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        m_leftButtonPressed = true;
        break;
    case Qt::RightButton:
        m_rightButtonPressed = true;
        break;
    case Qt::MiddleButton:
        m_wheelButtonPressed = true;
        break;
    default:
        break;
    }
    event->accept();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        m_leftButtonPressed = false;
        break;
    case Qt::RightButton:
        m_rightButtonPressed = false;
        break;
    case Qt::MiddleButton:
        m_wheelButtonPressed = false;
        break;
    default:
        break;
    }
    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0.f)
        m_scene->camera()->zoom(m_zoomStepValue);
    else
        m_scene->camera()->zoom(-m_zoomStepValue);

    updateGL();
    event->accept();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch ( event->key() )
    {
    case Qt::Key_Q:

        updateGL();
        break;
    case Qt::Key_D:

        updateGL();
        break;
    case Qt::Key_Z:

        updateGL();
        break;
    case Qt::Key_S:

        updateGL();
        break;

    case Qt::Key_M:
        m_displayModeIndex = ++m_displayModeIndex % EDisplayMode::DisplayModeCount;
        updateGL();
        break;

    case Qt::Key_B:
        m_scene->toggleDisplayBoundingBox();
        updateGL();
        break;
    default:
        break;
    }

    event->accept();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
    event->accept();
}

float GLWidget::pointSizeMin() const
{
    return 1.f;
}

float GLWidget::pointSizeMax() const
{
    GLfloat max;
    glGetFloatv(GL_POINT_SIZE_MAX, &max);

    return 50.f;
}

float GLWidget::pointSize() const
{
    return m_pointSize;
}

void GLWidget::pointSize(float size)
{
    glPointSize(size);
}
