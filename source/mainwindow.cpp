#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLayout>
#include <QLabel>
#include <QColorDialog>
#include <QMessageBox>

#include "glwidget.h"
#include "pgm3d.h"
#include "obj.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MeshViewer");

    connect(ui->pointsModeButton, SIGNAL(clicked()), this, SLOT(onDrawPoint()));
    connect(ui->linesModeButton,  SIGNAL(clicked()), this, SLOT(onDrawLine()));
    connect(ui->fillModeButton,   SIGNAL(clicked()), this, SLOT(onDrawFilled()));

    connect(ui->clearSceneButton,           SIGNAL(clicked()), this, SLOT(onClearScene()));
    connect(ui->sceneBackgroundColorButton, SIGNAL(clicked()), this, SLOT(onBackgroundColorScene()));
    connect(ui->detectHolesButton,          SIGNAL(clicked()), this, SLOT(onDetectHoles()));
    connect(ui->holesFillingButton,         SIGNAL(clicked()), this, SLOT(holesFillingAction()));
    connect(ui->fill_linesButton,           SIGNAL(clicked()), this, SLOT(onDrawFilledAndLines()));

    ui->alphaValue->setText(QString::number(ui->transparencySlider->value()));

    createActions();
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
    m_fileMenu->addAction(m_saveAsObjAction);
    m_fileMenu->addAction(m_clearSceneAction);
    m_fileMenu->addAction(m_quitAction);

    m_displayMenu = menuBar()->addMenu(tr("&Display"));
    m_displayMenu->addAction(m_toggleDisplayBoundingBoxAction);
	m_displayMenu->addAction(m_toggleDisplayOctreeAction);
    m_displayMenu->addSeparator();
    m_displayMenu->addAction(m_drawPointsAction);
    m_displayMenu->addAction(m_drawLinesAction);
    m_displayMenu->addAction(m_drawFilledAction);
    m_displayMenu->setEnabled(false);

	m_actionMenu = menuBar()->addMenu(tr("&Actions"));
    m_actionMenu->addAction(m_holesFillingAction);
}

void MainWindow::createActions()
{
    m_openAction = new QAction(tr("&Open"), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open an existing file"));
    connect(m_openAction, SIGNAL(triggered(bool)), this, SLOT(onOpenFile()));

    m_saveAsObjAction = new QAction(tr("&Save as Obj"), this);
    m_saveAsObjAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    m_saveAsObjAction->setStatusTip(tr("Save the model as OBJ file"));
    connect(m_saveAsObjAction, SIGNAL(triggered(bool)), this, SLOT(onSaveAsObj()));

    m_clearSceneAction = new QAction(tr("&Clear scene"), this);
    m_clearSceneAction->setShortcut(QKeySequence(tr("Ctrl+W")));
    m_clearSceneAction->setStatusTip(tr("Clear the scene"));
    connect(m_clearSceneAction, SIGNAL(triggered(bool)), this, SLOT(onClearScene()));

    m_quitAction = new QAction(tr("&Quit"), this);
    m_quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    m_quitAction->setStatusTip(tr("Exit the application"));
    connect(m_quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

    m_toggleDisplayBoundingBoxAction = new QAction(tr("&Render AABB"), this);
    m_toggleDisplayBoundingBoxAction->setStatusTip(tr("Display the bounding box of the model | Toggle press B"));
    connect(m_toggleDisplayBoundingBoxAction, SIGNAL(triggered(bool)), this, SLOT(onToggleDisplayBoundingBox()));

    m_toggleDisplayOctreeAction = new QAction(tr("&Render Octree"), this);
    m_toggleDisplayOctreeAction->setStatusTip(tr("Display the built octree of the model | Toggle press O"));
    connect(m_toggleDisplayOctreeAction, SIGNAL(triggered(bool)), this, SLOT(onToggleDisplayOctree()));

    m_drawPointsAction = new QAction(tr("&Points"), this);
    m_drawPointsAction->setStatusTip(tr("Render the model in point mode | Press M to switch"));
    connect(m_drawPointsAction, SIGNAL(triggered(bool)), this, SLOT(onDrawPoint()));

    m_drawLinesAction = new QAction(tr("&Lines"), this);
    m_drawLinesAction->setStatusTip(tr("Render the model in line mode | Press M to switch"));
    connect(m_drawLinesAction, SIGNAL(triggered(bool)), this, SLOT(onDrawLine()));

    m_drawFilledAction = new QAction(tr("&Filled"), this);
    m_drawFilledAction->setStatusTip(tr("Render the model with faces | Press M to switch"));
    connect(m_drawFilledAction, SIGNAL(triggered(bool)), this, SLOT(onDrawFilled()));

    connect(ui->transparencySlider, SIGNAL(valueChanged(int)), this, SLOT(onAlphaChanged(int)));
    connect(ui->pointSizeSlider,SIGNAL(valueChanged(int)), this, SLOT(onPointSizeChanged(int)));

    m_holesFillingAction = new QAction(tr("&Fill Holes"), this);
    m_holesFillingAction->setShortcut(QKeySequence('H'));
    m_holesFillingAction->setStatusTip(tr("Fill holes in the current model | Press H to process."));
    connect(m_holesFillingAction, SIGNAL(triggered(bool)), this, SLOT(holesFillingAction()));

    // Update infos in the right panel (vertices/faces count...)
    updateInfos();

    ui->pointSizeSlider->setMinimum(ui->openGLWidget->pointSizeMin());
    ui->pointSizeSlider->setMaximum(ui->openGLWidget->pointSizeMax());
    ui->pointSizeSlider->setValue(ui->openGLWidget->pointSize());
    ui->pointSizeValue->setText(QString::number(ui->openGLWidget->pointSize()));
}

void MainWindow::onOpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    "../models/",
                                                    tr("OBJ Files (*.obj);; PGM3D Files (*.pgm3d)"));

    if (fileName.isEmpty()) {
        return;
    }

    QFileInfo fi(fileName);
    QString ext = fi.suffix();

    Model3d* model = Q_NULLPTR;
    if (ext.toStdString() == "pgm3d") {
        model = new pgm3d(fileName);
    }
    else if (ext.toStdString() == "obj") {
        model = new obj(fileName);
    }
    else {
        QMessageBox::critical(0, "Error", "File format " + ext + " is not supported.");
        exit(EXIT_FAILURE);
    }

    if (model == nullptr) {
        QMessageBox::critical(0, "Error", "Error while loading the model.");
        exit(EXIT_FAILURE);
    }

    Mesh* mesh = model->mesh();

    ui->openGLWidget->scene()->addMesh(*mesh);
    ui->openGLWidget->updateGL();

    m_displayMenu->setEnabled(true);
    updateInfos();
}

void MainWindow::onSaveAsObj()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save file"), tr("../models/mesh.obj"), tr("OBJ Files (*.obj)"));

    ui->openGLWidget->scene()->saveMeshes(fileName);
}

void MainWindow::onClearScene()
{
    ui->openGLWidget->scene()->removeModels();
    updateInfos();

    if (ui->openGLWidget->scene()->meshCount() == 0) {
        m_displayMenu->setEnabled(false);
        m_actionMenu->setEnabled(false);
    }

    ui->openGLWidget->updateGL();
}

void MainWindow::holesFillingAction()
{
    ui->openGLWidget->scene()->fillHoles();
    updateInfos();
    ui->openGLWidget->updateGL();
}

void MainWindow::onAlphaChanged(int alpha)
{
    ui->alphaValue->setText(QString::number(alpha));
    float alpha_val = alpha / 255.f;
    ui->openGLWidget->scene()->setTransparency(alpha_val);
    ui->openGLWidget->scene()->simpleShadingProgram()->bind();
    ui->openGLWidget->scene()->simpleShadingProgram()->setUniformValue("alpha_val", alpha_val);
    ui->openGLWidget->scene()->simpleShadingProgram()->release();
    ui->openGLWidget->updateGL();
}

void MainWindow::onPointSizeChanged(int)
{
    ui->openGLWidget->pointSize(ui->pointSizeSlider->value());
    ui->pointSizeValue->setText(QString::number(ui->pointSizeSlider->value()));
    ui->openGLWidget->updateGL();
}

void MainWindow::onToggleDisplayBoundingBox()
{
    ui->openGLWidget->scene()->toggleDisplayBoundingBox();
    ui->openGLWidget->updateGL();
}

void MainWindow::onToggleDisplayOctree()
{
    ui->openGLWidget->scene()->toggleDisplayOctree();
	ui->openGLWidget->updateGL();
}

void MainWindow::onDrawPoint()
{
    setDraw(GLWidget::EDisplayMode::POINT);
}

void MainWindow::onDrawLine()
{
    setDraw(GLWidget::EDisplayMode::LINE);
}

void MainWindow::onDrawFilled()
{
    setDraw(GLWidget::EDisplayMode::FILL);
}

void MainWindow::onDrawFilledAndLines()
{
    ui->openGLWidget->scene()->toggleFilledLinesMode();
    ui->openGLWidget->updateGL();
}

void MainWindow::onDetectHoles()
{
    ui->openGLWidget->scene()->detectHoles();
    ui->openGLWidget->updateGL();
    updateInfos();
}

void MainWindow::setDraw(uint value)
{
    ui->openGLWidget->setDrawMode(value);
    ui->openGLWidget->updateGL();
}

void MainWindow::updateInfos() const
{
    uint meshCount = ui->openGLWidget->scene()->meshCount();
    uint verticesCount = ui->openGLWidget->scene()->vertexCount();
    uint trianglesCount = ui->openGLWidget->scene()->triangleCount();
    uint facesCount = ui->openGLWidget->scene()->faceCount();
    uint lightCount = ui->openGLWidget->scene()->lightCount();

    ui->verticesCount->setText(QString("Vertice(s): " + QString::number(verticesCount)));
    ui->facesCount->setText(QString("Face(s): " + QString::number(facesCount)));
    ui->modelCount->setText(QString("Model(s): " + QString::number(meshCount)));
    ui->lightCount->setText(QString("Light(s): " + QString::number(lightCount)));

	if (meshCount != 0) {
		bool validity = ui->openGLWidget->scene()->isValid();
		bool closed = ui->openGLWidget->scene()->isClosed();

		QString valid = validity ? "<span style=\"color: #27ae60;\">true</span>" : "<span style=\"color: #c0392b;\">false</span>";
		ui->topoValid->setText(QString("Validity: " + valid));

        QString close = closed ? "<span style=\"color: #27ae60;\">true</span>" : "<span style=\"color: #c0392b;\">false</span>";
		ui->topoClosed->setText(QString("Closed: " + close));
	}
    else {
		ui->topoValid->setText(QString("Validity: ???"));
		ui->topoClosed->setText(QString("Closed: ???"));
	}

    ui->pointSizeMax->setText(QString::number(ui->openGLWidget->pointSizeMax()));
}

void MainWindow::onBackgroundColorScene()
{
    QColor c = QColorDialog::getColor();
    Color4f color(c.red()/255.0, c.green()/255.0, c.blue()/255.0);
    ui->openGLWidget->changeSceneColor(color);
    ui->openGLWidget->updateGL();
}
