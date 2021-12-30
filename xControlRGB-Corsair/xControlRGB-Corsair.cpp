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

 xControlRGB-Corsair 1.0 (2020-10-10) / qwilk
	- Original release

 ============================================================================
*/

#include "xControlRGB-Corsair.h"

LPSTR szAppName = "xControlRGB-Corsair";		// The name of our window class, etc.
LPSTR szVersion = "xControlRGB-Corsair 1.0";	// Used in MessageBox titlebars

LPSTR szInfoVersion = "1.0";
LPSTR szInfoAuthor = "qwilk/@xoblite";
LPSTR szInfoRelDate = "2020-10-10";
LPSTR szInfoLink = "http://xoblite.net/";
LPSTR szInfoEmail = "qwilk@xoblite.net";
//LPSTR szInfoUpdateURL = "http://xoblite.net/plugins.txt";

bool debugLogging = false; // Enable additional logging to the xoblite console?

//===========================================================================

int beginPlugin(HINSTANCE hPluginInstance)
{
	WNDCLASS wc;
	hwndBlackbox = GetBBWnd();
	hInstance = hPluginInstance;
	handshakedWithCorsairCUESDK = false;

	//====================

	// Register plugin window class...
	ZeroMemory(&wc,sizeof(wc));
	wc.lpfnWndProc = WndProc;			// Plugin window procedure
	wc.hInstance = hPluginInstance;		// hInstance of .dll
	wc.lpszClassName = szAppName;		// Plugin window class name
	if (!RegisterClass(&wc)) 
	{
		MessageBox(NULL, "Error registering window class!", szVersion, MB_OK | MB_ICONERROR | MB_TOPMOST);
		return 1;
	}

	//====================

	// Create plugin window...
	hwndPlugin = CreateWindowEx(
						WS_EX_TOOLWINDOW,								// window style
						szAppName,										// window class name
						NULL,											// NULL -> does not show up in task manager!
						WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,	// window parameters
						0,												// x position
						0,												// y position
						0,												// window width
						0,												// window height
						NULL,											// parent window
						NULL,											// no menu
						hPluginInstance,								// hInstance of .dll
						NULL);
	if (!hwndPlugin)
	{						   
		MessageBox(0, "Error creating window!", szVersion, MB_OK | MB_ICONERROR | MB_TOPMOST);
		UnregisterClass(szAppName, hPluginInstance);
		return 1;
	}

	//====================

	// Register to receive applicable Blackbox messages...
	SendMessage(hwndBlackbox, BB_REGISTERMESSAGE, (WPARAM)hwndPlugin, (LPARAM)msgs);

	// Register to receive session notification (in our case, unlock screen) messages...
	WTSRegisterSessionNotification(hwndPlugin, NOTIFY_FOR_THIS_SESSION);

	// Delay further plugin startup very briefly to allow the core to finish loading all other plugins before
	// e.g. sending early messages to the console... (cf. e.g. # devices found, any "debug" SDK info on startup, etc)
	if (!SetTimer(hwndPlugin, XCONTROLRGB_CORSAIR_STARTUP_DELAY_TIMER, 200, (TIMERPROC)NULL))
	{
		// ...but if we for some reason can't create that delay timer, we start immediately...
		StartCorsairCUE(true);
		UpdateCorsairCUE();
	}

	return 0;
}

//====================

int beginPluginEx(HINSTANCE hPluginInstance, HWND hwndBBDock)
{
	return beginPlugin(hPluginInstance);
}

//===========================================================================

void endPlugin(HINSTANCE hPluginInstance)
{
	// Kill timers... (nb. typically long since killed upon endPlugin(), but just in case)
	KillTimer(hwndPlugin, XCONTROLRGB_CORSAIR_STARTUP_DELAY_TIMER);
	KillTimer(hwndPlugin, XCONTROLRGB_CORSAIR_UNLOCK_DELAY_TIMER);

	StopCorsairCUE();

	// Unregister session notification messages...
	WTSUnRegisterSessionNotification(hwndPlugin);
	// Unregister Blackbox messages...
	SendMessage(hwndBlackbox, BB_UNREGISTERMESSAGE, (WPARAM)hwndPlugin, (LPARAM)msgs);
	// Destroy window...
	DestroyWindow(hwndPlugin);
	// Unregister window class...
	UnregisterClass(szAppName, hPluginInstance);
}

//===========================================================================

LPCSTR pluginInfo(int field)
{
	switch (field)
	{
		case PLUGIN_NAME:
			return szAppName;
		case PLUGIN_VERSION:
			return szInfoVersion;
		case PLUGIN_AUTHOR:
			return szInfoAuthor;
		case PLUGIN_RELEASEDATE:
			return szInfoRelDate;
		case PLUGIN_LINK:
			return szInfoLink;
		case PLUGIN_EMAIL:
			return szInfoEmail;
		case PLUGIN_BROAMS:
		{
			// Note: The bro@ms will appear in the *reverse* order
			// compared to the below in the core's bro@ms submenu.
			return
			"@xControlRGB-Corsair GetCUESDK"
			"@xControlRGB-Corsair GetiCUE"
			"@xControlRGB-Corsair Readme"
			"@xControlRGB-Corsair About";
		}
//		case PLUGIN_UPDATE_URL:
//			return szInfoUpdateURL;

		//====================

		default:
			return szVersion;
	}
}

//===========================================================================

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//====================

		case WM_TIMER:
		{
			if (wParam == XCONTROLRGB_CORSAIR_STARTUP_DELAY_TIMER)
			{
				KillTimer(hwndPlugin, XCONTROLRGB_CORSAIR_STARTUP_DELAY_TIMER);
				StartCorsairCUE(true);
				UpdateCorsairCUE();
			}
			else if (wParam == XCONTROLRGB_CORSAIR_UNLOCK_DELAY_TIMER)
			{
				StartCorsairCUE(false);
				if (handshakedWithCorsairCUESDK)
				{
					KillTimer(hwndPlugin, XCONTROLRGB_CORSAIR_UNLOCK_DELAY_TIMER);
					UpdateCorsairCUE();
				}
			}
			return 0;
		}
		break;

		//====================

		case BB_RECONFIGURE:
		case BB_REDRAWGUI:
		{
			UpdateCorsairCUE();
		}
		break;

		//====================
/*
		case WM_POWERBROADCAST:
		{
			if (wParam == PBT_APMRESUMEAUTOMATIC)
			{
				if (debugLogging) SendMessage(GetBBWnd(), BB_CONSOLEMESSAGE, (WPARAM)CONSOLE_REGULAR_MESSAGE, (LPARAM)"xControlRGB-Corsair -> Resuming from low power state; initiating new handshake with the CUE SDK.");
				SetTimer(hwndPlugin, XCONTROLRGB_CORSAIR_UNLOCK_DELAY_TIMER, 2000, (TIMERPROC)NULL);
			}
			return TRUE;
		}
*/
		case WM_WTSSESSION_CHANGE:
		{
			if (wParam == WTS_SESSION_UNLOCK)
			{
//				if (debugLogging) SendMessage(GetBBWnd(), BB_CONSOLEMESSAGE, (WPARAM)CONSOLE_REGULAR_MESSAGE, (LPARAM)"xControlRGB-Corsair -> Screen unlocked; initiating new handshake with the CUE SDK.");
				SetTimer(hwndPlugin, XCONTROLRGB_CORSAIR_UNLOCK_DELAY_TIMER, 2000, (TIMERPROC)NULL);
			}
		}
		break;

		//====================

		// Broadcast messages (bro@ms)
		case BB_BROADCAST:
		{
			char temp[MAX_LINE_LENGTH];
			strcpy(temp, (LPCSTR)lParam);

			if (IsInString(temp, "@xControlRGB-Corsair"))
			{
				char token1[MAX_LINE_LENGTH], token2[MAX_LINE_LENGTH], extra[MAX_LINE_LENGTH];
				LPSTR tokens[2];
				tokens[0] = token1;
				tokens[1] = token2;

				token1[0] = token2[0] = extra[0] = '\0';
				BBTokenize (temp, tokens, 2, extra);

				//====================

				if (!_stricmp(token2, "Readme"))
				{
					char path[MAX_LINE_LENGTH], directory[MAX_LINE_LENGTH];
					int nLen;

					// The readme file should be located in the same folder as the plugin...
					GetModuleFileName(hInstance, path, sizeof(path));
					nLen = strlen(path) - 1;
					while ((nLen > 0) && (path[nLen] != '\\')) nLen--;
					path[nLen+1] = 0;
					strcpy(directory, path);
					strcat(path, "xControlRGB-Corsair.txt");
					if (FileExists(path)) BBExecute(GetDesktopWindow(), NULL, path, "", directory, SW_SHOWNORMAL, true);
					else BBExecute(GetDesktopWindow(), NULL, "http://xoblite.net/docs/plugins/", "", "", SW_SHOWNORMAL, true);
				}

				//====================

				else if (!_stricmp(token2, "GetiCUE"))
				{
					BBExecute(GetDesktopWindow(), NULL, "https://www.corsair.com/icue", "", "", SW_SHOWNORMAL, true);
				}
				else if (!_stricmp(token2, "GetCUESDK"))
				{
					BBExecute(GetDesktopWindow(), NULL, "https://github.com/CorsairOfficial/cue-sdk", "", "", SW_SHOWNORMAL, true);
				}

				//====================

				else if (!_stricmp(token2, "About"))
				{
					char about[MAX_LINE_LENGTH], shell[MAX_PATH];
					sprintf(about, "%s (%s)   \n\n© 2020 Karl Henrik Henriksson [qwilk/@xoblite]   \n\nqwilk@xoblite.net   \nhttp://xoblite.net/   \nhttps://github.com/xoblite/   ", szVersion, szInfoRelDate);
					strcpy(shell, GetBBVersion());
					if (IsInString(shell, "bb")) SendMessage(GetBBWnd(), BB_POPUPMESSAGE, (WPARAM)"About this plugin...", (LPARAM)about); // xoblite
					else MessageBox(0, about, "About this plugin...", MB_OK | MB_ICONINFORMATION); // bb4win or other branch
					break;
				}

				//====================
			}
		}
		break;

		//====================

		case WM_CLOSE:
			return 0;

		//====================

		default:
			return DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}

//===========================================================================

void ReadStyleSettings()
{
	// Check if the parent shell is xoblite or bb4win
	// -> "fetch from core buffer" if loaded by xoblite
	// -> "fetch from file" if loaded by bb4win or other branch
	char file[MAX_PATH];
	strcpy(file, GetBBVersion());
	if (IsInString(file, "bb")) strcpy(file, ""); // xoblite
	else strcpy(file, stylePath()); // bb4win or other branch

	// Fill the RGB colors array with up to 8 colors from the current style...
	// (nb. at least for now using one style element's colors only,
	// in order to avoid a potentially too "busy" look across the HW LEDs [?])
	if (ReadStyleElementColors(file, "toolbar.windowLabel")) return;
	else if (ReadStyleElementColors(file, "toolbar.clock")) return;
	else if (ReadStyleElementColors(file, "toolbar.label")) return;
	else if (ReadStyleElementColors(file, "menu.grip")) return;
	else if (ReadStyleElementColors(file, "menu.active")) return;
	else if (ReadStyleElementColors(file, "menu.hilite")) return;
	else if (ReadStyleElementColors(file, "menu.title")) return;

	else if (ReadStyleElementColors(file, "toolbar")) return;
	else if (ReadStyleElementColors(file, "menu.frame")) return;

	else ZeroMemory(HWLEDColors, sizeof(HWLEDColors)); // Failsafe -> Set all LEDs to 0x000000 -> All LEDs off...
}

//===========================================================================

bool ReadStyleElementColors(LPCSTR file, LPCSTR element)
{
	char param[MAX_LINE_LENGTH], temp[MAX_LINE_LENGTH];
	sprintf(param, "%s.appearance:", element); // First let's try modern style syntax...
	strcpy(temp, ReadString(file, param, ""));
	if (!strlen(temp))
	{
		sprintf(param, "%s:", element); // ...and then classic style syntax as fallback...
		strcpy(temp, ReadString(file, param, ""));
	}

	if (!strlen(temp) || IsInString(temp, "parentrelative")) return false;
	else
	{
		char msg[MAX_LINE_LENGTH];
		if (debugLogging) sprintf(msg, "xControlRGB-Corsair -> Reading color values from the %s style settings -> ", element);

		if (IsInString(temp, "Solid") && !IsInString(temp, "Split"))
		{
			if (debugLogging) strcat(msg, "1-color solid detected.");

			sprintf(param, "%s.color:", element);
			HWLEDColors[1] = ReadColor(file, param, "");

			HWLEDColors[2] = HWLEDColors[3] = HWLEDColors[4] = HWLEDColors[5] = HWLEDColors[6] = HWLEDColors[7] = HWLEDColors[8] = HWLEDColors[1];
			HWLEDColors[9] = HWLEDColors[10] = HWLEDColors[11] = HWLEDColors[12] = HWLEDColors[13] = HWLEDColors[14] = HWLEDColors[15] = HWLEDColors[16] = HWLEDColors[1];
			HWLEDColors[17] = HWLEDColors[18] = HWLEDColors[19] = HWLEDColors[20] = HWLEDColors[21] = HWLEDColors[22] = HWLEDColors[23] = HWLEDColors[24] = HWLEDColors[1];
			HWLEDColors[25] = HWLEDColors[26] = HWLEDColors[27] = HWLEDColors[28] = HWLEDColors[29] = HWLEDColors[30] = HWLEDColors[31] = HWLEDColors[32] = HWLEDColors[1];
		}
		else if (IsInString(temp, "Split") && !IsInString(temp, "Solid"))
		{
			if (debugLogging) strcat(msg, "4-color split gradient detected.");

			sprintf(param, "%s.color:", element); // Note: Using "color" rather than "color1" here for improved compatibility with older *box branches (xoblite will automatically check both syntaxes)
			HWLEDColors[1] = ReadColor(file, param, "");
			sprintf(param, "%s.colorTo:", element); // Note: Using "colorTo" rather than "color2" here for improved compatibility with older *box branches (xoblite will automatically check both syntaxes)
			HWLEDColors[2] = ReadColor(file, param, "");
			sprintf(param, "%s.color3:", element);
			HWLEDColors[3] = ReadColor(file, param, "");
			sprintf(param, "%s.color4:", element);
			HWLEDColors[4] = ReadColor(file, param, "");

			HWLEDColors[10] = HWLEDColors[16] = HWLEDColors[22] = HWLEDColors[28] = HWLEDColors[4];
			HWLEDColors[5] = HWLEDColors[9] = HWLEDColors[11] = HWLEDColors[15] = HWLEDColors[17] = HWLEDColors[21] = HWLEDColors[23] = HWLEDColors[27] = HWLEDColors[29] = HWLEDColors[3];
			HWLEDColors[6] = HWLEDColors[8] = HWLEDColors[12] = HWLEDColors[14] = HWLEDColors[18] = HWLEDColors[20] = HWLEDColors[24] = HWLEDColors[26] = HWLEDColors[30] = HWLEDColors[32] = HWLEDColors[2];
			HWLEDColors[7] = HWLEDColors[13] = HWLEDColors[19] = HWLEDColors[25] = HWLEDColors[31] = HWLEDColors[1];
		}
		else if (IsInString(temp, "Super"))
		{
			if (debugLogging) strcat(msg, "8-color super gradient detected.");

			sprintf(param, "%s.color1:", element);
			HWLEDColors[1] = ReadColor(file, param, "");
			sprintf(param, "%s.color2:", element);
			HWLEDColors[2] = ReadColor(file, param, "");
			sprintf(param, "%s.color3:", element);
			HWLEDColors[3] = ReadColor(file, param, "");
			sprintf(param, "%s.color4:", element);
			HWLEDColors[4] = ReadColor(file, param, "");
			sprintf(param, "%s.color5:", element);
			HWLEDColors[5] = ReadColor(file, param, "");
			sprintf(param, "%s.color6:", element);
			HWLEDColors[6] = ReadColor(file, param, "");
			sprintf(param, "%s.color7:", element);
			HWLEDColors[7] = ReadColor(file, param, "");
			sprintf(param, "%s.color8:", element);
			HWLEDColors[8] = ReadColor(file, param, "");

			HWLEDColors[22] = HWLEDColors[8];
			HWLEDColors[9] = HWLEDColors[21] = HWLEDColors[23] = HWLEDColors[7];
			HWLEDColors[10] = HWLEDColors[20] = HWLEDColors[24] = HWLEDColors[6];
			HWLEDColors[11] = HWLEDColors[19] = HWLEDColors[25] = HWLEDColors[5];
			HWLEDColors[12] = HWLEDColors[18] = HWLEDColors[26] = HWLEDColors[32] = HWLEDColors[4];
			HWLEDColors[13] = HWLEDColors[17] = HWLEDColors[27] = HWLEDColors[31] = HWLEDColors[3];
			HWLEDColors[14] = HWLEDColors[16] = HWLEDColors[28] = HWLEDColors[30] = HWLEDColors[2];
			HWLEDColors[15] = HWLEDColors[29] = HWLEDColors[1];
		}
		else
		{
			if (debugLogging)
			{
				if (IsInString(temp, "SplitSolid")) strcat(msg, "2-color split solid detected.");
				else strcat(msg, "2-color regular gradient detected.");
			}

			sprintf(param, "%s.color:", element); // Note: Using "color" rather than "color1" here for improved compatibility with older *box branches (xoblite will automatically check both syntaxes)
			HWLEDColors[1] = ReadColor(file, param, "");
			sprintf(param, "%s.colorTo:", element); // Note: Using "colorTo" rather than "color2" here for improved compatibility with older *box branches (xoblite will automatically check both syntaxes)
			HWLEDColors[2] = ReadColor(file, param, "");

			HWLEDColors[3] = HWLEDColors[5] = HWLEDColors[7] = HWLEDColors[1];
			HWLEDColors[4] = HWLEDColors[6] = HWLEDColors[8] = HWLEDColors[2];
			HWLEDColors[9] = HWLEDColors[11] = HWLEDColors[13] = HWLEDColors[15] = HWLEDColors[1];
			HWLEDColors[10] = HWLEDColors[12] = HWLEDColors[14] = HWLEDColors[16] = HWLEDColors[2];
			HWLEDColors[17] = HWLEDColors[19] = HWLEDColors[21] = HWLEDColors[23] = HWLEDColors[1];
			HWLEDColors[18] = HWLEDColors[20] = HWLEDColors[22] = HWLEDColors[24] = HWLEDColors[2];
			HWLEDColors[25] = HWLEDColors[27] = HWLEDColors[29] = HWLEDColors[31] = HWLEDColors[1];
			HWLEDColors[26] = HWLEDColors[28] = HWLEDColors[30] = HWLEDColors[32] = HWLEDColors[2];
		}

		if (debugLogging) SendMessage(GetBBWnd(), BB_CONSOLEMESSAGE, (WPARAM)CONSOLE_REGULAR_MESSAGE, (LPARAM)msg);

		return true;
	}
}

//===========================================================================

void StartCorsairCUE(bool showStartupInfo)
{
	CorsairProtocolDetails details = CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError())
	{
		// -> Handshake failed! :(
		handshakedWithCorsairCUESDK = false;

		if (showStartupInfo) SendMessage(GetBBWnd(), BB_CONSOLEMESSAGE, (WPARAM)CONSOLE_REGULAR_MESSAGE, (LPARAM)"xControlRGB-Corsair -> Error: Initial handshake with the CUE SDK failed. Has iCUE started yet? (will try again at the next style change)");
		return;
	}
	else
	{
		// -> Handshake successful! :D
		handshakedWithCorsairCUESDK = true;

		if (showStartupInfo && debugLogging)
		{
			char msg[MAX_LINE_LENGTH];
			sprintf(msg, "xControlRGB-Corsair -> Handshake with the CUE SDK successful. Running on top of SDK version %s (protocol v%d), CUE version %s (protocol v%d).", details.sdkVersion, details.sdkProtocolVersion, details.serverVersion, details.serverProtocolVersion);
			SendMessage(GetBBWnd(), BB_CONSOLEMESSAGE, (WPARAM)CONSOLE_REGULAR_MESSAGE, (LPARAM)msg);
		}
	}

	//====================

//	CorsairRequestControl(CAM_ExclusiveLightingControl);
//	CorsairSetLayerPriority(200);

	//====================

	if (showStartupInfo)
	{
		char msg[MAX_LINE_LENGTH];

		int numberOfDevices = CorsairGetDeviceCount();

		if (numberOfDevices > 0)
		{
			strcpy(msg, "xControlRGB-Corsair -> ");

			for (int i = 1; i <= numberOfDevices; i++)
			{
				CorsairDeviceInfo* cdi = CorsairGetDeviceInfo(i-1);

				strcat(msg, cdi->model);

				switch (cdi->type)
				{
					case CDT_Keyboard: { strcat(msg, " keyboard "); break; }
					case CDT_Mouse: { strcat(msg, " mouse "); break; }
					case CDT_Headset: { strcat(msg, " headset "); break; }
					case CDT_MouseMat: { strcat(msg, " mouse pad "); break; }
					case CDT_HeadsetStand: { strcat(msg, " headset stand "); break; }
					case CDT_CommanderPro: { strcat(msg, " DIY device "); break; }
					case CDT_LightingNodePro: { strcat(msg, " DIY device "); break; }
					case CDT_MemoryModule: { strcat(msg, " memory module "); break; }
					case CDT_Cooler: { strcat(msg, " cooler "); break; }
					default: { strcat(msg, " unknown device "); break; }
				}

				char numberofLEDs[50];
				sprintf(numberofLEDs, "(%d LED", cdi->ledsCount);
				if (cdi->ledsCount != 1) strcat(numberofLEDs, "s)");
				else strcat(numberofLEDs, ")");
				strcat(msg, numberofLEDs);

				if (numberOfDevices > 1)
				{
					if (i == (numberOfDevices-1)) strcat(msg, " and ");
					else if (i != numberOfDevices) strcat(msg, ", ");
				}
			}
			strcat(msg, " found.");

			SendMessage(GetBBWnd(), BB_CONSOLEMESSAGE, (WPARAM)CONSOLE_REGULAR_MESSAGE, (LPARAM)msg);
		}
		else SendMessage(GetBBWnd(), BB_CONSOLEMESSAGE, (WPARAM)CONSOLE_ERROR_MESSAGE, (LPARAM)"xControlRGB-Corsair -> No devices found!");
	}
}

//===========================================================================

void UpdateCorsairCUE()
{
	if (!handshakedWithCorsairCUESDK)
	{
		StartCorsairCUE(false);
		if (!handshakedWithCorsairCUESDK) return;
	}

	//====================

	int numberOfDevices = CorsairGetDeviceCount();
	if (numberOfDevices == 0) return;

	ReadStyleSettings();

	//====================

	for (int i = 0; i < numberOfDevices; i++)
	{
		CorsairLedPositions* clp = CorsairGetLedPositionsByDeviceIndex(i);
		std::sort(clp->pLedPosition, clp->pLedPosition + clp->numberOfLed, [](const CorsairLedPosition& a, const CorsairLedPosition& b) { return a.ledId < b.ledId;	});

		for (int n = 0; n < clp->numberOfLed; n++)
		{
			CorsairLedColor ledColors = { clp->pLedPosition[n].ledId };
			SetCorsairLedColorsFromRGB(&ledColors, HWLEDColors[n+1]);
			CorsairSetLedsColorsBufferByDeviceIndex(i, 1, &ledColors);
		}
	}

	CorsairSetLedsColorsFlushBuffer();
}

//===========================================================================

void StopCorsairCUE()
{
//	CorsairReleaseControl(CAM_ExclusiveLightingControl);
}

//===========================================================================

void SetCorsairLedColorsFromRGB(CorsairLedColor* clc, COLORREF color)
{
	if (color != NULL)
	{
		clc->r = GetRValue(color);
		clc->g = GetGValue(color);
		clc->b = GetBValue(color);
	}
	else clc->r = clc->g = clc->b = 0x00; // Failsafe
}

//===========================================================================
