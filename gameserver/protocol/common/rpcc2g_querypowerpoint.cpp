#include "pch.h"
#include "common/rpcc2g_querypowerpoint.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "unit/combatattribute.h"
#include "gamelogic/bag.h"
#include "gamelogic/rolefashion.h"
#include "gamelogic/skilllevel.h"
#include "gamelogic/designationMgr.h"
#include "unit/calcattribute.h"
#include "table/PowerPointMgr.h"
#include "gamelogic/titlerecord.h"
#include "unit/pet.h"
#include "gamelogic/atlas.h"
#include "gamelogic/spriterecord.h"
#include "guild/guildrecord.h"

// generate by ProtoGen at date: 2016/8/8 21:01:12

RPC_SERVER_IMPLEMETION(RpcC2G_QueryPowerPoint, QueryPowerPointArg, QueryPowerPointRes)

/*
	BQ的id是读FpStrengthNew.txt
*/

void RpcC2G_QueryPowerPoint::OnCall(const QueryPowerPointArg &roArg, QueryPowerPointRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	CalcRoleAttribute InitRoleAttribute;

	///> 背包里的
	std::vector<CombatAttribute> bagBasicAttr; //背包
	// 0是装备本身（包含套装属性）,1是强化属性，2是龙玉属性，3是锻造属性，4是附魔属性，5是纹章属性, 6是龙器属性
	pRole->Get<Bag>()->GetDifferentModuleAttr(bagBasicAttr); 
	for(UINT32 i = 0; i < 7; i++)
	{
		InitRoleAttribute.CalcAttributeConvert(&bagBasicAttr[i], pRole->GetProfession()); //一级属性到二级属性转换
	}
	double ppt = PowerPointMgr::Instance()->CalcPowerPoint(&bagBasicAttr[0], pRole->GetProfession());
	roRes.add_bqid(1); roRes.add_ppt(ppt); // 装备 BQ = 1
	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&bagBasicAttr[1], pRole->GetProfession());
	roRes.add_bqid(2); roRes.add_ppt(ppt); // 强化 BQ = 2
	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&bagBasicAttr[2], pRole->GetProfession());
	roRes.add_bqid(7); roRes.add_ppt(ppt); // 龙玉 BQ = 7
	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&bagBasicAttr[3], pRole->GetProfession());
	roRes.add_bqid(90); roRes.add_ppt(ppt); // 锻造 BQ = 90
	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&bagBasicAttr[4], pRole->GetProfession());
	roRes.add_bqid(91); roRes.add_ppt(ppt); // 附魔 BQ = 91
	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&bagBasicAttr[5], pRole->GetProfession());
	roRes.add_bqid(6); roRes.add_ppt(ppt); // 纹章 BQ = 6
	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&bagBasicAttr[6], pRole->GetProfession());
	roRes.add_bqid(92); roRes.add_ppt(ppt);	// 龙器 BQ = 92

	///> 图鉴
	CombatAttribute atlasAttr;
	pRole->Get<CAtlasSys>()->ApplyEffect(&atlasAttr);
	InitRoleAttribute.CalcAttributeConvert(&atlasAttr, pRole->GetProfession());

	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&atlasAttr, pRole->GetProfession());
	roRes.add_bqid(10); roRes.add_ppt(ppt); // 图鉴 BQ = 10

	///> 精灵
	CombatAttribute spriteAttr;
	pRole->Get<CSpriteRecord>()->ApplyEffect(&spriteAttr);
	InitRoleAttribute.CalcAttributeConvert(&spriteAttr, pRole->GetProfession());

	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&spriteAttr, pRole->GetProfession());
	roRes.add_bqid(11); roRes.add_ppt(ppt); // 精灵 BQ = 11

	///> 头衔
	CombatAttribute titleAttr;
	pRole->Get<TitleRecord>()->ApplyEffect(&titleAttr);
	InitRoleAttribute.CalcAttributeConvert(&titleAttr, pRole->GetProfession());

	ppt = PowerPointMgr::Instance()->CalcPowerPoint(&titleAttr, pRole->GetProfession());
	roRes.add_bqid(12); roRes.add_ppt(ppt); // 头衔 BQ = 12

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_QueryPowerPoint::OnDelayReplyRpc(const QueryPowerPointArg &roArg, QueryPowerPointRes &roRes, const CUserData &roUserData)
{
}
