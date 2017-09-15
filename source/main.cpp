#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


// TODO: generer matrice Projection
// TODO: generer matrice View
// TODO: matrice MVP(P*V*M * vertex)
// TODO: envoyer au shader
