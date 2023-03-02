#include "pch.h"
#include "rolepartcallback.h"
#include "rolemanager.h"
#include "util/timespecificmgr.h"

CRolePartCallBack::CRolePartCallBack()
{
	m_dwState = -1;
	m_dwSliceCount = 0;
	m_dwTimeSpecificHandle = 0;
}

CRolePartCallBack::~CRolePartCallBack()
{
	Uninit();
}

bool CRolePartCallBack::Init(const TimeSpecific& time, FuncRolePartCallBack funcCallBack)
{
	m_dwState = eCopyRoleManager;
	m_dwSliceCount = PARTROLE_SLICECOUNT;
	m_funcCallBack = funcCallBack;
	m_dwTimeSpecificHandle = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CRolePartCallBack::CallBack, this));
	return true;
}

void CRolePartCallBack::Uninit()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_dwTimeSpecificHandle);
}

bool CRolePartCallBack::CallBack()
{
	switch(m_dwState)
	{
	case eCopyRoleManager:
		{
			m_queRoleId.clear();
			for (auto i = RoleManager::Instance()->m_RoleDatas.begin(); i != RoleManager::Instance()->m_RoleDatas.end(); ++i)
			{
				m_queRoleId.push_back((*i)->GetID());
			}
			m_dwState = ePartCallBack;
			return false;
		}
	case ePartCallBack:
		{
			UINT32 i = 0;
			while (!m_queRoleId.empty())
			{
				m_funcCallBack(m_queRoleId.front());
				m_queRoleId.pop_front();
				if (++i >= m_dwSliceCount)
				{
					return false;
				}
			}
			m_dwState = eCopyRoleManager;
			return true;
		}
	default:
		break;
	}
	return true;
}