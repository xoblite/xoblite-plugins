/*
 ============================================================================
 xoblite™ -> an advanced "extended shell" for Microsoft® Windows® 10/11
 Copyright © 2002-2025 Karl Henrik Henriksson [qwilk/@xoblite]
 Copyright © 2001-2004 The Blackbox for Windows Development Team
 http://xoblite.net/ + https://github.com/xoblite/
 ============================================================================

  Blackbox for Windows is free software, released under the
  GNU General Public License (GPL version 2 or later), with an extension
  that allows linking of proprietary modules under a controlled interface.
  What this means is that plugins etc. are allowed to be released
  under any license the author wishes. Please note, however, that the
  original Blackbox gradient math code used in Blackbox for Windows
  is available under the BSD license.

  http://www.fsf.org/licenses/gpl.html
  http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface
  http://www.xfree86.org/3.3.6/COPYRIGHT2.html#5

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  For additional license information, please read the included license.html.

 ============================================================================
*/

#ifndef __BBAPI_H_
#define __BBAPI_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define WINVER 0x0601				// Note: Assuming minimum Windows 7 here, even though I have only tested xoblite bb5 on Windows 10... 
#define _WIN32_WINNT 0x0601			// ...

#define WIN32_LEAN_AND_MEAN			// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN				// Exclude rarely-used stuff from Windows headers (more aggressive)
#define NOCRYPT						// Exclude encryption stuff
#define NOSERVICE					// Exclude service stuff
#define NOIME						// ...etc...
#define NOMCX
#define NO_INTSHCUT_GUIDS
#define NO_SHDOCVW_GUIDS

#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1	// Enable automatic template overloads of standard CRT functions with more secure variants (e.g. strcpy -> strcpy_s, sprintf -> sprintf_s, etc)

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)	// Removes compiler dependencies
#endif

#include <windows.h>
#include <winnt.h>
#include <stdio.h>

//===========================================================================
//===========================================================================
//===========================================================================

#define MAX_LINE_LENGTH 4096	// Remark: Have any of the "bbCLeanModZero's" later redefined this to 1024???

//====================

// BImage: ParentRelative (i.e. no gradient drawn)
#define B_PARENTRELATIVE -1

// BImage: 2-colour regular gradients and 1-colour regular solid
#define B_HORIZONTAL 0
#define B_VERTICAL 1
#define B_DIAGONAL 2
#define B_CROSSDIAGONAL 3
#define B_PIPECROSS 4
#define B_ELLIPTIC 5
#define B_RECTANGLE 6
#define B_PYRAMID 7
#define B_SOLID 8

// Non-BImage: Previously loaded mage to be drawn instead of a *box gradient (experimental in xoblite bb5 RC6 == subject to change)
#define B_IMAGEFROMFILE 99

// BImage: 4-colour split gradients (introduced in xoblite bb4)
#define B_SPLITHORIZONTAL B_HORIZONTAL+100
#define B_SPLITVERTICAL B_VERTICAL+100
#define B_SPLITSOLID B_SOLID+100 // ...this one is 2-colour obviously :)

// BImage: 8-colour super gradients (introduced in xoblite bb5)
#define B_SUPERHORIZONTAL B_HORIZONTAL+150 // Preliminary - Request For Comments
#define B_SUPERVERTICAL B_VERTICAL+150 // Preliminary - Request For Comments

// BImage: 2-colour mirrored gradients (introduced in xoblite bb4)
#define B_MIRRORHORIZONTAL B_HORIZONTAL+200
#define B_MIRRORVERTICAL B_VERTICAL+200

// Other gradients (nb. not currently supported by xoblite, nor all other branches, i.e. included here for reference only)
#define B_WAVEHORIZONTAL B_HORIZONTAL+300
#define B_WAVEVERTICAL B_VERTICAL+300
#define B_BLOCKHORIZONTAL B_HORIZONTAL+400
#define B_BLOCKVERTICAL B_VERTICAL+400

// BImage: Bevel types+positions
#define BEVEL_FLAT 0
#define BEVEL_RAISED 1
#define BEVEL_SUNKEN 2
#define BEVEL1 1
#define BEVEL2 2

// BImage: Glyph types
#define GLYPH_MENU_EMPTY 0
#define GLYPH_MENU_SQUARE 1

#define GLYPH_WINDOW_SHADE 101
#define GLYPH_WINDOW_PIN 102
#define GLYPH_WINDOW_ALWAYSONTOP 103
#define GLYPH_WINDOW_MINIMIZE 104
#define GLYPH_WINDOW_MAXIMIZE 105
#define GLYPH_WINDOW_CLOSE 106

//====================

// Rounded corners (introduced in xoblite bb4 RCs, more broadly used in xoblite bb5)
#define CORNER_TOPLEFT (1<<0)
#define CORNER_TOPRIGHT (1<<1)
#define CORNER_BOTTOMLEFT (1<<2)
#define CORNER_BOTTOMRIGHT (1<<3)

// Metric selectors (used by GetMetricsEvolved())
#define METRIC_BORDER (1<<0)
#define METRIC_MARGIN (1<<1)
#define METRIC_BEVEL (1<<2)
//#define METRIC_FONT (1<<3)

//====================

// Blackbox messages

//#define BB_MSGFIRST			10000	// (definition reserved by bbLean?)

#define BB_REGISTERMESSAGE		10001
#define BB_UNREGISTERMESSAGE	10002

#define BB_QUIT					10101
#define BB_RESTART				10102
#define BB_RECONFIGURE			10103
#define BB_SETSTYLE				10104
//#define BB_EXITTYPE			10105	// (definition reserved by bb4win)
//#define BB_TOOLBARUPDATE		10106	// (definition reserved by bb4win)
#define BB_SETTHEME				10107
//#define BB_REINDEX			10108	// (definition reserved by bbCLeanModZero?)
//#define BB_ABORTINDEX			10109	// (definition reserved by bbCLeanModZero?)

#define BB_EDITFILE				10201	// Param3: 0=CurrentStyle 1=menu.rc, 2=plugins.rc, 3=extensions.rc, 4=blackbox.rc
//#define BB_EXECUTE			10202	// (definition reserved by bb4win)
//#define BB_ABOUTSTYLE			10203	// (definition reserved by bb4win)
//#define BB_ABOUTPLUGINS		10204	// (definition reserved by bb4win)

#define BB_MENU					10301	// Param3: 0=Main menu, 1=Workspaces menu, 2=Toolbar menu, 3=Systembar menu (no longer used -> fallback to toolbar menu), 4=Dock menu
#define BB_HIDEMENU				10302
//#define BB_TOGGLETRAY			10303	// This message has been renamed, use BB_TOGGLESYSTEMBAR instead!
#define BB_TOGGLESYSTEMBAR		10303	// Replaces BB_TOGGLETRAY
#define BB_SETTOOLBARLABEL		10304	// Used to set the toolbar label (returns to normal after 2 seconds)
#define BB_CONSOLEMESSAGE		10304	// Console introduced in xoblite bb4 RCs; re-using BB_SETTOOLBARLABEL definition since the console replaces the toolbar messages
#define BB_POPUPMESSAGE			20608	// ### PRELIMINARY xoblite bb5 ### (...using BB_CONSOLEMESSAGE times two for now... ;) )
#define BB_TOGGLEPLUGINS		10305
#define BB_SUBMENU				10306
#define BB_TOGGLESLIT			10307	// Legacy...
#define BB_TOGGLEDOCK			10307	// ...xoblite bb5
#define BB_TOGGLETOOLBAR		10308
#define BB_AUTOHIDE             10309	// (definition reserved by bbCLeanModZero?)
#define BB_TOGGLECONSOLE		10310	// PRELIMINARY (not yet finally checked for overlap/compatibility with other *box branches)

#define BB_SHUTDOWN				10401	// Param3: 0=Shutdown, 1=Reboot, 2=Logoff, 3=Hibernate, 4=Standby (Suspend), 5=LockWorkstation
#define BB_RUN					10402
//#define BB_COMMAND			10403	// (definition reserved by bb4win)

#define BB_DESKTOPINFO			10501
#define BB_LISTDESKTOPS			10502
#define BB_SWITCHTON			10503
#define BB_BRINGTOFRONT			10504
#define BB_WORKSPACE			10505	// Param3: 0=DeskLeft, 1=DeskRight, 2=AddDesktop, 3=DelDesktop, 4=DeskSwitchToN,
										//         5=GatherWindows 6=MoveWindowLeft, 7=MoveWindowRight
										// Note for reference: bb4win and/or bbCLeanModZero later adding WPARAM values 8=PrevWindow, 9=NextWindow, 10=LastDesk, [...] 20=MinimizeAll, 21=RestoreAll, [...], all the way up to 31.

#define BB_TASKSUPDATE			10506	// (definition reserved by bb4win)
#define BB_TRAYUPDATE			10507
//#define BB_CLEANTRAY			10508	// (definition reserved by bb4win)
//#define BB_CLEANTASKS			10509	// (definition reserved by bb4win)
#define BB_DRAGTODESKTOP		10510
//#define BB_SENDWINDOWTON		10511	// (definition reserved by bbCLeanModZero?)
//#define BB_MOVEWINDOWTON		10512	// (definition reserved by bbCLeanModZero?)

#define BB_ADDTASK				10601	// Shell hook messages... (nb. these are primarily for core internal task handling; task handling plugins should subscribe to BB_TASKSUPDATE etc instead)
#define BB_REMOVETASK			10602
#define BB_ACTIVATESHELLWINDOW	10603
#define BB_ACTIVETASK			10604
#define BB_MINMAXTASK			10605
#define BB_REDRAW				10610

#define BB_WINDOWSHADE			10606	// Window handling messages...
#define BB_WINDOWGROWHEIGHT		10607
#define BB_WINDOWGROWWIDTH		10608
#define BB_WINDOWLOWER			10609
//#define BB_MINIMIZE			10611	// This message has been renamed, use BB_WINDOWMINIMIZE instead!
#define BB_WINDOWMINIMIZE		10611
#define BB_WINDOWRAISE			10612
//#define BB_WINDOWMAXIMIZE		10613	// (definition reserved by bb4win)
//#define BB_WINDOWRESTORE		10614	// (definition reserved by bb4win)
//#define BB_WINDOWCLOSE		10615	// (definition reserved by bb4win)
//#define BB_WINDOWSIZE			10616	// (definition reserved by bbLean)
//#define BB_WINDOWMOVE			10617	// (definition reserved by bbLean)
//#define BB_WINDOWMINIMIZETOTRAY 10618	// (definition reserved by bbCLeanModZero?)

//#define BB_BBSB_NOTIFY		10882	// (definition reserved by bb4win)

//#define BB_GETBOOL			10870	// (definition reserved by bbLean?)

#define BB_REDRAWGUI			10881	// (definition reserved by bbLean for bbStyleMaker) -> Re-used, for now at least?, for xoblite's Designer mode... (which shouldn't trigger a classic full reconfigure on single style param changes etc, only a redraw of the specified element(s))
#define BBRG_TOOLBAR (1<<0)				// wParam bitflags for BB_REDRAWGUI
#define BBRG_MENU    (1<<1)				// ...
#define BBRG_WINDOW  (1<<2)
#define BBRG_DESK    (1<<3)
#define BBRG_DESKTOP BBRG_DESK
//#define BBRG_FOCUS   (1<<4)
//#define BBRG_PRESSED (1<<5)
//#define BBRG_STICKY  (1<<6)
//#define BBRG_FOLDER  (1<<7)
#define BBRG_SLIT    (1<<8)
#define BBRG_DOCK BBRG_SLIT
#define BBRG_HARDWARE (1<<16)

//#define BB_EXECUTEASYNC		10882	// (definition reserved by bbLean?)
//#define BB_DESKCLICK			10884	// (definition reserved by bbLean? -> Desktop clicked, lParam: 0=leftdown 1=left, 2=right, 3=mid, 4=x1, 5=x2, 6=x3 ?)
//#define BB_WINKEY				10886	// (definition reserved by bbLean?)
//#define BB_SENDDATA			10890	// (definition reserved by bbLean for bbStyleMaker?)
//#define BB_GETSTYLE			10891	// (definition reserved by bbLean for bbStyleMaker?)
//#define BB_GETSTYLESTRUCT		10892	// (definition reserved by bbLean for bbStyleMaker?)
//#define BB_SETSTYLESTRUCT		10893	// (definition reserved by bbLean for bbStyleMaker?)
//#define BB_FOLDERCHANGED		10897	// (definition reserved by bbLean?)
//#define BB_DRAGOVER			10898	// (definition reserved by bbLean?)
//#define BB_POSTSTRING			10899	// (definition reserved by bbLean?)

#define BB_BROADCAST			10901	// Broadcast messages (bro@m -> the bang killah! :D <vbg>)

#define BB_TRAYICONMESSAGE		10999	// System messages targeting xoblite's own tray icon

//====================

#define DOCK_ADD				11001	// Dock (nb. formerly known as the "slit") plugin docking messages
#define DOCK_REMOVE				11002
#define DOCK_UPDATE				11003

#define SLIT_ADD DOCK_ADD
#define SLIT_REMOVE DOCK_REMOVE
#define SLIT_UPDATE DOCK_UPDATE

//====================

//#define BB_MSGLAST			12000	// (definition reserved by bbLean?)

//===========================================================================

// pluginInfo
#define PLUGIN_NAME				1
#define PLUGIN_VERSION			2
#define PLUGIN_AUTHOR			3
#define PLUGIN_RELEASEDATE		4
#define PLUGIN_LINK				5
#define PLUGIN_EMAIL			6
#define PLUGIN_BROAMS			7
#define PLUGIN_UPDATE_URL		8

#define PLUGIN_RELEASE PLUGIN_RELEASEDATE // This message has been renamed, please use PLUGIN_RELEASEDATE instead...

//====================

// Support for external system tray plugins (used as LPARAM for BB_TRAYUPDATE)
#define TRAYICON_ADDED 0
#define TRAYICON_MODIFIED 1
#define TRAYICON_REMOVED 2
#define TRAYICON_REFRESH 3

// Support for external task handling plugins (used as LPARAM for BB_TASKSUPDATE)
#define TASKITEM_ADDED 0
#define TASKITEM_MODIFIED 1
#define TASKITEM_ACTIVATED 2
#define TASKITEM_REMOVED 3
#define TASKITEM_REFRESH 4
#define TASKITEM_FLASHED 5
//#define TASKITEM_LANGUAGE 6 // (definition reserved by bbCLeanModZero for Win9x support?)

//====================

// Console message types (originally introduced in xoblite bb4 RCs, extended and more broadly used in xoblite bb5)
#define CONSOLE_REGULAR_MESSAGE 0		// Timestamp + message
#define CONSOLE_INFORMATION_MESSAGE 1	// Information icon + message
#define CONSOLE_WARNING_MESSAGE 2		// Warning icon + message
#define CONSOLE_ERROR_MESSAGE 3			// Error icon + message
#define CONSOLE_SHELL_MESSAGE 4			// Shell icon + message
#define CONSOLE_PLAIN_MESSAGE 253		// Message only (no timestamp, no icon)
#define CONSOLE_INDENTED_MESSAGE 254	// Message only, indented (no timestamp, no icon)
#define CONSOLE_SEPARATOR 255			// Separator

//===========================================================================

// Constants for GetSettingPtr(int index)
enum
{
	 SN_TOOLBAR = 1             // StyleItem*
	,SN_TOOLBARBUTTON           // StyleItem*
	,SN_TOOLBARBUTTONP          // StyleItem*
	,SN_TOOLBARLABEL            // StyleItem*
	,SN_TOOLBARWINDOWLABEL      // StyleItem*
	,SN_TOOLBARCLOCK            // StyleItem*
	,SN_MENUTITLE               // StyleItem*
	,SN_MENUFRAME               // StyleItem*
	,SN_MENUHILITE              // StyleItem*

	,SN_MENUBULLET              // char*
	,SN_MENUBULLETPOS           // char*

	,SN_BORDERWIDTH             // int*
	,SN_BORDERCOLOR             // int*
	,SN_BEVELWIDTH              // int*
	,SN_FRAMEWIDTH              // int*
	,SN_HANDLEWIDTH             // int*

	,SN_ROOTCOMMAND             // char*

	,SN_MENUALPHA               // int*
	,SN_TOOLBARALPHA            // int*

	,SN_METRICSUNIX             // bool*
	,SN_BULLETUNIX              // bool*

	//===============================================
	// For compatibility with bbLeanSkin/bbIconBox...
	// ########### WORK IN PROGRESS / TBD ###########
	//===============================================

    ,SN_WINFOCUS_TITLE			// StyleItem*
    ,SN_WINFOCUS_LABEL			// StyleItem*
    ,SN_WINFOCUS_HANDLE			// StyleItem*
    ,SN_WINFOCUS_GRIP			// StyleItem*
    ,SN_WINFOCUS_BUTTON			// StyleItem*
    ,SN_WINFOCUS_BUTTONP		// StyleItem*

    ,SN_WINUNFOCUS_TITLE		// StyleItem*
    ,SN_WINUNFOCUS_LABEL		// StyleItem*
    ,SN_WINUNFOCUS_HANDLE		// StyleItem*
    ,SN_WINUNFOCUS_GRIP			// StyleItem*
    ,SN_WINUNFOCUS_BUTTON		// StyleItem*

    ,SN_WINFOCUS_FRAME_COLOR	// COLORREF*
    ,SN_WINUNFOCUS_FRAME_COLOR	// COLORREF*

    ,SN_NEWMETRICS				// bool (not a pointer)

	//===============================================
	// + Stuff added later by bb/C/Lean/Zero/Mod...?
	// ########### WORK IN PROGRESS / TBD ###########
	//===============================================

	,SN_ISSTYLE070				// bool*
	,SN_SLIT					// StyleItem*

	,SN_MENUSEPMARGIN			// int*
	,SN_MENUSEPCOLOR			// COLORREF*
	,SN_MENUSEPSHADOWCOLOR		// COLORREF*
	,SN_MENUGRIP				// StyleItem*

    ,SN_LAST
};

#define SN_HANDLEHEIGHT SN_HANDLEWIDTH // ...used by BBZero? (see enum above) + SN_HANDLEHEIGHT matches the corresponding bb4nix window.handleHeight definition

//===========================================================================
//===========================================================================
//===========================================================================

typedef struct StyleItemImage
{
	char path[MAX_PATH];
	HBITMAP bitmap;
	bool argb;
	int mode; // Note: Only "stretched" supported for now == included for forwards compatibility only.
	int width;
	int height;
	int left;
	int top;
	int right;
	int bottom;
} StyleItemImage;

typedef struct StyleItem
{
public:
	int bevelstyle;
	int bevelposition;
	int type;
	bool parentRelative;
	bool interlaced;

	COLORREF Color;			// -> Color1
	COLORREF ColorTo;		// -> Color2
	COLORREF TextColor;
	int FontHeight;
	int FontWeight;
	int Justify;
	int validated;			// bbLean (not used by xoblite)
	char Font[128];

    int nVersion;			// bbLean (not used by xoblite)
    int marginWidth;
    int borderWidth;
    COLORREF borderColor;
    COLORREF foregroundColor;
    COLORREF disabledColor;
    bool bordered;

	bool FontShadow;
	union // BlackboxZero 1.4+ preliminary? (read: we really need to re-align some basic API definitions/assumptions across branches some day)
	{
		struct { char ShadowX; char ShadowY; };
		unsigned short ShadowXY;
	};
	COLORREF ShadowColor;
	bool FontOutline;
	COLORREF OutlineColor;

	COLORREF Color3;
	COLORREF Color4;

	COLORREF Color5;
	COLORREF Color6;
	COLORREF Color7;
	COLORREF Color8;

	StyleItemImage Image;

//	char reserved[62];		// BlackboxZero failsafe: Adjust to maintain sizeof(StyleItem) == 300. (### DEPRECATED ###)

} StyleItem;

//#define PicColor TextColor
#define PicColor foregroundColor
#define BulletColor foregroundColor
#define Color1 Color
#define Color2 ColorTo

#define CORE_SHELL_STYLEITEM 315185 // "core" (c=3, o=15, r=18, e=5) -> Special "nVersion" value used to detect if a given StyleItem belongs to the core shell or not...

//====================

class Menu;
class MenuItem;

//===========================================================================
//===========================================================================
//===========================================================================

extern "C"
{
	DLL_EXPORT LPCSTR GetBBVersion();
	DLL_EXPORT HWND GetBBWnd();
	DLL_EXPORT LPCSTR GetOSInfo();
	DLL_EXPORT bool WINAPI GetBlackboxPath(LPSTR path, int maxLength);

	//====================

	DLL_EXPORT HINSTANCE BBExecute(HWND Owner, LPCSTR szOperation, LPCSTR szCommand, LPCSTR szArgs, LPCSTR szDirectory, int nShowCmd, bool noErrorMsgs);
	DLL_EXPORT void BBSmartExecute(LPSTR inputString);

	DLL_EXPORT void Log(LPCSTR variable, LPCSTR description);
	DLL_EXPORT int MBoxErrorFile(LPCSTR szFile);
	DLL_EXPORT int MBoxErrorValue(LPCSTR szValue);

	//====================

	DLL_EXPORT LPSTR Tokenize(LPCSTR sourceString, LPSTR targetString, LPSTR delimiter);
	DLL_EXPORT int BBTokenize (LPCSTR sourceString, LPSTR* targetStrings, DWORD numTokensToParse, LPSTR remainingString);
	DLL_EXPORT bool IsInString(LPCSTR inputString, LPCSTR searchString);
	bool IsInStringUnicode(LPWSTR inputString, LPWSTR searchString);

	//====================

	bool GetConfigFilePath(LPSTR filePath, LPCSTR setPath, LPCSTR checkName1, LPCSTR checkName2);
	DLL_EXPORT LPCSTR ConfigFileExists(LPCSTR filename, LPCSTR pluginDir);
	DLL_EXPORT bool FileExists(LPCSTR szFileName);
	DLL_EXPORT bool FolderExists(LPCSTR szFolderPath);

	DLL_EXPORT LPCSTR bbrcPath(LPCSTR bbrcFileName=NULL);
	DLL_EXPORT LPCSTR extensionsrcPath(LPCSTR extensionsrcFileName=NULL);
	DLL_EXPORT LPCSTR plugrcPath(LPCSTR pluginrcFileName=NULL);
	DLL_EXPORT LPCSTR menuPath(LPCSTR menurcFileName=NULL);
	DLL_EXPORT LPCSTR stylePath(LPCSTR styleFileName=NULL);

	DLL_EXPORT void GetBlackboxEditor(LPSTR editor);

	//====================

	DLL_EXPORT bool DownloadFile(LPCSTR url, LPCSTR path);

	DLL_EXPORT FILE *FileOpen(LPCSTR fileName);
	DLL_EXPORT bool FileClose(FILE *filePointer);
	DLL_EXPORT bool FileRead(FILE *filePointer, LPSTR readString);

	int GetParentFolder(LPSTR path);

	//====================

	DLL_EXPORT LPSTR ReadValue(LPCSTR fp, LPCSTR keyword, LPLONG ptr = NULL);
	DLL_EXPORT bool ReadBool(LPCSTR filePointer, LPCSTR string, bool defaultBool);
	DLL_EXPORT int ReadInt(LPCSTR filePointer, LPCSTR string, int defaultInt);
	DLL_EXPORT LPSTR ReadString(LPCSTR filePointer, LPCSTR string, LPSTR defaultString);
	DLL_EXPORT COLORREF ReadColor(LPCSTR filePointer, LPCSTR string, LPCSTR defaultString);
	DLL_EXPORT bool ReadNextCommand(FILE *filePointer, LPSTR readLine, DWORD lineLength);

	void WriteValue(LPCSTR fp, LPCSTR keyword, LPCSTR value);
	DLL_EXPORT void WriteBool(LPCSTR filePointer, LPCSTR keyword, bool value);
	DLL_EXPORT void WriteInt(LPCSTR filePointer, LPCSTR keyword, int value);
	DLL_EXPORT void WriteString(LPCSTR filePointer, LPCSTR keyword, LPSTR value);
	DLL_EXPORT void WriteColor(LPCSTR filePointer, LPCSTR keyword, COLORREF value);

	DLL_EXPORT void* GetSettingPtr(int index);

	DLL_EXPORT void ParseItem(LPCSTR szItem, StyleItem *item);

	DLL_EXPORT LPSTR StrRemoveEncap(LPSTR string);
//	LPWSTR StrRemoveEncapUnicode(LPWSTR string);
	DLL_EXPORT void ReplaceEnvVars(LPSTR string);
	DLL_EXPORT void ReplaceShellFolders(LPSTR string);
	DLL_EXPORT void ParseFontString(LPSTR string, StyleItem *item);
	DLL_EXPORT void CheckFontSubstitution(LPSTR font);
	COLORREF ParseLiteralColor(LPCSTR colour);

	int ParseDropFiles(HWND hwnd, WPARAM wParam);

	DLL_EXPORT bool CopyStringToClipboard(LPSTR string);
	DLL_EXPORT bool RetrieveStringFromClipboard(LPSTR target);

	//====================

	// ##############################################################################################
	// ### PRELIMINARY: Work-in-progress relative+adaptive positioning for plugins, cf. the core! ###
	DLL_EXPORT void GetPositionEvolved(LPSTR placement, SIZE size, POINT* pt);
	// ##############################################################################################

	// ###########################################################################################################
	// ### PRELIMINARY: Intended as an *internal* helper function to MakeGradientEvolved() and DrawTextEvolved(), see below! ###
	StyleItem* FindStyleElement(LPSTR styleElement); // Not to be exported! (*internal* helper function to MakeGradientEvolved() and DrawTextEvolved())
	// ### PRELIMINARY: Intended to provide better isolation/abstraction than MakeStyleGradient by not passing pointers to unreliable (e.g. non version-handled) SI structs etc. ###
	DLL_EXPORT bool MakeGradientEvolved(HDC hdc, RECT rect, LPSTR styleElement, bool drawBevel, bool drawBorder);
	// ### PRELIMINARY: Intended as the "DrawText" companions to MakeGradientEvolved(), see above! ###
//	bool DrawTextEvolvedEx(HDC hdc, RECT* rect, LPSTR styleElement, LPWSTR textUnicode, LPSTR textANSI, unsigned int fontHeightOverride, unsigned int format, bool getSize); // Unicode or ANSI (i.e. the more flexible option)
//	DLL_EXPORT bool DrawTextEvolved(HDC hdc, RECT* rect, LPSTR styleElement, LPSTR text, unsigned int fontHeightOverride, unsigned int format, bool getSize); // ANSI only (i.e. the legacy option)
	DLL_EXPORT bool DrawTextEvolved(HDC hdc, RECT* rect, LPSTR styleElement, LPWSTR textUnicode, LPSTR textANSI, unsigned int fontHeightOverride, unsigned int format, bool getSize); // Unicode or ANSI (i.e. the more flexible option)
	// ### PRELIMINARY: Intended as a "dynamic sizing" complement to MakeGradientEvolved() and DrawTextEvolved(), see above! ###
	DLL_EXPORT int GetMetricsEvolved(LPSTR styleElement, int metricsToInclude);
	// ###########################################################################################################

	DLL_EXPORT void MakeGradientSuper(HDC hdc, RECT rect, int type, COLORREF color1, COLORREF color2, COLORREF color3, COLORREF color4, COLORREF color5, COLORREF color6, COLORREF color7, COLORREF color8, bool bInterlaced, int bevelStyle, int bevelPosition, int bevelWidth, COLORREF borderColour, int borderWidth);
	DLL_EXPORT void MakeGradientEx(HDC hdc, RECT rect, int gradientType, COLORREF colourFrom, COLORREF colourTo, COLORREF splitFrom, COLORREF splitTo, bool interlaced, int bevelStyle, int bevelPosition, int bevelWidth, COLORREF borderColour, int borderWidth);
	DLL_EXPORT void MakeGradient(HDC hdc, RECT rect, int gradientType, COLORREF colourFrom, COLORREF colourTo, bool interlaced, int bevelStyle, int bevelPosition, int bevelWidth, COLORREF borderColour, int borderWidth);

	DLL_EXPORT void MakeStyleGradient(HDC hdc, RECT* rect, StyleItem* item, bool withBorder);
	DLL_EXPORT HFONT CreateStyleFont(StyleItem *item);

	DLL_EXPORT void CreateBorder(HDC hdc, RECT* rect, int borderColour, int borderWidth);
	DLL_EXPORT void CreateBorderSuper(HDC hdc, RECT rect, int type, COLORREF color1, COLORREF color2, COLORREF color3, COLORREF color4, COLORREF color5, COLORREF color6, COLORREF color7, COLORREF color8, int borderWidth);

	// ##########################################################################################################
	// ### PRELIMINARY: New internal functions related to per element images, introduced in xoblite bb5 RC6!  ###
	void CreateElementImage(StyleItem* styleItem);
	void DrawImageIntoRect(HDC hdc, RECT r, StyleItem* styleItem, bool useAlpha, bool drawBorder);
	// ##########################################################################################################

	void DrawTextWithEffectsEx(HDC hdc, RECT r, LPWSTR textUnicode, LPSTR textANSI, unsigned int format, COLORREF textColor, bool outline, COLORREF outlineColor, bool shadow, COLORREF shadowColor, int shadowX, int shadowY);
	DLL_EXPORT void DrawTextWithEffects(HDC hdc, RECT r, LPSTR text, unsigned int format, COLORREF textColor, bool outline, COLORREF outlineColor, bool shadow, COLORREF shadowColor, int shadowX, int shadowY);
	void DrawTextWithEffectsUnicode(HDC hdc, RECT r, LPWSTR text, unsigned int format, COLORREF textColor, bool outline, COLORREF outlineColor, bool shadow, COLORREF shadowColor, int shadowX, int shadowY);
	DLL_EXPORT void DrawSatHueIcon(HDC hdc, RECT r, HICON icon, int size, int saturation, int hue);

	// ##########################################################################################################
	// ### PRELIMINARY: DPI-adaptive common glyphs drawing function, introduced in xoblite bb5 RC6!  ###
	DLL_EXPORT void DrawGlyph(HDC hdc, RECT r, int glyph, COLORREF glyphColor);
	// ##########################################################################################################

	//====================

	// ##########################################################################################################
	// ### PRELIMINARY: New functions related to per pixel alpha transparency, introduced in xoblite bb4/bb5! ###
	DLL_EXPORT void AlphaRect(HDC hdc, RECT rect, unsigned char alpha);
	DLL_EXPORT void AlphaBorder(HDC hdc, RECT rect, unsigned char alpha, int borderWidth);
	DLL_EXPORT void AlphaCorner(HDC hdc, RECT rect, int corner, int bevelStyle, int bevelPosition, int bevelWidth, int borderWidth, unsigned char minAlpha, unsigned char maxAlpha);
	DLL_EXPORT void AlphaFromRGB(HDC hdc, RECT rect, unsigned char minAlpha, unsigned char maxAlpha, bool recolor, COLORREF color);
	void AlphaMask(HDC destHdc, RECT destRect, HDC srcHdc, RECT srcRect, bool useSrcRGBAsAlpha); // Work in progress... (may also be scrapped)
	DLL_EXPORT void AlphaApply(HDC hdc, RECT rect);
	DLL_EXPORT void PrepareCorner(HDC hdc, RECT rect, int corner, int bevelStyle, int bevelPosition, int bevelWidth, int borderWidth);
	DLL_EXPORT int PrepareCornerEvolved(HDC hdc, RECT rect, int corner, LPSTR styleElement);
	// ##########################################################################################################

	//====================

	DLL_EXPORT Menu *MakeMenu(LPCSTR HeaderText);
	DLL_EXPORT void DelMenu(Menu *PluginMenu);
	DLL_EXPORT Menu *MakeNamedMenu(LPCSTR HeaderText, LPCSTR MenuId, bool popup);

	DLL_EXPORT MenuItem *MakeSubmenu(Menu *ParentMenu, Menu *ChildMenu, LPCSTR Title);

	DLL_EXPORT MenuItem *MakeMenuItem(Menu *PluginMenu, LPCSTR Title, LPCSTR Cmd, bool ShowIndicator);
	DLL_EXPORT MenuItem* MakeMenuItemBool(Menu* PluginMenu, LPCSTR Title, LPCSTR Cmd, bool state, int selectionGroup);
	DLL_EXPORT MenuItem *MakeMenuItemInt(Menu *PluginMenu, LPCSTR Title, LPCSTR Cmd, int val, int minval, int maxval);
	DLL_EXPORT MenuItem *MakeMenuItemString(Menu *PluginMenu, LPCSTR Title, LPCSTR Cmd, LPCSTR init_string);
	DLL_EXPORT MenuItem* MakeMenuNOP(Menu* PluginMenu, LPCSTR Title);

	DLL_EXPORT void ShowMenu(Menu *PluginMenu);

	//====================

	DLL_EXPORT bool IsAppWindow(HWND hwnd);
	DLL_EXPORT bool SetTransparency(HWND hwnd, BYTE alpha);
	DLL_EXPORT bool SetTransparencyEx(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	DLL_EXPORT void SnapWindowToEdge(WINDOWPOS* windowPosition, int snapDistance, bool useScreenSize);

	//====================

	DLL_EXPORT void MakeSticky(HWND window);
	DLL_EXPORT void RemoveSticky(HWND window);
	DLL_EXPORT bool CheckSticky(HWND window);
	DLL_EXPORT void ToggleSticky(HWND window);
	void ClearSticky();

	//====================

    typedef struct string_node
	{
		struct string_node *next;
		char str[1];
	} string_node;

	class DesktopInfo
	{
		public:
			char name[32];	// Name of the workspace
			bool isCurrent;	// Is this the current workspace?
			int number;		// Workspace number
			int ScreensX;   // Total number of workspaces (for bbLean compatibility only)
			string_node *deskNames; // List of all workspace names (for bbLean compatibility only)
	};

    DLL_EXPORT void GetDesktopInfo(DesktopInfo *deskInfo);

	//====================

	// Support for external system trays (e.g. BBTray)
	// ### Note: Deprecated in xoblite bb5 with the removal of the tray subsystem (note: now always running on top of Explorer!),
	// kept for basic API compatibility with legacy plugins but "returns nothing". ###

	typedef struct
	{
		HWND	hWnd;
		UINT	uID;
		UINT	uCallbackMessage;
		HICON	hIcon;
		char	szTip[256];
		int		x,y;
		RECT	iconRect;
		// For balloon tooltips (for [legacy?] xoblite internal use only, not aligned with bb4win/bbLean!)
        char    szInfoTitle[64];
        char    szInfo[256];
        DWORD   dwInfoFlags;
        UINT    uTimeout;
        UINT    uVersion; // Temporary, we should definitely try to sync our tray icon structs among the devs... Perhaps general stuff + NOTIFYICONDATAV6?
		GUID guidItem; // Temporary, used by later versions of Windows
	} trayIconItem;

	// ### Note: xoblite bb5 onwards is always running alongside Explorer as an "extended shell" (i.e. rather than being an "alternative" == replacement shell like in the old days).
	// The following functions have therefore been kept for basic API compatibility only; the systray itself was removed in xoblite bb5 and hence these functions will "return nothing". ###
	DLL_EXPORT int GetTraySize();
	DLL_EXPORT trayIconItem* GetTrayIcon(int pointer);
	DLL_EXPORT bool GetUnderExplorer();

	//====================

	// Support for external taskbars (NOTE: Not all bb4win functions are supported!)

	DLL_EXPORT int GetTaskListSize();
	DLL_EXPORT void SetTaskWorkspace(HWND hwnd, int workspace);
	DLL_EXPORT int GetTaskWorkspace(HWND hwnd);
	DLL_EXPORT HWND GetTask(int pointer);
	DLL_EXPORT int GetActiveTask();
/*
	DLL_EXPORT struct tasklist* GetTaskListPtr(void);

	struct tasklist // Note: This was the legacy bb4win definition at least -> Possibly/likely revised by others later on (?)
	{
		struct tasklist *next;
		HWND  hwnd;
		HICON icon;
		int   wkspc;
		//bool hidden;
		char  caption[256];
	};
*/
	bool IsOnTaskbar(HWND hwnd);
	DLL_EXPORT HWND IsAppRunning(LPSTR path); // STILL EXPERIMENTAL (SUBJECT TO CHANGE)

	//====================

	// Note: The following stuff was included purely for basic compatibility with the
	// bb(C)Lean(Mod)(Zero) modified BBApi, to be able to load e.g. the bbinterfaceMod_iTunes plugin...

	#define BBWS_TOGGLESTICKY     12 /* lParam: hwnd or NULL for foregroundwindow */
	#define BBWS_MAKESTICKY       14 /* lParam: hwnd or NULL for foregroundwindow */
	#define BBWS_CLEARSTICKY      15 /* lParam: hwnd or NULL for foregroundwindow */
	#define BBWS_TOGGLEONTOP      17 /* lParam: hwnd or NULL for foregroundwindow */

	DLL_EXPORT HMONITOR GetMonitorRect(void* from, RECT* r, int Flags);
};

//===========================================================================

#endif /* __BBAPI_H_ */
