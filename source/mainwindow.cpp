#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "glwidget.h"
/*  */
#include "pgm3d.h"
#include "obj.h"
/*  */

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->alphaValue->setText(QString::number(ui->transparencySlider->value()));

    createActions(); // Create action before menu !
    createMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_clearSceneAction);
    m_fileMenu->addAction(m_quitAction);

    m_display = menuBar()->addMenu(tr("&Display"));
    m_display->addAction(m_toggleDisplayBoundingBoxAction);
    m_display->addSeparator();
    m_display->addAction(m_drawPointsAction);
    m_display->addAction(m_drawLinesAction);
    m_display->addAction(m_drawFilledAction);
	m_display->setEnabled(false);
}

void MainWindow::createActions()
{
    m_openAction = new QAction(tr("&Open"), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open an existing file"));
    connect(m_openAction, SIGNAL(triggered(bool)), this, SLOT(openFile()));

    m_saveAsObjAction = new QAction(tr("&Save as Obj"), this);
    m_saveAsObjAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    m_saveAsObjAction->setStatusTip(tr("Save the model as OBJ file"));

    m_clearSceneAction = new QAction(tr("&Clear scene"), this);
    m_clearSceneAction->setShortcut(QKeySequence(tr("Ctrl+W")));
    m_clearSceneAction->setStatusTip(tr("Clear the scene"));
    connect(m_clearSceneAction, SIGNAL(triggered(bool)), this, SLOT(clearScene()));

    m_quitAction = new QAction(tr("&Quit"), this);
    m_quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    m_quitAction->setStatusTip(tr("Exit the application"));
    connect(m_quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

    m_toggleDisplayBoundingBoxAction = new QAction(tr("&Render AABB"), this);
    m_toggleDisplayBoundingBoxAction->setStatusTip(tr("Display the bounding box of the model | Toggle press B"));

    m_drawPointsAction = new QAction(tr("&Points"), this);
    m_drawPointsAction->setStatusTip(tr("Render the model in point mode | Press M to switch"));
	connect(m_drawPointsAction, SIGNAL(triggered(bool)), this, SLOT(setDrawPoint()));

    m_drawLinesAction = new QAction(tr("&Lines"), this);
    m_drawLinesAction->setStatusTip(tr("Render the model in line mode | Press M to switch"));
	connect(m_drawLinesAction, SIGNAL(triggered(bool)), this, SLOT(setDrawLine()));

    m_drawFilledAction = new QAction(tr("&Filled"), this);
    m_drawFilledAction->setStatusTip(tr("Render the model with faces | Press M to switch"));
	connect(m_drawFilledAction, SIGNAL(triggered(bool)), this, SLOT(setDrawFilled()));

    connect(ui->transparencySlider, SIGNAL(valueChanged(int)), this, SLOT(updateAlpha(int)));
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    "../models/",
                                                    tr("OBJ Files (*.obj);; PGM3D Files (*.pgm3d)"));

    if(fileName.isEmpty())
        return;

    QFileInfo fi(fileName);
    QString ext = fi.suffix();

    Model3d* model = nullptr;
    if(ext.toStdString() == "pgm3d") {
        model = new pgm3d(fileName);
	}
    else if (ext.toStdString() == "obj") {
        model = new obj(fileName);
	}
	else {
		QMessageBox::critical(0, "Error", "File format " + ext + " is not supported.");
		exit(EXIT_FAILURE);
	}

	if(model == nullptr) {
		QMessageBox::critical(0, "Error", "Error while loading the model.");
		exit(EXIT_FAILURE);
	}

    Mesh* mesh = model->mesh();
    ui->openGLWidget->scene()->addMesh(*mesh);
    ui->openGLWidget->updateGL();

	m_display->setEnabled(true);
}

void MainWindow::clearScene()
{
	ui->openGLWidget->scene()->clear();
}

void MainWindow::updateAlpha(int alpha)
{
	ui->alphaValue->setText(QString::number(alpha));
	float alpha_val = alpha / 255.0;
	ui->openGLWidget->scene()->shaderProgram()->setUniformValue("alpha_val", alpha_val);
	ui->openGLWidget->updateGL();

}

void MainWindow::setDrawPoint()
{
	// TODO : Replace with enum value
	setDraw(0);
}

void MainWindow::setDrawLine()
{
	// TODO : Replace with enum value
	setDraw(1);
}

void MainWindow::setDrawFilled()
{
	// TODO : Replace with enum value
	setDraw(2);
}

void MainWindow::setDraw(unsigned int value)
{
	ui->openGLWidget->setDrawMode(value);
}
