#ifndef __GOALAWARDSDEF__H__
#define __GOALAWARDSDEF__H__

enum GOAL_AWARD_ID
{
    GOAL_AWARD_ID_1 = 1,    //�������

    GOAL_AWARD_ID_101 = 101,          //101ս��������ͷ
    GOAL_AWARD_ID_102 = 102,          //102ս����������
    GOAL_AWARD_ID_103 = 103,          //103ս�������Ŷ����
    GOAL_AWARD_ID_104 = 104,          //104ս���ۼ���ͷ
    GOAL_AWARD_ID_105 = 105,          //105ս���ۼ�����
    GOAL_AWARD_ID_106 = 106,          //106ս���ۼ����
    GOAL_AWARD_ID_107 = 107,          //107ս��mvp����
    GOAL_AWARD_ID_108 = 108,          //108ս��ʤ������

    GOAL_AWARD_ID_200 = 200,          //200������ʤ������
    GOAL_AWARD_ID_201 = 201,          //201���ݶ�սսʿʤ������
    GOAL_AWARD_ID_202 = 202,          //202���ݶ�ս������ʤ������
    GOAL_AWARD_ID_203 = 203,          //203���ݶ�սħ��ʦʤ������
    GOAL_AWARD_ID_204 = 204,          //204���ݶ�ս��ʦʤ������
    GOAL_AWARD_ID_205 = 205,          //205���ݶ�սѧ��ʤ������
    GOAL_AWARD_ID_206 = 206,          //206���ݶ�ս�̿�ʤ������
    GOAL_AWARD_ID_207 = 207,          //207���ݶ�ս����ʤ������

    GOAL_AWARD_ID_301 = 301,          //301Ͽ�ȵ�����ͷ
    GOAL_AWARD_ID_302 = 302,          //302Ͽ�ȵ�������
    GOAL_AWARD_ID_303 = 303,          //303Ͽ�ȵ����Ŷ����
    GOAL_AWARD_ID_304 = 304,          //304Ͽ�ȵ��ָ������
    GOAL_AWARD_ID_305 = 305,          //305Ͽ���ۼ���ͷ
    GOAL_AWARD_ID_306 = 306,          //306Ͽ���ۼ�����
    GOAL_AWARD_ID_307 = 307,          //307Ͽ���ۼ����
    GOAL_AWARD_ID_308 = 308,          //308Ͽ���ۼ�mvp
    GOAL_AWARD_ID_309 = 309,          //309Ͽ���޶����������ܶ�������
    GOAL_AWARD_ID_310 = 310,          //310Ͽ��ʤ������

    GOAL_AWARD_ID_401 = 401,          //401��Ѩͨ��ʱ��
    GOAL_AWARD_ID_402 = 402,          //402��Ѩͨ�ز�������
    GOAL_AWARD_ID_403 = 403,          //403��Ѩƽ�����

    GOAL_AWARD_ID_501 = 501,          //501����ͨ��ʱ��
    GOAL_AWARD_ID_502 = 502,          //502��������ͨ������
    GOAL_AWARD_ID_503 = 503,          //503����ƽ�������
    GOAL_AWARD_ID_504 = 504,          //504����ͨ�ز�������
    GOAL_AWARD_ID_505 = 505,          //505����ͨ�����������

    GOAL_AWARD_ID_601 = 601,          //601�������ͨ�ش���
    GOAL_AWARD_ID_602 = 602,          //602�ۼƺ��Ѷ�
    GOAL_AWARD_ID_603 = 603,          //603΢����Q�������ͨ�ش���

    GOAL_AWARD_ID_701 = 701,          //701�����ۼ���ս����
    GOAL_AWARD_ID_702 = 702,          //702��Ѩ�ۼ���ս����

    GOAL_AWARD_ID_801 = 801,          //801��԰����ȼ�
    GOAL_AWARD_ID_802 = 802,          //802��԰����С��������
    GOAL_AWARD_ID_803 = 803,          //803��԰����ȼ�
    GOAL_AWARD_ID_804 = 804,          //804��԰�ٰ�������

    GOAL_AWARD_ID_901 = 901,          //901���Ҷ��ۼƻ�ɱ

    GOAL_AWARD_ID_1001 = 1001,        //1001����boss�˺�����
    GOAL_AWARD_ID_1002 = 1002,        //1002����boss�ۼ����
    GOAL_AWARD_ID_1003 = 1003,        //1003����boss�ۼƴ���

    GOAL_AWARD_ID_1101 = 1101,       //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1102,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1103,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1104,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1105,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1106,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1107,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1108,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1109,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1110,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1111,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1112,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1113,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1114,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1115,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1116,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1117,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1118,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1119,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1120,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1121,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1122,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1123,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1124,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1125,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1126,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1127,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1128,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1129,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1130,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1131,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1132,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1133,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1134,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1135,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1136,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1137,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1138,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1139,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1140,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1141,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1142,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1143,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1144,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1145,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1146,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1147,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1148,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1149,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1150,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1151,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1152,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_1153,              //11�����ؿ�����ͨ��
    GOAL_AWARD_ID_11xx,              //11�����ؿ�����ͨ��END

    GOAL_AWARD_ID_1201 = 1201,       //12С����x����ͨ��
    GOAL_AWARD_ID_1202,              //12С����x����ͨ��
    GOAL_AWARD_ID_1203,              //12С����x����ͨ��
    GOAL_AWARD_ID_1204,              //12С����x����ͨ��
    GOAL_AWARD_ID_1205,              //12С����x����ͨ��
    GOAL_AWARD_ID_1206,              //12С����x����ͨ��
    GOAL_AWARD_ID_1207,              //12С����x����ͨ��
    GOAL_AWARD_ID_1208,              //12С����x����ͨ��
    GOAL_AWARD_ID_12xx,              //12С����x����ͨ��END

    GOAL_AWARD_ID_1301 = 1301,       //13С�������������
    GOAL_AWARD_ID_1302,              //13С�������������
    GOAL_AWARD_ID_1303,              //13С�������������
    GOAL_AWARD_ID_1304,              //13С�������������
    GOAL_AWARD_ID_1305,              //13С�������������
    GOAL_AWARD_ID_1306,              //13С�������������
    GOAL_AWARD_ID_1307,              //13С�������������
    GOAL_AWARD_ID_1308,              //13С�������������
    GOAL_AWARD_ID_13xx,              //13С�������������END

    GOAL_AWARD_ID_1401 = 1401,       //1401���ؿ�ʤ������
};

#endif
