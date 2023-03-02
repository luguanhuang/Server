#ifndef __IBSHOPDEF_H__
#define __IBSHOPDEF_H__
#include "pch.h"


enum IBShopType
{
	eIBWeekLimitBuy	=1,	//每周限购
	eIBCostItem,		//消耗品
	eIBJade,			//龙玉
	eIBFashion,			//时装
	eIBPet,				//坐骑
	eIBGit,				//礼包
	eIBVip,				//VIP
};


enum IBRefreshType
{
	eIBRefreshTimer      = 1,			//定时刷新
	eIBRefreshTimePeriod = 2,			//时间段限购
	eIBRefreshOpenGroupAfter	 = 3,   //开服几天后刷新
	eIBRefreshOpenGroupBefore    = 4,	//开服前几天
	eIBRefreshDayPeriod  = 7,		    //每日刷新
};

struct stTime
{
	stTime():nStartTime(0),nEndTime(0){}
	UINT32 nStartTime;
	UINT32 nEndTime;
};

struct stIBShopItem
{
	UINT32 nGoodsID;			//goodsid
	UINT32 nActivity;			//限时次数
	UINT32 nActivtiyTime;		//活动刷新时间
	UINT32 nHasBuyCount;		//已经购买的个数
	UINT32 nLimitStartTime;	    //限购开始时间
	UINT32 nLimitEndTime;		//限购结束时间
	UINT32 nLimitCount;			//限制购买个数
	UINT32 nCurrencyCount;		//货币数量
	UINT32 nDiscount;			//折扣标签
};


struct IBShopItemDetail
{
	UINT32 nGoodsID;		//商品ID
	UINT32 nType;			//商品类型
	UINT32 nSubType;		//商品子类
	std::string szName;		//商品名字
	UINT32 nItemId;			//物品id
	UINT32 nDiscount;		//物品折扣
	UINT32 nVipLevel;		//需要VIP等级
	UINT32 nBind;			//购买后物品为绑定
	UINT32 nLevelBuy;		//需要多少级才能购买
	UINT32 nLevelShow;		//需要多少级才能看到
	UINT32 nBuyCount;		//限购数量
	UINT32 nCurrencyType;	//购买类型
	UINT32 nCurrencyCount;	//购买个数
	UINT32 nLimitStartTime; //

};




#endif