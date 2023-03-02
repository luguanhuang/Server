#ifndef __REPORTDATA_DEF_H__
#define __REPORTDATA_DEF_H__

enum EnumBcover
{
	Tx_Reportdata_BcoverRank = 0, //���а�;
	Tx_Reportdata_BcoverNormal = 1,//�����а�;
};
enum EnumNestType
{
	NestHell = 0,//����Ȯ��Ѩ;
	NestLionScorpion = 1, //ʨЫ��Ѩ;
	NestTianQi = 2, //����
	NestDaZhuJiao = 3,//������
	DragonSea = 4,//����
	DragonGreen = 5,//����

};
enum EnumNestLevel
{
	NestLevel0 = 0,
	NestLevel1 = 1,
	NestLevel2 = 2,
	NestLevel3 = 3,
	NestLevel4 = 4,
};

enum EnumReportDataType
{
	Tx_Reportdata_Level = 1,		//�ȼ�;
	Tx_Reportdata_Money = 2,		//��ʯ;
	Tx_Reportdata_PkPoint = 3,		//����������;
	Tx_Reportdata_LoginTime = 8,	//�����¼ʱ��;
	Tx_Reportdata_PlatType = 12,	//ƽ̨����;
	Tx_Reportdata_PowerPoint = 17,	//ս��;
	Tx_Reportdata_TowerFloor = 19,	//�ڰ���������Ĳ���;
	Tx_Reportdata_Flower = 21,		//��ʷ����ֵ;
	Tx_Reportdata_FlowerRank = 22,	//��ʷ����ֵ����;
	Tx_Reportdata_PkTotalNum = 23,	//�������ܾ���;
	Tx_Reportdata_PkWinRate = 24,	//������ʤ��;
	Tx_Reportdata_CreateTime =25,	//ע��ʱ��;
	Tx_Reportdata_AreaID = 26,		//����ID;
	Tx_Reportdata_ServerID = 27,	//������ID;
	Tx_Reportdata_RoleID = 28,		//��ɫID;
	Tx_Reportdata_RoleName = 29,	//��ɫ����;
	Tx_Reportdata_GuildID = 30,		//��������ID;
	Tx_Reportdata_JoinGuildIDTime = 31,//���빫��ʱ��;
	Tx_Reportdata_ProfessionID = 35,//ְҵID;
	Tx_Reportdata_TotalPay = 43,	//�ۼƳ�ֵ���;
	Tx_Reportdata_PayNum = 44,		//���ʳ�ֵ���;
	Tx_Reportdata_VipLevel = 45,	//vip�ȼ�;
	Tx_Reportdata_PayTime = 46,		//��ֵʱ��;
	Tx_Reportdata_GuildName = 301,	//��������;
	Tx_Reportdata_GuildLv = 302,	//����ȼ�;
	Tx_Reportdata_GuildCreateTime = 306,//���ᴴ��ʱ��;
	Tx_Reportdata_GuildDismissTime = 307,//�����ɢʱ��;
	Tx_Reportdata_GuildMemberChange = 309,//��Ա�䶯;
	Tx_Reportdata_GuildPosChange = 311,//������Ա��ݱ��;
	Tx_Reportdata_GuildBindQQ = 312,	//����󶨵�qqȺ;
	Tx_Reportdata_GuildBindQQTime = 313, //��Ⱥ��ʱ��;
	Tx_Reportdata_PlayNestHell = 1100,	//����Ȯ��Ѩ����;
	Tx_Reportdata_PlayNestLionScorpion = 1101,//ʨЫ��Ѩ����;
	Tx_Reportdata_FirstPassNestHell = 1102,//�״ε���Ȯͨ��;
	Tx_Reportdata_FirstPassNestLionScorpion = 1103,//�״�ʨЫͨ��;
	Tx_Reportdata_WinNestHell = 1104,//����Ȯ��Ѩͨ��;
	Tx_Reportdata_WinNestLionScorpion = 1105, //ʨЫ��Ѩͨ��;
	Tx_Reportdata_PlayNestTianQi = 1106, //������Ѩ����;
	Tx_Reportdata_PlayNestDaZhuJiao = 1107, //�����̳�Ѩ����;
	Tx_Reportdata_FirstPassNestTianQi = 1108, //�״�����ͨ��;
	Tx_Reportdata_FirstPassNestDaZhuJiao = 1109, //�״δ�����ͨ��;
	Tx_Reportdata_WinNestTianQi = 1110, //������Ѩͨ��;
	Tx_Reportdata_WinNestDaZhuJiao = 1111,//������ͨ��;
	Tx_Reportdata_PlayDragonSea = 1112, //����������;
	Tx_Reportdata_PlayDragonGreen = 1113, //����������;
	Tx_Reportdata_FirstPassDragonSea = 1114,//�״κ�����ͨ��;
	Tx_Reportdata_FirstPassDragonGreen = 1115,//�״�������ͨ��;
	Tx_Reportdata_WinDragonSea = 1116,//������ͨ��;
	Tx_Reportdata_WinDragonGreen = 1117,//������ͨ��;

	Tx_Reportdata_CardsGroup1Num = 1300,//ͼ�����������Ǽ�������;
	Tx_Reportdata_CardsGroup2Num = 1301,//ͼ�������Ǽ�������;
	Tx_Reportdata_CardsGroupAddAttr = 1302,//ͼ�������ܼӳ�;
	Tx_Reportdata_WeekActiveDays = 1400,//�û���¼���ϱ�����ֵ���ۼ�1-7������¼�򱣳�ǰһ����ֵ���䣬��ĩ������;
	Tx_Reportdata_WeekEndLogin = 1401,//�û���¼���ϱ�����һ������Ϊ0�����������յ�¼�ϱ�����1;
	Tx_Reportdata_2DayWithin20Mins = 1402,//����3���Ծʱ��С��20����
	Tx_Reportdata_GuildRank = 1403,//���ڹ�������;
	Tx_Reportdata_PKDayTimes   = 1404,  // ������ÿ�մ���
	Tx_Reportdata_PKWeekTimes  = 1405,  //���������������볡��
	Tx_Reportdata_HeroBattleTimes  = 1406, //����Ӣ��ս�����볡��
	Tx_Reportdata_PvpDayTimes = 1407,	// ���챣���ӳ����볡��
	Tx_Reportdata_NestFastPassTime = 1409, //������Ѩ���ͨ��ʱ��;
	TX_Reportdata_GuildScoreLv = 1410, //���ڹ�����ֵȼ�
	Tx_Reportdata_NestDayTimes = 1411, //���쾺����Ѩ���볡��
	Tx_Reportdata_AbysssDayTimes = 1412, //���������Ԩ��������
	Tx_Reportdata_EndlessAbyss = 1413, //��������޾���Ԩ����
	Tx_Reportdata_GuildInheritTimes = 1414, //������빫�ᴫ��
	Tx_Reportdata_BossRushDayRank = 1415,//����ͨ��BOSSRUSH����
	Tx_Reportdata_HeroBattleKD = 1416,//Ӣ��ս��KD(��ɱ/������������Ϊ0��1�㣩
	Tx_Reportdata_HeroResoult  = 1417, //Ӣ��ս�������ʤ��1/ʧ��0��
	Tx_Reportdata_QuanMinPk = 1418, //ȫ��pk��
	Tx_Reportdata_BuyFashion = 1419, //����ʱװ
	Tx_Reportdata_SFashion = 1420, //ӵ��s��ʱװ
	Tx_Reportdata_AllFashion = 1421, //ӵ������ʱװ��0����A����1����S����
	Tx_Reportdata_AbyssParty = 1422, //�������������ظ�������;

	Tx_Reportdata_RecommondPoint = 1426, //��ǰ�ȼ�ս������ȥ��ǰ�ȼ��Ƽ�ս����(ÿ�ܵ�һ�ε�¼ʱ�ϱ�);
	Tx_Reportdata_MobaDayCount = 1427, //����Ӣ��Ͽ�Ȳ������;
	Tx_Reportdata_MobaResult = 1428, //Ӣ��Ͽ�Ƚ����ʤ��1/ʧ��0��;
	Tx_Reportdata_MobaKD = 1429, //Ӣ��Ͽ��KD(��ɱ/������������Ϊ0�ǰ�1�㣩;
	Tx_Reportdata_SuperRiskDayCount = 1430, //��������½ð�մ���;
	Tx_Reportdata_BuyIbShopCount = 1431,//������ʯ�̵���ߴ���;
	Tx_Reportdata_FriendCount = 1432, //��Ӻ��Ѹ���;
	Tx_Reportdata_MobaTotalCount = 1433, //����Ӣ��Ͽ�ȴ�������ʷ�ܲ��������;
	Tx_Reportdata_MobaWinCount = 1434, //Ӣ��Ͽ��ʤ����������ʷ�ܲ��������;
	Tx_Reportdata_PokerCount = 1435, //����С���˿˴�������ʷ�ܲ��������;
	Tx_Reportdata_HorseMatchCount = 1436, //����ȫ�������������ʷ�ܲ��������;
	Tx_Reportdata_SimpleDragonPass = 1437, //������ͨ��;
	Tx_Reportdata_ComposeCount = 1438, //ʱװ�ϳɲ������;
	Tx_Reportdata_ComposeSuccessCount = 1439,	//ʱװ�ϳɳɹ�����;
	Tx_Reportdata_ComposeFailCount = 1440, //ʱװ�ϳ�ʧ�ܴ���;
	Tx_Reportdata_ComposeSuccessSCount = 1441, //�ϳ�S��ʱװ�ĳɹ�����;
	Tx_Reportdata_ComposeSuccessACount = 1442, //�ϳ�A��ʱװ�ĳɹ�����;

	Tx_Reportdata_PkResult = 1502,		//���������;
	Tx_Reportdata_PkDailyTime = 1504,	//�����������ۼ���Ϸʱ��;
	Tx_Reportdata_PkRank = 1505,		//����������(ÿ�δ�����ϱ�);
	Tx_Reportdata_PvpWinRate = 1601,	//�����ӳ�ʤ��;
	Tx_Reportdata_PvpResult = 1602,		//�����ӳ����;
	Tx_Reportdata_PvpTotalNum = 1603,	//�����ӳ������ܳ���;
	Tx_Reportdata_PvpDailyTime = 1604,	//���챣���ӳ��ۼ���Ϸʱ��;


	Tx_Reportdata_DailyOnlineTime = 6000, //�����ۼ�����ʱ��ʱ��;

};

#endif