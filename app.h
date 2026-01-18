#ifndef APP_H
#define APP_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <gdiplus.h>
#include <stdbool.h>

using namespace Gdiplus;

// Message and Control IDs
#define WM_TRAYICON      (WM_USER + 1)
#define ID_TRAY_EXIT     2001
#define ID_TRAY_SETTINGS 2002
#define ID_TRAY_ABOUT    2003

// Settings Dialog Controls
#define IDC_SLIDER_X          3001
#define IDC_SLIDER_FONT       3002
#define IDC_LBL_VAL_X         3003
#define IDC_LBL_VAL_FONT      3004
#define IDC_CHK_STARTUP       3005
#define IDC_SLIDER_OPACITY    3006
#define IDC_LBL_VAL_OPACITY   3007
#define IDC_COMBO_POSITION    3008
#define IDC_COLOR_ACTIVE      3009
#define IDC_COLOR_INACTIVE    3010
#define IDC_CHK_SHOW_NUMBERS  3011
#define IDC_SLIDER_SPACING    3012
#define IDC_LBL_VAL_SPACING   3013
#define IDC_SLIDER_ROUNDING   3014
#define IDC_LBL_VAL_ROUNDING  3015
#define IDC_BTN_RESET         3016

// Position modes
#define POS_LEFT_OF_TRAY   0
#define POS_RIGHT_OF_TRAY  1
#define POS_CUSTOM         2

// App structure
typedef struct {
    HWND hwnd;
    HWND hSettings;
    ULONG_PTR gdiToken;
    HFONT hUiFont;
    HFONT hTitleFont;
    HICON hIcon;
    int totalDesktops;
    int currentDesktop;
    
    // Settings
    int offsetX;
    int fontSize;
    int opacity;
    int spacing;
    int rounding;
    int positionMode;
    int customX;
    int customY;
    bool startupEnabled;
    bool showNumbers;
    COLORREF colorActive;
    COLORREF colorInactive;
    
    const wchar_t* regPath;
    const wchar_t* runPath;
    const wchar_t* appName;
} App;

// Function declarations
bool App_Init(App* app, HINSTANCE hInst);
void App_SaveSettings(App* app);
void App_LoadSettings(App* app);
void App_ToggleStartup(App* app, bool enable);
void App_UpdatePosition(App* app);
void App_Render(App* app, int x, int y, int w, int h, int drawCount);
void App_OpenSettings(App* app);
void App_ResetSettings(App* app);
LRESULT CALLBACK App_WindowProc(HWND h, UINT m, WPARAM w, LPARAM l);

// Settings dialog functions
LRESULT CALLBACK Settings_WndProc(HWND h, UINT m, WPARAM w, LPARAM l);
void Settings_CreateControls(App* app, HWND hSettings);
void Settings_UpdateValues(App* app);

#endif // APP_H