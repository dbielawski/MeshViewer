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

// TODO: exporter les fichiers en OBJ
// TODO: gerer le centrage du model avec la cemera
// TODO: ajouter halfEdge

// TODO: threads pour chargement des models (surtout long PGM3D)
// TODO: implementer Octree
// TODO: afficher octree
