#include "pch.h"
#include "activerolerecord.h"
#include "util.h"


INSTANCE_SINGLETON(CActiveRoleRecord)


CActiveRoleRecord::CActiveRoleRecord()
{
}

CActiveRoleRecord::~CActiveRoleRecord()
{
}

bool CActiveRoleRecord::Init()
{
	return true;
}

void CActiveRoleRecord::Uninit()
{
}

void CActiveRoleRecord::Record(UINT32 dwLastLogoutTime, UINT64 qwRoleID, const std::string& strAccount)
{
	RoleActiveInfo oInfo;
	oInfo.m_qwRoleID = qwRoleID;
	oInfo.m_strAccount = strAccount;
	m_oInfoMap.insert(std::make_pair(dwLastLogoutTime, oInfo));
}

void CActiveRoleRecord::GetActiveRoleTopN(UINT32 dwTopN, CActiveRoleList& roRoleList)
{
	roRoleList.clear();

	UINT32 j = 0;
	for(iterator it = Begin(); it != End() && j < dwTopN; ++it, ++j)
	{
		RoleActiveInfo& roInfo = it->second;
		roRoleList.insert(std::make_pair(roInfo.m_qwRoleID, &roInfo));
	}
}