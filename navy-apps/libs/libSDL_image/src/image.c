#define SDL_malloc malloc
#define SDL_free free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"

SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc)
{
    assert(src->type == RW_TYPE_MEM);
    return NULL;
}

SDL_Surface *IMG_Load(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    int size = -1;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    assert(size > 0);
    char *buf = SDL_malloc();
    SDL_Surface *sur_temp = STBIMG_LoadFromMemory(buf, size);
    fclose(fp);
    SDL_free(buf);
    return sur_temp;
}

int IMG_isPNG(SDL_RWops *src)
{
    return 0;
}

SDL_Surface *IMG_LoadJPG_RW(SDL_RWops *src)
{
    return IMG_Load_RW(src, 0);
}

char *IMG_GetError()
{
    return "Navy does not support IMG_GetError()";
}
