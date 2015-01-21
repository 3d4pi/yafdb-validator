#include "main.h"

/* Program entry point */
int main(int argc, char *argv[])
{
    /* Main application container */
    QApplication app(argc, argv);

    /* Application version info */
    QCoreApplication::setApplicationName("Yafdb-Validator");
    QCoreApplication::setApplicationVersion("0.1");

    /* Command line parser object */
    QCommandLineParser parser;
    parser.setApplicationDescription("Yafdb-Validator");
    parser.addHelpOption();
    parser.addVersionOption();

    /* Register arguments */
    /* Mode */
    QCommandLineOption modeOption(QStringList() << "m" << "mode",
            QCoreApplication::translate("main", "Application mode"),
            QCoreApplication::translate("main", "validator(default) | exporter | ymlconverter"));
    parser.addOption(modeOption);

    /* Input image */
    QCommandLineOption sourceImagePathOption(QStringList() << "i" << "input-image",
            QCoreApplication::translate("main", "Input image path."),
            QCoreApplication::translate("main", "file path"));
    parser.addOption(sourceImagePathOption);

    /* Detector YML path */
    QCommandLineOption detectorYMLPathOption(QStringList() << "d" << "detector-yml",
            QCoreApplication::translate("main", "Detector YML path."),
            QCoreApplication::translate("main", "file path"));
    parser.addOption(detectorYMLPathOption);

    /* Destination YML path */
    QCommandLineOption destinationYMLPathOption(QStringList() << "o" << "destination-yml",
            QCoreApplication::translate("main", "Destination YML path."),
            QCoreApplication::translate("main", "file path"));
    parser.addOption(destinationYMLPathOption);

    /* Export path */
    QCommandLineOption exportPathOption(QStringList() << "e" << "export-path",
            QCoreApplication::translate("main", "Export path"),
            QCoreApplication::translate("main", "path"));
    parser.addOption(exportPathOption);

    /* Export zoom level */
    QCommandLineOption exportZoomOption(QStringList() << "z" << "export-zoom",
            QCoreApplication::translate("main", "Export zoom level"),
            QCoreApplication::translate("main", "zoomlevel (default 1.0)"));
    parser.addOption(exportZoomOption);

    /* Process given arguments */
    parser.process(app);

    /* Parse application mode in lower case */
    QString mode_name = parser.value(modeOption).toLower();

    /* Default mode */
    int mode = ApplicationMode::Validator;

    /* Check if mode is specified */
    if( mode_name.length() > 0 )
    {
        /* Validator */
        if(mode_name == "validator")
        {
            mode = ApplicationMode::Validator;

        /* Exporter */
        } else if(mode_name == "exporter") {
            mode = ApplicationMode::Exporter;

        /* YMLConverter */
        } else if( mode_name == "ymlconverter" ) {
            mode = ApplicationMode::YMLConverter;

        /* Invalid mode specified */
        } else {
            std::cout << "[ERROR] Invalid mode: " << mode_name.toStdString() << std::endl;
            exit( 0 );
        }
    }

    /* Parse given paths */
    QString sourceImagePath = parser.value(sourceImagePathOption);
    QString detectorYMLPath = parser.value(detectorYMLPathOption);
    QString destinationYMLPath = parser.value(destinationYMLPathOption);
    QString exportPath = parser.value(exportPathOption);

    /* Parse zoom level */
    QString exportZoom = parser.value(exportZoomOption);
    float export_zoom = exportZoom.length() > 0 ? exportZoom.toFloat() : 1.0;

    /* Local arguments validity variable */
    bool argcheck = true;

    /* CHeck source image */
    if( sourceImagePath.length() <= 0 )
    {
        /* Info output */
        std::cout << "Missing source image path." << std::endl;

        /* Assign result */
        argcheck = false;
    }

    /* Check destination YML */
    if( destinationYMLPath.length() <= 0 )
    {
        /* Info output */
        std::cout << "Missing destination YML path." << std::endl;

        /* Assign result */
        argcheck = false;
    }

    /* If some arguments are missing */
    if( !argcheck )
    {
        /* Display help */
        parser.showHelp();

        /* Exit program */
        exit( 0 );
    }

    /* Local switch variables */
    /* Main window */
    MainWindow* w = NULL;

    /* YML Parser */
    YMLParser yml_parser;

    /* Source image infos structure */
    image_info_struct image_info;

    /* Temp image for loading informations */
    IplImage * temp_image = NULL;

    /* Rect list for YML Parser */
    QList<ObjectRect*> loaded_rects;

    /* ID index for exporter */
    int out_id = 1;

    /* Application modes switch */
    switch(mode)
    {

    /* Validator */
    case ApplicationMode::Validator:

        /* Create main validator window */
        w = new MainWindow(0, sourceImagePath, detectorYMLPath, destinationYMLPath);

        /* Show validator window */
        w->show();
        break;

    /* Exporter */
    case ApplicationMode::Exporter:

        /* Check if invalid path is specified */
        if( exportPath.length() <= 0 )
        {
            /* Info output */
            std::cout << "Export path missing." << std::endl;

            /* Display hep message */
            parser.showHelp();

            /* Quit the program */
            exit( 0 );
        }

        /* Info output */
        std::cout << "Reading image..." << std::endl;

        /* Load image using OpenCV to get some additionnal details about the image */
        temp_image = cvLoadImage( sourceImagePath.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED );

        /* Save image details */
        image_info.channels = (temp_image->nChannels + 1);
        image_info.width = temp_image->width;
        image_info.height = temp_image->height;
        image_info.image = IplImage2QImage( temp_image );

        /* Unload temporary image */
        cvReleaseImage( &temp_image );

        /* Load YML */
        loaded_rects = yml_parser.loadYML( destinationYMLPath, YMLType::Validator );

        /* Info output */
        std::cout << "Exporting " << loaded_rects.length() << " images..." << std::endl;

        /* Iterate over loaded rects */
        foreach(ObjectRect* rect, loaded_rects)
        {
            /* Output path container */
            QString path;

            /* Rect type switch */
            switch(rect->getObjectType())
            {

            /* Face */
            case ObjectType::Face:

                /* Rect subtype switch */
                switch(rect->getObjectSubType())
                {

                /* Undefined subtype */
                case ObjectSubType::None:

                    /* Append path */
                    path = (exportPath + "/Face/None/");
                    break;

                /* Front */
                case ObjectSubType::Front:

                    /* Append path */
                    path = (exportPath + "/Face/Front/");
                    break;

                /* Profile */
                case ObjectSubType::Profile:

                    /* Append path */
                    path = (exportPath + "/Face/Profile/");
                    break;

                /* Back */
                case ObjectSubType::Back:

                    /* Append path */
                    path = (exportPath + "/Face/Back/");
                    break;

                /* Top */
                case ObjectSubType::Top:

                    /* Append path */
                    path = (exportPath + "/Face/Top/");
                    break;

                /* Eyes */
                case ObjectSubType::Eyes:

                    /* Append path */
                    path = (exportPath + "/Face/Eyes/");
                    break;
                }
                break;

            /* Number plate */
            case ObjectType::NumberPlate:
                path = (exportPath + "/NumberPlate/");
                break;

            /* "ToBlur" */
            case ObjectType::ToBlur:
                path = (exportPath + "/ToBlur/");
                break;
            }

            /* If manual status is valid */
            if( rect->getManualStatus().toLower() == "valid" )
            {
                /* Append valid path */
                path += "Valid/";

            /* If manual status is invalid */
            } else if ( rect->getManualStatus().toLower() == "invalid" ){

                /* Append invalid path */
                path += "Invalid/";

            /* If rect is not human validated */
            } else {

                /* If automatic status is valid */
                if( rect->getAutomaticStatus().toLower() == "valid" )
                {
                    /* Append specifiec valid path */
                    path += "Valid_Not_Validated/";
                } else {

                    /* Append specifiec valid path */
                    path += "Invalid_Not_Validated/";
                }
            }

            /* Create computed path if not exists */
            if(!QDir( path ).exists())
            {
                QDir().mkpath( path );
            }

            /* Append tile it to path */
            QString outpath = path +
                    QString::number( out_id ) +
                    ".png";

            /* Increment output id until file exists */
            while( QFile( outpath ).exists() )
            {
                out_id++;
                outpath = path + QString::number( out_id ) + ".png";
            }

            /* Export cropped tile */
            exportRect( rect, image_info, outpath, export_zoom);
        }

        /* Info output */
        std::cout << "Done" << std::endl;

        /* Exit the program */
        exit( 0 );
        break;

    /* YML Converter */
    case ApplicationMode::YMLConverter:

        /* Check if invalid path is specified */
        if( detectorYMLPath.length() <= 0 )
        {
            /* Info output */
            std::cout << "Missing detector YML path." << std::endl;

            /* Show help */
            parser.showHelp();

            /* Exit program */
            exit( 0 );
        }

        /* Info output */
        std::cout << "Reading image..." << std::endl;

        /* Load image using OpenCV to get some additionnal details about the image */
        temp_image = cvLoadImage( sourceImagePath.toStdString().c_str(), CV_LOAD_IMAGE_UNCHANGED );

        /* Save image details */
        image_info.channels = (temp_image->nChannels + 1);
        image_info.width = temp_image->width;
        image_info.height = temp_image->height;
        image_info.image = IplImage2QImage( temp_image );

        /* Unload temporary image */
        cvReleaseImage( &temp_image );

        /* Load rects from YML */
        loaded_rects = yml_parser.loadYML( detectorYMLPath, YMLType::Detector );

        /* Info output */
        std::cout << "Converting points..." << std::endl;

        /* Iterate over loaded rects */
        foreach(ObjectRect* rect, loaded_rects)
        {
            /* Convert spherical coordinates to local gnomonic */
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

        /* Info output */
        std::cout << "Writing YML." << std::endl;

        /* Write converted items to YML */
        yml_parser.writeYML( loaded_rects, destinationYMLPath );

        /* Info output */
        std::cout << "Done." << std::endl;

        /* Exit program */
        exit( 0 );

        break;
    }

    /* Wait until app finishes */
    return app.exec();
}
