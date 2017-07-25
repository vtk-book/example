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
	// DICOM ���� ���� ��� ������ �߰�
	vtkSmartPointer<vtkDirectory> vtkDir = vtkSmartPointer<vtkDirectory>::New();
	vtkDir->Open( dirPath );
	for( int i = 0; i < vtkDir->GetNumberOfFiles(); i++ ) {
		const char *filename = vtkDir->GetFile( i );
		// ������ �ƴ϶� ������ ��
		if( !vtkDir->FileIsDirectory( filename ) ) {
			std::string filePath = dirPath;
			filePath += "\\";
			filePath += filename;

			// DICOM ���� �߰�
			AddDicomFile( filePath.c_str() );
		}
	}
}

void DicomLoader::AddDicomFile( const char *filePath )
{
	// GDCM���� DICOM ���� �б�
	gdcm::Reader reader;
	reader.SetFileName( filePath );
	if( !reader.Read() ) return;

	// DICOM ���Ͽ��� Tag ������ �о�� �غ�
	gdcm::File &file = reader.GetFile();
	gdcm::DataSet &ds = file.GetDataSet();
	gdcm::StringFilter sf;
	sf.SetFile( file );

	// Group �з��� ���� �±� ���� �б� (Patient ID, Study ID, Series Num)
	gdcm::Tag tagPatientID( 0x0010, 0x0020 );
	gdcm::Tag tagStudyID( 0x0020, 0x0010 );
	gdcm::Tag tagSeriesNum( 0x0020, 0x0011 );

	std::string patientID;
	std::string studyID;
	std::string seriesNum;

	if( ds.FindDataElement( tagPatientID ) ) patientID = sf.ToString( tagPatientID );
	if( ds.FindDataElement( tagStudyID ) ) studyID = sf.ToString( tagStudyID );
	if( ds.FindDataElement( tagSeriesNum ) ) seriesNum = sf.ToString( tagSeriesNum );

	// DICOM ������ ���Ե� �׷� ã��
	vtkSmartPointer<DicomGroup> group;
	for( int i = 0; i < m_GroupList.size(); i++ ) {
		if( m_GroupList[i]->IsSameGroup( patientID, studyID, seriesNum ) ) {
			group = m_GroupList[i];
			break;
		}
	}

	// �� ã���� ����
	if( group == NULL ) {
		group = vtkSmartPointer<DicomGroup>::New();
		group->SetPatientID( patientID );
		group->SetStudyID( studyID );
		group->SetSeriesNum( seriesNum );

		m_GroupList.push_back( group );
	}

	// �׷쿡 ���� �߰�
	group->AddImageFile( filePath );
}

vtkSmartPointer<DicomGroup> DicomLoader::GetDicomGroup( int idx )
{
	// �׷� ��� ���� �˻�
	if( idx < -1 || idx >= (int)m_GroupList.size() ) return NULL;

	// �ش��ϴ� DICOM �׷� ��ȯ
	return m_GroupList[idx];
}

void DicomLoader::LoadVolumeData( vtkSmartPointer<DicomGroup> dicomGroup )
{
	// DICOM �׷� �˻�
	if( dicomGroup == NULL ) return;

	// ���� ���õ� �׷� ����
	m_CurrentGroup = dicomGroup;

	// ���õ� �׷��� ���� ���� �˻�
	int numFiles = (int)dicomGroup->GetFileList().size();
	if( numFiles == 0 ) return;

	// DICOM ���� ���� (Image Position (Patient) �±� ����)
	gdcm::IPPSorter ippSorter;
	bool bSortSuccess = ippSorter.Sort( dicomGroup->GetFileList() );
	
	// ������ �����ϸ� ���ĵ� ���� ����� �����ϸ� ���� ���� ����� �ε�
	std::vector<std::string> sortedFileNames;
	if( bSortSuccess ) sortedFileNames = ippSorter.GetFilenames();
	else sortedFileNames = dicomGroup->GetFileList();

	// vtkStringArray Ÿ������ ��ȯ
	vtkSmartPointer<vtkStringArray> fileArray = vtkSmartPointer<vtkStringArray>::New();
	for( int i = 0; i < (int)sortedFileNames.size(); i++ ) 
		fileArray->InsertNextValue( sortedFileNames[i].c_str() );

	// GDCM Image Reader�� �̿��Ͽ� DICOM �̹��� �ε�
	vtkSmartPointer<vtkGDCMImageReader> dcmReader = 
		vtkSmartPointer<vtkGDCMImageReader>::New();	
	// �̹����� �Ʒ����� ���� ����
	dcmReader->FileLowerLeftOn();
	// ���� ����� 1�� �̻�
	if( numFiles > 1 ) dcmReader->SetFileNames( fileArray );
	// ���� ����� 1�� (Mosaic Image�� ���ɼ� ����)
	else dcmReader->SetFileName( fileArray->GetValue( 0 ) );
	// �̹��� �δ� ������Ʈ
	dcmReader->Update();

	// Volume Data ���� ����
	m_VolumeData = vtkSmartPointer<VolumeData>::New();
	m_VolumeData->SetImageData( dcmReader->GetOutput() );
	m_VolumeData->SetOrientation( dcmReader->GetDirectionCosines() );
	
	// IPP ���� ���� ��, IPPSorter���� ���� z-spacing���� ������Ʈ
	if( bSortSuccess ) {
		double spacing[3];
		m_VolumeData->GetImageData()->GetSpacing( spacing );
		spacing[2] = ippSorter.GetZSpacing();
		m_VolumeData->GetImageData()->SetSpacing( spacing );
	}

	// Volume ������ �غ�
	m_VolumeData->ReadyForVolumeRendering();

	// Slice ������ �غ�
	m_VolumeData->ReadyForSliceRendering();
}
