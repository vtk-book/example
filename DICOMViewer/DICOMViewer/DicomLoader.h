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
	/// DICOM �׷� ���
	std::vector<vtkSmartPointer<DicomGroup>> m_GroupList;

	/// ���� ���õ� DICOM Group
	vtkSmartPointer<DicomGroup> m_CurrentGroup;

	/// ���� ���õ� Volume ������
	vtkSmartPointer<VolumeData> m_VolumeData;

public:
	/// DICOM ���丮 ����
	void OpenDicomDirectory( const char* dirPath );

	/// DICOM ���� �߰�
	void AddDicomFile( const char *filePath );

	/// DICOM �׷� ����
	int GetNumberOfGroups() { return (int)m_GroupList.size(); }

	/// DICOM �׷�
	vtkSmartPointer<DicomGroup> GetDicomGroup( int idx );

	/// ���� ���õ� DICOM Group
	vtkSmartPointer<DicomGroup> GetCurrentGroup() const { return m_CurrentGroup; }

	/// Volume ������
	vtkSmartPointer<VolumeData> GetVolumeData() const { return m_VolumeData; }

	/// DICOM �׷쿡�� Volume ������ �ε�
	void LoadVolumeData( vtkSmartPointer<DicomGroup> dicomGroup );
};
