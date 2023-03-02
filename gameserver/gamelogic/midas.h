#include "unit/role.h"
#ifndef __MIDAS_H__
#define __MIDAS_H__

enum MidasConsumeType
{
	MidasConsumeType_IBShop = 1,
	MidasConsumeType_ExchangeDragon = 2,
	MidasConsumeType_CustomBattle = 3,
	MidasConsumeType_BackFlow = 4,
	MidasConsumeType_LotteryDraw = 5,
	MidasConsumeType_SummonSpirit = 6,
	MidasConsumeType_BackFlowShop = 7,
};

class IConsumeListener
{
public:
	virtual ~IConsumeListener() {}
	virtual bool OnFinish(Role* role, INT32 index, INT32 iCount)=0;
};

class IBShopConsume : public IConsumeListener
{
public:
	virtual bool OnFinish(Role* role, INT32 index, INT32 iCount);
	static IBShopConsume Listener;
};

class ExchangeDragonConsume : public IConsumeListener
{
public:
	virtual bool OnFinish(Role* role, INT32 index, INT32 iCount);
	static ExchangeDragonConsume Listener;
};

class CustomBattleConsume : public IConsumeListener
{
public:
	virtual bool OnFinish(Role* role, INT32 index, INT32 iCount);
	static CustomBattleConsume Listener;
};

class BackFlowConsume: public IConsumeListener
{
public:
	virtual bool OnFinish(Role* role, INT32 index, INT32 iCount);
	static BackFlowConsume Listener;
};

class LotteryDrawConsume : public IConsumeListener
{
public:
	virtual bool OnFinish(Role* role, INT32 index, INT32 iCount);
	static LotteryDrawConsume Listener;
};

class GuildPartySummonSpiritConsume : public IConsumeListener
{
public:
	virtual bool OnFinish(Role* role, INT32 index, INT32 iCount);
	static GuildPartySummonSpiritConsume Listener;
};

class BackFlowShopConsume : public IConsumeListener
{
public:
	virtual bool OnFinish(Role* role, INT32 index, INT32 iCount);
	static BackFlowShopConsume Listener;
};

#endif