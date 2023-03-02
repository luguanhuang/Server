#ifndef __ITEMDEF_H__
#define __ITEMDEF_H__

#include "pb/enum.pb.h"

///> 虚拟道具 和 真实道具 的说明：
///> 金币，钻石，奖励经验值，技能点 属于虚拟道具，ItemID 小于VIRTUAL_ITEM_MAX
///> 包裹中可以看到的物品属于真实道具。

enum ItemEnum
{
	GOLD = 1,			///> 金币
	SKILL_POINT_TWO = 2,///> 技能点2
	EXP = 4,			///> 经验
	SKILL_POINT = 5,	///> 技能点1
	FATIGUE = 6,		///> 体力值
	DRAGON_COIN = 7,	///> 龙币
	VIP_POINT = 8,		///> VIP点（等比换算成充值）
	DIAMOND = 9,        ///> 钻石

	ACHIEVE_POINT = 10, ///> 成就点数
	PARTNER_COIN = 11,	///> 代币(战友币、基友币)
	MENTOR_POINT = 12,	///> 师徒值

	ARENA_COIN = 21,	///> 荣誉点（竞技场）
	GUILD_CONTRIBUTE = 22, ///> 公会贡献
	GUILD_MEDAL = 23,   ///> 公会勋章
	IRON = 24,			///> 铁精
	GUILD_TOTAL_CONTRIBUTE = 25,	///> 公会总贡献
	NEST_COIN = 27,			///> 巢穴币
	DICE = 28,				///> 骰子
	CARNIVAL_COIN = 30,		///> 嘉年华积分
	CARD_CHIP = 32,			///> 卡牌碎片



	VIRTUAL_ITEM_MAX = KKSG::Virtual_Max,
	
	BLUE_BIRD = 51,		///> 蓝鸟
	FINDBACK_TICKET = 70, //找回券
	NORMALMARRIAGE_RING = 588,//普通婚戒
	SPLENDIDMARRIAGE_RING = 589,//豪华婚戒
};

enum ItemType
{
	EQUIP = 1,			///> 装备
	PECK  = 2,			///> 礼包
	VIRTUAL_ITEM = 3,	///> 虚拟物品
	MATERAIL = 4,		///> 材料
	FASHION = 5,		///> 时装
	EMBLEM	= 6,		///> 纹章
	JADE = 7,			///> 龙玉
	FASHION_FRAGMENT = 8,///> 时装碎片
	CIRCLE_DWAW = 9,    ///> 圆盘抽奖
	SUPPLEMENT = 10,	///> 战斗辅助道具
	FISH = 11,          ///> 鱼
	EQUIP_PROTO = 12,   ///> 装备原型
	PET_BORN = 13,		///> 宠物丹
	PET_FOOD = 14,		///> 宠物饲料
	SCENE_COUNT_TICKET = 15, ///> 巢穴/深渊 参加券
	METAL_PLATE = 16,	///> 金属板
	SPRITE = 18,        ///> 精灵
	ATLAS = 19,			///> 图鉴
	SEED = 20,			///> 种子
	CROP = 21,			///> 工会种子
	COOK_BOOK = 22,		///> 食谱
	FOOD = 23,			///> 料理
	PANDORA	= 26,		///> 潘多拉
	PET_SKILL_BOOK = 28, ///> 坐骑技能学习书

	HERO_EXPERIENCE_TICKET = 30, ///> 英雄战场体验券
	ARTIFACT = 31,		///> 龙器

	DESIGNATION_ITEM = 32, ///> 称号物品	

	BagExpand = 34,		///> 背包扩充
	Tarja = 35,			///> 塔姐

	INSCRIPTION = 36,	///> 龙器铭文

	GUILDBONUS = 37,	///> 红包

	ItemTypeAll
};

///> 时装
enum FashionPosition
{
	FashionHeadgear,
	FashionUpperBody,
	FashionLowerBody,
	FashionGloves,
	FashionBoots ,
	FashionMainWeapon,
	FashionSecondaryWeapon,

	FashionWings,
	FashionTail,
	FashionDecal,

	FashionCount,
};

///> 装备
enum EquipPosition
{
	HeadGear ,      
	UpperBody,  
	LowerBody,      
	Gloves   ,     
	Boots    ,       
	MainWeapon, 
	SecondaryWeapon,
	Necklace,
	Earrings,
	Ring1,
	EquipCount,
};

///> 纹章
enum EmblemEnum
{
	AttributeEmblemCount = 8,
	FreeSkillEmblemCount = 4,
	ExtraSkillEmblemCount = 4,

	SkillEmblemCount = FreeSkillEmblemCount + ExtraSkillEmblemCount,
	EmblemCount = AttributeEmblemCount + SkillEmblemCount,

	ExtraSkillEmblemStartIndex = AttributeEmblemCount + FreeSkillEmblemCount,

	EmblemBoundary = 1000, // <= 为属性纹章， > 为技能纹章
};

///> 龙器
enum ArtifactPostion
{
	ArtifactCount = 4,
};

enum ItemQuality
{
	QualityLow = 0,

	QualityZero = 0,
	QualityI	= 1,
	QualityII	= 2,
	QualityIII	= 3,
	QualityIV	= 4,
	QualityV	= 5,

	QualityHigh = 5,
};

enum ItemBind
{
	Bind_No,
	Bind_Is,
	Bind_ALL,
};

enum FashionQuality
{
	FashionBQuality = 2,
	FashionAQuality = 3,
	FashionSQuality = 4,
};

namespace KKSG
{
	class ItemBrief;
}

struct ItemDesc
{
	UINT32 itemID;
	UINT32 itemCount;
	bool isbind;
	UINT32 cooldown;

	ItemDesc()
		:itemID(0), itemCount(0), isbind(true), cooldown(0) {}

	ItemDesc(UINT32 _itemID, UINT32 _itemCount)
		:itemID(_itemID), itemCount(_itemCount), isbind(true), cooldown(0) {}

	ItemDesc(UINT32 _itemID, UINT32 _itemCount, bool _isbind, UINT32 _cooldown = 0)
		:itemID(_itemID), itemCount(_itemCount), isbind(_isbind), cooldown(_cooldown) {}

	void ToKKSG(KKSG::ItemBrief* itembrief) const;
	void ToThis(const KKSG::ItemBrief& itembrief);
	
	bool FilterIsSame(bool _isbind, UINT32 _cooldown) const
	{
		if(isbind != _isbind) return false;
		return !!cooldown == !!_cooldown;
	}
	bool FilterIsSame(const ItemDesc& desc) const
	{
		return FilterIsSame(desc.isbind, desc.cooldown);
	}
};

KKSG::ErrorCode GetErrorCode(UINT32 itemId);


#endif // __ITEMDEF_H__