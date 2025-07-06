#include <uuid.h>

void uuid_test_create()
{
    uuid_t *uuid;
    uuid_create(&uuid);
}

void uuid_test_make()
{
    uuid_t *uuid;
    uuid_create(&uuid);
    uuid_make(uuid, UUID_MAKE_V1);
}