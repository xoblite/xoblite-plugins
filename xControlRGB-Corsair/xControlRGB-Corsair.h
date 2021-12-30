/*
 ============================================================================
 xControlRGB-Corsair -> RGB HW integration plugin -> xoblite bb5 and later
 Copyright © 2020 Karl Henrik Henriksson [qwilk/@xoblite]
 http://xoblite.net/ + https://github.com/xoblite
 ============================================================================

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 ============================================================================
*/

#ifndef __XCONTROLRGB_CORSAIR_H
#define __XCONTROLRGB_CORSAIR_H

#include "../BBApi.h"
#include "CUESDK/include/CUESDK.h" // -> Download from https://github.com/CorsairOfficial/cue-sdk
#include <windows.h>
#include <wtsapi32.h>
#include <algorithm>

#define XCONTROLRGB_CORSAIR_STARTUP_DELAY_TIMER 1
#define XCONTROLRGB_CORSAIR_UNLOCK_DELAY_TIMER 2

//===========================================================================

extern "C"
{
    __declspec(dllexport) int beginPlugin(HINSTANCE hMainInstance);
    __declspec(dllexport) int beginPluginEx(HINSTANCE hMainInstance, HWND hBBSlit);
    __declspec(dllexport) void endPlugin(HINSTANCE hMainInstance);
    __declspec(dllexport) LPCSTR pluginInfo(int field);
}

//===========================================================================

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ReadStyleSettings();
bool ReadStyleElementColors(LPCSTR file, LPCSTR element);

void StartCorsairCUE(bool showStartupInfo);
void UpdateCorsairCUE();
void StopCorsairCUE();
void SetCorsairLedColorsFromRGB(CorsairLedColor* clc, COLORREF color);

//===========================================================================

HINSTANCE hInstance;
HWND hwndPlugin, hwndBlackbox;

int msgs[] = {BB_RECONFIGURE, BB_BROADCAST, BB_REDRAWGUI, 0};

COLORREF HWLEDColors[129]; // RGB color array: Dummy 0 + Colours 1-128

bool handshakedWithCorsairCUESDK;

//===========================================================================

#endif /* __XCONTROLRGB_CORSAIR_H */
