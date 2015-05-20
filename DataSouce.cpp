#include "DataSouce.h"
#include <vcl.h>
#include <windows.h>

#include <Registry.hpp>
#include <memory>
using namespace std;

/*
 *  检查文件是否存在.
 */
bool CheckFilePath(UnicodeString &s)
{
	// 获取当前路径.
	UnicodeString sPath = GetCurrentDir();
	sPath += "\\";
	sPath += Date().FormatString("yyyy-mm-dd");
	sPath += ".mdb";
	s = sPath;

	// 检查文件是否存在.
	return FileExists(sPath);
}

// 获得名称列表.
void TestProcedure::InitGetNameList(void)
{
	std::vector<DataItemSet*>::iterator it;

	m_sNameList = "(";
	for (it = m_vDataSet.begin(); it != m_vDataSet.end(); ++it)
	{
		(*it)->NameGroupToString(m_sNameList);
	}
	m_sNameList.Delete(m_sNameList.Length(), 1);
	m_sNameList += ")";

	for (it = m_vDataSet.begin(); it != m_vDataSet.end(); ++it)
	{
		(*it)->NameGroupToVector(vColumnStrList);
	}
}

void TestProcedure::UpdateValueList(void)
{
	std::vector<DataItemSet*>::iterator it;
	m_sValueList = "(";
	for (it = m_vDataSet.begin(); it != m_vDataSet.end(); ++it)
	{
		(*it)->ValueGroupToString(m_sValueList);
	}

	m_sValueList.Delete(m_sValueList.Length(), 1);
	m_sValueList += ")";
}

void TestProcedure::SqlBuilderCreateTable(const UnicodeString & sTableName)
{
	std::vector<UnicodeString>v;
	std::vector<UnicodeString*>::const_iterator is;

	m_sSqlCreateTable = "create table";
	m_sSqlCreateTable += " " + sTableName + " ";
	m_sSqlCreateTable += "(id int IDENTITY(1,1) primary key,记录时间 datetime default now(),";

	for (is = vColumnStrList.begin(); is < vColumnStrList.end(); ++is)
	{
		m_sSqlCreateTable += *(*is) + " double,";
	}

	// 删除最后一个","
	m_sSqlCreateTable.Delete(m_sSqlCreateTable.Length(), 1);
	m_sSqlCreateTable += ")";
}

TestProcedure::TestProcedure(void)
{
	m_vDataSet = std::vector<DataItemSet*>();
	m_sNameList = "";
	m_sValueList = "";
	vColumnStrList = std::vector<UnicodeString*>();
	m_sSqlTableColumnList = "";
	m_sSqlCreateTable = "";

	for (int i = 0; i < TestGroupNum; ++i)
	{
		DataItemSet *pds = new DataItemSet(i);
		m_vDataSet.push_back(pds);
	}
	InitGetNameList(); // 初始化数据列表.
	// UpdateValueList(); // 初始化数据值表.
}

DataItemSet& TestProcedure:: operator[](int nIndex) const
{
	if (nIndex < 0 || nIndex >= static_cast<int>(m_vDataSet.size()))
	{
		std::out_of_range e("indexer out of range");
		throw e;
	}

	return *(const_cast<DataItemSet*>(m_vDataSet[nIndex]));
}

int TestProcedure::Length(void) const
{
	return (int)(m_vDataSet.size());
}

TestProcedure::~TestProcedure(void)
{
	std::vector<DataItemSet*>::iterator dit;
	std::vector<UnicodeString*>::iterator sit;

	for (dit = m_vDataSet.begin(); dit != m_vDataSet.end(); ++dit)
	{
		delete *dit;
	}

	for (sit = vColumnStrList.begin(); sit != vColumnStrList.end(); ++sit)
	{
		delete *sit;
	}

	m_vDataSet.clear();
	vColumnStrList.clear();
}

void TestProcedure::SqlCreateTable(const UnicodeString & sTableName,
	UnicodeString &sql)
{
	if (sTableName != m_sName)
	{
		SqlBuilderCreateTable(sTableName); // 创建列表的名称.
	}

	sql = UnicodeString(m_sSqlCreateTable);
}

void TestProcedure::SqlInsertTableValue(const UnicodeString &sTableName,
	UnicodeString &sql)
{

	UpdateValueList();

	sql = UnicodeString("insert into " + sTableName + " " + m_sNameList +
		" values " + m_sValueList);
}


bool TestProcedure::DataUpdated(void)
{
	std::vector<DataItemSet*>::iterator dit;

	for (dit = m_vDataSet.begin(); dit != m_vDataSet.end(); ++dit)
	{
		if((*dit)->GetUpdatedFlag() == false)
		{
			return false;
		}
	}

	return true;
}

void TestProcedure::ResetUpdated(void)
{

	std::vector<DataItemSet*>::iterator dit;

	for (dit = m_vDataSet.begin(); dit != m_vDataSet.end(); ++dit)
	{
		(*dit)->ResetUpdatedFlag();
	}
}
