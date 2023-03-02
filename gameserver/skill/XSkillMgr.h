#ifndef __XSKILLMGR_H__
#define __XSKILLMGR_H__

using namespace std;

class XEntity;
class XSkill;
class XSkillCore;
class XSkillData;

class XSkillMgr
{
public:
	XSkillMgr(XEntity* host);
	~XSkillMgr();

	XSkill* GetCarrier(int id);
	XSkillCore* GetPhysicalSkill();
	XSkillCore* GetSkill(UINT32 id);
	
	inline const unordered_map<UINT32, vector<UINT32> >& GetQTE() const { return _qte; }

	inline UINT32 GetPhysicalIdentity() { return _physical; }
	inline UINT32 GetUltraIdentity() { return _ultra; }
	inline UINT32 GetAppearIdentity() { return _appear; }
	inline UINT32 GetDashIdentity() { return _dash; }
	inline UINT32 GetDisappearIdentity() { return _disappear; }
	inline UINT32 GetRecoveryIdentity() { return _recovery; }
	inline UINT32 GetBrokenIdentity() { return _broken; }

	bool IsCooledDown(XSkillCore* core);
	bool IsCooledDown(UINT32 id);
	bool IsPhysicalAttack(UINT32 id);
	bool IsQTEAttack(UINT32 id);
	
	float GetCD(UINT32 id);
	void  CoolDown(UINT32);
	float GetAttackRange(UINT32 id);
	float GetAttackScope(UINT32 id);
	float GetElapsedCD(UINT32 id);
	float GetLeftCD(UINT32 id);

	void StatisticsPhysicalSkill();
	void Accelerate(UINT32 id, float delta, bool ratio);
	void ResetStaticCD(UINT32 id);
	void AccelerateStaticCD(UINT32 id, float delta);

	const vector<XSkillCore*>& GetSkillOrder(){ return _skill_order; }

	XSkillData* LoadSkillData(const string& skill_name, const string& location);

	void AttachPhysicalSkill(XSkillData* data);
	void AttachAppearSkill(XSkillData* data);
	void AttachDisappearSkill(XSkillData* data);
	void AttachUltraSkill(XSkillData* data);
	void AttachDashSkill(XSkillData* data);
	void AttachBrokenSkill(XSkillData* data);
	void AttachRecoverySkill(XSkillData* data);

	void AttachSkill(XSkillData* data, bool attach = true);
	void AttachSkillByID(UINT32 id);
	void DetachSkill(UINT32 id);

private:
	XSkillCore* CreateSkillCore(XSkillData* data);

private:
	XEntity* _host;

	unordered_map<int, XSkill*> _carriers;

	unordered_map<UINT32, XSkillCore*> _full_skill;
	unordered_map<UINT32, XSkillCore*> _core;
	vector<XSkillCore*> _skill_order;

	unordered_map<UINT32, vector<UINT32> > _qte;
	std::set<UINT32> _physicals;

	UINT32 _physical;
	UINT32 _ultra;
	UINT32 _appear;
	UINT32 _disappear;
	UINT32 _dash;
	UINT32 _recovery;
	UINT32 _broken;
};

#endif	//__XSKILLMGR_H__