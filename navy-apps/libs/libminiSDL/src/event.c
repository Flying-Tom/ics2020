#include <NDL.h>
#include <SDL.h>
#include <string.h>
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
    /*
    char buf[256], keynamebuf[32];
    uint8_t keytype = 0;
    if (NDL_PollEvent(buf, sizeof(buf)))
    {
        sscanf(buf + 3, "%s", keynamebuf);
        keytype = (buf[1] == 'd') ? SDL_KEYDOWN : SDL_KEYUP;
        for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++)
        {
            if (strcmp(keyname[i], keynamebuf) == 0)
            {
                event->type = keytype;
                event->key.type = keytype;
                event->key.keysym.sym = i;
                printf("%s\n", keynamebuf);
                break;
            }
        }
        return 1;
    }*/
    return 0;
}

int SDL_WaitEvent(SDL_Event *event)
{
    /*
    char buf[256], keynamebuf[32];
    uint8_t keytype = 0;
    NDL_PollEvent(buf, sizeof(buf));
    sscanf(buf + 3, "%s", keynamebuf);
    keytype = (buf[1] == 'd') ? SDL_KEYDOWN : SDL_KEYUP;
    for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++)
    {
        if (strcmp(keyname[i], keynamebuf) == 0)
        {
            event->type = keytype;
            event->key.type = keytype;
            event->key.keysym.sym = i;
            printf("%s\n", keynamebuf);
            break;
        }
    }
    return 1;*/
    char buf[256], keynamebuf[32];
    uint8_t keytype = 0;
    if (NDL_PollEvent(buf, sizeof(buf)))
    {
        sscanf(buf + 3, "%s", keynamebuf);
        keytype = (buf[1] == 'd') ? SDL_KEYDOWN : SDL_KEYUP;
        for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++)
        {
            if (strcmp(keyname[i], keynamebuf) == 0)
            {
                event->type = keytype;
                event->key.type = keytype;
                event->key.keysym.sym = i;
                printf("%s\n", keynamebuf);
                break;
            }
        }
        return 1;
    }
    return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask)
{
    return 0;
}

uint8_t *SDL_GetKeyState(int *numkeys)
{
    return NULL;
}
