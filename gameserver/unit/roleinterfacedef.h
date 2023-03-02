
#ifndef __ROLEINTERFACEDEF_H__
#define __ROLEINTERFACEDEF_H__

#include "typelist.h"

class Role;
class StageMgr;
class Bag;
class LotteryRecord;
class CCheckinRecord;
class CActivityRecord;
class RoleFashion;
class CArenaRecord;
class PayRecord;
class BuyRecord;
class CRewardRecord;
class CShopRecord;
class CFlowerRecord;
class CGuildRecord;
class RoleSkillSet;
class CombatAttribute;
class CPkRecord;
class CTShowRecord;
class CCampRecord;
class CFindBackRecord;
class CTowerRecord;
class DesignationRecord;
class CLevelSealRecord;
class AchieveV2Record;
class MyLoginRecord;
class MyPvpRecord;
class QARecord;
class CDragonRecord;
class CLiveRecord;
class ChatRecord;
class CPayV2Record;
class PetSys;
class TitleRecord;
class CFirstPassRecord;
class IBShopRecord;
class CAtlasSys;
class CSpriteRecord;
class CSuperRiskRecord;
class TaskRecord;
class CIdipRecord;
class SpActivityRecord;
class BuffRecord;
class RolePushInfoRecord;
class QQVipRecord;
class CTeamRecord;
class CHeroBattleRecord;
class CReportDataRecord;
class CRoleSystem;
class MilitaryRankRecord;
class PlatformShareRecord;
class RoleMisc;
class RolePartner;
class WeekEnd4v4Record;
class CTaJieHelpRecord;
class DragonGroupRecord;
class CBattleFieldRecord;
class CNpcFeelingRecord;
class CCompeteDragonRecord;
class DragonGuildRecord;

//每行4个，保持队形
typedef TYPELIST_59(
	Role, CRoleSystem, StageMgr, Bag, 
	LotteryRecord, CCheckinRecord, CActivityRecord, RoleFashion, 
	CArenaRecord, PayRecord, BuyRecord, CRewardRecord, 
	CShopRecord, RoleSkillSet, CombatAttribute, CFlowerRecord,
	CGuildRecord, CPkRecord, CTShowRecord, CCampRecord,
	CTowerRecord, CFindBackRecord, TaskRecord, SpActivityRecord, 
	MyLoginRecord, MyPvpRecord, DesignationRecord, CLevelSealRecord, 
	AchieveV2Record, QARecord, CDragonRecord, CLiveRecord, 
	CPayV2Record, ChatRecord, PetSys, TitleRecord, 
	CFirstPassRecord, IBShopRecord, CAtlasSys, CSpriteRecord, 
	CSuperRiskRecord, CIdipRecord, BuffRecord, RolePushInfoRecord, 
	QQVipRecord, CTeamRecord, CHeroBattleRecord, CReportDataRecord, 
	MilitaryRankRecord, PlatformShareRecord, RoleMisc, RolePartner, 
	WeekEnd4v4Record, CTaJieHelpRecord, DragonGroupRecord, CBattleFieldRecord,
	CNpcFeelingRecord, CCompeteDragonRecord, DragonGuildRecord) ROLEINTERFACETYPES;

#endif // __ROLEINTERFACEDEF_H__
