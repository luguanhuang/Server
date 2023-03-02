#ifndef __ROLESUMMARYBASE_H__
#define __ROLESUMMARYBASE_H__

#include "time.h"
#include "pb/project.pb.h"

enum RoleSummaryField
{
	///> 基本信息
	SUMMARY_ID				=	1 << 0,
	SUMMARY_LEVEL			=	1 << 1,
	SUMMARY_PROFESSION		=	1 << 2,
	SUMMARY_NAME			=	1 << 3,
	SUMMARY_NICKID			=	1 << 17,
	SUMMARY_VIPLEVEL		=	1 << 10,
	SUMMARY_PAYMEMBER		=	1 << 26,
	///> 身上外观
	SUMMARY_FASHION			=	1 << 6,
	SUMMARY_EQUIPOUT		=   1 << 21,
	SUMMARY_OP				=   1 << 20,
	SUMMARY_DISPLAYFASHION  =   1 << 30,
	///> 头顶
	SUMMARY_DESIGNATION		=   1 << 13, 
	SUMMARY_TITLE			=   1 << 19,
	SUMMARY_GUILD			=	1 << 14,
	SUMMARY_MILITARY_RANK	=	1 << 29,
	SUMMARY_PAYSCOREPRE		=   1 << 16,
	///> 属性
	SUMMARY_ATTR			=   1 << 5,
	SUMMARY_EQUIP			=	1 << 7,
	SUMMARY_SKILL			=	1 << 8,
	SUMMARY_EMBLEM			=	1 << 9,
	SUMMARY_ARTIFACT		=	1 << 15,
	///> 宠物
	SUMMARY_PETS            =   1 << 25,
	///> 精灵
	SUMMARY_SPRITELEADER	=	1 << 23,
	SUMMARY_SPRITE			=	1 << 24,
	///> 竞技场
	SUMMARY_ARENA			=	1 << 4,
	///> 其他
	SUMMARY_LASTLOGOUT		=	1 << 11,
	SUMMARY_PUSHINFO		=	1 << 22,

	///> 不是rolesummary字段
	SUMMARY_STATE			=	1 << 12,

	SUMMARY_QQVIP			=	1 << 27,
	SUMMARY_STARTUP			=	1 << 28,

	SUMMARY_BASIC			=	SUMMARY_ID | SUMMARY_LEVEL | SUMMARY_PROFESSION | SUMMARY_NAME | SUMMARY_NICKID | SUMMARY_VIPLEVEL,

	SUMMARY_OUTLOOK			=   SUMMARY_BASIC | SUMMARY_OP | SUMMARY_FASHION | SUMMARY_EQUIPOUT | SUMMARY_SPRITELEADER
								| SUMMARY_GUILD | SUMMARY_DESIGNATION | SUMMARY_TITLE | SUMMARY_DISPLAYFASHION,

	SUMMARY_ALL				=	(1U << 31) - 1,
	/*SUMMARY_ALL             =   SUMMARY_BASIC | SUMMARY_FASHION | SUMMARY_EQUIPOUT | SUMMARY_OP 
								| SUMMARY_ATTR | SUMMARY_SKILL | SUMMARY_EMBLEM | SUMMARY_EQUIP 
								| SUMMARY_DESIGNATION | SUMMARY_GUILD | SUMMARY_TITLE
								| SUMMARY_SPRITELEADER | SUMMARY_SPRITE | SUMMARY_PETS
								| SUMMARY_ARENA | SUMMARY_LASTLOGOUT |  SUMMARY_PUSHINFO | SUMMARY_PAYMEMBER | SUMMARY_QQVIP | SUMMARY_STARTUP 
								| SUMMARY_MILITARY_RANK | SUMMARY_DISPLAYFASHION | SUMMARY_ARTIFACT,*/

};

class RoleSummaryBase
{
public:
	RoleSummaryBase():m_nMaxPPT(0) { };
	virtual ~RoleSummaryBase() { };

	UINT64 GetID() const							{ return m_oData.roleid(); }
	UINT32 GetLevel() const							{ return m_oData.level(); }
	UINT32 GetNickID() const						{ return m_oData.nickid(); }
	UINT32 GetTitleID() const						{ return m_oData.titleid(); }
	const std::string& GetName() const				{ return m_oData.name(); }
	const UINT32 GetProfession() const				{ return m_oData.profession(); }
	const UINT32 GetPowerPoint() const				{ return m_oData.powerpoint(); }
	const UINT32 GetCoverDesignationID() const		{ return m_oData.desigation(); }
	const UINT32 GetVipLevel() const                { return m_oData.viplevel(); }
	const std::string& GetAccount() const			{ return m_oData.account(); }
	const std::string& GetDeviceId() const			{ return m_oData.device_id(); }
	UINT32 GetPlatType() const						{ return m_oData.plat_type(); }
	UINT32 GetSpriteLeaderID() const                { return 0 == m_oData.sprites_size() ? 0 : m_oData.sprites(0).spriteid(); }
	UINT32 GetPkPoint() const						{ return m_oData.pkpoint(); }
	const std::string& GetToken() const				{ return m_oData.token(); }

	UINT32 GetPayMemberID();

	void SetName(const std::string& name)	{ m_oData.set_name(name); }

	bool Init(const KKSG::RoleSummaryStored& rodata);
	const KKSG::RoleSummaryStored& GetData() { return m_oData; }

	///> 构造appearance
	void Build(KKSG::UnitAppearance* appearance, int mask);

	void SetUnitBasic(KKSG::UnitAppearance* pUnitAppearnce);

	void SetUnitAttr(KKSG::UnitAppearance* pUnitAppearnce);
	void SetUnitSkill(KKSG::UnitAppearance* pUnitAppearnce);

	void SetUnitFashion(KKSG::UnitAppearance* pUnitAppearnce);
	void SetUnitEquip(KKSG::UnitAppearance* pUnitAppearnce);
	void SetUnitEquipLook(KKSG::UnitAppearance* pUnitAppearnce);
	void SetUnitEmblem(KKSG::UnitAppearance* pUnitAppearance);
	void SetUnitArtifact(KKSG::UnitAppearance* pUnitAppearance);

	void SetMilitaryRank(KKSG::UnitAppearance* pUnitAppearnce);
	void SetUnitDisplayFashionLook(KKSG::UnitAppearance* pUnitAppearnce);
	void SetPayScore(KKSG::UnitAppearance* pUnitAppearnce);

	bool IsQQVip();
	bool IsQQSVip();
	bool IsQQYearVip();
	KKSG::StartUpType GetStartUpType();

	void GetFashion(std::vector<UINT32>& fashions);
	UINT32 GetMilitaryRank();

	inline void SetMaxPPT(const UINT32 nPPT) 
	{
		if (nPPT>m_nMaxPPT)
			m_nMaxPPT = nPPT;
	}

	inline const UINT32 GetMaxPPT()
	{
		return m_nMaxPPT;
	}

	bool IsBackFlowRole();

private:
	UINT32 m_nMaxPPT;
protected:
	KKSG::RoleSummaryStored m_oData;
};

#endif