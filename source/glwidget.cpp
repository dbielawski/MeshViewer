#include "glwidget.h"

#include "scene.h"

#include "pgm3d.h"
#include "obj.h"
#include "mesh.h"
#include "camera.h"

#include <QGLShader>
#include <iostream>
#include <QKeyEvent>
#include <QMouseEvent>

#include "assert.h"

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
    m_zoomValue = 1.f;
}

GLWidget::~GLWidget()
{
    delete m_scene;
}

void GLWidget::initializeGL()
{
    glClearColor(m_scene->backgroundColor().r,
                 m_scene->backgroundColor().g,
                 m_scene->backgroundColor().b, 1.f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPointSize(15.f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Model3d* model = new obj(":/models/cube_tr.obj");
    Mesh* m_mesh = model->mesh();

    m_scene->init();
    m_scene->addMesh(*m_mesh);
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
    //        std::cout << "mouseMoveEvent" << std::endl;

    int dx = event->x() - m_previousMousePosition.x();
    int dy = event->y() - m_previousMousePosition.y();

    if (m_wheelButtonPressed)
    {
//        m_mesh->mView.rotate(dx, 0.f, 1.f);
//        m_mesh->mView.rotate(dy, 1.f, 0.f);

        m_scene->camera()->rotateAround(dx, Vector3f(0.f, 1.f, 0.f));
        //m_scene->camera()->rotateAround(dy, Vector3f(1.f, 0.f, 0.f));
    }

    m_previousMousePosition = event->pos();

    updateGL();
    event->accept();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    //    std::cout << "mousePressEvent" << std::endl;

    switch (event->button())
    {
    case Qt::LeftButton:
        break;
    case Qt::RightButton:
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
        break;
    case Qt::RightButton:
        break;
    case Qt::MiddleButton:
        m_wheelButtonPressed = false;
        break;
    default:
        break;
    }

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    //    std::cout << "key event in board:" <<  event->buttons() << std::endl;
    //    std::cout << "wheelEvent" << std::endl;

    if (event->delta() > 0.f)
    {
//        std::cout << "zoomm in " << std::endl;
        m_zoomValue += 0.1f;
    }
    else
    {
//        std::cout << "zoomm out" << std::endl;
        m_zoomValue -= 0.1f;
    }

    updateGL();
    event->accept();

}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    // TODO: faire au propre
    switch ( event->key() )
    {
    case Qt::Key_Q:
        m_scene->camera()->rotate(10, 0.f, 1.f, 0.f);
        updateGL();
        break;
    case Qt::Key_D:
        m_scene->camera()->rotate(-10, 0.f, 1.f, 0.f);
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
        // Toggle display AABB
        updateGL();
        break;
    default:
        break;
    }

    //    std::cout << "key event in board:" <<  event->key() << std::endl;

    event->accept();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{

}


