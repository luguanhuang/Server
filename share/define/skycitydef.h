#ifndef __SKYCITYDEF_H__
#define __SKYCITYDEF_H__


namespace SkyCityDef
{

	//�����Ϣ
	struct  FightRoleEx
	{
		UINT64 nRoleID;					//��ɫID
		std::string name;				//��ɫ����
		UINT32 nJob;					//ְҵ
		UINT32 nLevel;					//�ȼ�
		UINT32 nKiller;					//��ɱ����
		double nDamage;					//�˺�
		bool   bLost;					//����
		UINT32 nPPT;					//ս����
		UINT32 nFloor;					//��ɫ����MS��״̬
		UINT32 nTeamID;
		UINT32 nDeadCount;					//��������
		FightRoleEx():nRoleID(0),nJob(0),nLevel(0),nKiller(0),nDamage(0),bLost(false),nPPT(0),nFloor(0),nTeamID(0),nDeadCount(0){}
	};
	//ս��
	struct	FightTeam
	{
		UINT32 nTeamID;							//����ID
		UINT32 nFloor;							//ս�����ڵ�¥��
		UINT32 nKiller;							//��ɱ����
		UINT64 nHurt;							//�˺�
		UINT32 nPPT;							//ս����
		UINT32 nResult;							//�������
		int	   nFail;							//ʧ�ܴ���
		FightTeam():nTeamID(0),nFloor(0),nKiller(0),nHurt(0),nPPT(0),nResult(0),nFail(0){}
		std::map<UINT64, FightRoleEx *> mapRole;	//ս��
	};
	//���
	struct GroupTeam
	{
		FightTeam *pSelf;
		FightTeam *pTarget;
		UINT32 nSceneID;						//ս������SCENEID
		UINT32 nFloor;							//ս������¥��
		UINT32 nGroupID;
		GroupTeam():pSelf(NULL),pTarget(NULL),nSceneID(0),nFloor(0),nGroupID(0){}
		void Reset()	//ս����Ϣ����
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

	//���˱�����Ϣ
	struct SinglePPT
	{
		UINT64 nRoleID;
		UINT32 nPPT;
	};

};

#endif
