#pragma once

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRendererCollection.h>
#include <vtkCornerAnnotation.h>

#include "DicomLoader.h"

class DVManager
{
	//////////////////////////////////////////////////////////////////////////
	/// Singleton 패턴을 이용한 전역 단일 매니저 생성
	//////////////////////////////////////////////////////////////////////////
private:
	DVManager();
	virtual ~DVManager();

	static DVManager* m_Instance;
	static void Destroy() { delete m_Instance; m_Instance = NULL; }

public:
	/// 전역 매니저 객체
	static DVManager* Mgr();
	
public:
	/// View Type
	static const int NUM_VIEW = 4;
	enum ViewType { VIEW_AXIAL, VIEW_CORONAL, VIEW_SAGITTAL, VIEW_3D };

protected:
	/// Vtk Render Windows
	vtkSmartPointer<vtkRenderWindow> m_vtkWindow[NUM_VIEW];

	/// DICOM Loader
	vtkSmartPointer<DicomLoader> m_DicomLoader;

	/// 정보 표시
	vtkSmartPointer<vtkCornerAnnotation> m_Annotation[NUM_VIEW];

public:
	/// Vtk Render Windows
	vtkSmartPointer<vtkRenderWindow> GetVtkWindow( int viewType );

	/// Vtk Window 초기화 (OnInitDialog)
	void InitVtkWindow( int viewType, void* hWnd );

	/// Vtk Window 크기 조절 (OnSize)
	void ResizeVtkWindow( int viewType, int width, int height );

	/// DICOM Loader 객체
	vtkSmartPointer<DicomLoader> GetDicomLoader();

	/// View 타입에 따른 VTK 렌더러
	vtkSmartPointer<vtkRenderer> GetRenderer( int viewType );

	/// DICOM Group 선택 시, 화면 업데이트 및 초기화
	void OnSelectDicomGroup( vtkSmartPointer<DicomGroup> group );

	/// Volume 그리기 초기화
	void ClearVolumeDisplay();

	/// Volume 그리기 업데이트
	void UpdateVolumeDisplay();

	/// 스크롤바를 통한 슬라이스 인덱스 변경
	void ScrollSliceIndex( int viewType, int pos );

	/// Volume 렌더링 모드 변경
	void ChangeVolumeRenderMode( int renderMode );

	/// 정보 표시 업데이트
	void UpdateAnnotation();

	/// DICOM 슬라이스 정보 표시
	void UpdateSliceAnnotation( int viewType );
};

