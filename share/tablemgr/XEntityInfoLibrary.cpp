#include "pch.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/XEntityPresentation.h"
#include "util/XCommon.h"

using namespace std;

INSTANCE_SINGLETON(XEntityInfoLibrary)

XEntityInfoLibrary::XEntityInfoLibrary(void)
{
	_present_data = new XEntityPresentation();
	_enemy_data = new XEntityStatistics();
	_role_data = new ProfessionTable();
}


XEntityInfoLibrary::~XEntityInfoLibrary(void)
{
	if(_present_data) delete _present_data;
	if(_enemy_data) delete _enemy_data;
	if(_role_data) delete _role_data;

	_present_data = 0;
	_enemy_data = 0;
	_role_data = 0;
}

bool XEntityInfoLibrary::Init()
{
	return LoadFile();
}

void XEntityInfoLibrary::Uninit()
{
	ClearFile();
}

bool XEntityInfoLibrary::CheckFile()
{
	XEntityPresentation tmpXEntityPresentation;
	if(!tmpXEntityPresentation.LoadFile("table/XEntityPresentation.txt"))
	{
		LogWarn("load table/XEntityPresentation.txt failed");
		return false;
	}

	XEntityStatistics tmpXEntityStatistics;
	if(!tmpXEntityStatistics.LoadFile("table/XEntityStatistics.txt"))
	{
		LogWarn("load table/XEntityStatistics.txt failed");
		return false;
	}
	// check
	std::map<UINT32, UINT32> enemyids;
	for (UINT32 i = 0; i < tmpXEntityStatistics.Table.size(); ++i)
	{
		XEntityStatistics::RowData* data = tmpXEntityStatistics.Table[i];
		if (data->MaxSuperArmor < 999999999)
		{
			enemyids[data->ID] = data->PresentID;
		}
	}
	for (auto j = enemyids.begin(); j != enemyids.end(); ++j)
	{
		XEntityPresentation::RowData* data = tmpXEntityPresentation.GetByPresentID(j->second);	
		if (NULL != data)
		{
			if (data->Hit_f.empty() || data->Hit_l.empty() || data->Hit_r.empty() || data->HitCurves.empty())
			{
				SSError<<"hit config is empty, enemy id:"<<j->first<<END;
				//return false;
			}
		}
	}

	ProfessionTable tmpProfessionTable;
	if(!tmpProfessionTable.LoadFile("table/Profession.txt"))
	{
		LogWarn("load table/Profession.txt");
		return false;
	}

	return true;
}

bool XEntityInfoLibrary::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	XEntityPresentation tmpXEntityPresentation;
	tmpXEntityPresentation.LoadFile("table/XEntityPresentation.txt");
	_present_data->CopyFrom(&tmpXEntityPresentation);

	XEntityStatistics tmpXEntityStatistics;
	tmpXEntityStatistics.LoadFile("table/XEntityStatistics.txt");
	_enemy_data->CopyFrom(&tmpXEntityStatistics);

	_role_data->Clear();
	_role_data->LoadFile("table/Profession.txt");
	m_oProfInitSkill.clear();
	const ProfessionTable& oTable = RoleInfo();
	for (UINT32 i = 0; i < oTable.Table.size(); ++i)
	{
		auto &row = *oTable.Table[i];

		m_oProfInitSkill.push_back(InitSkill());
		auto &initskill = m_oProfInitSkill.back();
		initskill.prof = row.ID;
		
		for (auto j = row.InitSkill.begin(); j != row.InitSkill.end(); ++j)
		{
			BindInfo info;
			Sequence<string, 2> &seq = *j;
			info.skillhash = XCommon::XHash(seq[0].c_str());
			info.slot = convert<int>(seq[1]);

			LogInfo("Set skill [%s -> %d]", seq[0].c_str(), info.slot);

			initskill.skillhashs.push_back(info);
		}

		for (auto k= row.InitBindSkill.begin(); k != row.InitBindSkill.end(); ++k)
		{
			BindInfo info;
			Sequence<string, 2> &seq = *k;
			info.skillhash = XCommon::XHash(seq[0].c_str());
			info.slot = convert<int>(seq[1]);

			LogInfo("Set skill [%s -> %d]", seq[0].c_str(), info.slot);
			initskill.bindSkillHashs.push_back(info);
		}
	}

	return true;
}

void XEntityInfoLibrary::ClearFile()
{
	_present_data->Clear();
	_enemy_data->Clear();
	_role_data->Clear();
}

UINT32 XEntityInfoLibrary::TemplateID2PresentID_Enemy(UINT32 enemy_id)
{
	return _enemy_data->GetByID(enemy_id)->PresentID;
}

const string& XEntityInfoLibrary::TemplateID2Prefab_Enemy(UINT32 enemy_id)
{
	UINT32 present_id = TemplateID2PresentID_Enemy(enemy_id);
	return _present_data->GetByPresentID(present_id)->Prefab;
}

float XEntityInfoLibrary::TemplateID2FightTogetherDis(UINT32 enemy_id)
{
	return _enemy_data->GetByID(enemy_id)->FightTogetherDis;
}

const string& XEntityInfoLibrary::TemplateID2AIBehavior(UINT32 enmy_id)
{
	return _enemy_data->GetByID(enmy_id)->AiBehavior;
}

int XEntityInfoLibrary::TemplateID2HitLevel(UINT32 enemy_id)
{
	if(NULL == _enemy_data->GetByID(enemy_id))
	{//暂时先这样处理 测试
		return 1;
	}
	return _enemy_data->GetByID(enemy_id)->aihit;
}

UINT32 XEntityInfoLibrary::TemplateID2PresentID_Role(UINT32 basic_role_id)
{
	return _role_data->GetByProfID(basic_role_id % 10)->PresentID;
}

const string& XEntityInfoLibrary::TemplateID2Prefab_Role(UINT32 basic_role_id)
{
	UINT32 present_id = TemplateID2PresentID_Role(basic_role_id);
	return _present_data->GetByPresentID(present_id)->Prefab;
}

int XEntityInfoLibrary::GetAttackType(int profession)
{
	int basic = profession % 10;
	ProfessionTable::RowData* data = _role_data->GetByProfID(basic);
	return NULL == data ? 1 : data->AttackType;
}

InitSkill* XEntityInfoLibrary::GetInitSkill(int profID)
{
	for (auto j = m_oProfInitSkill.begin(); j != m_oProfInitSkill.end(); ++j)
	{
		if (j->prof == profID)
		{
			return &*j;
		}
	}
	return NULL;
}
