#include "pch.h"
#include "chat/fmmgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "chat/ptcc2m_leavelargeroom.h"
#include "chat/ptcm2c_largeroomrolentf.h"
#include "chat/ptcm2c_largeroomloginparamntf.h"
#include "chat/ptcm2f_leavefmlargeroom.h"
#include "chat/ptcm2c_kickfmauchor.h"
#include "network/fmlink.h"
#include "../config.h"
#include "shm/shmmsmgr.h"
#include "loghelper/tlogr.h"


INSTANCE_SINGLETON(FMMgr)


FMMgr::FMMgr():m_nLastTime(0),m_nCount(0){}

FMMgr::~FMMgr(){}

bool FMMgr::Init()
{	
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 60000, -1, __FILE__, __LINE__);
	Recover();
	return true;
}


void FMMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
	ShmMsMgr::Instance()->GetFmRoleShm().Uninit();
}


void FMMgr::CloseRoom()
{
	FMMgr::Instance()->ClearAudio();
	FMMgr::Instance()->SendWhiteList();
	FMMgr::Instance()->ClearRole();
}

void FMMgr::AddRole(UINT64 nRole)
{
	m_setRoleID.insert(nRole);
	CShmFMRole *pShmRole = ShmMsMgr::Instance()->GetFmRoleShm().Get(nRole);
	if (pShmRole==NULL)
	{
		LogError("can't get shm");
		return ;
	}
	pShmRole->nRole  = nRole;
}

void FMMgr::SendWhiteList()
{
	PtcM2C_LargeRoomRoleNtf ntf;
	for (auto iter = m_mapRadio.begin(); iter!=m_mapRadio.end(); iter++)
	{
		ntf.m_Data.add_name(iter->first);
		ntf.m_Data.add_roleid(iter->second);
	}
	for(auto iter = m_setRoleID.begin(); iter!= m_setRoleID.end(); iter++)
	{
		CRole  *pRole = CRoleManager::Instance()->GetByRoleID(*iter);
		if (pRole)
			pRole->Send(ntf);
	}
}


void FMMgr::ReloadWhiteList(KKSG::FmReloadInfo *data)
{
	KKSG::FMWhiteRoleList *pWhiteList    = data->mutable_whitelist();
	std::set<UINT64> setRole;
	for (size_t i = 0; i < data->kickroles_size();i++)
	{
		UINT64 nRoleID = GetAudioRoleID(data->kickroles(i));
		if (nRoleID)
		{
			CRole *pRole =	CRoleManager::Instance()->GetByRoleID(nRoleID);
			if (pRole)
			{
				PtcM2C_KickFMAuchor ntc;
				pRole->Send(ntc);
				PtcM2C_LargeRoomRoleNtf ntf;
				pRole->Send(ntf);
			}
			DelRole(nRoleID);
		}
	}
	m_mapRadio.clear();
	for (size_t i = 0; i < data->radiolist().roleid_size();i++)
	{
		m_mapRadio.insert(std::pair<std::string,UINT64>(data->radiolist().name(i),data->radiolist().roleid(i)));
	}
	m_mapRoleNick.clear();
	for (int i = 0; i<pWhiteList->role_size(); i++)
	{
		KKSG::FMWhiteRoleData*pRadio = pWhiteList->mutable_role(i);
		m_mapRoleNick.insert(std::pair<std::string,std::string>(pRadio->acc(),pRadio->nick()));
	}
	SendWhiteList();
}

void FMMgr::SendWhiteList(CRole *pRole)
{
	if (pRole==NULL)
		return;
	PtcM2C_LargeRoomRoleNtf ntf;
	for (auto iter = m_mapRadio.begin(); iter!=m_mapRadio.end(); iter++)
	{
		ntf.m_Data.add_name(iter->first);
		ntf.m_Data.add_roleid(iter->second);
	}
	pRole->Send(ntf);
}


void FMMgr::GetRoleList(UINT64 nRoleID,std::vector<UINT64> &roleset)
{
	if (IsAudio(nRoleID)==true)
	{
		for(auto iter = m_setRoleID.begin(); iter!= m_setRoleID.end(); iter++)
		{
			CRole  *pRole = CRoleManager::Instance()->GetByRoleID(*iter);
			if (pRole)
			{
				roleset.push_back(pRole->GetID());
			}
		}
	}else
	{
		roleset.push_back(nRoleID);
		for(auto iter = m_mapRadio.begin(); iter!= m_mapRadio.end();iter++)
		{
			roleset.push_back(iter->second);
		}
	}
}


void FMMgr::OnLogin(CRole *pRole)
{
	if (!pRole)
	{
		return;
	}
	if (MSConfig::Instance()->GetFMFlag())
	{
		PtcM2C_LargeRoomLoginParamNtf ntf;
		auto iter = m_mapRoleNick.find(pRole->GetAccount());
		if (iter!=m_mapRoleNick.end())
		{
			ntf.m_Data.set_speaker(true);
			LogInfo("--------SendAudioType------1-roleid=%llu accname=%s", pRole->GetID(),pRole->GetAccount().c_str());
		}else
		{
			ntf.m_Data.set_speaker(false);
			LogInfo("--------SendAudioType------2-roleid=%llu accname=%s", pRole->GetID(),pRole->GetAccount().c_str());
		}
		pRole->Send(ntf);
	}
}

void FMMgr::OnLoginOut(CRole *pRole)
{
	LeaveRoom(pRole->GetAccount(), pRole->GetID());
	DelRole(pRole->GetID());
}


bool FMMgr::IsInRoom(UINT64 nRoleID)
{
	auto iter =m_setRoleID.find(nRoleID);
	if (iter!=m_setRoleID.end())
	{
		return true;
	}
	return false;
}


UINT64 FMMgr::DelRole(UINT64 nRole)
{
	auto iter = m_setRoleID.find(nRole);
	if (iter != m_setRoleID.end())
	{
		m_setRoleID.erase(nRole);
		ShmMsMgr::Instance()->GetFmRoleShm().Remove(nRole);
	}
	return true;
}

void FMMgr::ClearRole()
{
	for(auto iter=m_setRoleID.begin(); iter!=m_setRoleID.end();)
	{
		ShmMsMgr::Instance()->GetFmRoleShm().Remove(*iter);
		m_setRoleID.erase(iter++);
	}
}

void FMMgr::AddWhiteList(const std::string &acc, const std::string &nick)
{
	m_mapRoleNick[acc] = nick;
}

void FMMgr::ClearWhiteList()
{
	m_mapRoleNick.clear();
}

void FMMgr::LeaveRoom(const std::string &szName, UINT64 roleid)
{
	auto miter = m_mapRoleNick.find(szName);
	if (miter!=m_mapRoleNick.end())
	{
		LogInfo("--LeaveRoom-----%s",szName.c_str());
		PtcM2F_LeaveFMLargeRoom ntf;
		ntf.m_Data.set_acc(szName);
		FMLink::Instance()->SendTo(ntf);
		DelAudio(miter->second);
	}else
	{
		DoLog(szName, roleid,1);
	}
}

void FMMgr::AddAudio(std::string &name,UINT64 roleid)
{
	m_mapRadio.insert(std::pair<std::string,UINT64>(name,roleid));
}

void FMMgr::DelAudio(const std::string &name)
{
	for(auto iter = m_mapRadio.begin(); iter != m_mapRadio.end(); iter++)
	{
		if (iter->first == name)
		{
			m_mapRadio.erase(iter++);
			break;
		}
	}
}

void FMMgr::ClearAudio()
{
	m_mapRadio.clear();
}


void FMMgr::ClearFMData()
{
	m_mapRadio.clear();
	SendWhiteList();
	m_setRoleID.clear();
	m_mapRoleNick.clear();
}


UINT32 FMMgr::GetAudioSize()
{
	return m_mapRadio.size();
}

bool FMMgr::IsAnchor(const std::string &acc)
{
	auto iter = m_mapRoleNick.find(acc);
	if (iter!=m_mapRoleNick.end())
	{
		return true;
	}
	return false;
}

bool FMMgr::IsAudio(UINT64 nRole)
{
	for (auto iter = m_mapRadio.begin(); iter!=m_mapRadio.end();iter++)
	{
		if (iter->second==nRole)
		{
			return true;
		}
	}
	return false;
}

UINT64 FMMgr::GetAudioRoleID(const std::string &nickname)
{
	for(auto iter = m_mapRadio.begin(); iter!=m_mapRadio.end(); iter++)
	{
		if (iter->first == nickname)
		{
			return 	iter->second;
		}
	}
	return 0;
}

void FMMgr::Recover()
{
	std::vector<CShmFMRole*> vccShmFm;
	ShmMsMgr::Instance()->GetFmRoleShm().Load();
	ShmMsMgr::Instance()->GetFmRoleShm().GetAll(vccShmFm);
	for (auto iter = vccShmFm.begin(); iter!= vccShmFm.end(); iter++)
	{
		m_setRoleID.insert((*iter)->nRole);
	}
}


void FMMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	for(auto iter = m_setRoleID.begin(); iter!=m_setRoleID.end();)
	{
		CRole  *pRole = CRoleManager::Instance()->GetByRoleID(*iter);
		if (pRole==NULL)
		{
			ShmMsMgr::Instance()->GetFmRoleShm().Remove(*iter);
			m_setRoleID.erase(iter++);
		}else
		{
			iter++;
		}
	}
	m_nCount = 0;
}


void  FMMgr::DoLog(const std::string &szName, UINT64 roleid,int op,const std::string &strChat)
{
	TFmFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = roleid;
	oLog.m_strOpenId = szName;
	oLog.m_op = op;
	oLog.m_strChat = strChat;
	oLog.Do();
}