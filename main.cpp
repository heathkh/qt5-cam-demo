#include "cameracapture.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CameraCapture cameraCapture;
    cameraCapture.show();

    return app.exec();
};
