#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "gdiplus.lib")

#include "app.h"
#include <vector>
#include <string>

void App_SaveSettings(App* app) {
    HKEY hKey;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, app->regPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, L"OffsetX", 0, REG_DWORD, (BYTE*)&app->offsetX, sizeof(DWORD));
        RegSetValueEx(hKey, L"FontSize", 0, REG_DWORD, (BYTE*)&app->fontSize, sizeof(DWORD));
        RegSetValueEx(hKey, L"Opacity", 0, REG_DWORD, (BYTE*)&app->opacity, sizeof(DWORD));
        RegSetValueEx(hKey, L"Spacing", 0, REG_DWORD, (BYTE*)&app->spacing, sizeof(DWORD));
        RegSetValueEx(hKey, L"Rounding", 0, REG_DWORD, (BYTE*)&app->rounding, sizeof(DWORD));
        RegSetValueEx(hKey, L"PositionMode", 0, REG_DWORD, (BYTE*)&app->positionMode, sizeof(DWORD));
        RegSetValueEx(hKey, L"ColorActive", 0, REG_DWORD, (BYTE*)&app->colorActive, sizeof(DWORD));
        RegSetValueEx(hKey, L"ColorInactive", 0, REG_DWORD, (BYTE*)&app->colorInactive, sizeof(DWORD));
        DWORD showNum = app->showNumbers ? 1 : 0;
        RegSetValueEx(hKey, L"ShowNumbers", 0, REG_DWORD, (BYTE*)&showNum, sizeof(DWORD));
        RegCloseKey(hKey);
    }
}

void App_LoadSettings(App* app) {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, app->regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dwType = REG_DWORD, dwSize = sizeof(DWORD);
        RegQueryValueEx(hKey, L"OffsetX", NULL, &dwType, (BYTE*)&app->offsetX, &dwSize);
        RegQueryValueEx(hKey, L"FontSize", NULL, &dwType, (BYTE*)&app->fontSize, &dwSize);
        RegQueryValueEx(hKey, L"Opacity", NULL, &dwType, (BYTE*)&app->opacity, &dwSize);
        RegQueryValueEx(hKey, L"Spacing", NULL, &dwType, (BYTE*)&app->spacing, &dwSize);
        RegQueryValueEx(hKey, L"Rounding", NULL, &dwType, (BYTE*)&app->rounding, &dwSize);
        RegQueryValueEx(hKey, L"PositionMode", NULL, &dwType, (BYTE*)&app->positionMode, &dwSize);
        RegQueryValueEx(hKey, L"ColorActive", NULL, &dwType, (BYTE*)&app->colorActive, &dwSize);
        RegQueryValueEx(hKey, L"ColorInactive", NULL, &dwType, (BYTE*)&app->colorInactive, &dwSize);
        DWORD showNum;
        if (RegQueryValueEx(hKey, L"ShowNumbers", NULL, &dwType, (BYTE*)&showNum, &dwSize) == ERROR_SUCCESS) {
            app->showNumbers = (showNum != 0);
        }
        RegCloseKey(hKey);
    }
    if (RegOpenKeyEx(HKEY_CURRENT_USER, app->runPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        app->startupEnabled = (RegQueryValueEx(hKey, app->appName, NULL, NULL, NULL, NULL) == ERROR_SUCCESS);
        RegCloseKey(hKey);
    }
}

void App_ResetSettings(App* app) {
    app->offsetX = 10;
    app->fontSize = 11;
    app->opacity = 255;
    app->spacing = 6;
    app->rounding = 4;
    app->positionMode = POS_LEFT_OF_TRAY;
    app->showNumbers = true;
    app->colorActive = RGB(0, 120, 215);
    app->colorInactive = RGB(70, 70, 70);
    App_SaveSettings(app);
    App_UpdatePosition(app);
    if (app->hSettings) {
        Settings_UpdateValues(app);
    }
}

void App_ToggleStartup(App* app, bool enable) {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, app->runPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            wchar_t szPath[MAX_PATH];
            GetModuleFileName(NULL, szPath, MAX_PATH);
            RegSetValueEx(hKey, app->appName, 0, REG_SZ, (BYTE*)szPath, (wcslen(szPath) + 1) * sizeof(wchar_t));
        } else {
            RegDeleteValue(hKey, app->appName);
        }
        RegCloseKey(hKey);
        app->startupEnabled = enable;
    }
}

bool App_Init(App* app, HINSTANCE hInst) {
    // Initialize defaults
    app->hwnd = NULL;
    app->hSettings = NULL;
    app->hUiFont = NULL;
    app->hTitleFont = NULL;
    app->gdiToken = 0;
    app->totalDesktops = 1;
    app->currentDesktop = 0;
    app->offsetX = 10;
    app->fontSize = 11;
    app->opacity = 255;
    app->spacing = 6;
    app->rounding = 4;
    app->positionMode = POS_LEFT_OF_TRAY;
    app->startupEnabled = false;
    app->showNumbers = true;
    app->colorActive = RGB(0, 120, 215);
    app->colorInactive = RGB(70, 70, 70);
    app->regPath = L"Software\\VDM_Indicator";
    app->runPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    app->appName = L"VDM_Taskbar_Indicator";

    App_LoadSettings(app);
    
    GdiplusStartupInput gpsi;
    GdiplusStartup(&app->gdiToken, &gpsi, NULL);
    
    INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_BAR_CLASSES };
    InitCommonControlsEx(&icex);

    // Load icon
    app->hIcon = (HICON)LoadImage(hInst, L"appicon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (!app->hIcon) {
        app->hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }

    app->hUiFont = CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
    app->hTitleFont = CreateFont(20, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");

    WNDCLASS wc = {0};
    wc.lpfnWndProc = App_WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"VDM_SafeAnchor_Class";
    RegisterClass(&wc);

    WNDCLASS swc = {0};
    swc.lpfnWndProc = Settings_WndProc;
    swc.hInstance = hInst;
    swc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    swc.lpszClassName = L"VDM_Settings_Class";
    RegisterClass(&swc);

    HWND hTaskbar = FindWindow(L"Shell_TrayWnd", NULL);

    app->hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
        L"VDM_SafeAnchor_Class", L"", WS_POPUP, 
        0, 0, 1, 1, hTaskbar, NULL, hInst, app
    );

    if (!app->hwnd) return false;

    NOTIFYICONDATA nid = { sizeof(nid) };
    nid.hWnd = app->hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = app->hIcon;
    wcscpy(nid.szTip, L"Virtual Desktop Indicator");
    Shell_NotifyIcon(NIM_ADD, &nid);

    App_UpdatePosition(app);
    SetTimer(app->hwnd, 1, 100, NULL);
    ShowWindow(app->hwnd, SW_SHOW);
    return true;
}

void App_UpdatePosition(App* app) {
    HWND hTray = FindWindow(L"Shell_TrayWnd", NULL);
    HWND hNotify = FindWindowEx(hTray, NULL, L"TrayNotifyWnd", NULL);
    RECT rect;
    if (hNotify) GetWindowRect(hNotify, &rect);
    else GetWindowRect(hTray, &rect);

    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VirtualDesktops", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD size = 0;
        if (RegQueryValueEx(hKey, L"VirtualDesktopIDs", NULL, NULL, NULL, &size) == ERROR_SUCCESS) 
            app->totalDesktops = (size > 0) ? (size / 16) : 1;
        RegCloseKey(hKey);
    }

    DWORD sessId;
    ProcessIdToSessionId(GetCurrentProcessId(), &sessId);
    wchar_t path[256];
    _snwprintf(path, 256, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\SessionInfo\\%d\\VirtualDesktops", sessId);
    if (RegOpenKeyEx(HKEY_CURRENT_USER, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        GUID cur;
        DWORD s = sizeof(GUID);
        if (RegQueryValueEx(hKey, L"CurrentVirtualDesktop", NULL, NULL, (BYTE*)&cur, &s) == ERROR_SUCCESS) {
            HKEY hL;
            if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VirtualDesktops", 0, KEY_READ, &hL) == ERROR_SUCCESS) {
                DWORD dS = 0;
                RegQueryValueEx(hL, L"VirtualDesktopIDs", NULL, NULL, NULL, &dS);
                if (dS > 0) {
                    std::vector<BYTE> buf(dS);
                    RegQueryValueEx(hL, L"VirtualDesktopIDs", NULL, NULL, buf.data(), &dS);
                    for(int i = 0; i < app->totalDesktops; i++) {
                        if(memcmp(buf.data() + (i * 16), &cur, 16) == 0) {
                            app->currentDesktop = i;
                            break;
                        }
                    }
                }
                RegCloseKey(hL);
            }
        }
        RegCloseKey(hKey);
    }

    int drawCount = (app->totalDesktops < 4) ? 4 : app->totalDesktops;
    int drawBox = (int)(app->fontSize * 2.0);
    int w = drawCount * (drawBox + app->spacing) + 10;
    int h = rect.bottom - rect.top;
    
    int posX, posY;
    if (app->positionMode == POS_RIGHT_OF_TRAY) {
        posX = rect.right + app->offsetX;
    } else {
        posX = rect.left - w - app->offsetX;
    }
    posY = rect.top;

    App_Render(app, posX, posY, w, h, drawCount);
    SetWindowPos(app->hwnd, HWND_TOPMOST, posX, posY, w, h, SWP_NOACTIVATE | SWP_NOSENDCHANGING | SWP_SHOWWINDOW);
}

void App_Render(App* app, int x, int y, int w, int h, int drawCount) {
    HDC hdcS = GetDC(NULL);
    HDC hdcM = CreateCompatibleDC(hdcS);
    HBITMAP hB = CreateCompatibleBitmap(hdcS, w, h);
    HBITMAP hOld = (HBITMAP)SelectObject(hdcM, hB);
    
    Graphics g(hdcM);
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    g.SetTextRenderingHint(TextRenderingHintAntiAlias);
    g.Clear(Color(0, 0, 0, 0));
    
    int drawBox = (int)(app->fontSize * 2.0);
    FontFamily ff(L"Segoe UI");
    Font font(&ff, (REAL)app->fontSize, FontStyleBold, UnitPoint);
    StringFormat sf;
    sf.SetAlignment(StringAlignmentCenter);
    sf.SetLineAlignment(StringAlignmentCenter);

    for (int i = 0; i < drawCount; i++) {
        RectF r((REAL)(5 + i * (drawBox + app->spacing)), (REAL)((h - drawBox) / 2), (REAL)drawBox, (REAL)drawBox);
        
        Color c;
        if (i < app->totalDesktops) {
            if (i == app->currentDesktop) {
                c = Color(app->opacity, GetRValue(app->colorActive), GetGValue(app->colorActive), GetBValue(app->colorActive));
            } else {
                c = Color(app->opacity, GetRValue(app->colorInactive), GetGValue(app->colorInactive), GetBValue(app->colorInactive));
            }
        } else {
            c = Color(100, 50, 50, 50);
        }

        if (app->rounding > 0) {
            GraphicsPath path;
            REAL rad = (REAL)app->rounding;
            path.AddArc(r.X, r.Y, rad * 2, rad * 2, 180, 90);
            path.AddArc(r.X + r.Width - rad * 2, r.Y, rad * 2, rad * 2, 270, 90);
            path.AddArc(r.X + r.Width - rad * 2, r.Y + r.Height - rad * 2, rad * 2, rad * 2, 0, 90);
            path.AddArc(r.X, r.Y + r.Height - rad * 2, rad * 2, rad * 2, 90, 90);
            path.CloseFigure();
            
            SolidBrush b(c);
            g.FillPath(&b, &path);
        } else {
            SolidBrush b(c);
            g.FillRectangle(&b, r);
        }
        
        if (i < app->totalDesktops && app->showNumbers) {
            SolidBrush white(Color(255, 255, 255, 255));
            wchar_t num[4];
            _itow(i + 1, num, 10);
            g.DrawString(num, -1, &font, r, &sf, &white);
        }
    }
    
    POINT ptS = {0, 0}, ptW = {x, y};
    SIZE sz = {w, h};
    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    UpdateLayeredWindow(app->hwnd, hdcS, &ptW, &sz, hdcM, &ptS, 0, &bf, ULW_ALPHA);
    
    SelectObject(hdcM, hOld);
    DeleteObject(hB);
    DeleteDC(hdcM);
    ReleaseDC(NULL, hdcS);
}

LRESULT CALLBACK App_WindowProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    App* app = (App*)GetWindowLongPtr(h, GWLP_USERDATA);
    
    if (m == WM_NCCREATE) {
        SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)l)->lpCreateParams);
        return TRUE;
    }
    
    if (!app) return DefWindowProc(h, m, w, l);
    
    if (m == WM_TIMER) {
        App_UpdatePosition(app);
    }
    
    if (m == WM_TRAYICON && l == WM_RBUTTONUP) {
        HMENU mPop = CreatePopupMenu();
        AppendMenu(mPop, MF_STRING, ID_TRAY_SETTINGS, L"⚙ Settings");
        AppendMenu(mPop, MF_SEPARATOR, 0, NULL);
        AppendMenu(mPop, MF_STRING, ID_TRAY_EXIT, L"✖ Exit");
        
        POINT pt;
        GetCursorPos(&pt);
        SetForegroundWindow(h);
        TrackPopupMenu(mPop, TPM_RIGHTBUTTON, pt.x, pt.y, 0, h, NULL);
        DestroyMenu(mPop);
    }
    
    if (m == WM_COMMAND) {
        if (LOWORD(w) == ID_TRAY_SETTINGS) App_OpenSettings(app);
        if (LOWORD(w) == ID_TRAY_EXIT) PostQuitMessage(0);
    }
    
    if (m == WM_DESTROY) {
        NOTIFYICONDATA nid = { sizeof(nid) };
        nid.hWnd = h;
        nid.uID = 1;
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
    }
    
    return DefWindowProc(h, m, w, l);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int) {
    HANDLE hMutex = CreateMutex(NULL, TRUE, L"Global\\VDM_Indicator_Unique_Mutex_Name");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBox(NULL, L"Virtual Desktop Indicator is already running!", L"Already Running", MB_OK | MB_ICONINFORMATION);
        return 0;
    }
    
    App app;
    if (App_Init(&app, h)) {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    if (hMutex) {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }
    
    GdiplusShutdown(app.gdiToken);
    return 0;
}