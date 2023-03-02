#ifndef __ITEMDEF_H__
#define __ITEMDEF_H__

#include "pb/enum.pb.h"

///> ������� �� ��ʵ���� ��˵����
///> ��ң���ʯ����������ֵ�����ܵ� ����������ߣ�ItemID С��VIRTUAL_ITEM_MAX
///> �����п��Կ�������Ʒ������ʵ���ߡ�

enum ItemEnum
{
	GOLD = 1,			///> ���
	SKILL_POINT_TWO = 2,///> ���ܵ�2
	EXP = 4,			///> ����
	SKILL_POINT = 5,	///> ���ܵ�1
	FATIGUE = 6,		///> ����ֵ
	DRAGON_COIN = 7,	///> ����
	VIP_POINT = 8,		///> VIP�㣨�ȱȻ���ɳ�ֵ��
	DIAMOND = 9,        ///> ��ʯ

	ACHIEVE_POINT = 10, ///> �ɾ͵���
	PARTNER_COIN = 11,	///> ����(ս�ѱҡ����ѱ�)
	MENTOR_POINT = 12,	///> ʦͽֵ

	ARENA_COIN = 21,	///> �����㣨��������
	GUILD_CONTRIBUTE = 22, ///> ���ṱ��
	GUILD_MEDAL = 23,   ///> ����ѫ��
	IRON = 24,			///> ����
	GUILD_TOTAL_CONTRIBUTE = 25,	///> �����ܹ���
	NEST_COIN = 27,			///> ��Ѩ��
	DICE = 28,				///> ����
	CARNIVAL_COIN = 30,		///> ���껪����
	CARD_CHIP = 32,			///> ������Ƭ



	VIRTUAL_ITEM_MAX = KKSG::Virtual_Max,
	
	BLUE_BIRD = 51,		///> ����
	FINDBACK_TICKET = 70, //�һ�ȯ
	NORMALMARRIAGE_RING = 588,//��ͨ���
	SPLENDIDMARRIAGE_RING = 589,//�������
};

enum ItemType
{
	EQUIP = 1,			///> װ��
	PECK  = 2,			///> ���
	VIRTUAL_ITEM = 3,	///> ������Ʒ
	MATERAIL = 4,		///> ����
	FASHION = 5,		///> ʱװ
	EMBLEM	= 6,		///> ����
	JADE = 7,			///> ����
	FASHION_FRAGMENT = 8,///> ʱװ��Ƭ
	CIRCLE_DWAW = 9,    ///> Բ�̳齱
	SUPPLEMENT = 10,	///> ս����������
	FISH = 11,          ///> ��
	EQUIP_PROTO = 12,   ///> װ��ԭ��
	PET_BORN = 13,		///> ���ﵤ
	PET_FOOD = 14,		///> ��������
	SCENE_COUNT_TICKET = 15, ///> ��Ѩ/��Ԩ �μ�ȯ
	METAL_PLATE = 16,	///> ������
	SPRITE = 18,        ///> ����
	ATLAS = 19,			///> ͼ��
	SEED = 20,			///> ����
	CROP = 21,			///> ��������
	COOK_BOOK = 22,		///> ʳ��
	FOOD = 23,			///> ����
	PANDORA	= 26,		///> �˶���
	PET_SKILL_BOOK = 28, ///> ���＼��ѧϰ��

	HERO_EXPERIENCE_TICKET = 30, ///> Ӣ��ս������ȯ
	ARTIFACT = 31,		///> ����

	DESIGNATION_ITEM = 32, ///> �ƺ���Ʒ	

	BagExpand = 34,		///> ��������
	Tarja = 35,			///> ����

	INSCRIPTION = 36,	///> ��������

	GUILDBONUS = 37,	///> ���

	ItemTypeAll
};

///> ʱװ
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

///> װ��
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

///> ����
enum EmblemEnum
{
	AttributeEmblemCount = 8,
	FreeSkillEmblemCount = 4,
	ExtraSkillEmblemCount = 4,

	SkillEmblemCount = FreeSkillEmblemCount + ExtraSkillEmblemCount,
	EmblemCount = AttributeEmblemCount + SkillEmblemCount,

	ExtraSkillEmblemStartIndex = AttributeEmblemCount + FreeSkillEmblemCount,

	EmblemBoundary = 1000, // <= Ϊ�������£� > Ϊ��������
};

///> ����
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