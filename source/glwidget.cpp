#include "glwidget.h"

#include "scene.h"

#include "pgm3d.h"
#include "obj.h"
#include "mesh.h"
#include "camera.h"

#include <QGLShader>
#include <iostream>
#include <QKeyEvent>


GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent),
    m_scene(new Scene)
{
    QGLFormat glFormat;
    glFormat.setVersion(3, 3);
    //    glFormat.setProfile(QGLFormat::CoreProfile); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers(true);

    this->setFormat(glFormat);
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

    Model3d* model = new obj(":/models/cube.obj");
    m_mesh = model->mesh();

    //    m_scene->addMesh(*m_mesh);

    //    m_shaderProgram = new QGLShaderProgram;
    //    m_shaderProgram->addShaderFromSourceFile(QGLShader::Vertex, ":shaders/simpleshader.vert");
    //    m_shaderProgram->addShaderFromSourceFile(QGLShader::Fragment, ":shaders/simpleshader.frag");
    //    m_shaderProgram->link();

    //    m_scene->camera()->setViewport(0, 0, this->width(), this->height());
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    //m_scene->camera()->setViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    m_shaderProgram->bind();
    //    m_shaderProgram->setUniformValue("mat_view", m_scene->camera()->viewMatrix());
    //    m_shaderProgram->setUniformValue("mat_proj", m_scene->camera()->projectionMatrix());
    //    m_scene->render(*m_shaderProgram);

    m_mesh->render();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    event->accept();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch ( event->key() )
    {
    case Qt::Key_Q:
        //m_scene->camera()->rotate(-10.f, 0.f, 1.f, 0.f);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        break;
    case Qt::Key_D:
        //m_scene->camera()->rotate(10.f, 0.f, 1.f, 0.f);
        glClearColor(0.f, 1.f, 0.f, 1.f);
        break;
    default:
        break;
        updateGL();
    }

    event->accept();
}


