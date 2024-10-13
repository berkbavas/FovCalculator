#include "Core/Controller.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    int id = QFontDatabase::addApplicationFont(":/Resources/Fonts/Segoe/Segoe-UI-Variable-Static-Text.ttf");
    if (id == -1)
    {
        QFont font = qApp->font();
        font.setPixelSize(12);
        font.setBold(false);
        qApp->setFont(font);
        qDebug() << "Font cannot be loaded. Using default font:" << qApp->font();
    }
    else
    {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont font(family);
        font.setPixelSize(12);
        qApp->setFont(font);
        qDebug() << "Font is loaded. Using" << qApp->font();
    }

    FovCalculator::Controller controller;
    controller.Run();

    return app.exec();
}
