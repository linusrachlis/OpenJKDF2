#include "stdDisplay.h"

#include "stdPlatform.h"
#include "jk.h"
#include "Win95/Video.h"

void stdDisplay_SetGammaTable(int len, int *table)
{
    stdDisplay_gammaTableLen = len;
    stdDisplay_paGammaTable = table;
}

#ifdef WIN32

#else
#include <SDL2/SDL.h>

int stdDisplay_Startup()
{
    stdDisplay_bStartup = 1;
    return 1;
}

int stdDisplay_FindClosestDevice(void* a)
{
    Video_dword_866D78 = 0;
    return 0;
}

int stdDisplay_Open(int a)
{
    stdDisplay_pCurDevice = &stdDisplay_aDevices[0];
    stdDisplay_bOpen = 1;
    return 1;
}

void stdDisplay_Close()
{
    stdDisplay_bOpen = 0;
}

int stdDisplay_FindClosestMode(render_pair *a1, struct stdVideoMode *render_surface, unsigned int max_modes)
{
    Video_curMode = 0;
    stdDisplay_bPaged = 1;
    stdDisplay_bModeSet = 1;
    return 0;
}

int stdDisplay_SetMode(unsigned int modeIdx, const void *palette, int paged)
{
    stdDisplay_pCurVideoMode = &Video_renderSurface[modeIdx];
    
    _memcpy(&Video_otherBuf.format, &stdDisplay_pCurVideoMode->format, sizeof(Video_otherBuf.format));
    _memcpy(&Video_menuBuffer.format, &stdDisplay_pCurVideoMode->format, sizeof(Video_menuBuffer.format));
    
    SDL_Surface* otherSurface = SDL_CreateRGBSurface(0, 640, 480, 8,
                                        0,
                                        0,
                                        0,
                                        0);
    SDL_Surface* menuSurface = SDL_CreateRGBSurface(0, 640, 480, 8,
                                        0,
                                        0,
                                        0,
                                        0);
    
    rdColor24* pal24 = palette;
    SDL_Color* tmp = malloc(sizeof(SDL_Color) * 256);
    for (int i = 0; i < 256; i++)
    {
        tmp[i].r = pal24[i].r;
        tmp[i].g = pal24[i].g;
        tmp[i].b = pal24[i].b;
        tmp[i].a = 0xFF;
    }
    
    SDL_SetPaletteColors(otherSurface->format->palette, tmp, 0, 256);
    SDL_SetPaletteColors(menuSurface->format->palette, tmp, 0, 256);
    free(tmp);
    
    //SDL_SetSurfacePalette(otherSurface, palette);
    //SDL_SetSurfacePalette(menuSurface, palette);
    
    Video_otherBuf.sdlSurface = otherSurface;
    Video_menuBuffer.sdlSurface = menuSurface;
    
    Video_menuBuffer.format.width_in_bytes = menuSurface->pitch;
    Video_otherBuf.format.width_in_bytes = otherSurface->pitch;
    
    Video_menuBuffer.format.width_in_pixels = 640;
    Video_otherBuf.format.width_in_pixels = 640;
    Video_menuBuffer.format.width = 640;
    Video_otherBuf.format.width = 640;
    Video_menuBuffer.format.height = 480;
    Video_otherBuf.format.height = 480;
    
    Video_menuBuffer.format.format.bpp = 8;
    Video_otherBuf.format.format.bpp = 8;
    
    return 1;
}

int stdDisplay_ClearRect(stdVBuffer *buf, int fillColor, rdRect *rect)
{
    return 1;
}

int stdDisplay_DDrawGdiSurfaceFlip()
{
    return 1;
}

int stdDisplay_SetMasterPalette(uint8_t* pal)
{
    rdColor24* pal24 = (rdColor24*)pal;
    SDL_Color* tmp = malloc(sizeof(SDL_Color) * 256);
    for (int i = 0; i < 256; i++)
    {
        tmp[i].r = pal24[i].r;
        tmp[i].g = pal24[i].g;
        tmp[i].b = pal24[i].b;
        tmp[i].a = 0xFF;
    }
    
    SDL_SetPaletteColors(Video_otherBuf.sdlSurface->format->palette, tmp, 0, 256);
    SDL_SetPaletteColors(Video_menuBuffer.sdlSurface->format->palette, tmp, 0, 256);
    free(tmp);
    return 1;
}

stdVBuffer* stdDisplay_VBufferNew(stdVBufferTexFmt *fmt, int create_ddraw_surface, int gpu_mem, int is_paletted)
{
    stdVBuffer* out = std_pHS->alloc(sizeof(stdVBuffer));
    
    _memset(out, 0, sizeof(*out));
    
    _memcpy(&out->format, fmt, sizeof(out->format));
    
    // force 0 reads
    //out->format.width = 0;
    //out->format.width_in_bytes = 0;
    //out->surface_lock_alloc = std_pHS->alloc(texture_size_in_bytes);
    
    uint32_t rbitmask = ((1 << fmt->format.r_bits) - 1) << fmt->format.r_shift;
    uint32_t gbitmask = ((1 << fmt->format.g_bits) - 1) << fmt->format.g_shift;
    uint32_t bbitmask = ((1 << fmt->format.b_bits) - 1) << fmt->format.b_shift;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, fmt->width, fmt->height, fmt->format.bpp, rbitmask, gbitmask, bbitmask, 0);
    
    out->format.width_in_bytes = surface->pitch;
    out->format.width_in_pixels = fmt->width;
    
    out->sdlSurface = surface;
    
    return out;
}

int stdDisplay_VBufferLock(stdVBuffer *buf)
{
    if (!buf) return 0;

    SDL_LockSurface(buf->sdlSurface);
    buf->surface_lock_alloc = buf->sdlSurface->pixels;
    return 1;
}

void stdDisplay_VBufferUnlock(stdVBuffer *buf)
{
    if (!buf) return;
    
    buf->surface_lock_alloc = NULL;
    SDL_UnlockSurface(buf->sdlSurface);
}

int stdDisplay_VBufferCopy(stdVBuffer *vbuf, stdVBuffer *vbuf2, unsigned int blit_x, int blit_y, rdRect *rect, int alpha_maybe)
{
    if (!vbuf || !vbuf2) return 1;
    
    rdRect fallback = {0,0,vbuf->format.width, vbuf->format.height};
    if (!rect)
    {
        rect = &fallback;
        memcpy(vbuf->sdlSurface->pixels, vbuf2->sdlSurface->pixels, 640*480);
        return;
    }
    
    if (vbuf->palette)
    {
        rdColor24* pal24 = vbuf->palette;
        SDL_Color* tmp = malloc(sizeof(SDL_Color) * 256);
        for (int i = 0; i < 256; i++)
        {
            tmp[i].r = pal24[i].r;
            tmp[i].g = pal24[i].g;
            tmp[i].b = pal24[i].b;
            tmp[i].a = 0xFF;
        }
    
        SDL_SetPaletteColors(vbuf->sdlSurface->format->palette, tmp, 0, 256);
        free(tmp);
    }
    
    if (vbuf2->palette)
    {
        rdColor24* pal24 = vbuf2->palette;
        SDL_Color* tmp = malloc(sizeof(SDL_Color) * 256);
        for (int i = 0; i < 256; i++)
        {
            tmp[i].r = pal24[i].r;
            tmp[i].g = pal24[i].g;
            tmp[i].b = pal24[i].b;
            tmp[i].a = 0xFF;
        }
        
        SDL_SetPaletteColors(vbuf2->sdlSurface->format->palette, tmp, 0, 256);
        free(tmp);
    }

    SDL_Rect dstRect = {blit_x, blit_y, rect->width, rect->height};
    SDL_Rect srcRect = {rect->x, rect->y, rect->width, rect->height};
    
    uint8_t* srcPixels = vbuf2->sdlSurface->pixels;
    uint8_t* dstPixels = vbuf->sdlSurface->pixels;
    uint32_t srcStride = vbuf2->format.width_in_bytes;
    uint32_t dstStride = vbuf->format.width_in_bytes;
    for (int i = 0; i < rect->width; i++)
    {
        for (int j = 0; j < rect->height; j++)
        {
            uint8_t pixel = srcPixels[(i + srcRect.x) + ((j + srcRect.y)*srcStride)];;
            if (!pixel) continue;

            dstPixels[(i + dstRect.x) + ((j + dstRect.y)*dstStride)] = pixel;
        }
    }
    
    //SDL_BlitSurface(vbuf2->sdlSurface, &srcRect, vbuf->sdlSurface, &dstRect); //TODO error check
    return 1;
}

int stdDisplay_VBufferFill(stdVBuffer *vbuf, int fillColor, rdRect *rect)
{
    SDL_Rect dstRect = {rect->x, rect->y, rect->width, rect->height};
    
    printf("%x; %u %u %u %u\n", fillColor, rect->x, rect->y, rect->width, rect->height);
    SDL_FillRect(vbuf, &dstRect, fillColor); //TODO error check

    return 1;
}

int stdDisplay_VBufferSetColorKey(stdVBuffer *vbuf, int color)
{
    //DDCOLORKEY v3; // [esp+0h] [ebp-8h] BYREF

    if ( vbuf->bSurfaceLocked )
    {
        /*if ( vbuf->bSurfaceLocked == 1 )
        {
            v3.dwColorSpaceLowValue = color;
            v3.dwColorSpaceHighValue = color;
            vbuf->ddraw_surface->lpVtbl->SetColorKey(vbuf->ddraw_surface, 8, &v3);
            return 1;
        }*/
        vbuf->transparent_color = color;
    }
    else
    {
        vbuf->transparent_color = color;
    }
    return 1;
}
#endif