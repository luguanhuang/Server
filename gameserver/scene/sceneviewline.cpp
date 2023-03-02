#include "pch.h"
#include "sceneviewline.h"
#include "sender.hpp"
#include "unit/enemy.h"
#include "unit/unit.h"
#include "scenefinder.h"
#include "unit/enemymanager.h"
#include "foreach.h"
#include "component/XActionSender.h"
#include "scene/ptcg2c_unitappear.h"
#include "scene/ptcg2c_unitdisappear.h"
#include "component/XNetComponent.h"
#include "table/globalconfig.h"
#include "time.h"
#include "unit/roleview.h"
#include "util/gametime.h"
#include "gamelogic/team.h"
#include "gamelogic/teammgr.h"
#include "broadcast/broadcast.h"

SceneViewLine::SceneViewLine(Grid* grid)
	:m_grid(grid)
{
	m_gridSize = grid->GetViewLenx() * grid->GetViewLenz();
	m_grid2roles = new RoleSet[m_gridSize];
}

SceneViewLine::~SceneViewLine()
{
	delete[] m_grid2roles;
}

void SceneViewLine::Update()
{
	UINT64 nowtime = TimeUtil::GetMilliSecond();

	PtcG2C_UnitAppear appear;
	PtcG2C_UnitDisappear disappear;
	for (auto i = m_role2viewset.begin(); i != m_role2viewset.end(); ++i)
	{
		Role* role = i->second->m_role;
		// last view set
		RoleViewSet* viewset = i->second;

		RoleSet& roleset = viewset->m_cansee;

		// detect if need update view, n million second once
		if (nowtime < viewset->m_lasttime)
		{
			continue;
		}
		viewset->m_lasttime = nowtime + GetGlobalConfig().ViewGridSec*1000;

		int leavecount = 0;
		Role* leaveset[21] = {0};
		std::unordered_set<Role*> newset;
		newset.rehash(32);
		// get new view set
		int offset =  m_grid->GetViewGridOffset(role->GetXRole()->GetPosition_p().x, role->GetXRole()->GetPosition_p().z);
		int x = 0;
		int z = 0;
		m_grid->GetViewGridXY(role->GetXRole()->GetPosition_p().x, role->GetXRole()->GetPosition_p().z, x, z);
		FillNewSet(role, offset, x, z, roleset.m_set, newset);

		// filter
		for (auto k = roleset.m_set.begin(); k != roleset.m_set.end(); ++k)
		{
			auto l = newset.find(*k);
			if (l == newset.end())
			{
				// leave
				if (leavecount <= 20)
				{
					leaveset[leavecount++] = *k;
				}
			}
			else
			{
				// enter
				newset.erase(l);
			}
		}
		// leave
		if (0 != leavecount)
		{
			for (int n = 0; n < leavecount; ++n)
			{
				Role* other = leaveset[n];
				// to optimize pack once
				// to do...	
				KKSG::UnitAppearance *pAppearance = &disappear.m_Data;
				pAppearance->set_uid(other->GetID());

				///> (B, C, D...)离开A的视野
				roleset.RemoveRole(other);
				role->Send(disappear);
				disappear.m_Data.Clear();

				RoleViewSet* otherview = GetViewSet(other);
				if (NULL != otherview)
				{
					///> (B, C, D...)不能被A看到
					otherview->m_canbesee.RemoveRole(role);
				}
			}
		}
		// enter
		if (!newset.empty())
		{
			UINT32 oncelimit = 0;
			std::unordered_set<Role*> tempnewset;
			for (auto m = newset.begin(); m != newset.end(); ++m)
			{
				if (oncelimit >= VIEWGRID_ONCELIMIT)
				{
					break;
				}
				Role* other = *m;
				auto i = tempnewset.find(other);
				if (i != tempnewset.end())
				{
					continue;
				}
				bool isneedpre = other->GetRoleView()->GetIsNeedPre();
				UINT64 bindroleid = other->GetRoleView()->GetBindRole();
				///> 如果是副驾驶，需要优先同步主驾驶
				if (isneedpre)
				{
					Role* bindrole = IsIn(bindroleid);
					if (NULL != bindrole)
					{
						auto i = tempnewset.find(bindrole);
						if (i == tempnewset.end())
						{
							tempnewset.insert(bindrole);
							KKSG::UnitAppearance *pAppearance = appear.m_Data.add_units();
							PackAppear(pAppearance, bindrole, role, roleset);
							++oncelimit;
						}
					}
				}
				// other
				tempnewset.insert(other);
				KKSG::UnitAppearance *pAppearance = appear.m_Data.add_units();
				PackAppear(pAppearance, other, role, roleset);
				++oncelimit;
				
				///> 如果other是主驾驶，立即同步副驾驶数据
				if (0 != bindroleid && !isneedpre)
				{
					Role* bindrole = IsIn(bindroleid);
					if (NULL != bindrole)
					{
						auto i = tempnewset.find(bindrole);
						if (i == tempnewset.end())
						{
							tempnewset.insert(bindrole);
							KKSG::UnitAppearance *pAppearance = appear.m_Data.add_units();
							PackAppear(pAppearance, bindrole, role, roleset);
							++oncelimit;
						}
					}
				}
			}
			role->Send(appear);

			appear.m_Data.Clear();
		}
	}
}

void SceneViewLine::Broadcast(Unit* poSender, const CProtocol &roPtc, Unit *pExludeUnit)
{
	if (NULL == poSender || !poSender->IsRole())
	{
		SSInfo<<"no sender, broadcast:"<<roPtc.m_dwType<<" size:"<<m_role2viewset.size()<<END;
		for (auto i = m_role2viewset.begin(); i != m_role2viewset.end(); ++i)
		{
			Role* role = i->second->m_role;
			if (role != pExludeUnit)
			{
				role->Send(roPtc);
			}
		}
		return;
	}
	Role* sender = static_cast<Role*>(poSender);
	///> 先给自己广播
	if (sender != pExludeUnit)
	{
		sender->Send(roPtc);
	}
	///> 广播给自己在别人的视野里中
	RoleViewSet* viewset = GetViewSet(sender);
	if (NULL != viewset)
	{
		// all can be see
		for (auto j = viewset->m_canbesee.Begin(); j != viewset->m_canbesee.End(); ++j)
		{
			Role* other = *j;
			if (other != pExludeUnit)
			{
				other->Send(roPtc);
			}
		}
	}
}

void SceneViewLine::BroadcastCanBeSee(Unit* poSender, const CProtocol& roPtc)
{
	if (!poSender->IsRole())
	{
		return;
	}
	Role* role = (Role*)poSender;
	RoleViewSet* viewset = GetViewSet(role);
	int count = 0;
	if (NULL != viewset)
	{
		for (auto i = viewset->m_canbesee.Begin(); i != viewset->m_canbesee.End(); ++i)
		{
			Role* other = *i;
			if (role != other)
			{
				other->Send(roPtc);
				++count;
			}
		}
	}
	if (count > 500)
	{
		SSWarn<<"broad cast too many, count:"<<count<<END;
	}
}

void SceneViewLine::AddRole(Role *poRole)
{
	auto i = m_role2viewset.find(poRole->GetID());
	if (i != m_role2viewset.end())
	{
		SSWarn << "Add Role " << poRole << " FAILED!" << END;
		assert(false);
		return;
	}

	// update view grid
	int offset =  m_grid->GetViewGridOffset(poRole->GetXRole()->GetPosition_p().x, poRole->GetXRole()->GetPosition_p().z);
	///> 合法性检测
	if(!IsValidOffset(offset))
	{
		LogError("role [%llu] move to invalid offset:%d ", offset);
		assert(false);
		return;
	}
	m_grid2roles[offset].AddRole(poRole);

	RoleViewSet* viewset = new RoleViewSet(poRole);
	viewset->m_lastoffset = offset;
	m_role2viewset.insert(std::make_pair(poRole->GetID(), viewset));
}

void SceneViewLine::RemoveRole(Role *poRole)
{
	auto i = m_role2viewset.find(poRole->GetID());
	if (i != m_role2viewset.end())
	{
		///> 玩家离开，立即清除视野相关信息
		// update view grid
		if(!IsValidOffset(i->second->m_lastoffset))
		{
			LogError("role [%llu] invalid offset:%d", poRole->GetID(), i->second->m_lastoffset);
			return;
		}
		m_grid2roles[i->second->m_lastoffset].RemoveRole(poRole);

		// update other view
		RoleViewSet* viewset = i->second;
		// all can see
		for (auto k = viewset->m_cansee.Begin(); k != viewset->m_cansee.End(); ++k)
		{
			Role* other = *k;
			if (other == poRole)
			{
				continue;
			}
			///> 在别人的"被其他人看到列表"中,需要删除
			RoleViewSet* otherset = GetViewSet(other);
			if (NULL != otherset)
			{
				otherset->m_canbesee.RemoveRole(poRole);
			}
		}
		// all can be see
		for (auto k = viewset->m_canbesee.Begin(); k != viewset->m_canbesee.End(); ++k)
		{
			Role* other = *k;
			if (other == poRole)
			{
				continue;
			}
			///> 在别人的"视野列表"中,需要被删掉
			RoleViewSet* otherset = GetViewSet(other);
			if (NULL != otherset)
			{
				otherset->m_cansee.RemoveRole(poRole);
			}
		}
		delete i->second;
		m_role2viewset.erase(i);
	}
	else
	{
		SSWarn << "Remove Role " << poRole << " FAILED!" << END;
	}
}

void SceneViewLine::UpdateViewGrid(Role* role)
{
	int lastoffset = -1;
	auto i = m_role2viewset.find(role->GetID());
	if (i == m_role2viewset.end())
	{
		SSError<<"not find role in line, roleid:"<<role->GetID()<<END;
		return;
	}

	lastoffset = i->second->m_lastoffset;

	int offset =  m_grid->GetViewGridOffset(role->GetXRole()->GetPosition_p().x, role->GetXRole()->GetPosition_p().z);
	if (lastoffset != offset && IsValidOffset(offset))
	{
		m_grid2roles[lastoffset].RemoveRole(role);
		m_grid2roles[offset].AddRole(role);

		i->second->m_lastoffset = offset;
	}
	return;
}

void SceneViewLine::FillNewSet(Role* role, int offset, int x, int z, const std::list<Role*>& lastset, std::unordered_set<Role*>& newset)
{
	int scale[4] = {0, 0, 0, 0};
	m_grid->GetViewGridScale(x, z, scale);

	///> 筛选优先可视列表
	///> 必然可见
	const std::set<UINT64>& mustsee = role->GetRoleView()->GetMustSee();
	if (!mustsee.empty())
	{
		for (auto i = mustsee.begin(); i != mustsee.end(); ++i)
		{
			UINT64 mustseerole = *i;
			Role* other = IsIn(mustseerole);
			if (NULL != other && other != role)
			{
				FillOne(other, scale, role, newset, true);
			}
		}
	}
	///> 组队
	TeamCopy* team = TeamCopyMgr::Instance()->GetTeamByRoleID(role->GetID());
	if (NULL != team)
	{
		const std::vector<TeamCopyMember>& members = team->GetMember();
		for (UINT32 i = 0; i < members.size(); ++i)
		{
			Role* other = IsIn(members[i].RoleID());
			if (NULL != other && other != role)
			{
				if (FillOne(other, scale, role, newset))
				{
					return;
				}
			}
		}
	}
	///> 好友+工会
	const std::set<ViewUnit>& views = role->GetRoleView()->m_views;
	for (auto j = views.begin(); j != views.end(); ++j)
	{
		Role* other = IsIn(j->roleid);
		if (NULL != other && other != role)
		{
			if (FillOne(other, scale, role, newset))
			{
				return;
			}
		}
	}

	///> 判断原来视野内玩家是否离开
	for (auto j = lastset.begin(); j != lastset.end(); ++j)
	{
		Role* other = *j;
		if (other != role)
		{
			if (FillOne(other, scale, role, newset))
			{
				return;
			}
		}
	}

	///> 数量不足补其他玩家（遍历可能失败，设置最大遍历限制）
	int viewgrids[9] = {offset, 0, 0, 0, 0, 0, 0, 0, 0};
	m_grid->GetAllViewGrid(offset, viewgrids);
	UINT32 limit = 0;
	for (int k = 0; k < 9; ++k)
	{
		if (0 == viewgrids[k])
		{
			return;
		}
		if (IsValidOffset(viewgrids[k]))
		{
			RoleSet& roleset = m_grid2roles[viewgrids[k]];
			for (auto n = roleset.Begin(); n != roleset.End(); ++n)
			{
				if (++limit > VIEWGRID_LIMIT)
				{
					SSInfo<<"find max limit, roleid:"<<role->GetID()<<END;
					return;
				}
				Role* other = *n;
				if (other == role)
				{
					continue;
				}
				///> 被别人看到有限制
				RoleViewSet* viewset = GetViewSet(other);
				if (NULL == viewset)
				{
					SSWarn<<"role is not find:"<<other->GetID()<<END;
					continue;
				}
				else
				{
					if (viewset->m_canbesee.m_roleid2role.size() >= GetGlobalConfig().ViewGridCanBeSee)
					{
						continue;
					}
				}
				newset.insert(other);
				UINT64 bindroleid = other->GetRoleView()->GetBindRole();
				if (0 != bindroleid)
				{
					Role* bindrole = IsIn(bindroleid);
					if (NULL != bindrole && bindrole != role)
					{
						newset.insert(bindrole);
					}
				}

				if ((int)newset.size() >= VIEWGRID_COUNT)
				{
					return;
				}
			}
		}
	}
}

bool SceneViewLine::FillOne(Role* other, int scale[], Role* originrole, std::unordered_set<Role*>& newset, bool ismustsee)
{
	///> 被别人看到有限制
	RoleViewSet* viewset = GetViewSet(other);
	if (NULL == viewset)
	{
		SSWarn<<"role is not find:"<<other->GetID()<<END;
		return false;
	}
	else
	{
		if (viewset->m_canbesee.m_roleid2role.size() >= GetGlobalConfig().ViewGridCanBeSee)
		{
			return false;
		}
	}
	///> 是否在自己视野内
	int otherx = 0;
	int otherz = 0;
	m_grid->GetViewGridXY(other->GetXRole()->GetPosition_p().x, other->GetXRole()->GetPosition_p().z, otherx, otherz);

	bool inrange = (otherx >= scale[0] && scale[1] >= otherx && otherz >= scale[2] && otherz <= scale[3]);
	if (ismustsee || inrange)
	{
		newset.insert(other);
		UINT64 bindroleid = other->GetRoleView()->GetBindRole();
		if (0 != bindroleid)
		{
			Role* bindrole = IsIn(bindroleid);
			if (NULL != bindrole && bindrole != originrole)
			{
				newset.insert(bindrole);
			}
		}
		if ((int)newset.size() >= VIEWGRID_COUNT)
		{
			return true;
		}
	}
	return false;

	//int offset = m_grid->GetViewGridOffset(role->GetXRole()->GetPosition_p().x, role->GetXRole()->GetPosition_p().z);
	//for(int i = 0; i < 9; ++i)
	//{
	//	if (0 == viewgrids[i])
	//	{
	//		return false;
	//	}
	//	if(offset == viewgrids[i])
	//	{
	//		newset.insert(role);
	//		if ((int)newset.size() >= VIEWGRID_COUNT)
	//		{
	//			return true;
	//		}
	//		return false;
	//	}
	//}
	//return false;
}

RoleViewSet* SceneViewLine::GetViewSet(Role* role)
{
	auto i = m_role2viewset.find(role->GetID());
	if (i == m_role2viewset.end())
	{
		SSWarn<<"not find role in line, roleid:"<<role->GetID()<<END;
		return NULL;
	}
	return i->second;
}

void SceneViewLine::GetCanSee(Role* role, std::vector<Role*>& roles)
{
	RoleViewSet* viewset = GetViewSet(role);
	if (NULL != viewset)
	{
		for (auto i = viewset->m_cansee.Begin(); i != viewset->m_cansee.End(); ++i)
		{
			roles.push_back(*i);
		}
	}
}

void SceneViewLine::GetCanBeSee(Role* role, std::vector<Role*>& roles, int limit)
{
	RoleViewSet* viewset = GetViewSet(role);
	if (NULL != viewset)
	{
		int count = 0;
		for (auto i = viewset->m_canbesee.Begin(); i != viewset->m_canbesee.End(); ++i)
		{
			roles.push_back(*i);
			if (++count >= limit)
			{
				break;
			}
		}
	}
}

void SceneViewLine::UpdateBindRoleView(Role* self, Role* other)
{
	BindRoleView(self, other);
	BindRoleView(other, self);
}

void SceneViewLine::BindRoleView(Role* role, Role* bindrole)
{
	RoleViewSet* viewset = GetViewSet(role);
	if (NULL == viewset)
	{
		return;
	}

	PtcG2C_UnitAppear appear;
	bindrole->GetAppearance(*appear.m_Data.add_units());

	for (auto i = viewset->m_canbesee.Begin(); i != viewset->m_canbesee.End(); ++i)
	{
		Role* canbeseerole = *i;
		if (canbeseerole == bindrole)
		{
			continue;
		}
		RoleViewSet* canbeseeviewset = GetViewSet(canbeseerole);
		if (NULL == canbeseeviewset)
		{
			continue;
		}
		if (!canbeseeviewset->m_cansee.IsIn(bindrole))
		{
			///> bindrole可见
			canbeseeviewset->m_cansee.AddRole(bindrole);
			canbeseerole->Send(appear);						

			///> bindrole更新自己的canbesee
			RoleViewSet* bindview = GetViewSet(bindrole);
			if (NULL != bindview)
			{
				bindview->m_canbesee.AddRole(canbeseerole);
			}
		}
	}
}

void SceneViewLine::PackAppear(KKSG::UnitAppearance *pAppearance, Role* other, Role* self, RoleSet& roleset)
{
	other->GetAppearance(*pAppearance);

	///> A看到(B, C, D...)
	roleset.AddRole(other);

	RoleViewSet* otherview = GetViewSet(other);
	if (NULL != otherview)
	{
		///> (B, C, D...)能被A看到
		otherview->m_canbesee.AddRole(self);
	}
}
