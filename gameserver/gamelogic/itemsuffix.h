#ifndef __ITEM_SUFFIX_H__
#define __ITEM_SUFFIX_H__

#include "unit/combatattribute.h"
#include "util/XCommon.h"
#include "table/SkillEmblem.h"

struct ItemChangeAttr
{
	UINT32 AttrID;
	UINT32 AttrValue;

	ItemChangeAttr();
	ItemChangeAttr(UINT32 nAttrId, UINT32 AttrValue);
};

struct EqualAttrID
{
	UINT32 m_AttrID;
	EqualAttrID(UINT32 dwAttrID) : m_AttrID(dwAttrID){}
	bool operator()(const ItemChangeAttr& attr) 
	{
		return attr.AttrID == m_AttrID;
	}
};

struct ChangeAttrOperator
{
	CombatAttribute *m_combatAttr;
	ChangeAttrOperator(CombatAttribute *combatAttr) : m_combatAttr(combatAttr){}
	void operator()(const ItemChangeAttr &attr)
	{	
		m_combatAttr->AddAttr((CombatAttrDef)attr.AttrID, attr.AttrValue);
	}
};

template <typename T>
void ApplyEffectAux(T &t, CombatAttribute *combatAttr)
{
	std::for_each(t.begin(), t.end(), ChangeAttrOperator(combatAttr));
}

struct EnhanceInfo
{
	EnhanceInfo()
	{
		level = 0;
		count = 0;
	}

	UINT32 level;
	UINT32 count;
	std::vector<ItemChangeAttr> attrs;	
};

struct EmblemInfo
{
	EmblemInfo()
	{
		thirdSlot = 0;
		slotType = KKSG::EmblemSlotType_None;
	}
	UINT32 thirdSlot;//0代表没有，1代表有孔未鉴定，2代表有孔且鉴定	
	KKSG::EmblemSlotType slotType;
};

struct SkEmblem
{
	SkEmblem() {}
	SkEmblem(const SkillEmblem::RowData* _conf)
	{
		skillHashs.insert(XCommon::XHash(_conf->SkillName.c_str()));
		skillHashs.insert(XCommon::XHash(_conf->ExSkillScript.c_str()));
		for (auto i = _conf->OtherSkillScripts.begin(); i != _conf->OtherSkillScripts.end(); ++i)
		{
			skillHashs.insert(XCommon::XHash((*i).c_str()));
		}
		conf = _conf;
	}
	bool IsEqualHash(UINT32 _hash) const
	{
		if (_hash == 0) return false;
		return skillHashs.find(_hash) != skillHashs.end();
	}
	bool IsHaveSameHash(SkEmblem& other)
	{
		for (auto i = skillHashs.begin(); i != skillHashs.end(); ++i)
		{
			if (other.skillHashs.find(*i) != other.skillHashs.end())
			{
				return true;
			}
		}
		return false;
	}
	std::set<UINT32> skillHashs;
	const SkillEmblem::RowData* conf;
};

struct JadeSingle
{
	JadeSingle()
	{
		slotPos = 0;
		jadeid = 0;
	}

	UINT32 slotPos;
	UINT32 jadeid;
	std::vector<ItemChangeAttr> attrs;

	void Reset();

	void ApplyEffect(CombatAttribute *pAttr)
	{
		ApplyEffectAux(attrs, pAttr);
	}

	bool operator<(const JadeSingle &Jade) const
	{
		return jadeid < Jade.jadeid;
	}
};

struct EqualJadeSlot
{	
	EqualJadeSlot(UINT32 dwSlotPos):m_dwSlotPos(dwSlotPos){}

	UINT32 m_dwSlotPos;
	bool operator() (const JadeSingle& stJadeSingle)
	{
		return m_dwSlotPos == stJadeSingle.slotPos;
	}
};

struct JadeInfo
{
	JadeInfo()
	{
		//slotInfo = 0;
	}

	//UINT32 slotInfo;
	std::vector<JadeSingle> vecJadeSingle;

	JadeSingle* GetJadeSingle(UINT32 dwSlotPos);

	void ApplyEffect(CombatAttribute* pAttr)
	{
		for(auto i = vecJadeSingle.begin(); i != vecJadeSingle.end(); ++i)
		{
			i->ApplyEffect(pAttr);
		}
	}
};

struct RandomAttr
{
	std::vector<ItemChangeAttr> attrs;
};

struct ForgeAttr
{
	std::vector<ItemChangeAttr> attrs;
	ItemChangeAttr unReplacedAttr;
	std::unordered_set<UINT32> haveAttrs;
};

struct EnchantEachData
{
	EnchantEachData(UINT32 _attrValue = 0, UINT32 _enchantid = 0) 
	{
		attrValue = _attrValue;
		enchantid = _enchantid;
	}
	UINT32 attrValue;
	UINT32 enchantid;
};

namespace KKSG
{
	class ItemEnchant;
}

struct EnchantAttr
{
	EnchantAttr()
	{
		chooseAttrid = 0;
		mapAllAttrs.clear();
		mapBaodiCount.clear();
	}

	UINT32 chooseAttrid;
	std::map<UINT32, EnchantEachData> mapAllAttrs;
	std::map<UINT32, UINT32> mapBaodiCount;

	void ToKKSG(KKSG::ItemEnchant* enchant);
	void ToThis(const KKSG::ItemEnchant* enchant);

	void Update(UINT32 attrid, UINT32 attrvalue, UINT32 enchantid);

	UINT32 GetBaodiCount(UINT32 enchantid);
	void AddBaodiCount(UINT32 enchantid);
	void ClearBaodiCount(UINT32 enchantid);

	void ApplyEffect(CombatAttribute* combatAttr);

	KKSG::ErrorCode ChangeAttribute(UINT32 attrid);
};

struct FuseAttr
{
	FuseAttr()
	{
		fuseLevel = 0;
		fuseExpCount = 0;
	}
	UINT32 fuseLevel;
	UINT32 fuseExpCount;

	void ApplyEffect(CombatAttribute* combatAttr, UINT32 itemID, Role* pRole);
};


#endif
