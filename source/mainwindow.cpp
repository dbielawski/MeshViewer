#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>
#include <QLayout>

/*  */
#include "pgm3d.h"
#include "obj.h"
/*  */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_viewer = new GLWidget;

    ui->centralWidget->layout()->addWidget(m_viewer);

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
}

void MainWindow::createActions()
{
    m_openAction = new QAction(tr("&Open"), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open an existing file"));
    connect(m_openAction, SIGNAL(triggered(bool)), this, SLOT(openFile()));

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

    m_drawLinesAction = new QAction(tr("&Lines"), this);
    m_drawLinesAction->setStatusTip(tr("Render the model in line mode | Press M to switch"));

    m_drawFilledAction = new QAction(tr("&Filled"), this);
    m_drawFilledAction->setStatusTip(tr("Render the model with faces | Press M to switch"));
}

void MainWindow::openFile()
{
    // TODO: filtrer par type de fichier
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), tr("*.pgm3d"));

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"),
                                                    "../models/",
                                                    tr("OBJ Files (*.obj);; PGM3D Files (*.pgm3d)"));

    if(fileName.isEmpty())
        return;

    QFileInfo fi(fileName);
    QString ext = fi.suffix();

    Model3d* model;
    if(ext.toStdString() == "pgm3d")
        model = new pgm3d(fileName);
    else if (ext.toStdString() == "pgm3d")
        model = new obj(fileName);

    Mesh* mesh = model->mesh();
    m_viewer->scene()->addMesh(*mesh);
    m_viewer->updateGL();
}

void MainWindow::clearScene()
{
    // delete model
}
