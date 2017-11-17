#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>

#include "utils.h"


class GLWidget;

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
    void onOpenFile();
    void onSaveAsObj();
    void onClearScene();

    void onAlphaChanged(int);
    void onPointSizeChanged(int);

    void onToggleDisplayBoundingBox();
	void onToggleDisplayOctree();
	
	void holeFillingAction();

    void onDrawPoint();
    void onDrawLine();
    void onDrawFilled();

    void onBackgroundColorScene();

private:
    void createMenus();
    void createActions();

	void setDraw(unsigned int);

    void updateInfos() const;

    Ui::MainWindow *ui;

    QMenu*      m_fileMenu;
    QAction*    m_openAction;
    QAction*    m_saveAsObjAction;
    QAction*    m_clearSceneAction;
    QAction*    m_quitAction;

    QMenu*      m_displayMenu;
    QAction*    m_toggleDisplayBoundingBoxAction;
    QAction*    m_toggleDisplayOctreeAction;
    QAction*    m_drawPointsAction;
    QAction*    m_drawLinesAction;
    QAction*    m_drawFilledAction;
	
	QMenu* 		m_actionMenu;
	QAction*	m_holeFillingAction;
};

#endif // MAINWINDOW_H
