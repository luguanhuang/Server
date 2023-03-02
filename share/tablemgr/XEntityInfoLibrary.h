#ifndef __XEntityInfoLibrary_H__
#define __XEntityInfoLibrary_H__

#include "table/XEntityStatistics.h"
#include "table/ProfessionTable.h"
#include "table/XEntityPresentation.h"

struct BindInfo
{
	UINT32 skillhash;
	int    slot;
};

struct InitSkill
{
	int prof;
	std::vector<BindInfo> skillhashs;
	std::vector<BindInfo> bindSkillHashs;
};

class XEntityInfoLibrary
{
	XEntityInfoLibrary(void);
	~XEntityInfoLibrary(void);

	DECLARE_SINGLETON(XEntityInfoLibrary)
public:

	bool Init();	
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	inline const XEntityPresentation& Present() const { return *_present_data; }
	inline const XEntityStatistics& EnemyInfo() const { return *_enemy_data; }
	inline const ProfessionTable& RoleInfo() const { return *_role_data; }

	inline bool HasEnemy(UINT32 enemy_id) const { return TemplateID2Data_Enemy(enemy_id) != NULL; }
	inline bool HasRole(UINT32 role_id) const { return TemplateID2Data_Role(role_id) != NULL; }

	UINT32 TemplateID2PresentID_Enemy(UINT32 enemy_id);
	const std::string& TemplateID2Prefab_Enemy(UINT32 enemy_id);
	const string& TemplateID2AIBehavior(UINT32 enmy_id);

	inline const XEntityStatistics::RowData* TemplateID2Data_Enemy(UINT32 enemy_id) const { return _enemy_data->GetByID(enemy_id); }

	UINT32 TemplateID2PresentID_Role(UINT32 basic_role_id);
	const std::string& TemplateID2Prefab_Role(UINT32 basic_role_id);

	float TemplateID2FightTogetherDis(UINT32 basic_role_id);
	int TemplateID2HitLevel(UINT32 enemy_id);

	int GetAttackType(int profession);

	InitSkill *GetInitSkill(int profID);

	inline const ProfessionTable::RowData* TemplateID2Data_Role(UINT32 role_id) const { return _role_data->GetByProfID(role_id); }
	inline const XEntityPresentation::RowData* PresentID2Data(UINT32 present_id) const { return _present_data->GetByPresentID(present_id); }

private:
	XEntityPresentation* _present_data;
	XEntityStatistics*  _enemy_data;
	ProfessionTable* _role_data;

	std::list<InitSkill> m_oProfInitSkill;
};

#endif	//XEntityInfoLibrary