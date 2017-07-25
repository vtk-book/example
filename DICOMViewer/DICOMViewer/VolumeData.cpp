#include "stdafx.h"
#include "VolumeData.h"

VolumeData::VolumeData()
{
	m_CurrentPresetMode = MIP;

	for( int i = 0; i < 3; i++ ) m_SliceIndex[i] = 0;
}


VolumeData::~VolumeData()
{
}

void VolumeData::ReadyForVolumeRendering()
{
	// Volume Mapper 준비
	vtkSmartPointer<vtkSmartVolumeMapper> smartMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	smartMapper->SetInputData( m_ImageData );

	// 투명도 함수, 컬러 함수 준비
	double scalarRange[2];
	m_ImageData->GetScalarRange( scalarRange );
	m_OpacityFunc = vtkSmartPointer<vtkPiecewiseFunction>::New();
	m_OpacityFunc->AdjustRange( scalarRange );
	m_ColorFunc = vtkSmartPointer<vtkColorTransferFunction>::New();
	
	// Volume 속성 준비
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetScalarOpacity( m_OpacityFunc );
	volumeProperty->SetColor( m_ColorFunc );
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();

	// Volume 회전 변환 
	double origin[3];
	m_ImageData->GetOrigin( origin );
	vtkSmartPointer<vtkTransform> userTransform = vtkSmartPointer<vtkTransform>::New();
	userTransform->Translate( origin );
	userTransform->Concatenate( GetOrientation() );
	userTransform->Translate( -origin[0], -origin[1], -origin[2] );
	userTransform->Update();

	// Volume 렌더링 객체 생성
	m_VolumeRendering = vtkSmartPointer<vtkVolume>::New();
	m_VolumeRendering->SetMapper( smartMapper );
	m_VolumeRendering->SetProperty( volumeProperty );
	m_VolumeRendering->SetUserTransform( userTransform );

	// 최대 밝기값 기준 렌더링 모드 준비
	SetCurrentPresetMode( MIP );
}

void VolumeData::SetCurrentPresetMode( int val )
{
	// Volume Rendering 준비 여부
	if( m_VolumeRendering == NULL ) return;

	// 현재 모드 설정
	m_CurrentPresetMode = val;

	// Volume Mapper 가져오기
	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = 
		vtkSmartVolumeMapper::SafeDownCast( m_VolumeRendering->GetMapper() );
	if( volumeMapper == NULL ) return;

	// 범위
	double scalarRange[2];
	m_ImageData->GetScalarRange( scalarRange );
	double nMin= scalarRange[0];
	double nMax= scalarRange[1];

	// 초기화
	m_ColorFunc->RemoveAllPoints();
	m_OpacityFunc->RemoveAllPoints();
	
	// 투명도 함수 및 컬러 함수 설정
	switch( m_CurrentPresetMode ) {
	case MIP:
		// 최대 밝기값 기준 연속적인 투명도 함수 설정
 		m_ColorFunc->AddRGBPoint( nMin, 1.0, 1.0, 1.0 );
 		m_ColorFunc->AddRGBPoint( nMax, 1.0, 1.0, 1.0 );

		m_OpacityFunc->AddPoint( nMin, 0.0 );
		m_OpacityFunc->AddPoint( nMax, 1.0 );

		// 최대 밝기 모드로 블렌드 모드 설정
		volumeMapper->SetBlendModeToMaximumIntensity();
		break;
	case SKIN:
		// 피부가 잘 보이는 밝기 값에 대해 색 및 투명도 설정
		m_ColorFunc->AddRGBPoint( nMin, 0, 0, 0 );
		m_ColorFunc->AddRGBPoint( -1000, .62, .36, .18 );
		m_ColorFunc->AddRGBPoint( -500, .88, .60, .29 );
		m_ColorFunc->AddRGBPoint( nMax, .83, .66, 1 );

		m_OpacityFunc->AddPoint( nMin, 0, 0.5, 0.0 );
		m_OpacityFunc->AddPoint( -1000, 0, 0.5, 0.0 );
		m_OpacityFunc->AddPoint( -500, 1.0, 0.33, 0.45 );
		m_OpacityFunc->AddPoint( nMax, 1.0, 0.5, 0.0 );

		// 조합 모드로 블렌드 모드 설정
		volumeMapper->SetBlendModeToComposite();
		break;
	case BONE:
		// 뼈와 혈관이 잘 보이는 밝기 값에 대해 색 및 투명도 설정
		m_ColorFunc->AddRGBPoint( nMin, 0, 0, 0 );
		m_ColorFunc->AddRGBPoint( 142.68, 0, 0, 0 );
		m_ColorFunc->AddRGBPoint( 145.02, 0.62, 0.0, 0.02 );
		m_ColorFunc->AddRGBPoint( 192.17, 0.91, 0.45, 0.0 );
		m_ColorFunc->AddRGBPoint( 217.24, 0.97, 0.81, 0.61 );
		m_ColorFunc->AddRGBPoint( 384.35, 0.91, 0.91, 1.0 );
		m_ColorFunc->AddRGBPoint( nMax, 1, 1, 1 );

		m_OpacityFunc->AddPoint( nMin, 0.0 );
		m_OpacityFunc->AddPoint( 142.68, 0.0 );
		m_OpacityFunc->AddPoint( 145.02, 0.12 );
		m_OpacityFunc->AddPoint( 192.17, 0.56 );
		m_OpacityFunc->AddPoint( 217.24, 0.78 );
		m_OpacityFunc->AddPoint( 384.35, 0.83 );
		m_OpacityFunc->AddPoint( nMax, 0.83 );

		// 조합 모드로 블렌드 모드 설정
		volumeMapper->SetBlendModeToComposite();
		break;
	}
}

vtkSmartPointer<vtkImageActor> VolumeData::GetSliceActor( int sliceType )
{
	// 슬라이스 타입 검사
	if( sliceType < 0 || sliceType >= 3 ) return NULL;

	// 슬라이스 타입에 해당하는 Actor 반환
	return m_SliceActor[sliceType];
}

void VolumeData::ReadyForSliceRendering()
{
	// 슬라이스 이미지 렌더링 속성 초기화
	// DICOM 이미지 최대/최소 밝기값
	double range[2];
	m_ImageData->GetScalarRange( range );
	// Volume 인덱스 범위
	int ext[6];
	m_ImageData->GetExtent( ext );

	// DICOM 데이터에 따라 초기 Window / Level 값을 조정
	m_SliceProperty = vtkSmartPointer<vtkImageProperty>::New();
	m_SliceProperty->SetColorLevel( (range[1] + range[0]) / 2 );
	m_SliceProperty->SetColorWindow( range[1] - range[0] );

	// 각 슬라이스 타입에 따라 설정
	for( int sliceType = 0; sliceType < 3; sliceType++ ) {
		// 슬라이스 인덱스의 중간위치 계산
		switch( sliceType ) {
		case AXIAL:
			m_SliceIndex[sliceType] = (ext[4] + ext[5]) / 2;
			break;
		case CORONAL:
			m_SliceIndex[sliceType] = (ext[2] + ext[3]) / 2;
			break;
		case SAGITTAL:
			m_SliceIndex[sliceType] = (ext[0] + ext[1]) / 2;
			break;
		}

		// Image Reslice 생성
		m_VolumeSlice[sliceType] = vtkSmartPointer<vtkImageReslice>::New();
		m_VolumeSlice[sliceType]->SetInputData( m_ImageData );
		m_VolumeSlice[sliceType]->SetOutputDimensionality( 2 );		// 2D로 슬라이스
		m_VolumeSlice[sliceType]->SetResliceAxes( 
			GetResliceMatrix( sliceType, m_SliceIndex[sliceType] ) );	// 슬라이스 행렬 설정
		m_VolumeSlice[sliceType]->Update();

		// 이미지 Actor 생성
		m_SliceActor[sliceType] = vtkSmartPointer<vtkImageActor>::New();
		m_SliceActor[sliceType]->GetMapper()
			->SetInputData( m_VolumeSlice[sliceType]->GetOutput() );

		// 각 슬라이스에 공통된 이미지 속성 정의
		m_SliceActor[sliceType]->SetProperty( m_SliceProperty );
	}
}

vtkSmartPointer<vtkMatrix4x4> VolumeData::GetResliceMatrix( int sliceType, int sliceIdx )
{
	// 슬라이스 타입 검사
	if( sliceType < 0 || sliceType >= 3 ) return NULL;

	// Slice 타입에 따른 Orientation 행렬
	double sliceMat[3][16] = {
		{ // Axial 축 행렬
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		},
		{ // Coronal 축 행렬
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, -1, 0, 0,
			0, 0, 0, 1
		},
		{ // Sagittal 축 행렬
			0, 0, -1, 0,
			1, 0, 0, 0,
			0, -1, 0, 0,
			0, 0, 0, 1
		}
	};

	// Slice 행렬 생성
	vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
	mat->DeepCopy( sliceMat[sliceType] );

	// DICOM Volume 데이터 가져오기
	double origin[3];
	double bounds[6];
	int ext[6];
	m_ImageData->GetOrigin( origin );		// Volume 원점
	m_ImageData->GetBounds( bounds );	// Volume 위치 범위
	m_ImageData->GetExtent( ext );			// Volume 인덱스 범위

	// Slice 행렬의 위치를 원점으로 초기화
	for( int i = 0; i < 3; i++ ) mat->SetElement( i, 3, origin[i] );
	
	// Slice 인덱스에 따른 위치 계산
	double pos;
	switch( sliceType ) {
	case AXIAL:
		// 슬라이스 범위를 벗어나면 최대/최소 값으로 설정
		if( sliceIdx < ext[4] ) sliceIdx = ext[4];
		if( sliceIdx > ext[5] ) sliceIdx = ext[5];
		// slice 인덱스의 실제 위치 계산
		pos = bounds[4] + (bounds[5] - bounds[4]) * (double)(sliceIdx / (double)ext[5]);
		// z축 위치를 해당하는 slice 인덱스의 위치로 설정
		mat->SetElement( 2, 3, pos );
		break;
	case CORONAL:
		// 슬라이스 범위를 벗어나면 최대/최소 값으로 설정
		if( sliceIdx < ext[2] ) sliceIdx = ext[2];
		if( sliceIdx > ext[3] ) sliceIdx = ext[3];
		// slice 인덱스의 실제 위치 계산
		pos = bounds[2] + (bounds[3] - bounds[2]) * (double)(sliceIdx / (double)ext[3]);
		// y축 위치를 해당하는 slice 인덱스의 위치로 설정
		mat->SetElement( 1, 3, pos );
		break;
	case SAGITTAL:
		// 슬라이스 범위를 벗어나면 최대/최소 값으로 설정
		if( sliceIdx < ext[0] ) sliceIdx = ext[0];
		if( sliceIdx > ext[1] ) sliceIdx = ext[1];
		// slice 인덱스의 실제 위치 계산
		pos = bounds[0] + (bounds[1] - bounds[0]) * (double)(sliceIdx / (double)ext[1]);
		// x축 위치를 해당하는 slice 인덱스의 위치로 설정
		mat->SetElement( 0, 3, pos );
		break;
	}

	return mat;
}

int VolumeData::GetSliceIndex( int sliceType )
{
	// 슬라이스 타입 검사
	if( sliceType < 0 || sliceType >= 3 ) return 0;

	// 현재 슬라이스 인덱스 반환
	return m_SliceIndex[sliceType];
}

void VolumeData::SetSliceIndex( int sliceType, int sliceIndex )
{
	// 슬라이스 타입 검사
	if( sliceType < 0 || sliceType >= 3 ) return;

	// 현재 슬라이스 인덱스 설정
	m_SliceIndex[sliceType] = sliceIndex;

	// 슬라이스 행렬 업데이트
	m_VolumeSlice[sliceType]->SetResliceAxes( 
		GetResliceMatrix( sliceType, m_SliceIndex[sliceType] ) );
	m_VolumeSlice[sliceType]->Update();
}
