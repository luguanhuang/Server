#include "pch.h"
#include "friendconfig.h"
#include "friendop.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "friend/ptcm2c_frienddegreeupntfnew.h"

INSTANCE_SINGLETON(FriendConfig);

FriendConfig::FriendConfig()
{

}

FriendConfig::~FriendConfig()
{

}

bool FriendConfig::Init()
{
	if (!m_degreetable.LoadFile("table/FriendDegree.txt"))
	{
		SSWarn<<"load file table/FriendDegree.txt failed"<<END;
		return false;
	}

	if (!friendship_addition_buff_.LoadFile("table/FriendshipAdditionBuff.txt"))
	{
		SSWarn<<"load file table/FriendshipAdditionBuff.txt failed"<<END;
		return false;
	}

	return true;
}

void FriendConfig::Uninit()
{

}

float FriendConfig::GetDegree(UINT32 id)
{
	FriendDegreeTable::RowData* rowdata = m_degreetable.GetByid(id);
	if (NULL == rowdata)
	{
		return 0.0f;
	}
	return rowdata->degree[0];
}

UINT32 FriendConfig::GetLimit(UINT32 id)
{
	FriendDegreeTable::RowData* rowdata = m_degreetable.GetByid(id);
	if (NULL == rowdata)
	{
		return 0;
	}
	return (UINT32)rowdata->degree[1];
}

UINT32 FriendConfig::OnLevelUp(UINT64 roleid, UINT64 friendid, UINT32 all, UINT32 day)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL != role)
	{
		PtcM2C_FriendDegreeUpNtfNew ntf;
		ntf.m_Data.set_roleid(friendid);
		ntf.m_Data.set_friendname(CRoleSummaryMgr::Instance()->GetRoleName(friendid));
		ntf.m_Data.set_alldegree(all);
		ntf.m_Data.set_daydegree(day);
		role->Send(ntf);
	}
	return 0;
}

UINT32 FriendConfig::SceneToType(UINT32 scenetype, UINT32 sceneid)
{
	switch (scenetype)
	{
	case (KKSG::SCENE_ABYSSS):
		{
			return DegreeElite;
		}
	case (KKSG::SCENE_NEST):
		{
			return DegreeNest;
		}
	case (KKSG::SCENE_TOWER):
		{
			return DegreeTower;
		}
	case (KKSG::SCENE_PVP):
		{
			return DegreePVP;
		}
	case (KKSG::SCENE_GODDESS):
		{
			return DegreeGoddess;
		}
	case (KKSG::SCENE_DRAGON):
		{
			return DegreeDragon;
		}
	case (KKSG::SCENE_ENDLESSABYSS):
		{
			return DegreeEndless;
		}
	default:
		break;
	}
	return (UINT32)(-1);
}

void FriendConfig::AddDegree(UINT32 type, std::vector<UINT64> roles)
{
	std::vector<std::pair<UINT64, UINT64>> friends;
	for (UINT32 i = 0; i < roles.size(); ++i)
	{
		FriendOp op(roles[i]);
		for (UINT32 j = i + 1; j < roles.size(); ++j)
		{
			if (op.IsFriend(roles[j]))
			{
				friends.push_back(std::make_pair(roles[i], roles[j]));
			}
		}
	}

	for (auto j = friends.begin(); j != friends.end(); ++j)
	{
		FriendOp op(j->first);
		op.SetOther(j->second);
		op.AddDegree(type);
	}
}

// 通过友情值，获取buff
Sequence<uint, 2> &  FriendConfig::GetBuffSequenceByDegree(UINT32 degree)
{
	uint friendship_level =0;

	for (auto it = friendship_addition_buff_.Table.begin(); it != friendship_addition_buff_.Table.end(); ++it)
	{
		if (degree > (*it)->friendshipAddition)
		{
			++friendship_level;
		}
	}

	auto config =  friendship_addition_buff_.GetByfriendshipAdditionLevel(friendship_level);
	if (config == NULL && friendship_level > 0)
	{
		int size = friendship_addition_buff_.Table.size();
		return friendship_addition_buff_.Table[size-1]->buff;
	}

	return config->buff;
}