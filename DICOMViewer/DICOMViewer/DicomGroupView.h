#pragma once

class CDicomGroupView :
	public CDockablePane
{
public:
	CDicomGroupView();
	virtual ~CDicomGroupView();

	/// DICOM 그룹 트리 컨트롤의 ID
	enum { ID_DICOM_GROUP_TREE = 1 };

protected:
	/// DICOM 그룹 트리 컨트롤
	CTreeCtrl m_DicomGroupTree;

	/// DICOM 그룹 트리에서 이용할 아이콘 이미지 목록
	CImageList m_Images;
	
public:
	/// DICOM Tree 업데이트
	void UpdateDicomTree();

	/// DICOM Tree에서 이름으로 트리 항목 찾기, 없으면 새로 생성
	HTREEITEM GetDicomGroupItem( CString itemText, HTREEITEM parentItem );

	/// DICOM Tree 모두 펼치기
	void ExpandAllDicomGroupTree();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnNMDblclk( NMHDR *pNMHDR, LRESULT *pResult );
};

