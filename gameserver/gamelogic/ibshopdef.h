#ifndef __IBSHOPDEF_H__
#define __IBSHOPDEF_H__
#include "pch.h"


enum IBShopType
{
	eIBWeekLimitBuy	=1,	//ÿ���޹�
	eIBCostItem,		//����Ʒ
	eIBJade,			//����
	eIBFashion,			//ʱװ
	eIBPet,				//����
	eIBGit,				//���
	eIBVip,				//VIP
};


enum IBRefreshType
{
	eIBRefreshTimer      = 1,			//��ʱˢ��
	eIBRefreshTimePeriod = 2,			//ʱ����޹�
	eIBRefreshOpenGroupAfter	 = 3,   //���������ˢ��
	eIBRefreshOpenGroupBefore    = 4,	//����ǰ����
	eIBRefreshDayPeriod  = 7,		    //ÿ��ˢ��
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
	UINT32 nActivity;			//��ʱ����
	UINT32 nActivtiyTime;		//�ˢ��ʱ��
	UINT32 nHasBuyCount;		//�Ѿ�����ĸ���
	UINT32 nLimitStartTime;	    //�޹���ʼʱ��
	UINT32 nLimitEndTime;		//�޹�����ʱ��
	UINT32 nLimitCount;			//���ƹ������
	UINT32 nCurrencyCount;		//��������
	UINT32 nDiscount;			//�ۿ۱�ǩ
};


struct IBShopItemDetail
{
	UINT32 nGoodsID;		//��ƷID
	UINT32 nType;			//��Ʒ����
	UINT32 nSubType;		//��Ʒ����
	std::string szName;		//��Ʒ����
	UINT32 nItemId;			//��Ʒid
	UINT32 nDiscount;		//��Ʒ�ۿ�
	UINT32 nVipLevel;		//��ҪVIP�ȼ�
	UINT32 nBind;			//�������ƷΪ��
	UINT32 nLevelBuy;		//��Ҫ���ټ����ܹ���
	UINT32 nLevelShow;		//��Ҫ���ټ����ܿ���
	UINT32 nBuyCount;		//�޹�����
	UINT32 nCurrencyType;	//��������
	UINT32 nCurrencyCount;	//�������
	UINT32 nLimitStartTime; //

};




#endif