#ifndef __SKYCITYDEF_H__
#define __SKYCITYDEF_H__


namespace SkyCityDef
{

	//玩家信息
	struct  FightRoleEx
	{
		UINT64 nRoleID;					//角色ID
		std::string name;				//角色名称
		UINT32 nJob;					//职业
		UINT32 nLevel;					//等级
		UINT32 nKiller;					//击杀人数
		double nDamage;					//伤害
		bool   bLost;					//掉线
		UINT32 nPPT;					//战斗力
		UINT32 nFloor;					//角色所在MS的状态
		UINT32 nTeamID;
		UINT32 nDeadCount;					//死亡次数
		FightRoleEx():nRoleID(0),nJob(0),nLevel(0),nKiller(0),nDamage(0),bLost(false),nPPT(0),nFloor(0),nTeamID(0),nDeadCount(0){}
	};
	//战队
	struct	FightTeam
	{
		UINT32 nTeamID;							//队伍ID
		UINT32 nFloor;							//战队所在的楼层
		UINT32 nKiller;							//击杀人数
		UINT64 nHurt;							//伤害
		UINT32 nPPT;							//战斗力
		UINT32 nResult;							//比赛结果
		int	   nFail;							//失败次数
		FightTeam():nTeamID(0),nFloor(0),nKiller(0),nHurt(0),nPPT(0),nResult(0),nFail(0){}
		std::map<UINT64, FightRoleEx *> mapRole;	//战队
	};
	//组队
	struct GroupTeam
	{
		FightTeam *pSelf;
		FightTeam *pTarget;
		UINT32 nSceneID;						//战队所在SCENEID
		UINT32 nFloor;							//战队所在楼层
		UINT32 nGroupID;
		GroupTeam():pSelf(NULL),pTarget(NULL),nSceneID(0),nFloor(0),nGroupID(0){}
		void Reset()	//战队信息重置
		{
			nSceneID = 0;
			if (pSelf)
			{
				pSelf->nKiller	= 0;
				pSelf->nHurt	= 0;
				pSelf->nResult	= 0;
			}
			if(pTarget)
			{
				pTarget->nKiller= 0;
				pTarget->nHurt	= 0;
				pTarget->nResult= 0;
			}
		}

		FightTeam *GetFightTeam(UINT32 nTeamID)
		{
			if (!nTeamID)
			{
				return NULL;
			}
			if (pSelf&&pSelf->nTeamID==nTeamID)
			{
				return pSelf;
			}
			if (pTarget&&pTarget->nTeamID==nTeamID)
			{
				return pTarget;
			}
			return NULL;
		}

		FightTeam *GetOtherFightTeam(UINT32 nTeamID)
		{
			if (!nTeamID)
			{
				return NULL;
			}
			if (pSelf&&pSelf->nTeamID!=nTeamID)
			{
				return pSelf;
			}
			if (pTarget&&pTarget->nTeamID!=nTeamID)
			{
				return pTarget;
			}
			return NULL;
		}

		FightRoleEx *FindFightRole(UINT64 nRole)
		{
			if (pSelf)
			{
				std::map<UINT64, FightRoleEx *>::iterator iter = pSelf->mapRole.find(nRole);
				if (iter!=pSelf->mapRole.end())
					return iter->second;
			}
			if (pTarget)
			{
				std::map<UINT64, FightRoleEx *>::iterator iter = pTarget->mapRole.find(nRole);
				if (iter!=pTarget->mapRole.end())
					return iter->second;
			}
			return NULL;
		}
	};

	//个人报名信息
	struct SinglePPT
	{
		UINT64 nRoleID;
		UINT32 nPPT;
	};

};

#endif
