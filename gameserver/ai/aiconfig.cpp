#include "pch.h"
#include "aiconfig.h"
#include "util/XRandom.h"

INSTANCE_SINGLETON(AIConfig)

#define MAX_PROF_INDEX 8
#define MAX_SKILL_NUM 50

AIConfig::AIConfig()
{

}

AIConfig::~AIConfig()
{

}

bool AIConfig::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void AIConfig::Uninit()
{

}

SkillCombo::RowData* AIConfig::GetRowData(UINT32 index)
{
	if (m_oTable.Table.size() > index)
		return m_oTable.Table[index];
	else 
		return NULL;
}

SkillCombo::RowData* AIConfig::GetRowData(string name)
{
	for (size_t i=0; i<m_oTable.Table.size(); i++)
	{
		SkillCombo::RowData* data = m_oTable.Table[i];

		if (data->skillname == name)
			return data;
	}

	return NULL;
}


bool AIConfig::HasStartSkill(int profIndex)
{
	return StartSkills.find(profIndex) != StartSkills.end();
}

bool AIConfig::HasComboSkill(string skillname)
{
	return NextSkills.find(skillname) != NextSkills.end();
}


std::vector<std::string>& AIConfig::GetStartSkill(int profIndex)
{
	return StartSkills[profIndex];
}

std::vector<std::string>& AIConfig::GetStartSkillById(int templateid)
{
	char skillname[64];
	std::vector<std::string> thevec;
	sprintf(skillname, "%d", templateid);
	//string str = skillname;
	auto it = NextSkills.begin();

	while(it != NextSkills.end())
	{
		if (it->first == skillname)
			return it->second;

		it ++;
	}

	return Skills;
}

std::vector<std::string>& AIConfig::GetComboSkill(string skillname)
{
	return NextSkills[skillname];
}

int AIConfig::GetProfIndex(Unit* unit)
{
	int profIndex = unit->GetTemplateID()%10 -1;
	int profClass = (int)(unit->GetTemplateID()/10) -1;

	if (unit->GetTemplateID() >= 1000 || unit->GetTemplateID() <= 10)
	{
		return profIndex*2 + XRandom::randInt(0, 2);
	}
	else
	{
		return profIndex*2 + profClass;
	}
}

UINT32 AIConfig::GetHateValue(UINT32 casterType, UINT32 receiverType)
{
	auto it = type2hateData.find(receiverType);
	if (it == type2hateData.end())
		return 0;

	auto it2 = it->second.normalInfo.find(casterType);
	if (it2 == it->second.normalInfo.end())
		return 0;

	return it2->second;
}

bool AIConfig::CheckFile()
{
	HateList temphatelisttable;
	if (!temphatelisttable.LoadFile("table/HateList.txt"))
	{
		SSWarn<<"load file table/HateList.txt failed"<<END;
		return false;
	}
	SkillCombo tempskillcombotable;
	if (!tempskillcombotable.LoadFile("table/SkillCombo.txt"))
	{
		SSWarn<<"load file table/SkillCombo.txt failed"<<END;
		return false;
	}
	return true;
}

void AIConfig::ClearFile()
{
	m_oHateListTable.Clear();
	m_oTable.Clear();
}

bool AIConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();

	m_oHateListTable.LoadFile("table/HateList.txt");

	type2hateData.clear();
	for (auto it = m_oHateListTable.Table.begin(); it != m_oHateListTable.Table.end(); ++it)
	{
		UINT32 tag = 1 << (*it)->TypeID;
		HateData& data = type2hateData[tag];

		for (auto it2 = (*it)->HateValue.begin(); it2 != (*it)->HateValue.end(); ++it2)
		{
			data.normalInfo[ 1 << (*it2)[0]] = (*it2)[1];
		}
	}

	//////////////////////////////////////////////////////////////////////////
	m_oTable.LoadFile("table/SkillCombo.txt");
	StartSkills.clear();
	for (int i=0; i<MAX_PROF_INDEX; i++)
	{
		SkillCombo::RowData* data = GetRowData(i);
		std::vector<string> skillnames;

		if (data == NULL) continue;

		if (data->nextskill0 != "") skillnames.push_back(data->nextskill0);
		if (data->nextskill1 != "") skillnames.push_back(data->nextskill1);
		if (data->nextskill2 != "") skillnames.push_back(data->nextskill2);
		if (data->nextskill3 != "") skillnames.push_back(data->nextskill3);
		if (data->nextskill4 != "") skillnames.push_back(data->nextskill4);

		StartSkills.insert(make_pair( i, skillnames ));
	}
	NextSkills.clear();
	for (int i=MAX_PROF_INDEX*2; i<MAX_PROF_INDEX*MAX_SKILL_NUM; i++)
	{
		SkillCombo::RowData* data = GetRowData(i);
		std::vector<string> skillnames;

		if (data == NULL) break;

		string skillname = data->skillname;
		if (data->nextskill0 != "") skillnames.push_back(data->nextskill0);
		if (data->nextskill1 != "") skillnames.push_back(data->nextskill1);
		if (data->nextskill2 != "") skillnames.push_back(data->nextskill2);
		if (data->nextskill3 != "") skillnames.push_back(data->nextskill3);
		if (data->nextskill4 != "") skillnames.push_back(data->nextskill4);

		NextSkills.insert(make_pair( skillname, skillnames ));
	}

	return true;
}
