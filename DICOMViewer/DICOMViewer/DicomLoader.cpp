#include "stdafx.h"
#include "DicomLoader.h"
#include "DVManager.h"

#include <vtkDirectory.h>
#include <vtkStringArray.h>

#include <vtkGDCMImageReader.h>
#include <gdcmReader.h>
#include <gdcmGlobal.h>
#include <gdcmTag.h>
#include <gdcmStringFilter.h>
#include <gdcmSplitMosaicFilter.h>
#include <gdcmIPPSorter.h>

#include "DicomGroup.h"


DicomLoader::DicomLoader()
{
}

DicomLoader::~DicomLoader()
{
}

void DicomLoader::OpenDicomDirectory( const char* dirPath )
{
	// DICOM 폴더 내의 모든 파일을 추가
	vtkSmartPointer<vtkDirectory> vtkDir = vtkSmartPointer<vtkDirectory>::New();
	vtkDir->Open( dirPath );
	for( int i = 0; i < vtkDir->GetNumberOfFiles(); i++ ) {
		const char *filename = vtkDir->GetFile( i );
		// 폴더가 아니라 파일일 때
		if( !vtkDir->FileIsDirectory( filename ) ) {
			std::string filePath = dirPath;
			filePath += "\\";
			filePath += filename;

			// DICOM 파일 추가
			AddDicomFile( filePath.c_str() );
		}
	}
}

void DicomLoader::AddDicomFile( const char *filePath )
{
	// GDCM으로 DICOM 파일 읽기
	gdcm::Reader reader;
	reader.SetFileName( filePath );
	if( !reader.Read() ) return;

	// DICOM 파일에서 Tag 정보를 읽어올 준비
	gdcm::File &file = reader.GetFile();
	gdcm::DataSet &ds = file.GetDataSet();
	gdcm::StringFilter sf;
	sf.SetFile( file );

	// Group 분류를 위한 태그 정보 읽기 (Patient ID, Study ID, Series Num)
	gdcm::Tag tagPatientID( 0x0010, 0x0020 );
	gdcm::Tag tagStudyID( 0x0020, 0x0010 );
	gdcm::Tag tagSeriesNum( 0x0020, 0x0011 );

	std::string patientID;
	std::string studyID;
	std::string seriesNum;

	if( ds.FindDataElement( tagPatientID ) ) patientID = sf.ToString( tagPatientID );
	if( ds.FindDataElement( tagStudyID ) ) studyID = sf.ToString( tagStudyID );
	if( ds.FindDataElement( tagSeriesNum ) ) seriesNum = sf.ToString( tagSeriesNum );

	// DICOM 파일이 포함될 그룹 찾기
	vtkSmartPointer<DicomGroup> group;
	for( int i = 0; i < m_GroupList.size(); i++ ) {
		if( m_GroupList[i]->IsSameGroup( patientID, studyID, seriesNum ) ) {
			group = m_GroupList[i];
			break;
		}
	}

	// 못 찾으면 생성
	if( group == NULL ) {
		group = vtkSmartPointer<DicomGroup>::New();
		group->SetPatientID( patientID );
		group->SetStudyID( studyID );
		group->SetSeriesNum( seriesNum );

		m_GroupList.push_back( group );
	}

	// 그룹에 파일 추가
	group->AddImageFile( filePath );
}

vtkSmartPointer<DicomGroup> DicomLoader::GetDicomGroup( int idx )
{
	// 그룹 목록 범위 검사
	if( idx < -1 || idx >= (int)m_GroupList.size() ) return NULL;

	// 해당하는 DICOM 그룹 반환
	return m_GroupList[idx];
}

void DicomLoader::LoadVolumeData( vtkSmartPointer<DicomGroup> dicomGroup )
{
	// DICOM 그룹 검사
	if( dicomGroup == NULL ) return;

	// 현재 선택된 그룹 저장
	m_CurrentGroup = dicomGroup;

	// 선택된 그룹의 파일 개수 검사
	int numFiles = (int)dicomGroup->GetFileList().size();
	if( numFiles == 0 ) return;

	// DICOM 파일 정렬 (Image Position (Patient) 태그 기준)
	gdcm::IPPSorter ippSorter;
	bool bSortSuccess = ippSorter.Sort( dicomGroup->GetFileList() );
	
	// 정렬이 성공하면 정렬된 파일 목록을 실패하면 기존 파일 목록을 로드
	std::vector<std::string> sortedFileNames;
	if( bSortSuccess ) sortedFileNames = ippSorter.GetFilenames();
	else sortedFileNames = dicomGroup->GetFileList();

	// vtkStringArray 타입으로 변환
	vtkSmartPointer<vtkStringArray> fileArray = vtkSmartPointer<vtkStringArray>::New();
	for( int i = 0; i < (int)sortedFileNames.size(); i++ ) 
		fileArray->InsertNextValue( sortedFileNames[i].c_str() );

	// GDCM Image Reader를 이용하여 DICOM 이미지 로딩
	vtkSmartPointer<vtkGDCMImageReader> dcmReader = 
		vtkSmartPointer<vtkGDCMImageReader>::New();	
	// 이미지를 아래에서 위로 읽음
	dcmReader->FileLowerLeftOn();
	// 파일 목록이 1개 이상
	if( numFiles > 1 ) dcmReader->SetFileNames( fileArray );
	// 파일 목록이 1개 (Mosaic Image일 가능성 있음)
	else dcmReader->SetFileName( fileArray->GetValue( 0 ) );
	// 이미지 로더 업데이트
	dcmReader->Update();

	// Volume Data 새로 생성
	m_VolumeData = vtkSmartPointer<VolumeData>::New();
	m_VolumeData->SetImageData( dcmReader->GetOutput() );
	m_VolumeData->SetOrientation( dcmReader->GetDirectionCosines() );
	
	// IPP 정렬 성공 시, IPPSorter에서 계산된 z-spacing으로 업데이트
	if( bSortSuccess ) {
		double spacing[3];
		m_VolumeData->GetImageData()->GetSpacing( spacing );
		spacing[2] = ippSorter.GetZSpacing();
		m_VolumeData->GetImageData()->SetSpacing( spacing );
	}

	// Volume 렌더링 준비
	m_VolumeData->ReadyForVolumeRendering();

	// Slice 렌더링 준비
	m_VolumeData->ReadyForSliceRendering();
}
