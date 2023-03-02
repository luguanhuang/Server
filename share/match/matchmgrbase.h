#ifndef __MATCHMGRBASE_H__
#define __MATCHMGRBASE_H__

#include "matchbase.h"
#include "common/roleopdelay.h"

class MatchMgrBase
{
public:
	MatchMgrBase();
	virtual ~MatchMgrBase();

	void Update();

	bool AddMatchUnit(int type, const KKSG::KMatchUnit& kunit);

	void DelMatchUnitTeam(UINT32 serverID, int teamID);
	void DelMatchUnitRole(UINT32 serverID, UINT64 roleID);

	MatchUnit* Get(KMatchID matchID);
	void Del(std::vector<KMatchID>& waitDel, MatchDelReason reason = MDR_NONE);
	void Del(std::list<KMatchID>& waitDel, MatchDelReason reason = MDR_NONE);
	KMatchID GetMatchIDByKey(const KMatchKey& key);

	//fightUnit:����ս���漰����ƥ�䵥λ
	//data:һЩ�淨���������
	void MatchOneFight(MatchUnitMgrBase* pUnitMgr, std::list<KMatchID>& fightUnits, KKSG::KMatchFightData& data);

	virtual void CreateSceneBegin(const KKSG::KMatchFightData& typeData, std::vector<MatchUnit*>& matched) = 0;

	std::unordered_map<KMatchID, MatchUnit*>& AllMatchUnits() { return m_matchUnits; }

	void AddUnitMgr(int type, MatchUnitMgrBase* pMgr);
	MatchUnitMgrBase* GetUnitMgr(int type);

	void InfoPrint();
	void Clear();

private:
	//Ψһ��/ɾ�ĵط�
	KMatchID Add(int type, const KKSG::KMatchUnit& kunit);
	void Del(KMatchID matchID, MatchDelReason reason);

	//�������
	bool AddCond(const KKSG::KMatchUnit& kunit);
	//��������ģ���ͬ��
	void AddSyn(MatchUnit* pUnit);
	void DelSyn(MatchUnit* pUnit);

	virtual void OnDelReason(MatchUnit* pUnit, MatchDelReason reason) {}
	virtual void AddRoleDelay(UINT64 roleID, UINT32 now);

	KMatchID NewMatchID();

private:

	//ƥ�䵥ԪΨһ
	std::unordered_map<KMatchID, MatchUnit*> m_matchUnits;

	//roleID->MatchID (���� �����Զ������ʽƥ������) ��ֹͬһ������ظ�ƥ��
	std::unordered_map<UINT64, KMatchID> m_role2MatchID;

	//Key(serverid roleid teamid) �������ٲ���
	std::map<KMatchKey, KMatchID> m_key2MatchID;

	//��ͬƥ�����Ͷ�Ӧ��mgr
	std::map<int, MatchUnitMgrBase*> m_type2Mgr;

	//ƥ��ɹ���ʱ���ڲ��ü���ƥ�� ��ֹƥ��ɹ����Ҫ�г�����ʱ���ֲ�����ƥ��
	RoleOpDelay m_delay;

};

#endif