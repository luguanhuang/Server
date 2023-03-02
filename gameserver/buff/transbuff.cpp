#include "pch.h"
#include "transbuff.h"
#include "unit/role.h"
#include "entity/XObject.h"
#include "scene/scene.h"
#include "util/gametime.h"
#include "gamelogic/itemconfig.h"
#include "buff/XBuffTemplateManager.h"
#include "buff/XBuffEnum.h"

TransformBuff::TransformBuff()
{
	Reset();
}

TransformBuff::~TransformBuff()
{

}

void TransformBuff::ToKKSG(KKSG::STransformBuff* sbuff)
{
	sbuff->mutable_buff()->set_buffid(buff.buffID);
	sbuff->mutable_buff()->set_bufflevel(buff.BuffLevel);
	sbuff->mutable_buff()->set_effecttime(buff.effecttime);

	sbuff->set_iseffecting(effectingoption);
	sbuff->set_ispassive(ispassive);
}

void TransformBuff::ToThis(const KKSG::STransformBuff* sbuff)
{
	buff.buffID = sbuff->buff().buffid();
	buff.BuffLevel = sbuff->buff().bufflevel();
	buff.effecttime = sbuff->buff().effecttime();

	effectingoption = sbuff->iseffecting();
	ispassive = sbuff->ispassive();
}

void TransformBuff::Reset()
{
	ispassive = false;
	effectingoption = false;
	buff.Reset();
}

void TransformBuff::Del(Role* role, int reason)
{
	if (0 == reason)
	{
		effectingoption = false;
	}

	XRemoveBuffActionArgs args;
	args.nBuffId = buff.buffID;
	XObject* object = role->GetXObject();
	object->DispatchAction(&args);
}

void TransformBuff::Add(Role* role, UINT32 nowtime, UINT64 casterid)
{
	XAddBuffActionArgs args;

	BuffDesc tempbuff;
	tempbuff.buffID = buff.buffID;
	tempbuff.BuffLevel = buff.BuffLevel;
	if (UINT32(-1) != buff.effecttime)
	{
		tempbuff.effecttime = buff.effecttime - nowtime;
	}

	args.vecBuffs.push_back(tempbuff);
	args.Caster = casterid;
	if (NULL != role->GetCurrScene())
	{
		args.bClientOnly = !role->GetCurrScene()->IsSyncScene();
	}

	role->GetXObject()->DispatchAction(&args);
}

UINT32 TransformBuff::GetTimeLeft()
{
	if (!Valid())
	{
		return 0;
	}
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	UINT32 timeleft = buff.effecttime > nowtime ? buff.effecttime - nowtime : 0;
	return timeleft;
}

bool TransformBuff::IsSmallBuff(UINT32 buffid)
{
	return XBuffTemplateManager::Instance()->IsScaleBuff(buffid);
}

bool TransformBuff::CanTransOther(UINT32 itemid)
{
	ItemTransformConf* transconf = ItemConfig::Instance()->GetItemTransformConf(itemid);
	if (NULL == transconf)
	{
		return false;
	}
	return transconf->type == 1;
}

