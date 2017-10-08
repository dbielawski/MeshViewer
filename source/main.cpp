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
// WARNING: comment afficher mode lignes ??? oO

// TODO: deux shaders (un pour le model, lautre pour les AABB, Octree..)
// TODO: exporter les fichiers en OBJ
// TODO: gerer le centrage du model avec la cemera
// TODO: implementer Octree
// TODO: afficher octree
// TODO: ajouter halfEdge
// TODO: threads pour chargement des models (surtout long PGM3D)
