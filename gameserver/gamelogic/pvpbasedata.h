#ifndef __PVPBASEDATA_H__
#define __PVPBASEDATA_H__

#include "pb/project.pb.h"

//���õ�---��
enum PvpBattleState
{
	PVP_ROLE_NONE,
	PVP_ROLE_WAITLOAD,
	PVP_ROLE_LOADED,
	PVP_ROLE_QUIT,

	PVP_ROLE_ERR
};

enum PvpCampType
{
	PVP_CAMP_NONE = 0,
	PVP_CAMP_RED = 1,//== group 1
	PVP_CAMP_BLUE = 2,// == group 2

	PVP_CAMP_ERR = 3//Ҳ���� ��������ƽ��
};
//��������

class PvpBattleData
{
public:
	PvpBattleData()
	{
		roleid = 0;
		killLeaderCount = 0;
		rolestate = PVP_ROLE_ERR;
		isleader = false;
		camp = PVP_CAMP_ERR; 
		lastDieTime = (time_t)(0);// 0 ����û����

	}

	UINT64 roleid;
	PvpBattleState rolestate;
	PvpCampType camp;
	KKSG::RoleSmallInfo smallInfo;

	int killLeaderCount;//��ɱ�ӳ�����
	bool isleader;
	
	time_t lastDieTime;

	void TestPrint();
};

#endif