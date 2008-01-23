#include <QApplication>
#include "MainWindow.h"

int main( int argz , char* argv[])
{
    QApplication app( argz , argv );
    MainWindow Window;
    Window.show();
    return app.exec();
}
