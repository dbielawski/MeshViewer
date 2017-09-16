#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

// FIXEME: fixer le bug d'affichage des models
// TODO: implementer classe camera
// TODO: ajouter interaction dans la classe GLWidget
// TODO: nettoyer le code, camera scene ...
// TODO: implementer AABB
// TODO: faire afficher la AABB
// TODO: implementer Octree
// TODO: ajouter la lumiere
