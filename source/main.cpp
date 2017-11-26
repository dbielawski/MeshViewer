#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - w.width()) / 2;
    int y = (screenGeometry.height() - w.height()) / 2;
    w.move(x, y);

    w.show();

    return a.exec();
}

// TODO: Corriger la rotation
// TODO: ear clipping
// TODO: Epaississement basique (stefka) + sauvegarde du nouveau maillage epaissie
// TODO: Rapport (4 pages) diapo (validity ???)
