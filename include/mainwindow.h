#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "utils.h"

#include "glwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile();
    void clearScene();

private:
    void createMenus();
    void createActions();

    Ui::MainWindow *ui;
    GLWidget* m_viewer;


    QMenu*      m_fileMenu;
    QAction*    m_openAction;
    QAction*    m_clearSceneAction;
    QAction*    m_quitAction;
};

#endif // MAINWINDOW_H
