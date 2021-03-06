/*
 * Copyright (c) 2010 Toni Spets <toni.spets@iki.fi>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef MAIN_H 
#define MAIN_H

#include <windows.h>
#include "ddraw.h"
#include "debug.h"

#define CUTSCENE_WIDTH 640
#define CUTSCENE_HEIGHT 400

#define WM_AUTORENDERER WM_USER+111
#define WM_WINEFULLSCREEN WM_USER+112

extern char OpenglVersion[];

struct IDirectDrawImpl;
struct IDirectDrawImplVtbl;
struct IDirectDrawSurfaceImpl;

extern struct IDirectDrawImpl *ddraw;

typedef struct IDirectDrawImpl
{
    struct IDirectDrawImplVtbl *lpVtbl;

    ULONG Ref;

    DWORD width;
    DWORD height;
    DWORD bpp;
    BOOL windowed;
    BOOL border;
    BOOL boxing;
    BOOL windowed_init;
    DEVMODE mode;
    struct IDirectDrawSurfaceImpl *primary;
    char title[128];
    HMODULE real_dll;

    /* real export from system32\ddraw.dll */
    HRESULT (WINAPI *DirectDrawCreate)(GUID FAR*, LPDIRECTDRAW FAR*, IUnknown FAR*);
    CRITICAL_SECTION cs;

    struct
    {
        int maxfps;
        int width;
        int height;
        int bpp;

        HDC hDC;
        int *tex;

        HANDLE thread;
        BOOL run;
        HANDLE ev;
        HANDLE sem;
        DEVMODE mode;
        struct { int width; int height; int x; int y; } viewport;

        LONG paletteUpdated;
        LONG surfaceUpdated;
    } render;

    HWND hWnd;
    LRESULT (CALLBACK *WndProc)(HWND, UINT, WPARAM, LPARAM);
    struct { float x; float y; } cursor;
    BOOL locked;
    BOOL adjmouse;
    BOOL devmode;
    BOOL vsync;
    BOOL vhack;
	BOOL isredalert;
	BOOL iscnc1;
    LONG incutscene;
    DWORD (WINAPI *renderer)(void);
    char screenshotKey;
    BOOL fullscreen;
    BOOL maintas;
    BOOL fakecursorpos;
    BOOL noactivateapp;
    char shader[MAX_PATH];
    BOOL autorenderer;
    BOOL wine;
    int sleep;
    
} IDirectDrawImpl;

typedef struct IDirectDrawImplVtbl IDirectDrawImplVtbl;

struct IDirectDrawImplVtbl
{
    HRESULT(__stdcall *QueryInterface) (IDirectDrawImpl *, const IID* const riid, LPVOID * ppvObj);
    ULONG(__stdcall *AddRef) (IDirectDrawImpl *);
    ULONG(__stdcall *Release) (IDirectDrawImpl *);

    HRESULT(__stdcall *Compact)(IDirectDrawImpl *);
    HRESULT(__stdcall *CreateClipper)(IDirectDrawImpl *, DWORD, LPDIRECTDRAWCLIPPER *, IUnknown *);
    HRESULT(__stdcall *CreatePalette)(IDirectDrawImpl *, DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE *, IUnknown *);
    HRESULT(__stdcall *CreateSurface)(IDirectDrawImpl *, LPDDSURFACEDESC, LPDIRECTDRAWSURFACE *, IUnknown *);
    HRESULT(__stdcall *DuplicateSurface)( IDirectDrawImpl *, LPDIRECTDRAWSURFACE, LPDIRECTDRAWSURFACE *);
    HRESULT(__stdcall *EnumDisplayModes)( IDirectDrawImpl *, DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMMODESCALLBACK);
    HRESULT(__stdcall *EnumSurfaces)(IDirectDrawImpl *, DWORD, LPDDSURFACEDESC, LPVOID,LPDDENUMSURFACESCALLBACK);
    HRESULT(__stdcall *FlipToGDISurface)(IDirectDrawImpl *);
    HRESULT(__stdcall *GetCaps)(IDirectDrawImpl *, LPDDCAPS, LPDDCAPS);
    HRESULT(__stdcall *GetDisplayMode)(IDirectDrawImpl *, LPDDSURFACEDESC);
    HRESULT(__stdcall *GetFourCCCodes)(IDirectDrawImpl *, LPDWORD, LPDWORD);
    HRESULT(__stdcall *GetGDISurface)(IDirectDrawImpl *, LPDIRECTDRAWSURFACE *);
    HRESULT(__stdcall *GetMonitorFrequency)(IDirectDrawImpl *, LPDWORD);
    HRESULT(__stdcall *GetScanLine)(IDirectDrawImpl *, LPDWORD);
    HRESULT(__stdcall *GetVerticalBlankStatus)(IDirectDrawImpl *, LPBOOL);
    HRESULT(__stdcall *Initialize)(IDirectDrawImpl *, GUID *);
    HRESULT(__stdcall *RestoreDisplayMode)(IDirectDrawImpl *);
    HRESULT(__stdcall *SetCooperativeLevel)(IDirectDrawImpl *, HWND, DWORD);
    HRESULT(__stdcall *SetDisplayMode)(IDirectDrawImpl *, DWORD, DWORD,DWORD);
    HRESULT(__stdcall *WaitForVerticalBlank)(IDirectDrawImpl *, DWORD, HANDLE);
};

typedef enum PROCESS_DPI_AWARENESS {
    PROCESS_DPI_UNAWARE = 0,
    PROCESS_SYSTEM_DPI_AWARE = 1,
    PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

#endif
