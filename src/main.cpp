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
            QCoreApplication::translate("main", "validator(default) | exporter | ymlconverter"));
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
        } else if( mode_name == "ymlconverter" ) {
            mode = ApplicationMode::YMLConverter;
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

    bool argcheck = true;

    if( sourceImagePath.length() <= 0 )
    {
        std::cout << "Missing source image path." << std::endl;
        argcheck = false;
    }

    if( destinationYMLPath.length() <= 0 )
    {
        std::cout << "Missing destination YML path." << std::endl;
        argcheck = false;
    }

    if( !argcheck )
    {
        parser.showHelp();
        exit( 0 );
    }

    MainWindow* w = NULL;

    YMLParser yml_parser;
    image_info_struct image_info;
    IplImage * temp_image = NULL;
    QList<ObjectRect*> loaded_rects;
    int out_id = 1;

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
            parser.showHelp();
            exit( 0 );
        }

        // Read source image
        std::cout << "Reading image..." << std::endl;

        temp_image = cvLoadImage( sourceImagePath.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED );

        image_info.channels = (temp_image->nChannels + 1);
        image_info.width = temp_image->width;
        image_info.height = temp_image->height;
        image_info.image = IplImage2QImage( temp_image );

        loaded_rects = yml_parser.loadYML( destinationYMLPath, YMLType::Validator );

        std::cout << "Exporting " << loaded_rects.length() << " images..." << std::endl;

        foreach(ObjectRect* rect, loaded_rects)
        {
            QString path;

            switch(rect->getType())
            {
            case ObjectType::Face:
                switch(rect->getSubType())
                {
                case ObjectSubType::None:
                    path = (exportPath + "/Face/None/");
                    break;
                case ObjectSubType::Front:
                    path = (exportPath + "/Face/Front/");
                    break;
                case ObjectSubType::Profile:
                    path = (exportPath + "/Face/Profile/");
                    break;
                case ObjectSubType::Back:
                    path = (exportPath + "/Face/Back/");
                    break;
                case ObjectSubType::Eyes:
                    path = (exportPath + "/Face/Eyes/");
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

            if( rect->getManualStatus().toLower() == "valid" )
            {
                path += "Valid/";
            } else if ( rect->getManualStatus().toLower() == "invalid" ){
                path += "Invalid/";
            } else {
                if( rect->getAutomaticStatus().toLower() == "valid" )
                {
                    path += "Valid_Not_Validated/";
                } else {
                    path += "Invalid_Not_Validated/";
                }
            }

            if(!QDir( path ).exists())
            {
                QDir().mkpath( path );
            }

            QString outpath = path +
                    QString::number( out_id ) +
                    ".png";

            while( QFile( outpath ).exists() )
            {
                out_id++;
                outpath = path + QString::number( out_id ) + ".png";
            }

            exportRect( rect, image_info, outpath, export_zoom);
        }

        std::cout << "Done" << std::endl;
        exit( 0 );
        break;
    case ApplicationMode::YMLConverter:

        if( detectorYMLPath.length() <= 0 )
        {
            std::cout << "Missing detector YML path." << std::endl;

            parser.showHelp();
            exit( 0 );
        }

        // Read source image
        std::cout << "Reading image..." << std::endl;

        temp_image = cvLoadImage( sourceImagePath.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED );

        image_info.channels = (temp_image->nChannels + 1);
        image_info.width = temp_image->width;
        image_info.height = temp_image->height;
        image_info.image = IplImage2QImage( temp_image );

        loaded_rects = yml_parser.loadYML( detectorYMLPath, YMLType::Detector );


        std::cout << "Converting points..." << std::endl;

        foreach(ObjectRect* rect, loaded_rects)
        {
            rect->mapFromSpherical(image_info.width,
                                   image_info.height,
                                   1920 / 2,
                                   1080 / 2,
                                   0.0,
                                   0.0,
                                   20.0 * (LG_PI / 180.0),
                                   20.0 * (LG_PI / 180.0),
                                   120.0 * (LG_PI / 180.0));
        }

        std::cout << "Writing YML." << std::endl;
        yml_parser.writeYML( loaded_rects, destinationYMLPath );

        std::cout << "Done." << std::endl;
        exit( 0 );

        break;
    }

    return app.exec();
}
