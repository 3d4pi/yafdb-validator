## yafdb-validate<br />Yafdb validation tool.

>Yafdb validation tool

### Description
This tool is used to validate objects detected by Yafdb on panoramas, and also create new objects on it.

### Dependencies

#### Details

1. [Qt 5](http://www.qt.io)
2. [OpenCV](http://opencv.org)

#### Installation

    sudo apt-get install qt5-default libopencv-dev

### Compilation

    cd yafdb-validate # cd to source directory
    mkdir build && cd build # Create build directory to build app in it
    qmake .. # Generate Makefile
    make -j4 # Compile program


### Usage
    Usage: ./yafdb-validate [options]
    Yafdb-Validator

    Options:
    -h, --help                                                 Displays this
    help.
    -v, --version                                              Displays version
    information.
    -m, --mode <validator(default) | exporter | ymlconverter>  Application mode
    -i, --input-image <file path>                              Input image path.
    -d, --detector-yml <file path>                             Detector YML path.
    -o, --destination-yml <file path>                          Destination YML
    path.
    -e, --export-path <path>                                   Export path
    -z, --export-zoom <zoomlevel (default 1.0)>                Export zoom level


### Example usage scenarios
    ./yafdb-validate -i data/footage/results/result_1403185221_724762.jpeg -d data/footage/results/blurring/yml_configs/result_1403185221_724762.yml -o data/footage/results/blurring/yml_configs/result_1403185221_724762_validated.yml


### Copyright

Copyright (c) 2014-2015 FOXEL SA - [http://foxel.ch](http://foxel.ch)<br />
This program is part of the FOXEL project <[http://foxel.ch](http://foxel.ch)>.

Please read the [COPYRIGHT.md](COPYRIGHT.md) file for more information.


### License

This program is licensed under the terms of the
[GNU Affero General Public License v3](http://www.gnu.org/licenses/agpl.html)
(GNU AGPL), with two additional terms. The content is licensed under the terms
of the
[Creative Commons Attribution-ShareAlike 4.0 International](http://creativecommons.org/licenses/by-sa/4.0/)
(CC BY-SA) license.

Please read <[http://foxel.ch/license](http://foxel.ch/license)> for more
information.
