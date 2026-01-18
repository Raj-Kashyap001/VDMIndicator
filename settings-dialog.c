#include "app.h"
#include <string>

void Settings_UpdateValues(App* app) {
    if (!app->hSettings) return;
    
    SendMessage(GetDlgItem(app->hSettings, IDC_SLIDER_X), TBM_SETPOS, TRUE, app->offsetX);
    SetWindowText(GetDlgItem(app->hSettings, IDC_LBL_VAL_X), std::to_wstring(app->offsetX).c_str());
    
    SendMessage(GetDlgItem(app->hSettings, IDC_SLIDER_FONT), TBM_SETPOS, TRUE, app->fontSize);
    SetWindowText(GetDlgItem(app->hSettings, IDC_LBL_VAL_FONT), std::to_wstring(app->fontSize).c_str());
    
    SendMessage(GetDlgItem(app->hSettings, IDC_SLIDER_OPACITY), TBM_SETPOS, TRUE, app->opacity);
    SetWindowText(GetDlgItem(app->hSettings, IDC_LBL_VAL_OPACITY), std::to_wstring(app->opacity).c_str());
    
    SendMessage(GetDlgItem(app->hSettings, IDC_SLIDER_SPACING), TBM_SETPOS, TRUE, app->spacing);
    SetWindowText(GetDlgItem(app->hSettings, IDC_LBL_VAL_SPACING), std::to_wstring(app->spacing).c_str());
    
    SendMessage(GetDlgItem(app->hSettings, IDC_SLIDER_ROUNDING), TBM_SETPOS, TRUE, app->rounding);
    SetWindowText(GetDlgItem(app->hSettings, IDC_LBL_VAL_ROUNDING), std::to_wstring(app->rounding).c_str());
    
    SendMessage(GetDlgItem(app->hSettings, IDC_COMBO_POSITION), CB_SETCURSEL, app->positionMode, 0);
    
    SendMessage(GetDlgItem(app->hSettings, IDC_CHK_STARTUP), BM_SETCHECK, 
                app->startupEnabled ? BST_CHECKED : BST_UNCHECKED, 0);
    
    SendMessage(GetDlgItem(app->hSettings, IDC_CHK_SHOW_NUMBERS), BM_SETCHECK, 
                app->showNumbers ? BST_CHECKED : BST_UNCHECKED, 0);
}

COLORREF ShowColorDialog(HWND hwnd, COLORREF currentColor) {
    static COLORREF customColors[16] = {
        RGB(255, 255, 255), RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 255, 0),
        RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 0, 255), RGB(0, 255, 255),
        RGB(128, 128, 128), RGB(192, 192, 192), RGB(128, 0, 0), RGB(0, 128, 0),
        RGB(0, 0, 128), RGB(128, 128, 0), RGB(128, 0, 128), RGB(0, 128, 128)
    };
    
    CHOOSECOLOR cc;
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = customColors;
    cc.rgbResult = currentColor;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    
    if (ChooseColor(&cc)) {
        return cc.rgbResult;
    }
    return currentColor;
}

void Settings_CreateControls(App* app, HWND hSettings) {
    int y = 20;
    int labelWidth = 150;
    int valueWidth = 80;
    int sliderWidth = 360;
    int xLabel = 30;
    int xValue = 560;
    int xSlider = 25;
    
    // Title
    HWND hTitle = CreateWindow(L"STATIC", L"Virtual Desktop Indicator Settings", 
                               WS_CHILD | WS_VISIBLE | SS_CENTER,
                               0, y, 650, 30, hSettings, NULL, NULL, NULL);
    SendMessage(hTitle, WM_SETFONT, (WPARAM)app->hTitleFont, TRUE);
    y += 50;

    // X Offset
    CreateWindow(L"STATIC", L"Position Offset:", WS_CHILD | WS_VISIBLE,
                 xLabel, y, labelWidth, 20, hSettings, NULL, NULL, NULL);
    HWND hValX = CreateWindow(L"STATIC", L"0", WS_CHILD | WS_VISIBLE | SS_RIGHT,
                              xValue, y, valueWidth, 20, hSettings, (HMENU)(INT_PTR)IDC_LBL_VAL_X, NULL, NULL);
    SendMessage(hValX, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 25;
    
    HWND hSliderX = CreateWindow(TRACKBAR_CLASSW, L"", WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
                                 xSlider, y, sliderWidth, 35, hSettings, (HMENU)IDC_SLIDER_X, NULL, NULL);
    SendMessage(hSliderX, TBM_SETRANGE, TRUE, MAKELONG(0, 500));
    SendMessage(hSliderX, TBM_SETTICFREQ, 50, 0);
    y += 45;

    // Font Size
    CreateWindow(L"STATIC", L"Font Size:", WS_CHILD | WS_VISIBLE,
                 xLabel, y, labelWidth, 20, hSettings, NULL, NULL, NULL);
    HWND hValFont = CreateWindow(L"STATIC", L"0", WS_CHILD | WS_VISIBLE | SS_RIGHT,
                                 xValue, y, valueWidth, 20, hSettings, (HMENU)(INT_PTR)IDC_LBL_VAL_FONT, NULL, NULL);
    SendMessage(hValFont, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 25;
    
    HWND hSliderFont = CreateWindow(TRACKBAR_CLASSW, L"", WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
                                    xSlider, y, sliderWidth, 35, hSettings, (HMENU)IDC_SLIDER_FONT, NULL, NULL);
    SendMessage(hSliderFont, TBM_SETRANGE, TRUE, MAKELONG(6, 24));
    SendMessage(hSliderFont, TBM_SETTICFREQ, 2, 0);
    y += 45;

    // Opacity
    CreateWindow(L"STATIC", L"Opacity:", WS_CHILD | WS_VISIBLE,
                 xLabel, y, labelWidth, 20, hSettings, NULL, NULL, NULL);
    HWND hValOpacity = CreateWindow(L"STATIC", L"0", WS_CHILD | WS_VISIBLE | SS_RIGHT,
                                    xValue, y, valueWidth, 20, hSettings, (HMENU)(INT_PTR)IDC_LBL_VAL_OPACITY, NULL, NULL);
    SendMessage(hValOpacity, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 25;
    
    HWND hSliderOpacity = CreateWindow(TRACKBAR_CLASSW, L"", WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
                                       xSlider, y, sliderWidth, 35, hSettings, (HMENU)IDC_SLIDER_OPACITY, NULL, NULL);
    SendMessage(hSliderOpacity, TBM_SETRANGE, TRUE, MAKELONG(50, 255));
    SendMessage(hSliderOpacity, TBM_SETTICFREQ, 25, 0);
    y += 45;

    // Spacing
    CreateWindow(L"STATIC", L"Box Spacing:", WS_CHILD | WS_VISIBLE,
                 xLabel, y, labelWidth, 20, hSettings, NULL, NULL, NULL);
    HWND hValSpacing = CreateWindow(L"STATIC", L"0", WS_CHILD | WS_VISIBLE | SS_RIGHT,
                                    xValue, y, valueWidth, 20, hSettings, (HMENU)(INT_PTR)IDC_LBL_VAL_SPACING, NULL, NULL);
    SendMessage(hValSpacing, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 25;
    
    HWND hSliderSpacing = CreateWindow(TRACKBAR_CLASSW, L"", WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
                                       xSlider, y, sliderWidth, 35, hSettings, (HMENU)IDC_SLIDER_SPACING, NULL, NULL);
    SendMessage(hSliderSpacing, TBM_SETRANGE, TRUE, MAKELONG(0, 20));
    SendMessage(hSliderSpacing, TBM_SETTICFREQ, 2, 0);
    y += 45;

    // Corner Rounding
    CreateWindow(L"STATIC", L"Corner Rounding:", WS_CHILD | WS_VISIBLE,
                 xLabel, y, labelWidth, 20, hSettings, NULL, NULL, NULL);
    HWND hValRounding = CreateWindow(L"STATIC", L"0", WS_CHILD | WS_VISIBLE | SS_RIGHT,
                                     xValue, y, valueWidth, 20, hSettings, (HMENU)(INT_PTR)IDC_LBL_VAL_ROUNDING, NULL, NULL);
    SendMessage(hValRounding, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 25;
    
    HWND hSliderRounding = CreateWindow(TRACKBAR_CLASSW, L"", WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
                                        xSlider, y, sliderWidth, 35, hSettings, (HMENU)IDC_SLIDER_ROUNDING, NULL, NULL);
    SendMessage(hSliderRounding, TBM_SETRANGE, TRUE, MAKELONG(0, 12));
    SendMessage(hSliderRounding, TBM_SETTICFREQ, 1, 0);
    y += 50;

    // Position Mode
    CreateWindow(L"STATIC", L"Position Mode:", WS_CHILD | WS_VISIBLE,
                 xLabel, y, labelWidth, 20, hSettings, NULL, NULL, NULL);
    HWND hCombo = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                               200, y, 200, 100, hSettings, (HMENU)IDC_COMBO_POSITION, NULL, NULL);
    SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Left of System Tray");
    SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Right of System Tray");
    SendMessage(hCombo, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 35;

    // Color Buttons
    CreateWindow(L"STATIC", L"Colors:", WS_CHILD | WS_VISIBLE,
                 xLabel, y, labelWidth, 20, hSettings, NULL, NULL, NULL);
    
    HWND hBtnActive = CreateWindow(L"BUTTON", L"Active Desktop Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                   200, y, 180, 28, hSettings, (HMENU)IDC_COLOR_ACTIVE, NULL, NULL);
    SendMessage(hBtnActive, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    
    HWND hBtnInactive = CreateWindow(L"BUTTON", L"Inactive Desktop Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                     400, y, 180, 28, hSettings, (HMENU)IDC_COLOR_INACTIVE, NULL, NULL);
    SendMessage(hBtnInactive, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 40;

    // Checkboxes
    HWND hChkStartup = CreateWindow(L"BUTTON", L"Start with Windows", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                                    xLabel, y, 250, 24, hSettings, (HMENU)IDC_CHK_STARTUP, NULL, NULL);
    SendMessage(hChkStartup, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 30;

    HWND hChkNumbers = CreateWindow(L"BUTTON", L"Show Desktop Numbers", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                                    xLabel, y, 250, 24, hSettings, (HMENU)IDC_CHK_SHOW_NUMBERS, NULL, NULL);
    SendMessage(hChkNumbers, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);
    y += 40;

    // Reset Button
    HWND hBtnReset = CreateWindow(L"BUTTON", L"Reset to Defaults", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                  250, y, 150, 32, hSettings, (HMENU)IDC_BTN_RESET, NULL, NULL);
    SendMessage(hBtnReset, WM_SETFONT, (WPARAM)app->hUiFont, TRUE);

    // Set fonts for all static labels
    EnumChildWindows(hSettings, [](HWND hwndChild, LPARAM lParam) -> BOOL {
        wchar_t className[256];
        GetClassName(hwndChild, className, 256);
        if (wcscmp(className, L"STATIC") == 0) {
            SendMessage(hwndChild, WM_SETFONT, lParam, TRUE);
        }
        return TRUE;
    }, (LPARAM)app->hUiFont);

    Settings_UpdateValues(app);
}

void App_OpenSettings(App* app) {
    if (app->hSettings) {
        SetForegroundWindow(app->hSettings);
        return;
    }
    
    app->hSettings = CreateWindow(L"VDM_Settings_Class", L"Settings", 
                                  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                  CW_USEDEFAULT, CW_USEDEFAULT, 680, 640,
                                  NULL, NULL, GetModuleHandle(NULL), app);
    
    if (app->hIcon) {
        SendMessage(app->hSettings, WM_SETICON, ICON_SMALL, (LPARAM)app->hIcon);
        SendMessage(app->hSettings, WM_SETICON, ICON_BIG, (LPARAM)app->hIcon);
    }
    
    Settings_CreateControls(app, app->hSettings);
    ShowWindow(app->hSettings, SW_SHOW);
}

LRESULT CALLBACK Settings_WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    App* app = (App*)GetWindowLongPtr(h, GWLP_USERDATA);
    
    if (m == WM_NCCREATE) {
        SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)l)->lpCreateParams);
        return TRUE;
    }
    
    if (!app) return DefWindowProc(h, m, w, l);
    
    if (m == WM_HSCROLL) {
        int id = GetDlgCtrlID((HWND)l);
        int pos = (int)SendMessage((HWND)l, TBM_GETPOS, 0, 0);
        
        if (id == IDC_SLIDER_X) {
            app->offsetX = pos;
            SetWindowText(GetDlgItem(h, IDC_LBL_VAL_X), std::to_wstring(pos).c_str());
        }
        else if (id == IDC_SLIDER_FONT) {
            app->fontSize = pos;
            SetWindowText(GetDlgItem(h, IDC_LBL_VAL_FONT), std::to_wstring(pos).c_str());
        }
        else if (id == IDC_SLIDER_OPACITY) {
            app->opacity = pos;
            SetWindowText(GetDlgItem(h, IDC_LBL_VAL_OPACITY), std::to_wstring(pos).c_str());
        }
        else if (id == IDC_SLIDER_SPACING) {
            app->spacing = pos;
            SetWindowText(GetDlgItem(h, IDC_LBL_VAL_SPACING), std::to_wstring(pos).c_str());
        }
        else if (id == IDC_SLIDER_ROUNDING) {
            app->rounding = pos;
            SetWindowText(GetDlgItem(h, IDC_LBL_VAL_ROUNDING), std::to_wstring(pos).c_str());
        }
        
        App_SaveSettings(app);
        App_UpdatePosition(app);
    }
    
    if (m == WM_COMMAND) {
        if (LOWORD(w) == IDC_CHK_STARTUP) {
            bool checked = (SendMessage((HWND)l, BM_GETCHECK, 0, 0) == BST_CHECKED);
            App_ToggleStartup(app, checked);
        }
        else if (LOWORD(w) == IDC_CHK_SHOW_NUMBERS) {
            app->showNumbers = (SendMessage((HWND)l, BM_GETCHECK, 0, 0) == BST_CHECKED);
            App_SaveSettings(app);
            App_UpdatePosition(app);
        }
        else if (LOWORD(w) == IDC_COMBO_POSITION && HIWORD(w) == CBN_SELCHANGE) {
            app->positionMode = (int)SendMessage((HWND)l, CB_GETCURSEL, 0, 0);
            App_SaveSettings(app);
            App_UpdatePosition(app);
        }
        else if (LOWORD(w) == IDC_COLOR_ACTIVE) {
            COLORREF newColor = ShowColorDialog(h, app->colorActive);
            if (newColor != app->colorActive) {
                app->colorActive = newColor;
                App_SaveSettings(app);
                App_UpdatePosition(app);
            }
        }
        else if (LOWORD(w) == IDC_COLOR_INACTIVE) {
            COLORREF newColor = ShowColorDialog(h, app->colorInactive);
            if (newColor != app->colorInactive) {
                app->colorInactive = newColor;
                App_SaveSettings(app);
                App_UpdatePosition(app);
            }
        }
        else if (LOWORD(w) == IDC_BTN_RESET) {
            if (MessageBox(h, L"Reset all settings to default values?", L"Confirm Reset", 
                          MB_YESNO | MB_ICONQUESTION) == IDYES) {
                App_ResetSettings(app);
            }
        }
    }
    
    if (m == WM_CLOSE) {
        DestroyWindow(h);
        app->hSettings = NULL;
        return 0;
    }
    
    return DefWindowProc(h, m, w, l);
}