#ifndef TYPES_H
#define TYPES_H

struct ObjectItemRectType
{
    enum Type
    {
       Valid = 1, Invalid = 2, Manual = 3
    };
};

struct ObjectRectType
{
    enum Type
    {
       Valid = 1, Invalid = 2, Manual = 3
    };
};

struct ObjectRectState
{
    enum Type
    {
       None = 0, Valid = 1, Invalid = 2
    };
};

struct ObjectType
{
    enum Type
    {
       None = 0, Face = 1, NumberPlate = 2, ToBlur = 3
    };
};

struct ObjectValidState
{
    enum Type
    {
       Valid = 1, Invalid = 2, None = 3
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
        Clear = 0,
        All = 1,
        OnlyUntyped = 2,
        OnlyFaces = 3,
        OnlyUnapprovedFaces = 4,
        OnlyNumberPlates = 5,
        OnlyUnapprovedNumberPlates = 6,
        OnlyPreInvalidated = 7
    };
};


#endif // TYPES_H
