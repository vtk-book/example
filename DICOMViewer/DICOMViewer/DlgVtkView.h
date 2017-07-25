#pragma once


// CDlgVtkView 대화 상자입니다.

class CDlgVtkView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVtkView)

public:
	CDlgVtkView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgVtkView();
	
// 대화 상자 데이터입니다.
	enum { IDD = IDD_VTK_VIEW };

protected:
	/// 이 Dialog의 View Type
	int m_ViewType;

	/// Scroll Bar 객체
	CScrollBar m_ScrollBar;

public:
	/// 이 Dialog의 View Type 얻기 / 설정
	int GetViewType() const { return m_ViewType; }
	void SetViewType( int val ) { m_ViewType = val; }
	
	/// Volume 데이터가 로드되면 각 Slice별 인덱스 범위에 따라 스크롤바 업데이트
	void UpdateScrollBar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnSize( UINT nType, int cx, int cy );
};
