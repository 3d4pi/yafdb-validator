#ifndef TYPES_H
#define TYPES_H

struct ObjectType
{
    enum Type
    {
       None = 0, Face = 1, NumberPlate = 2, BlurOnly = 3
    };
};

struct RectType
{
    enum Type
    {
       Manual = 1, Auto = 2
    };
};

struct RectMoveType
{
    enum Type
    {
        All = 1, Only_Point1 = 2, Only_Point2 = 3
    };
};

struct BatchMode
{
    enum Type
    {

        Manual = 0, Auto = 1
    };
};

struct BatchViewMode
{
    enum Type
    {

        Clear = 0, All = 1, OnlyUntyped = 2, OnlyUnapprovedFaces = 3
    };
};


#endif // TYPES_H
