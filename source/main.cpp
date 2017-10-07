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

// FIXEME: rotation de la camera sur l'axe des Y
// FIXEME: normales ??
// WARNING: comment afficher mode lignes ??? oO

// TODO: Tester les obj avec des normales
// TODO: deux shaders (un simple avec lumiere, lautre sans lumiere..)
// TOOD: slider pour la taille des points mode points..
// TODO: implementer Octree
// TODO: afficher octree
// TODO: ajouter halfEdge
// TODO: threads pour chargement des models (surtout long PGM3D)
