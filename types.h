#ifndef TYPES_H
#define TYPES_H

struct ObjectType
{
    enum Type
    {
       Face = 1, NumberPlate = 2, BlurOnly = 3
    };
};

struct RectType
{
    enum Type
    {
        None = 0, Valid = 1, Invalid = 2, Manual = 3
    };
};

struct RectMoveType
{
    enum Type
    {
        All = 1, Only_Point1 = 2, Only_Point2 = 3
    };
};

#endif // TYPES_H
