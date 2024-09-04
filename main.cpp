#include "poleis-autoupdater.h"
#include <QtCore>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication *app = new QApplication(argc, argv);

    PoleisAutoUpdater *updater = new PoleisAutoUpdater();
    updater->Update();

    return app->exec();
}