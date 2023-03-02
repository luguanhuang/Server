#include "pch.h"
#include "guildinherit.h"
#include "table/globalconfig.h"
#include "unit/role.h"
#include "entity/XRole.h"
#include "unit/rolemanager.h"
#include "foreach.h"
#include "time.h"
#include "util/XRandom.h"
#include "guild/ptcg2c_synguildinheritexp.h"
#include "unit/roleoutlook.h"
#include "gamelogic/rolenethandler.h"
#include "guild/ptcg2m_noticeguildinheritend.h"
#include "network/mslink.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/noticemgr.h"
#include "guild/guildrecord.h"
#include "scene/scene.h"

INSTANCE_SINGLETON(CGuildInherit);
CGuildInherit::CGuildInherit()
{
}

CGuildInherit::~CGuildInherit()
{
}

bool CGuildInherit::Init()
{
	if (!m_table.LoadFile("table/GuildInheritExp.txt"))
	{
		SSWarn<<"load file table/GuildInheritExp.txt failed"<<END;
		return false;
	}
	m_timehandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	return true;
}

void CGuildInherit::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timehandler);
}

void CGuildInherit::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 nowTime = (UINT32)time(NULL);
	std::vector<UINT64> todelete;
	for (std::list<SInheritData*>::iterator iter = m_roles.begin(); iter != m_roles.end(); ++iter)
	{
		SInheritData* data = *iter;
		if (data == NULL)
		{
			continue;
		}

		if (!data->isallready)
		{
			if (data->startTime < nowTime)
			{
				todelete.push_back(data->roleOne);
				_End(data);
			}
		}
		else
		{
			if (nowTime == data->startTime || ((nowTime - data->startTime) % GetGlobalConfig().GuildInheritTime) != 0)
			{
				continue;
			}

			UINT32 turn = (nowTime - data->startTime) / GetGlobalConfig().GuildInheritTime; 
			if (turn > 0 && turn <= GetGlobalConfig().GuildInheritTimes)
			{
				bool islast = false;
				if (turn == GetGlobalConfig().GuildInheritTimes)
				{
					islast = true;
				}
				_AddExp(*data, turn, islast, todelete);
			}

			if (turn >= GetGlobalConfig().GuildInheritTimes)
			{
				if (data->type == GUILD_INHERIT)
				{
					_SynMsEnd(data->roleOne, data->roleTwo);
				}
				todelete.push_back(data->roleOne);
				_End(data);
			}
		}
	}
	for (auto i = todelete.begin(); i != todelete.end(); ++i)
	{
		_DelOne(*i);
	}
}

void CGuildInherit::_NoticeStart(UINT64 roleOne, UINT64 roleTwo)
{
	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(roleOne);
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(roleTwo);
	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		Del(roleOne);
		Del(roleTwo);
		return ;
	}

	PtcG2C_synGuildInheritExp msg;
	msg.m_Data.set_turn(0);
	msg.m_Data.set_islast(false);
	msg.m_Data.set_expone(0);
	msg.m_Data.set_exptwo(0);
	msg.m_Data.set_roleone(roleOne);
	msg.m_Data.set_roletwo(roleTwo);

	if (pRoleOne->GetLevel() > pRoleTwo->GetLevel())
	{
		msg.m_Data.set_teacherid(pRoleOne->GetID());
	}
	else
	{
		msg.m_Data.set_teacherid(pRoleTwo->GetID());
	}

	//Role* pRoleOne = RoleManager::Instance()->FindByRoleID(roleOne);
	if (pRoleOne != NULL)
	{
		pRoleOne->Send(msg);
	}

	//Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(roleTwo);
	if (pRoleTwo != NULL)
	{
		pRoleTwo->Send(msg);
	}
}

void CGuildInherit::_NoticeEnd(UINT64 roleOne, UINT64 roleTwo)
{
	PtcG2C_synGuildInheritExp msg;
	msg.m_Data.set_turn(1);
	msg.m_Data.set_islast(true);
	msg.m_Data.set_expone(0);
	msg.m_Data.set_exptwo(0);
	msg.m_Data.set_roleone(roleOne);
	msg.m_Data.set_roletwo(roleTwo);

	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(roleOne);
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(roleTwo);

	if (pRoleOne != NULL && pRoleTwo != NULL)
	{
		if (pRoleOne->GetLevel() > pRoleTwo->GetLevel())
		{
			msg.m_Data.set_teacherid(pRoleOne->GetID());
		}
		else
		{
			msg.m_Data.set_teacherid(pRoleTwo->GetID());
		}
	}
	else if (pRoleOne != NULL && pRoleTwo == NULL)
	{
		msg.m_Data.set_teacherid(pRoleOne->GetID());
	}
	else if (pRoleTwo != NULL && pRoleOne == NULL)
	{
		msg.m_Data.set_teacherid(pRoleTwo->GetID());
	}

	if (pRoleOne != NULL)
	{
		pRoleOne->Send(msg);
	}

	if (pRoleTwo != NULL)
	{
		pRoleTwo->Send(msg);
	}
}

void CGuildInherit::_AddExp(SInheritData& data, UINT32 turn, bool isLast, std::vector<UINT64>& delVec)
{
	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(data.roleOne);
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(data.roleTwo);
	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		SSWarn << "CGuildInherit::_AddExp role is NULL and One : " << data.roleOne << " Two : " << data.roleTwo << END;
		delVec.push_back(data.roleOne);
		//Del(data.roleOne);
		//Del(data.roleTwo);
		return ;
	}

	PtcG2C_synGuildInheritExp msg;
	msg.m_Data.set_turn(turn);
	msg.m_Data.set_islast(isLast);
	if (pRoleOne->GetLevel() < pRoleTwo->GetLevel())
	{
		foreach (i in m_table.Table)
		{
			if (pRoleOne->GetLevel() >= (int)(*i)->Level.seq[0] && pRoleOne->GetLevel() <= (int)(*i)->Level.seq[1])
			{
				pRoleOne->AddExp((*i)->StudentExp);
				pRoleOne->GetNetHandler()->SendExpAndLevelNtf();
				msg.m_Data.set_roleone(pRoleOne->GetID());
				msg.m_Data.set_expone((*i)->StudentExp);
			}

			if (pRoleTwo->GetLevel() >= (int)(*i)->Level.seq[0] && pRoleTwo->GetLevel() <= (int)(*i)->Level.seq[1])
			{
				pRoleTwo->AddExp((*i)->TeacherExp);
				pRoleTwo->GetNetHandler()->SendExpAndLevelNtf();
				msg.m_Data.set_roletwo(pRoleTwo->GetID());
				msg.m_Data.set_exptwo((*i)->TeacherExp);
			}
		}
		msg.m_Data.set_teacherid(pRoleTwo->GetID());
	}
	else
	{
		foreach (i in m_table.Table)
		{
			if (pRoleOne->GetLevel() >= (int)(*i)->Level.seq[0] && pRoleOne->GetLevel() <= (int)(*i)->Level.seq[1])
			{
				pRoleOne->AddExp((*i)->TeacherExp);
				pRoleOne->GetNetHandler()->SendExpAndLevelNtf();
				msg.m_Data.set_roleone(pRoleOne->GetID());
				msg.m_Data.set_expone((*i)->TeacherExp);
			}

			if (pRoleTwo->GetLevel() >= (int)(*i)->Level.seq[0] && pRoleTwo->GetLevel() <= (int)(*i)->Level.seq[1])
			{
				pRoleTwo->AddExp((*i)->StudentExp);
				pRoleTwo->GetNetHandler()->SendExpAndLevelNtf();
				msg.m_Data.set_roletwo(pRoleTwo->GetID());
				msg.m_Data.set_exptwo((*i)->StudentExp);
			}
		}
		msg.m_Data.set_teacherid(pRoleOne->GetID());
	}

	pRoleOne->Send(msg);
	pRoleTwo->Send(msg);
}

void CGuildInherit::Push(UINT64 roleidone, UINT64 roleidtwo, INHERIT_TYPE type)
{
	_DelOne(roleidone);
	_DelTwo(roleidone);
	_DelOne(roleidtwo);
	_DelTwo(roleidtwo);

	SInheritData* data = new SInheritData();
	data->roleOne = roleidone;
	data->roleTwo = roleidtwo;
	data->type = type;
	data->startTime = (UINT32)time(NULL) + 30;
	m_roles.insert(data);
}

void CGuildInherit::Add(Role* role, INHERIT_TYPE type)
{
	SInheritData* data1 = m_roles.find<RoleOneIndex>(role->GetID());
	if (data1 != NULL && data1->type != type)
	{
		return ;
	}

	if (NULL != data1)
	{
		data1->isoneready = true;
		if (data1->istwoready)
		{
			_PosTwo(role, data1->onepos, data1->twopos, type);
			role->GetRoleView()->SetMustSee(data1->roleTwo);
			_Ready(data1);	
		}
		else
		{
			_PosOne(role, data1->onepos, type);
			role->GetRoleView()->SetMustSee(data1->roleTwo);
		}
		return;
	}

	SInheritData* data2 = m_roles.find<RoleTwoIndex>(role->GetID());
	if (data2 != NULL && data2->type != type)
	{
		return ;
	}
	if (NULL != data2)
	{
		data2->istwoready = true;
		if (data2->isoneready)
		{
			_PosTwo(role, data2->twopos, data2->onepos, type);
			role->GetRoleView()->SetMustSee(data2->roleOne);
			_Ready(data2);	
		}
		else
		{
			_PosOne(role, data2->twopos, type);
			role->GetRoleView()->SetMustSee(data2->roleOne);
		}
		return;
	}
}

void CGuildInherit::DoLeaveScene(UINT64 roleId)
{
	SInheritData* data1 = m_roles.find<RoleOneIndex>(roleId);
	if (data1 != NULL && data1->isallready)
	{
		Del(roleId);
	}

	SInheritData* data2 = m_roles.find<RoleTwoIndex>(roleId);
	if (data2 != NULL && data2->isallready)
	{
		Del(roleId);
	}
}

void CGuildInherit::Del(UINT64 roleid)
{
	SInheritData* data1 = m_roles.find<RoleOneIndex>(roleid);
	if (NULL != data1)
	{
		_NoticeEnd(data1->roleOne, data1->roleTwo);
		_End(data1);				
		m_roles.erase<RoleOneIndex>(roleid);
		delete data1;
		return;
	}
	SInheritData* data2 = m_roles.find<RoleTwoIndex>(roleid);
	if (NULL != data2)
	{
		_NoticeEnd(data2->roleOne, data2->roleTwo);
		_End(data2);				
		m_roles.erase<RoleTwoIndex>(roleid);
		delete data2;
		return;
	}
}
bool CGuildInherit::IsMapRight(UINT64 roleOne, UINT64 roleTwo)
{
	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(roleOne);
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(roleTwo);
	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		return false;
	}

	if ((pRoleOne->GetCurrScene()->GetSceneTemplateID() == HALL_MAP_ID || pRoleOne->GetCurrScene()->GetSceneTemplateID() == GUILD_MAP_ID)
		&& (pRoleTwo->GetCurrScene()->GetSceneTemplateID() == HALL_MAP_ID || pRoleTwo->GetCurrScene()->GetSceneTemplateID() == GUILD_MAP_ID))
	{
		return true;
	}

	return false;
}
void CGuildInherit::_Ready(SInheritData* data)
{
	if (data == NULL)
	{
		return ;
	}

	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(data->roleOne);
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(data->roleTwo);
	if (pRoleOne == NULL || pRoleTwo == NULL
		|| pRoleOne->GetCurrScene() == NULL || pRoleTwo->GetCurrScene() == NULL
		|| (data->type == GUILD_INHERIT && (pRoleOne->GetCurrScene()->GetSceneTemplateID() != GUILD_MAP_ID || pRoleTwo->GetCurrScene()->GetSceneTemplateID() != GUILD_MAP_ID)) 
		|| (data->type == TEACHER_INHERIT && (pRoleOne->GetCurrScene()->GetSceneTemplateID() != HALL_MAP_ID || pRoleTwo->GetCurrScene()->GetSceneTemplateID() != HALL_MAP_ID)))
	{
		return ;
	}

	if (data->type == GUILD_INHERIT)
	{
		Role* pTea = NULL;
		Role* pStu = NULL;
		if (pRoleOne->GetLevel() > pRoleTwo->GetLevel())
		{
			pTea = pRoleOne;
			pStu = pRoleTwo;
		}
		else
		{
			pTea = pRoleTwo;
			pStu = pRoleOne;
		}

		if (pTea->Get<CGuildRecord>()->GetInheritTeaTime() >= GetGlobalConfig().GuildInheritTeaLimit
			|| pStu->Get<CGuildRecord>()->GetInheritStuTime() >= GetGlobalConfig().GuildInheritStuLimit)
		{
			return ;
		}
		else
		{
			pTea->Get<CGuildRecord>()->AddInheritTeaTime();
			pStu->Get<CGuildRecord>()->AddInheritStuTime();
			pTea->Get<CGuildRecord>()->SetGuildInheritTime(time(NULL));
		}
	}
	/*else if (data->type == TEACHER_INHERIT)
	{
		if (pRoleOne->Get<CGuildRecord>()->GetTeacherInheritTime() >= GetGlobalConfig().GuildInheritTeaLimit
			|| pRoleTwo->Get<CGuildRecord>()->GetTeacherInheritTime() >= GetGlobalConfig().GuildInheritTeaLimit)
		{
			pRoleOne->Get<CGuildRecord>()->AddTeacherInheritTime();
			pRoleTwo->Get<CGuildRecord>()->AddTeacherInheritTime();
		}
	}*/


	data->startTime = (UINT32)time(NULL);
	data->isallready = true;
	SSInfo<<"roleid1:"<<data->roleOne<<" roleid2:"<<data->roleTwo<<" ready"<<END;
	_NoticeStart(data->roleOne, data->roleTwo);

	PtcG2M_NoticeGuildInheritEnd msg;
	msg.m_Data.set_roleone(data->roleOne);
	msg.m_Data.set_roletwo(data->roleTwo);
	msg.m_Data.set_isend(false);
	MSLink::Instance()->SendTo(msg);
}

void CGuildInherit::_End(SInheritData* data)
{
	// notify
	Role* role1 = RoleManager::Instance()->FindByRoleID(data->roleOne);
	if (NULL != role1)
	{
		_DoState(role1, false);
	}
	Role* role2 = RoleManager::Instance()->FindByRoleID(data->roleTwo);
	if (NULL != role2)
	{
		_DoState(role2, false);
	}
	SSInfo<<"roleid1:"<<data->roleOne<<" roleid2:"<<data->roleTwo<<" end"<<END;
}

void CGuildInherit::_PosOne(Role* role, SInheritPos& self, INHERIT_TYPE type)
{
	Sequence<float, 4> pos;
	if (type == GUILD_INHERIT)
	{
		pos = GetGlobalConfig().InheritPos;
	}
	else if (type == TEACHER_INHERIT)
	{
		pos = GetGlobalConfig().TeacherInheritPos;
	}
	
	//const Sequence<float, 4>& pos = GetGlobalConfig().InheritPos;
	self.pos.x = pos.seq[0] + (float)XRandom::randInt(0, (UINT32)pos.seq[2]);
	self.pos.z = pos.seq[1] + (float)XRandom::randInt(0, (UINT32)pos.seq[2]);
	self.face = role->GetXRole()->GetFaceDegree();

	role->GetXRole()->CorrectLocation(self.pos, self.face, true);
	// notify
	_DoState(role, true);

}

void CGuildInherit::_PosTwo(Role* role, SInheritPos& self, const SInheritPos& other, INHERIT_TYPE type)
{
	Sequence<float, 4> pos;
	if (type == GUILD_INHERIT)
	{
		pos = GetGlobalConfig().InheritPos;
	}
	else if (type == TEACHER_INHERIT)
	{
		pos = GetGlobalConfig().TeacherInheritPos;
	}

	Vector3 temp = Vector3::FloatToAngle(other.face);
	temp.Normalize();
	self.pos = other.pos + (temp * pos.seq[3]);
	self.face = other.face - 180.f;

	role->GetXRole()->CorrectLocation(self.pos, self.face, true);
	// notify
	_DoState(role, true);
}

void CGuildInherit::_DoState(Role* role, bool isstart)
{
	role->SetState(isstart ? KKSG::OutLook_Inherit : KKSG::OutLook_Normal);
	RoleOutLook outlook(role);
	outlook.Fill(SUMMARY_STATE, role);
	outlook.BroadCast();
}

bool CGuildInherit::IsInherit(UINT64 roleId)
{
	for (std::list<SInheritData*>::iterator iter = m_roles.begin(); iter != m_roles.end(); ++iter)
	{
		if ((*iter)->roleOne == roleId || (*iter)->roleTwo == roleId)
		{
			return true;
		}
	}

	return false;
}

void CGuildInherit::_SynMsEnd(UINT64 roleOne, UINT64 roleTwo)
{
	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(roleOne);
	if (pRoleOne != NULL)
	{
		BagGiveItemTransition bagGiveOne(pRoleOne);
		bagGiveOne.SetReason(ItemFlow_GuildInherit, ItemFlow_GuildInheritGive);
		bagGiveOne.GiveItem(GUILD_CONTRIBUTE, GetGlobalConfig().GuildInheritCon);
		bagGiveOne.NotifyClient();
		NoticeMgr::Instance()->NoticeGuildEnd(pRoleOne->GetID());

		if (pRoleOne->Get<CGuildRecord>()->GetInheritTeaTime() >= GetGlobalConfig().GuildInheritTeaLimit)
		{
			NoticeMgr::Instance()->NoticeGuildRereq(pRoleOne->GetID());
		}
		if (pRoleOne->Get<CGuildRecord>()->GetInheritStuTime() >= GetGlobalConfig().GuildInheritStuLimit)
		{
			NoticeMgr::Instance()->NoticeGuildReq(pRoleOne->GetID());
		}
	}

	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(roleTwo);
	if (pRoleTwo != NULL)
	{
		BagGiveItemTransition bagGiveTwo(pRoleTwo);
		bagGiveTwo.SetReason(ItemFlow_GuildInherit, ItemFlow_GuildInheritGive);
		bagGiveTwo.GiveItem(GUILD_CONTRIBUTE, GetGlobalConfig().GuildInheritCon);
		bagGiveTwo.NotifyClient();
		NoticeMgr::Instance()->NoticeGuildEnd(pRoleTwo->GetID());

		if (pRoleTwo->Get<CGuildRecord>()->GetInheritTeaTime() >= GetGlobalConfig().GuildInheritTeaLimit)
		{
			NoticeMgr::Instance()->NoticeGuildRereq(pRoleTwo->GetID());
		}
		if (pRoleTwo->Get<CGuildRecord>()->GetInheritStuTime() >= GetGlobalConfig().GuildInheritStuLimit)
		{
			NoticeMgr::Instance()->NoticeGuildReq(pRoleTwo->GetID());
		}
	}

	PtcG2M_NoticeGuildInheritEnd msg;
	msg.m_Data.set_roleone(roleOne);
	msg.m_Data.set_roletwo(roleTwo);
	msg.m_Data.set_isend(true);
	MSLink::Instance()->SendTo(msg);
}