#ifndef TYPES_H
#define TYPES_H

struct ApplicationMode
{
    enum Type
    {
        Validator = 0, Exporter = 1, YMLConverter = 2
    };
};

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
        None = 0, Valid = 1, Invalid = 2, ToBlur = 3
    };
};

struct ObjectType
{
    enum Type
    {
        None = 0, Face = 1, NumberPlate = 2, ToBlur = 3
    };
};

struct ObjectSubType
{
    enum Type
    {
        None = 0, Front = 1, Profile = 2, Back = 3, Top = 4, Eyes = 5
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

        Manual = 0, Auto = 1, ToBlur = 3
    };
};

struct EditMode
{
    enum Type
    {

        Single = 1, Scene = 2
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
        OnlyPreInvalidated = 7,
        OnlyToBlur = 8
    };
};


#endif // TYPES_H
