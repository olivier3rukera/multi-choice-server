#include "display.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Display multiApp;
    QFile fichier("./MultiChoice/MultiChoiceServer/MultiChoiceServer/styleServer.css");
    fichier.open(QIODevice::ReadOnly);
    QString style(fichier.readAll());
    multiApp.setStyleSheet(style);
    fichier.close();
    multiApp.show();

    return app.exec();
}
