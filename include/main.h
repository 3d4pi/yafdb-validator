#ifndef MAIN_H
#define MAIN_H

/* Application working modes struct */
struct ApplicationMode
{
    enum Type
    {
        /* Start the standard validator */
        Validator = 0,

        /* Start the tiles exporter */
        Exporter = 1,

        /* Start the YML converter */
        YMLConverter = 2
    };
};

#endif // MAIN_H
