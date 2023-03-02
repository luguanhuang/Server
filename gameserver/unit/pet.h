#ifndef __PET_H__
#define __PET_H__

#include "gamelogic/itemsuffix.h"
#include "unit/roleserilizeinterface.h"
#include "pb/project.pb.h"
#include "time.h"
#include "timer.h"


struct RolePetInfo
{
	std::map<UINT64, KKSG::PetSingle> m_rolePetMap;
};

struct PetRecord //��¼
{
	UINT32 touchStartTime;   // ������ʼʱ��(ÿСʱ������) 
	UINT32 touchHourAttr;    // ��ǰСʱ�Ѿ���ȡ�ļӳ���ֵ
	UINT32 touchTodayAttr;   // �����Ѿ���ȡ�ļӳ���ֵ
	UINT32 followStartTime;  // ���￪ʼʱ��
	UINT32 followTodayAttr;  // ��������Ѿ���ȡ�ļӳ���ֵ
	UINT32 hungryStartTime;  // �����ȿ�ʼʱ�� 
	UINT32 moodStartTime;    // ���鿪ʼʱ��
	UINT32 max_level;	 // �ﵽ�������ȼ�
	PetRecord()
	{
		touchStartTime = 0;
		touchHourAttr = 0;
		touchTodayAttr = 0;
		followStartTime = 0;
		followTodayAttr = 0;
		moodStartTime = (UINT32)time(NULL);
		hungryStartTime = (UINT32)time(NULL);
		max_level = 0;
	}
};

///////////////////////////////////////////////////////////////  

struct ItemChangeAttr; 

class Pet
{
public:
	Pet();
	Pet(UINT32 petId, Role* pRole);
	~Pet();

public:
	void GetAllAttrs(std::vector<ItemChangeAttr>& attrsVec);
	bool AddExp(UINT32 addexp, Role* pRole, std::map<UINT32, UINT32>& skillMap);
	UINT32 Touch(bool& isChenge); 
	void FollowOn();
	void FollowDown();
	bool Feed(UINT32 exp, UINT32 hungry, UINT32 mood, Role* pRole, UINT32& delPer, std::map<UINT32, UINT32>& skillMap, bool& ismoodAdd, bool& ishungryAdd);
	void Update();
	bool AddFixedSkill(std::map<UINT32, UINT32>& skillMap);
	void AddRandSkill(std::map<UINT32, UINT32>& skillMap);
	void ToKKSG(KKSG::PetSingle& petsingle);
	void ReadKKSG(const KKSG::PetSingle& petsingle, Role* pRole);
	UINT32 GetTotalExp();
	void SetPetExp(const UINT32 new_exp,Role* role);
	void AddSkill(Role* pRole, UINT32 bookId);
	bool CanAddSkill(UINT32 skillId);
	void RecalcPower(Role* pRole);
	void SetCanPairRide(bool istrue);

	inline UINT32 GetHungry() { return hungry; }
	inline UINT32 GetMood() { return mood; }
	inline bool IsFull() { return hungry >= 100; }
	inline void SetHungry(UINT32 num) { hungry = num; }

private:
	void _UpdateMood();
	void _UpdateHungry();
	void _UpdateFollow();
	bool _ChangeHungry(int num);
	bool _ChangeMood(int num);	
	void _NoticeLvlup(Role* pRole, UINT32 type, UINT32 delExp = 0);
	void _NoticeSkillBook(Role* pRole, UINT32 addSkill, UINT32 delSkill);
	bool _CanStudySkill(UINT32 skillId);
	bool _HasFixSkill(UINT32 skillId);
	void _CheckDelSkill(UINT32 bookId, UINT32& delSkillId);
	void _TryDelSkill(UINT32& delSkillId);
	void _DelSkill();
	UINT32 _CheckFixedSkill();
	UINT32 _GetHungryPercent();
	UINT32 _GetNextFixSkill();

public:
	UINT64 uid;
	UINT32 petid;
	UINT32 level;
	UINT32 exp;
	UINT32 sex;
	UINT32 power;
	UINT32 mood;
	UINT32 hungry;
	UINT32 max_level_;
	UINT32 fixedSkill;
	bool canpairride;
	PetRecord record;
	std::vector<UINT32> fixedSkills; // �̶�����
	std::vector<UINT32> randSkills;  // �������
};

////////////////////////////////////////////////////////////////////////////
class CombatAttribute;

class PetSys : public RoleSerializeInterface, ITimer
{

public:
	PetSys();
	PetSys(Role* pRole);
	~PetSys();
	
public:
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo); 
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

public:
	UINT32 OPSetFight(UINT64 uid);
	UINT32 OPSetPairRide(UINT64 uid, bool istrue);
	UINT32 OPFollow(UINT64 uid, bool isSync = true);
	UINT32 OPTouch(UINT64 uid, bool& ischange);
	UINT32 OPExpandSeat();
	UINT32 OPReleasePet(UINT64 uid);
	UINT32 OPFeed(UINT64 uid, UINT32 itemid, UINT32 itemcount, bool& ismoodAdd, bool& ishungryAdd);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	UINT32 OPPetExpTransfer(const UINT64 pet_id,const UINT64 dest_pet_id); // ���ﾭ��ת��
	std::map<UINT64, Pet>& GetAllPet() { return m_petsMap; }

public:
	inline Pet* GetFollow() { return GetPet(m_followPetId); }
	inline Pet* GetFight() { return GetPet(m_fightPetId); }
	inline UINT64 GetFollowId() { return m_followPetId; }
	inline UINT64 GetCopilotRoleId() { return m_ispassive ? 0 : m_followroleid; }
	inline bool IsCopilot() { return m_ispassive ? (m_followroleid != 0) : false; }
	inline bool GetIsPassive() { return m_ispassive; }
	inline UINT64 GetFollowRoleId() { return m_ispassive ? m_followroleid : 0; }
	inline UINT64 GetFightId() { return m_fightPetId; }
	inline UINT32 GetSeat() { return m_petSeats; }
	inline void SetTempPetId(UINT64 tempPetId) { m_tempPetId = tempPetId; }
	inline void SetFollowRoleId(UINT64 roleid) { m_followroleid = roleid; }
	inline void SetPassive(bool ispassive) { m_ispassive = ispassive; }
	inline UINT64 GetTempPetId() { return m_tempPetId; }
	UINT64 GetLastFollowId();
	UINT32 GetFollowPetPPT();

	void AddTempPet(UINT32 petId);
	void DelTempPet();
	UINT32 GetFollowPetId();
	UINT32 GetFightPetId();
	UINT32 GetFollowConfId();
	bool IsOpen();
	void CheckOpen();
	Pet* GetPet(UINT64 uid);
	KKSG::ErrorCode CanAddPet(UINT32 petId);
	KKSG::ErrorCode AddPet(UINT32 petId);
	std::string GetStr();
	void ApplyEffect(CombatAttribute *roleattrs);  // ����pet��������
	UINT32 OnEnterScene();
	void DoOnlineChek(); // ���߼��  �Ƿ���Ҫ�����Ⱥ�㶨ʱ��������
	void UpdateRankList(UINT64 petId, bool isRelease = false); // �������а�(ս�������仯��ʱ��)
	void UpdateAllPetRankList();
	void ToKKSG(KKSG::PetSysData* pData);
	void ReadKKSG(const KKSG::PetSysData* pData);
	// gm �ӿ�
	void TestAddExp(const UINT32 test_exp);
	UINT32 GetHorseSize();
	// pair ride
	void SetPairRide(UINT64 other, bool ispassive);
	void ResetPairRide(bool isstatechange);
	void ResetPairRideData(bool isstatechange);
	bool CanPairRideSelf();
	bool CanPairRideOther(bool isinitiative);
	bool PushPairRideInvite(KKSG::PetInviteInfo& invite);
	void ClearPairRideInvite() { m_invites.clear(); }
	UINT32 GetPairRideInviteCount() { return m_invites.size(); }
	std::deque<KKSG::PetInviteInfo>* GetPairRideInvite() { return &m_invites;}

private:
	void _Init();
	UINT32 _GetInitPetSeat();
	UINT32 _GetPetSeatCost(UINT32 seatNum);
	void _AddBuff();
	void _ClearBuff();
	void _AddTempBuff();
	void _ClearTempBuff();
	void _NotifyPetChange(Pet& pet, UINT32 type); // ��������ͬ��
	void _NotifyFeedPet(Pet& pet, UINT32 delExp, std::map<UINT32, UINT32>& getSkillMap); // ι����������ͬ��
	UINT64 _GetLastFollowId();
	void _SynPetChange();   // ���������л�  ͬ���������
	bool _CanFollowPet();
	void NotifyTransferPetChange(Pet& souce_pet,Pet& dest_pet,std::map<UINT32, UINT32>& getSkillMap); // ���齻������������ͬ��
	void _SynTempPet();

private:
	Role* m_role;
	UINT32 m_petSeats;     // ��λ��
	UINT64 m_followPetId;  // ����id
	UINT64 m_fightPetId;   // ��սid
	UINT64 m_lastFollowId; // ��¼�ϴθ�������id 
	UINT64 m_tempPetId;    // ��ʱ�������
	std::map<UINT64, Pet> m_petsMap;
	HTIMER m_timehandler;

	///> ˫���������(ֻ���ڴ棬���������ݿ�)
							 //			     A <--- B
	bool m_ispassive;		 //				false   true
	UINT64 m_followroleid;   ///> ����Ľ�ɫ B      A
	///> ���������б�
	std::deque<KKSG::PetInviteInfo> m_invites;
};


#endif