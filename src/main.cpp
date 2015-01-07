#include "mainwindow.h"
#include "batchview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("Yafdb-Validator");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Yafdb-Validator");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption sourceImagePathOption(QStringList() << "i" << "input-image",
            QCoreApplication::translate("main", "Input image path."),
            QCoreApplication::translate("main", "file path"));
    parser.addOption(sourceImagePathOption);

    QCommandLineOption detectorYMLPathOption(QStringList() << "d" << "detector-yml",
            QCoreApplication::translate("main", "Detector YML path."),
            QCoreApplication::translate("main", "file path"));
    parser.addOption(detectorYMLPathOption);

    QCommandLineOption destinationYMLPathOption(QStringList() << "o" << "destination-yml",
            QCoreApplication::translate("main", "Destination YML path."),
            QCoreApplication::translate("main", "file path"));
    parser.addOption(destinationYMLPathOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    QString sourceImagePath = parser.value(sourceImagePathOption);
    QString detectorYMLPath = parser.value(detectorYMLPathOption);
    QString destinationYMLPath = parser.value(destinationYMLPathOption);

    if( sourceImagePath.length() <= 0 || destinationYMLPath.length() <= 0 )
    {
        parser.showHelp();
        exit( 0 );
    }

    MainWindow w(0, sourceImagePath, detectorYMLPath, destinationYMLPath);
    w.show();

    return app.exec();
}
