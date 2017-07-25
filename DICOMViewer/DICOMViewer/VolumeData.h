#pragma once

#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageReslice.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageProperty.h>

class VolumeData :
	public vtkObject
{
public:
	/// vtkSmartPointer를 통한 생성 및 해제
	vtkTypeMacro( VolumeData, vtkObject );
	static VolumeData *New() { return new VolumeData; };

protected:
	/// New() 함수 이외의 생성 방지
	VolumeData();
	virtual ~VolumeData();
	
public:
	// Volume Slice 타입
	enum SliceType { AXIAL, CORONAL, SAGITTAL };

	// 미리 정의된 Volume 렌더링 투명도 함수 및 컬러 함수
	enum RenderingPreset { MIP, SKIN, BONE, BONE_ARTERIES };

protected:
	/// VTK 3차원 이미지 (Volume) 데이터
	vtkSmartPointer<vtkImageData> m_ImageData;

	/// Volume 데이터의 회전이동 정보
	vtkSmartPointer<vtkMatrix4x4> m_Orientation;

	/// Volume 렌더링 객체
	vtkSmartPointer<vtkVolume> m_VolumeRendering;
	
	/// Volume 렌더링 투명도 함수
	vtkSmartPointer<vtkPiecewiseFunction> m_OpacityFunc;

	/// Volume 렌더링 컬러 함수
	vtkSmartPointer<vtkColorTransferFunction> m_ColorFunc;

	/// 현재 Volume 렌더링 모드
	int m_CurrentPresetMode;

	/// Slice 렌더링 객체 (Axial, Coronal, Sagittal 슬라이스)
	vtkSmartPointer<vtkImageActor> m_SliceActor[3];

	/// Slice 렌더링을 위한 Volume 단면 
	vtkSmartPointer<vtkImageReslice> m_VolumeSlice[3];

	/// Slice 렌더링 속성
	vtkSmartPointer<vtkImageProperty> m_SliceProperty;

	/// Slice 인덱스
	int m_SliceIndex[3];

public:	
	/// VTK Volume 데이터 가져오기/설정하기
	vtkSmartPointer<vtkImageData> GetImageData() const { return m_ImageData; }
	void SetImageData( vtkSmartPointer<vtkImageData> val ) { m_ImageData = val; }
	
	/// Volume 데이터의 회전이동 정보 가져오기/설정하기
	vtkSmartPointer<vtkMatrix4x4> GetOrientation() const { return m_Orientation; }
	void SetOrientation( vtkSmartPointer<vtkMatrix4x4> val ) { m_Orientation = val; }

	/// Volume 렌더링 객체
	vtkSmartPointer<vtkVolume> GetVolumeRendering() const { return m_VolumeRendering; }

	/// 3D Volume 렌더링 준비
	void ReadyForVolumeRendering();
	
	/// 현재 Volume 렌더링 모드
	int GetCurrentPresetMode() const { return m_CurrentPresetMode; }

	/// Volume 렌더링 모드 설정
	void SetCurrentPresetMode( int val );

	/// Slice 렌더링 객체
	vtkSmartPointer<vtkImageActor> GetSliceActor( int sliceType );

	/// Slice 렌더링 준비
	void ReadyForSliceRendering();

	/// Slice 타입과 인덱스에 따른 Reslice 행렬 계산
	vtkSmartPointer<vtkMatrix4x4> GetResliceMatrix( int sliceType, int sliceIdx );
	
	/// 현재 Slice 인덱스
	int GetSliceIndex( int sliceType );

	/// Slice 인덱스 설정
	void SetSliceIndex( int sliceType, int sliceIndex );
};
