#include "Core/Controller.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    FovCalculator::Controller* controller = new FovCalculator::Controller();
    controller->Run();

    return app.exec();
}
