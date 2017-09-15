#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QFileDialog>
#include <QLayout>

/*  */
#include "pgm3d.h"
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
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), tr("*.pgm3d"));

    // Tester s'il est vide, alors on arrete

    // Charger le fichier
    // Load Model
}

void MainWindow::clearScene()
{
    // delete model
}
