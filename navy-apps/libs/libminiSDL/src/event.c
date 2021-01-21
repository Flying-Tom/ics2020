#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {"NONE", _KEYS(keyname)};

#define keynum sizeof(keyname) / sizeof(keyname[0])
static uint8_t KeyState[keynum] = {};

int SDL_PushEvent(SDL_Event *ev)
{
    assert(0);
    return 0;
}

int SDL_PollEvent(SDL_Event *ev)
{
    char buf[256];
    if (NDL_PollEvent(buf, sizeof(buf)))
    {
        ev->type = (buf[1] == 'd') ? SDL_KEYDOWN : SDL_KEYUP;
        ev->key.type = ev->type;
        for (int i = 0; i < keynum; i++)
        {
            if (strcmp(keyname[i], buf + 3) == 0)
            {
                ev->key.keysym.sym = i;
                KeyState[ev->key.keysym.sym] = ev->type;
                return 1;
            }
        }
    }

    return 0;
}

int SDL_WaitEvent(SDL_Event *event)
{
    while (SDL_PollEvent(event) == 0)
        ;
    return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask)
{
    assert(0);
    return 0;
}

uint8_t *SDL_GetKeyState(int *numkeys)
{
    //*numkeys = keynum;
    assert(numkeys == NULL);
    return KeyState;
}
