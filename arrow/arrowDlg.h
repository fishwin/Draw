

#pragma once

#include <vector>

class CarrowDlg : public CDialogEx
{
public:
	CarrowDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARROW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
  afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
  enum DrawMode {
      inval = 0,
      line,
      arrow,
      rectangle,
      ellipse
  };
  enum DrawingMode {
      invalid = 0,
      drawingline,
      drawingarrow,
      drawingrectangle,
      drawingellipse
  };

  struct DrawCell {
    Color pencolor;
    REAL  penwidth;
    Gdiplus::GraphicsPath* drawpath;
    DrawCell(Color c, REAL w, Gdiplus::GraphicsPath* gp) {
      pencolor = c;
      penwidth = w;
      drawpath = new Gdiplus::GraphicsPath;
      drawpath->AddPath(gp, FALSE);
    }
  };

  std::vector<DrawCell> vec_drawcells_;
  std::vector<DrawCell> vec_arrow_drawcells_;
  Color pen_color_;
  REAL  pen_width_;
  DrawMode draw_mode_;
  DrawingMode drawing_mode_;
  bool is_clear_;
  Gdiplus::Point arrow_points_[6];
  Gdiplus::Point rectangle_points_[4];
  CPoint start_pt_;
  CPoint old_pt_;
  CRect  selected_rect_;
  HCURSOR h_cur_;

private:
  void DrawArrow(Gdiplus::Point arrow_points[], CPoint oript, CPoint detpt);
  void DrawRectangle(Gdiplus::Point rectangle_points[], CPoint oript, CPoint detpt);
  void DrawEclipse(CPoint oript, CPoint detpt);
  void DrawLine(CPoint oript, CPoint detpt);

public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  // Line
  afx_msg void OnBnClickedButton1();
  // Arrow
  afx_msg void OnBnClickedButton2();
  // Rectangle
  afx_msg void OnBnClickedButton4();
  // Eclipse
  afx_msg void OnBnClickedButton3();
  // Clear
  afx_msg void OnBnClickedButton5();

  afx_msg void OnBnClickedButton6();
  afx_msg void OnBnClickedButton7();
  afx_msg void OnBnClickedButton8();

private:
  Gdiplus::GraphicsPath* gpath_line_;
public:
  afx_msg void OnStnClickedPencolor();
};
