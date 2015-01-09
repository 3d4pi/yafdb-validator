#include "mainwindow.h"
#include "batchview.h"
#include "types.h"
#include "ymlparser.h"
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

    QCommandLineOption modeOption(QStringList() << "m" << "mode",
            QCoreApplication::translate("main", "Application mode"),
            QCoreApplication::translate("main", "validator(default) | exporter"));
    parser.addOption(modeOption);

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

    QCommandLineOption exportPathOption(QStringList() << "e" << "export-path",
            QCoreApplication::translate("main", "Export path"),
            QCoreApplication::translate("main", "path"));
    parser.addOption(exportPathOption);

    QCommandLineOption exportZoomOption(QStringList() << "z" << "export-zoom",
            QCoreApplication::translate("main", "Export zoom level"),
            QCoreApplication::translate("main", "zoomlevel (default 1.0)"));
    parser.addOption(exportZoomOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    QString mode_name = parser.value(modeOption).toLower();

    int mode = ApplicationMode::Validator;

    if( mode_name.length() > 0 )
    {
        if(mode_name == "validator")
        {
            mode = ApplicationMode::Validator;
        } else if(mode_name == "exporter") {
            mode = ApplicationMode::Exporter;
        } else {
            std::cout << "[ERROR] Invalid mode: " << mode_name.toStdString() << std::endl;
            exit( 0 );
        }
    }

    QString sourceImagePath = parser.value(sourceImagePathOption);
    QString detectorYMLPath = parser.value(detectorYMLPathOption);
    QString destinationYMLPath = parser.value(destinationYMLPathOption);
    QString exportZoom = parser.value(exportZoomOption);
    QString exportPath = parser.value(exportPathOption);

    float export_zoom = exportZoom.length() > 0 ? exportZoom.toFloat() : 1.0;

    if( sourceImagePath.length() <= 0 || destinationYMLPath.length() <= 0 )
    {
        parser.showHelp();
        exit( 0 );
    }

    MainWindow* w = NULL;

    switch(mode)
    {
    case ApplicationMode::Validator:
        w = new MainWindow(0, sourceImagePath, detectorYMLPath, destinationYMLPath);
        w->show();
        break;
    case ApplicationMode::Exporter:

        if( exportPath.length() <= 0 )
        {
            std::cout << "Export path missing." << std::endl;
            exit( 0 );
        }

        std::cout << "Reading image..." << std::endl;

        IplImage * temp_image = NULL;
        temp_image = cvLoadImage( sourceImagePath.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED );

        image_info_struct image_info;
        image_info.channels = (temp_image->nChannels + 1);
        image_info.width = temp_image->width;
        image_info.height = temp_image->height;
        image_info.image = IplImage2QImage( temp_image );

        YMLParser parser;

        QList<ObjectRect*> loaded_rects = parser.loadYML( destinationYMLPath, YMLType::Validator );

        std::cout << "Exporting " << loaded_rects.length() << " images..." << std::endl;

        foreach(ObjectRect* rect, loaded_rects)
        {
            QString path;

            switch(rect->getType())
            {
            case ObjectType::Face:
                switch(rect->getSubType())
                {
                case ObjectSubType::Front:
                    path = (exportPath + "/Face/Front/");
                    break;
                case ObjectSubType::Profile:
                    path = (exportPath + "/Face/Profile/");
                    break;
                case ObjectSubType::None:
                    path = (exportPath + "/Face/");
                    break;
                }
                break;
            case ObjectType::NumberPlate:
                path = (exportPath + "/NumberPlate/");
                break;
            case ObjectType::ToBlur:
                path = (exportPath + "/ToBlur/");
                break;
            }

            if(!QDir( path ).exists())
            {
                QDir().mkpath( path );
            }

            exportRect( rect, image_info, path +
                        QString::number(rect->getId()) +
                        ".png",
                        export_zoom);
        }

        std::cout << "Done" << std::endl;
        exit( 0 );

    }

    return app.exec();
}
