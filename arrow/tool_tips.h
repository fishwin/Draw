

#ifndef TOOLTIPS_TOOL_TIPS_H_H
#define TOOLTIPS_TOOL_TIPS_H_H

#include "stdafx.h"

namespace tooltips {
class ToolTip {
public:
  ToolTip(HWND hmainwnd);
  ~ToolTip();

  void AddRectToolTip(RECT rect, CString tip_string);

  void AddControlToolTip(UINT ctr_id, CString tip_string);

private:
  void InitData();

  HWND h_main_wnd_;
  HWND h_tip_wnd_;
};
}

#endif  // TOOLTIPS_TOOL_TIPS_H_H
