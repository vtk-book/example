#pragma once

#include <vector>
#include <string>

#include <vtkObject.h>
#include <vtkSmartPointer.h>

#include "DicomGroup.h"
#include "VolumeData.h"

class DicomLoader :
	public vtkObject
{
public:
	vtkTypeMacro( DicomLoader, vtkObject );
	static DicomLoader *New() { return new DicomLoader; };

protected:
	DicomLoader();
	virtual ~DicomLoader();

protected:
	/// DICOM 그룹 목록
	std::vector<vtkSmartPointer<DicomGroup>> m_GroupList;

	/// 현재 선택된 DICOM Group
	vtkSmartPointer<DicomGroup> m_CurrentGroup;

	/// 현재 선택된 Volume 데이터
	vtkSmartPointer<VolumeData> m_VolumeData;

public:
	/// DICOM 디렉토리 열기
	void OpenDicomDirectory( const char* dirPath );

	/// DICOM 파일 추가
	void AddDicomFile( const char *filePath );

	/// DICOM 그룹 개수
	int GetNumberOfGroups() { return (int)m_GroupList.size(); }

	/// DICOM 그룹
	vtkSmartPointer<DicomGroup> GetDicomGroup( int idx );

	/// 현재 선택된 DICOM Group
	vtkSmartPointer<DicomGroup> GetCurrentGroup() const { return m_CurrentGroup; }

	/// Volume 데이터
	vtkSmartPointer<VolumeData> GetVolumeData() const { return m_VolumeData; }

	/// DICOM 그룹에서 Volume 데이터 로드
	void LoadVolumeData( vtkSmartPointer<DicomGroup> dicomGroup );
};
