#include <NDL.h>
#include <SDL.h>
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
    "NONE",
    _KEYS(keyname)};

int SDL_PushEvent(SDL_Event *ev)
{
    return 0;
}

int SDL_PollEvent(SDL_Event *ev)
{
    char buf[64];
    if (NDL_PollEvent(buf, sizeof(buf)))
    {
        switch (buf[1])
        {
        case 'd':
            ev->type = SDL_KEYDOWN;
            assert(false);
            break;
        case 'u':
            ev->type = SDL_KEYUP;
            break;
        default:
            assert(false);
        }
    }
    return 0;
}

int SDL_WaitEvent(SDL_Event *event)
{
    return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask)
{
    return 0;
}

uint8_t *SDL_GetKeyState(int *numkeys)
{
    return NULL;
}
