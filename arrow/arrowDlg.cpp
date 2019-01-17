

#include "stdafx.h"
#include "arrow.h"
#include "arrowDlg.h"
#include "afxdialogex.h"

#include "tool_tips.h"

class CAboutDlg : public CDialogEx {
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CarrowDlg::CarrowDlg(CWnd* pParent)
	  : CDialogEx(IDD_ARROW_DIALOG, pParent) {
  draw_mode_ = inval;
  drawing_mode_ = invalid;
  is_clear_ = false;
  gpath_line_ = new Gdiplus::GraphicsPath;
  selected_rect_ = { 30, 25, 700, 450 };
  pen_color_ = Color::Red;
  pen_width_ = 5;
}

void CarrowDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CarrowDlg, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_BUTTON1, &CarrowDlg::OnBnClickedButton1)
  ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONUP()
  ON_BN_CLICKED(IDC_BUTTON2, &CarrowDlg::OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON4, &CarrowDlg::OnBnClickedButton4)
  ON_BN_CLICKED(IDC_BUTTON3, &CarrowDlg::OnBnClickedButton3)
  ON_BN_CLICKED(IDC_BUTTON5, &CarrowDlg::OnBnClickedButton5)
//  ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BUTTON6, &CarrowDlg::OnBnClickedButton6)
ON_BN_CLICKED(IDC_BUTTON7, &CarrowDlg::OnBnClickedButton7)
ON_BN_CLICKED(IDC_BUTTON8, &CarrowDlg::OnBnClickedButton8)
ON_STN_CLICKED(IDC_PENCOLOR, &CarrowDlg::OnStnClickedPencolor)
END_MESSAGE_MAP()


BOOL CarrowDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
  h_cur_ = LoadCursor(NULL, IDC_CROSS);

  CEdit* penwidth = (CEdit*)GetDlgItem(IDC_PENWIDTH);
  penwidth->EnableWindow(FALSE);
  CString temp_String;
  temp_String.Format(L"%d", (int)pen_width_);
  penwidth->SetWindowText(temp_String);

  // tool tips
  tooltips::ToolTip tp(m_hWnd);
  tp.AddControlToolTip(IDC_BUTTON1, L"Brush");
  tp.AddControlToolTip(IDC_BUTTON2, L"Arrow");
  tp.AddControlToolTip(IDC_BUTTON3, L"Eclipse");
  tp.AddControlToolTip(IDC_BUTTON4, L"rectangle");
  tp.AddControlToolTip(IDC_BUTTON5, L"Clear");
  tp.AddControlToolTip(IDC_BUTTON6, L"Pen Color");

 // tp.AddRectToolTip(selected_rect_, L"hello world");

	return TRUE;
}

void CarrowDlg::OnPaint() {
  PAINTSTRUCT ps;
  CDC* cdc = BeginPaint(&ps);
  CRect rect;
  GetClientRect(&rect);
  CDC dcMem;
  CBitmap bmp;
  dcMem.CreateCompatibleDC(cdc);
  bmp.CreateCompatibleBitmap(cdc, rect.Width(), rect.Height());
  dcMem.SelectObject(&bmp);
  dcMem.FillSolidRect(rect, cdc->GetBkColor());

  RECT rt;
  ((CStatic*)GetDlgItem(IDC_PENCOLOR))->GetClientRect(&rt);
  Graphics ggg(dcMem.GetSafeHdc());
  Gdiplus::SolidBrush sbrush(Color::Red);
  ggg.FillRectangle(&sbrush, 521, 475, rt.right - rt.left, rt.bottom - rt.top);

  CPen pen(PS_SOLID, 6, RGB(100, 100, 0));
  CPen *oldPen = dcMem.SelectObject(&pen);
  dcMem.Rectangle(selected_rect_);
  dcMem.SelectObject(oldPen);

  if (!is_clear_) {
    Gdiplus::Graphics g(dcMem.GetSafeHdc());
    for (size_t i = 0; i < vec_drawcells_.size(); i++) {
      Pen pen2(vec_drawcells_[i].pencolor);
      pen2.SetWidth(vec_drawcells_[i].penwidth);
      g.SetSmoothingMode(SmoothingModeAntiAlias);
      g.DrawPath(&pen2, vec_drawcells_[i].drawpath);
    }
    for (size_t i = 0; i < vec_arrow_drawcells_.size(); i++) {
      Gdiplus::SolidBrush sbrush(vec_arrow_drawcells_[i].pencolor);
      vec_arrow_drawcells_[i].drawpath->SetFillMode(FillModeWinding);
      g.FillPath(&sbrush, vec_arrow_drawcells_[i].drawpath);
    }

    Pen pen3(pen_color_);
    pen3.SetWidth(pen_width_);
    if (drawing_mode_ == drawingrectangle) {
      g.DrawPolygon(&pen3, rectangle_points_, 4);
    }
    if (drawing_mode_ == drawingellipse) {
      g.DrawEllipse(&pen3, start_pt_.x, start_pt_.y, old_pt_.x - start_pt_.x, old_pt_.y - start_pt_.y);
    }
    if (drawing_mode_ == drawingarrow) {
        Gdiplus::SolidBrush orangeBrush(pen_color_);
        g.FillPolygon(&orangeBrush, arrow_points_, 6);
    }
  }
  cdc->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
  dcMem.DeleteDC();
  bmp.DeleteObject();
  EndPaint(&ps);
  if (is_clear_) {
    for (size_t i = 0; i < vec_drawcells_.size(); i++) {
      vec_drawcells_[i].drawpath->Reset();
    }
    for (size_t i = 0; i < vec_arrow_drawcells_.size(); i++) {
      vec_arrow_drawcells_[i].drawpath->Reset();
    }
    is_clear_ = false;
    draw_mode_ = inval;
  }
}

void CarrowDlg::OnLButtonDown(UINT nFlags, CPoint point) {
  if (draw_mode_ == arrow) {
    drawing_mode_ = drawingarrow;
    arrow_points_[0] = { point.x,point.y };
    arrow_points_[1] = { point.x,point.y };
    arrow_points_[2] = { point.x,point.y };
    arrow_points_[3] = { point.x,point.y };
    arrow_points_[4] = { point.x,point.y };
    arrow_points_[5] = { point.x,point.y };
  }
  if (draw_mode_ == line) {
    start_pt_ = point;
    drawing_mode_ = drawingline;
  }
  if (draw_mode_ == rectangle) {
    drawing_mode_ = drawingrectangle;
    rectangle_points_[0] = { point.x,point.y };
    rectangle_points_[1] = { point.x,point.y };
    rectangle_points_[2] = { point.x,point.y };
    rectangle_points_[3] = { point.x,point.y };
  }
  if (draw_mode_ == ellipse) {
    drawing_mode_ = drawingellipse;
    start_pt_ = old_pt_ = point;
  }
  CDialogEx::OnLButtonDown(nFlags, point);
}


void CarrowDlg::OnMouseMove(UINT nFlags, CPoint point) {
  if (drawing_mode_ == drawingline) {
    SetCursor(h_cur_);
    DrawLine(start_pt_, point);
    start_pt_ = point;
  }
  if (drawing_mode_ == drawingarrow) {
    SetCursor(h_cur_);
    DrawArrow(arrow_points_, CPoint(arrow_points_[0].X, arrow_points_[0].Y), point);
  }
  if (drawing_mode_ == drawingrectangle) {
    SetCursor(h_cur_);
    DrawRectangle(rectangle_points_, CPoint(rectangle_points_[0].X, rectangle_points_[0].Y), point);
  }
  if (drawing_mode_ == drawingellipse) {
    SetCursor(h_cur_);
    old_pt_ = point;
    DrawEclipse(start_pt_, point);
  }

  CDialogEx::OnMouseMove(nFlags, point);
}

void CarrowDlg::OnLButtonUp(UINT nFlags, CPoint point) {
  GraphicsPath temp_path;
  if (drawing_mode_ == drawingline) {
    drawing_mode_ = invalid;
    temp_path.AddPath(gpath_line_, FALSE);
    gpath_line_->Reset();
  }
  if (drawing_mode_ == drawingarrow) {
    GraphicsPath temp_arrow_path;
    temp_arrow_path.AddPolygon(arrow_points_, 6);
    InvalidateRect(selected_rect_, false);
    drawing_mode_ = invalid;
    vec_arrow_drawcells_.push_back(DrawCell(pen_color_, pen_width_, &temp_arrow_path));
  }
  if (drawing_mode_ == drawingrectangle) {
    Gdiplus::GraphicsPath gpath_rectangle;
    temp_path.AddPolygon(rectangle_points_, 4);
    temp_path.AddPath(&gpath_rectangle, FALSE);
    gpath_rectangle.Reset();
    InvalidateRect(selected_rect_, false);
    drawing_mode_ = invalid;
  }
  if (drawing_mode_ == drawingellipse) {
    temp_path.AddEllipse(Rect(start_pt_.x, start_pt_.y, old_pt_.x - start_pt_.x, old_pt_.y - start_pt_.y));
    InvalidateRect(selected_rect_, false);
    drawing_mode_ = invalid;
  }
  vec_drawcells_.push_back(DrawCell(pen_color_, pen_width_, &temp_path));
  CDialogEx::OnLButtonUp(nFlags, point);
}


void CarrowDlg::DrawArrow(Gdiplus::Point arrow_points[], CPoint oript, CPoint detpt) {
  if (!(oript.x >= selected_rect_.left && oript.x <= selected_rect_.right && oript.y > selected_rect_.top && oript.y < selected_rect_.bottom)) {
    return;
  }
  if (!(detpt.x >= selected_rect_.left && detpt.x <= selected_rect_.right && detpt.y > selected_rect_.top && detpt.y < selected_rect_.bottom)) {
    return;
  }
  if (drawing_mode_ == drawingarrow) {
    double theta = 3.1415926 / (pen_width_ * 6) * pen_width_;
    double Px, Py, P1x, P1y, P2x, P2y;
    Px = arrow_points[0].X - detpt.x;
    Py = arrow_points[0].Y - detpt.y;
    P1x = Px*cos(theta) - Py*sin(theta);
    P1y = Px*sin(theta) + Py*cos(theta);
    P2x = Px*cos(-theta) - Py*sin(-theta);
    P2y = Px*sin(-theta) + Py*cos(-theta);
    double x1, x2;
    x1 = sqrt(P1x*P1x + P1y*P1y);
    P1x = P1x * pen_width_ * 5 / x1;
    P1y = P1y * pen_width_ * 5 / x1;
    x2 = sqrt(P2x*P2x + P2y*P2y);
    P2x = P2x * pen_width_ * 5 / x2;
    P2y = P2y * pen_width_ * 5 / x2;
    P1x = P1x + detpt.x;
    P1y = P1y + detpt.y;
    P2x = P2x + detpt.x;
    P2y = P2y + detpt.y;
    double P3x, P3y, P4x, P4y;
    if (P2x > P1x) {
      P3x = P1x + ((P2x - P1x) / 3);
      P3y = (P3x - P1x)*(P2y - P1y) / (P2x - P1x) + P1y;
      P4x = P2x - ((P2x - P1x) / 3);
      P4y = (P4x - P1x)*(P2y - P1y) / (P2x - P1x) + P1y;
    } else if(P2x < P1x) {
      P3x = P1x - ((P1x - P2x)/3);
      P3y = (P3x - P1x)*(P2y - P1y) / (P2x - P1x) + P1y;
      P4x = P2x + ((P1x - P2x) / 3);
      P4y = (P4x - P1x)*(P2y - P1y) / (P2x - P1x) + P1y;
    } else {
      P3x = P4x = P2x;
      if (P2y > P1y) {
        P3y = P1y + ((P2y - P1y) / 3);
        P4y = P2y - ((P2y - P1y) / 3);
      } else {
        P3y = P1y - ((P1y - P2y) / 3);
        P4y = P2y + ((P1y - P2y) / 3);
      }
    }

    arrow_points[0] = Gdiplus::Point(oript.x, oript.y);
    arrow_points[1] = Gdiplus::Point((int)P3x, (int)P3y);
    arrow_points[2] = Gdiplus::Point((int)P1x, (int)P1y);
    arrow_points[3] = Gdiplus::Point(detpt.x, detpt.y);
    arrow_points[4] = Gdiplus::Point((int)P2x, (int)P2y);
    arrow_points[5] = Gdiplus::Point((int)P4x, (int)P4y);
    InvalidateRect(selected_rect_, false);
  }
}

void CarrowDlg::DrawRectangle(Gdiplus::Point rectangle_points[], CPoint oript, CPoint detpt) {
  if (!(oript.x >= selected_rect_.left && oript.x <= selected_rect_.right && oript.y > selected_rect_.top && oript.y < selected_rect_.bottom)) {
    return;
  }
  if (!(detpt.x >= selected_rect_.left && detpt.x <= selected_rect_.right && detpt.y > selected_rect_.top && detpt.y < selected_rect_.bottom)) {
    return;
  }
  if (drawing_mode_ == drawingrectangle) {
    rectangle_points[0] = Gdiplus::Point(oript.x, oript.y);
    rectangle_points[1] = Gdiplus::Point(detpt.x, oript.y);
    rectangle_points[2] = Gdiplus::Point(detpt.x, detpt.y);
    rectangle_points[3] = Gdiplus::Point(oript.x, detpt.y);
    InvalidateRect(selected_rect_, false);
  }
}

void CarrowDlg::DrawEclipse(CPoint oript, CPoint detpt) {
  if (!(oript.x >= selected_rect_.left && oript.x <= selected_rect_.right && oript.y > selected_rect_.top && oript.y < selected_rect_.bottom)) {
    return;
  }
  if (!(detpt.x >= selected_rect_.left && detpt.x <= selected_rect_.right && detpt.y > selected_rect_.top && detpt.y < selected_rect_.bottom)) {
    return;
  }
  if (drawing_mode_ == drawingellipse) {
    InvalidateRect(selected_rect_, false);
  }
}


void CarrowDlg::DrawLine(CPoint oript, CPoint detpt) {
  if (!(oript.x >= selected_rect_.left && oript.x <= selected_rect_.right && oript.y > selected_rect_.top && oript.y < selected_rect_.bottom)) {
    return;
  }
  if (!(detpt.x >= selected_rect_.left && detpt.x <= selected_rect_.right && detpt.y > selected_rect_.top && detpt.y < selected_rect_.bottom)) {
    return;
  }

  gpath_line_->AddLine(Gdiplus::Point(oript.x, oript.y), Gdiplus::Point(detpt.x, detpt.y));
  CDC* cdc = GetDC();
  Gdiplus::Graphics g(cdc->GetSafeHdc());
  Gdiplus::Pen pen(pen_color_);
  pen.SetWidth(pen_width_);
  g.SetSmoothingMode(SmoothingModeAntiAlias);
  g.DrawPath(&pen, gpath_line_);
}

// Line
void CarrowDlg::OnBnClickedButton1() {
  draw_mode_ = line;
}

// Arrow
void CarrowDlg::OnBnClickedButton2() {
  draw_mode_ = arrow;
}

// Rectangle
void CarrowDlg::OnBnClickedButton4() {
  draw_mode_ = rectangle;
}

// Eclipse
void CarrowDlg::OnBnClickedButton3() {
  draw_mode_ = ellipse;
}

// Clear
void CarrowDlg::OnBnClickedButton5() {
  is_clear_ = true;
  Invalidate();
}

void CarrowDlg::OnDestroy() {
  if (gpath_line_) {
    delete gpath_line_;
  }

  for (size_t i = 0; i < vec_drawcells_.size(); i++) {
    if (vec_drawcells_[i].drawpath) {
      delete vec_drawcells_[i].drawpath;
    }
  }

  CDialogEx::OnDestroy();
}

void CarrowDlg::OnBnClickedButton6() {
   CColorDialog dlg;
   if (IDOK == dlg.DoModal()) {
     COLORREF color = dlg.GetColor();
     pen_color_ = Color(GetRValue(color), GetGValue(color), GetBValue(color));
   }
   RECT rt;
   ((CStatic*)GetDlgItem(IDC_PENCOLOR))->GetClientRect(&rt);
   CDC* cdc = GetDC();
   Graphics g(cdc->GetSafeHdc());
   Gdiplus::SolidBrush sbrush(pen_color_);
   g.FillRectangle(&sbrush, 521, 475, rt.right - rt.left, rt.bottom - rt.top);
}


void CarrowDlg::OnBnClickedButton7() {
  if (pen_width_ > 10) {
    return;
  }
  pen_width_ = pen_width_ + 1;
  CString temp_string;
  temp_string.Format(L"%d", (int)pen_width_);
  ((CEdit*)GetDlgItem(IDC_PENWIDTH))->SetWindowText(temp_string);
}


void CarrowDlg::OnBnClickedButton8() {
  if (pen_width_ < 2) {
    return;
  }
  pen_width_ = pen_width_ - 1;
  CString temp_string;
  temp_string.Format(L"%d", (int)pen_width_);
  ((CEdit*)GetDlgItem(IDC_PENWIDTH))->SetWindowText(temp_string);
}


void CarrowDlg::OnStnClickedPencolor()
{
  CColorDialog dlg;
  if (IDOK == dlg.DoModal()) {
    COLORREF color = dlg.GetColor();
    pen_color_ = Color(GetRValue(color), GetGValue(color), GetBValue(color));
  }
  RECT rt;
  ((CStatic*)GetDlgItem(IDC_PENCOLOR))->GetClientRect(&rt);
  CDC* cdc = GetDC();
  Graphics g(cdc->GetSafeHdc());
  Gdiplus::SolidBrush sbrush(pen_color_);
  g.FillRectangle(&sbrush, 521, 475, rt.right - rt.left, rt.bottom - rt.top);
}
