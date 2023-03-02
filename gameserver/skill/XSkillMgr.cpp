#include "pch.h"
#include "XSkillMgr.h"
#include "component/XSkillComponent.h"
#include "entity/XEntity.h"
#include "util/XCommon.h"
#include "XSkillCore.h"
#include "XSkill.h"
#include "XJAComboSkill.h"
#include "XArtsSkill.h"
#include "XUltraSkill.h"

#include "gamelogic/skilllevel.h"
#include "utility/XPath.h"
#include "table/SkillReader.h"
#include "XCombinedSkill.h"
#include "component/XPresentComponent.h"
#include "scene/scene.h"

XSkillMgr::XSkillMgr(XEntity* host)
	:_host(host), 
	 _physical(0),
	 _ultra(0),
	 _appear(0),
	 _disappear(0),
	 _dash(0),
	 _recovery(0),
	 _broken(0)
{

}

XSkillMgr::~XSkillMgr()
{
	unordered_map<int, XSkill*>::iterator it1 = _carriers.begin();
	while (it1 != _carriers.end())
	{
		delete it1->second;
		++it1;
	}

	unordered_map<UINT32, XSkillCore*>::iterator it2 = _full_skill.begin();
	while (it2 != _full_skill.end())
	{
		delete it2->second;
		++it2;
	}

	_carriers.clear();
	_core.clear();
	_full_skill.clear();
	_skill_order.clear();
	_physicals.clear();
	_qte.clear();
}

void XSkillMgr::AttachPhysicalSkill(XSkillData* data)
{
	_physical = XCommon::XHash(data->Name.c_str());
	AttachSkill(data);
}

void XSkillMgr::AttachAppearSkill(XSkillData* data)
{
	_appear = XCommon::XHash(data->Name.c_str());
	AttachSkill(data, false);
}

void XSkillMgr::AttachDisappearSkill(XSkillData* data)
{
	_disappear = XCommon::XHash(data->Name.c_str());
	AttachSkill(data, false);
}

void XSkillMgr::AttachDashSkill(XSkillData* data)
{
	_dash = XCommon::XHash(data->Name.c_str());
	AttachSkill(data);
}

void XSkillMgr::AttachBrokenSkill(XSkillData* data)
{
	_broken = XCommon::XHash(data->Name.c_str());
	AttachSkill(data, false);
}

void XSkillMgr::AttachRecoverySkill(XSkillData* data)
{
	_recovery = XCommon::XHash(data->Name.c_str());
	AttachSkill(data, false);
}

void XSkillMgr::AttachUltraSkill(XSkillData* data)
{
	_ultra = XCommon::XHash(data->Name.c_str());
	AttachSkill(data);
}

void XSkillMgr::AttachSkillByID(UINT32 id)
{
	unordered_map<UINT32, XSkillCore*>::iterator it = _full_skill.find(id);
	if(it != _full_skill.end())
	{
		AttachSkill(it->second->GetSoul());
	}
}

void XSkillMgr::AttachSkill(XSkillData* data, bool attach)
{
	UINT32 id = XCommon::XHash(data->Name.c_str());

	if(_full_skill.find(id) == _full_skill.end())
	{
		XSkillCore* pCore = CreateSkillCore(data);
		_full_skill[id] = pCore;
	}

	XSkillCore* pSkillCore = _full_skill[id];
	/*
	 * in case PVP version due to data is not as the same as pSkillCore->GetSoul();
	 */
	data = pSkillCore->GetSoul();

	if (pSkillCore->Level() == 0 && !_host->IsTransform() && !data->ForCombinedOnly) return;

	if(_core.find(id) != _core.end()) return;

	_core[id] = pSkillCore;

	if (!data->ForCombinedOnly && attach) 
	{
		if (id != _physical)
			_skill_order.push_back(pSkillCore);
	}
	//QTE
	if(data->Logical.CanCastAt_QTE != 0)
	{
		for(int i = 0; i < 32; i++)
		{
			if((1 << i) & data->Logical.CanCastAt_QTE)
			{
				vector<UINT32>* vec = NULL;

				unordered_map<UINT32, vector<UINT32> >::iterator it = _qte.find(i);
				if(it == _qte.end())
				{
					_qte.insert(std::make_pair(i, vector<UINT32>()));
					vec = &(_qte[i]);
				}
				else
					vec = &(it->second);

				vec->push_back(pSkillCore->ID());
			}
		}
	}

	int carrier_id = data->TypeToken;
	if(carrier_id == 3)
	{
		//add sub-combined skills
		for(size_t i = 0; i < data->Combined.size(); i++)
		{
			UINT32 combind_id = XCommon::XHash(data->Combined[i].Name.c_str());

			if(_full_skill.find(combind_id) == _full_skill.end())
			{
				XSkillData* pData = LoadSkillData(data->Combined[i].Name, data->Prefix);
				XSkillCore* pCore = CreateSkillCore(pData);

				_full_skill[combind_id] = pCore;
			}

			_core[combind_id] = _full_skill[combind_id];
		}
	}

	if(_carriers.find(carrier_id) == _carriers.end())
	{
		XSkill* pSkill = NULL;
		
		switch(carrier_id)
		{
		case 0: pSkill = new XJAComboSkill(_host); break;
		case 1: pSkill = new XArtsSkill(_host); break;
		case 2: pSkill = new XUltraSkill(_host); break;
		case 3: pSkill = new XCombinedSkill(_host); break;
		}

		if (NULL != pSkill)
		{
			_carriers[carrier_id] = pSkill;
		}
	}
}

XSkillCore* XSkillMgr::CreateSkillCore(XSkillData* data)
{
	if(data->PVP_Script_Name.length() > 0)
	{
		Scene* current = _host->GetUnit()->GetCurrScene();
		bool IsPVP = current ? Scene::IsPVPCombat(current) : _host->GetUnit()->IsEnterPVP();

		if(IsPVP)
		{
			if(_host->PresentComponent())
			{
				string name = data->Name;
				// SkillData是共享的，不要删除
				data = LoadSkillData(data->PVP_Script_Name, data->Prefix);

				data->Name = name;
			}
		}
	}
	
	return new XSkillCore(_host, data);
}

void XSkillMgr::DetachSkill(UINT32 id)
{
	unordered_map<UINT32, XSkillCore*>::iterator it = _core.find(id);
	if(it != _core.end())
	{
		/*
		 * skill core only be deleted at _full_skill destroyed.
		 */
		/*delete it->second;
		it->second = NULL;*/

		if(it->first == _physical) _physical = 0;
		if(it->first == _ultra) _ultra = 0;
		if(it->first == _appear) _appear = 0;
		if(it->first == _disappear) _disappear = 0;

		_physicals.erase(it->first);

		std::vector<XSkillCore*>::iterator vit = _skill_order.begin();
		while (vit != _skill_order.end())
		{
			if((*vit) == it->second)
				vit = _skill_order.erase(vit);
			else
				++vit;
		}
		
		_core.erase(it);
	}
}

bool XSkillMgr::IsPhysicalAttack(uint id)
{
	return _physicals.find(id) != _physicals.end();
}

bool XSkillMgr::IsQTEAttack(UINT32 id)
{
	XSkillCore* core = GetSkill(id);
	return core != NULL ? core->GetSoul()->Logical.CanCastAt_QTE > 0 : false;
}

XSkill* XSkillMgr::GetCarrier(int id)
{
	unordered_map<int, XSkill*>::iterator it = _carriers.find(id);
	if (it == _carriers.end())
	{
		XSkill* pSkill = NULL;

		switch(id)
		{
			case 0: pSkill = new XJAComboSkill(_host); break;
			case 1: pSkill = new XArtsSkill(_host); break;
			case 2: pSkill = new XUltraSkill(_host); break;
			case 3: pSkill = new XCombinedSkill(_host); break;
		}

		if (NULL != pSkill)
		{
			_carriers[id] = pSkill;
		}

		return pSkill;
	}
	else
		return it->second;
}

XSkillCore* XSkillMgr::GetPhysicalSkill()
{
	return GetSkill(_physical);
}

XSkillCore* XSkillMgr::GetSkill(UINT32 id)
{
	unordered_map<UINT32, XSkillCore*>::iterator it = _core.find(id);
	return (it == _core.end()) ? NULL : it->second;
}

bool XSkillMgr::IsCooledDown(XSkillCore* core)
{
	return core->CooledDown();
}

bool XSkillMgr::IsCooledDown(UINT32 id)
{
	return IsCooledDown(GetSkill(id));
}

float XSkillMgr::GetCD(UINT32 id)
{
	return GetSkill(id)->GetCoolDown();
}

void XSkillMgr::CoolDown(UINT32 id)
{
	XSkillCore* core = GetSkill(id);
	if(core) core->CoolDown();
}

void XSkillMgr::Accelerate(UINT32 id, float delta, bool ratio)
{
	XSkillCore* core = GetSkill(id);
	if(core) core->AccelerateCD(delta, ratio);
}

void XSkillMgr::ResetStaticCD(UINT32 id)
{
	XSkillCore* core = GetSkill(id);
	if(core) core->ResetStaticCD();
}

void XSkillMgr::AccelerateStaticCD(UINT32 id, float delta)
{
	XSkillCore* core = GetSkill(id);
	if(core) core->AccelerateStaticCD(delta);
}

float XSkillMgr::GetAttackRange(UINT32 id)
{
	return GetSkill(id)->GetRange(0);
}

float XSkillMgr::GetAttackScope(UINT32 id)
{
	return GetSkill(id)->GetScope(0);
}

float XSkillMgr::GetElapsedCD(UINT32 id)
{
	return GetSkill(id)->GetElapsedCD();
}

float XSkillMgr::GetLeftCD(UINT32 id)
{
	return GetSkill(id)->GetLeftCD();
}

void XSkillMgr::StatisticsPhysicalSkill()
{
	_physicals.clear();
	XSkillCore* core = GetPhysicalSkill();

	while (core)
	{
		_physicals.insert(core->ID());

		if(core->GetSoul()->Logical.Association && !core->GetSoul()->Logical.Association_Skill.empty())
			_physicals.insert(XCommon::XHash(core->GetSoul()->Logical.Association_Skill.c_str()));

		if(core->GetSoul()->Ja.size() > 0)
		{
			uint hash = XCommon::XHash(core->GetSoul()->Ja[0].Name.c_str());
			if(hash != 0)_physicals.insert(hash);

			core = GetSkill(hash);
		}
		else
			core = NULL;
	}
}

XSkillData* XSkillMgr::LoadSkillData(const string& skill_name, const string& location)
{
	XSkillData* data = (skill_name.length() == 0 || skill_name == "E") ? NULL : SkillReader::LoadSkill((location + skill_name + ".txt").c_str());
	if(data) data->Prefix = location;
	return data;
}