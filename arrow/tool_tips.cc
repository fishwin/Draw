

#include "stdafx.h"
#include "tool_tips.h"
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

tooltips::ToolTip::ToolTip(HWND hmainwnd) {
  h_main_wnd_ = hmainwnd;
  h_tip_wnd_ = NULL;
  InitData();
}

tooltips::ToolTip::~ToolTip() {}

void tooltips::ToolTip::InitData() {
  INITCOMMONCONTROLSEX iccex;
  iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  iccex.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&iccex);
  HINSTANCE hInstance = (HINSTANCE)GetWindowLong(h_main_wnd_, GWL_HINSTANCE);
  h_tip_wnd_ = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, h_main_wnd_, NULL, hInstance, NULL);
}

void tooltips::ToolTip::AddRectToolTip(RECT rect, CString tip_string) {
  TOOLINFO	tti;
  memset(&tti, 0, sizeof(TOOLINFO));
  tti.cbSize = sizeof(TOOLINFO);
  tti.uFlags = TTF_SUBCLASS;
  tti.hwnd = h_main_wnd_;
  tti.rect = rect;
  tti.uId = 0;
  tti.lpszText = tip_string.GetBuffer();
  tip_string.ReleaseBuffer();
  SendMessage(h_tip_wnd_, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&tti);
  SendMessage(h_tip_wnd_, TTM_SETDELAYTIME, (WPARAM)(DWORD)TTDT_AUTOMATIC, (LPARAM)(INT)MAKELONG(500, 0));
  SendMessage(h_tip_wnd_, TTM_SETMAXTIPWIDTH, 0, (LPARAM)(INT)200);
}

void tooltips::ToolTip::AddControlToolTip(UINT ctr_id, CString tip_string) {
  TOOLINFO	tti;
  memset(&tti, 0, sizeof(TOOLINFO));
  tti.cbSize = sizeof(TOOLINFO);
  tti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
  tti.uId = (UINT_PTR)GetDlgItem(h_main_wnd_, ctr_id);
  tti.hinst = (HINSTANCE)GetWindowLong(h_main_wnd_, GWL_HINSTANCE);
  tti.lpszText = tip_string.GetBuffer();
  tip_string.ReleaseBuffer();

  SendMessage(h_tip_wnd_, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&tti);
  SendMessage(h_tip_wnd_, TTM_SETDELAYTIME, (WPARAM)(DWORD)TTDT_AUTOMATIC, (LPARAM)(INT)MAKELONG(500, 0));
  SendMessage(h_tip_wnd_, TTM_SETMAXTIPWIDTH, 0, (LPARAM)(INT)200);
}

