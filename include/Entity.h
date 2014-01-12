#ifndef ENTITY_H
#define ENTITY_H
#include "Common.h"

typedef struct
{
    int x;
    int y;
    char character;
}Ent_player;

union Ent_data
{
    Ent_player player;
};

enum Ent_type
{
    ENT_UNKNOWN = 0,
    ENT_PLAYER
};

typedef struct
{
    Ent_type ent_type;
    Ent_data data;

} Ent;


class Entity
{
    public:
        Entity();
        virtual Ent toEnt()
        {
            log(LG_DEBUG, "Entity::toEnt");
            Ent tmp;
            tmp.ent_type = ENT_UNKNOWN;
            return tmp;
        }
        virtual ~Entity();
    protected:
    private:
};

#endif // ENTITY_H
