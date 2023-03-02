#ifndef __GOALAWARDSDEF__H__
#define __GOALAWARDSDEF__H__

enum GOAL_AWARD_ID
{
    GOAL_AWARD_ID_1 = 1,    //玩家升级

    GOAL_AWARD_ID_101 = 101,          //101战场单局人头
    GOAL_AWARD_ID_102 = 102,          //102战场单局助攻
    GOAL_AWARD_ID_103 = 103,          //103战场单局团队输出
    GOAL_AWARD_ID_104 = 104,          //104战场累计人头
    GOAL_AWARD_ID_105 = 105,          //105战场累计助攻
    GOAL_AWARD_ID_106 = 106,          //106战场累计输出
    GOAL_AWARD_ID_107 = 107,          //107战场mvp次数
    GOAL_AWARD_ID_108 = 108,          //108战场胜利次数

    GOAL_AWARD_ID_200 = 200,          //200天梯总胜利场次
    GOAL_AWARD_ID_201 = 201,          //201天梯对战战士胜利场次
    GOAL_AWARD_ID_202 = 202,          //202天梯对战弓箭手胜利场次
    GOAL_AWARD_ID_203 = 203,          //203天梯对战魔法师胜利场次
    GOAL_AWARD_ID_204 = 204,          //204天梯对战牧师胜利场次
    GOAL_AWARD_ID_205 = 205,          //205天梯对战学者胜利场次
    GOAL_AWARD_ID_206 = 206,          //206天梯对战刺客胜利场次
    GOAL_AWARD_ID_207 = 207,          //207天梯对战舞娘胜利场次

    GOAL_AWARD_ID_301 = 301,          //301峡谷单局人头
    GOAL_AWARD_ID_302 = 302,          //302峡谷单局助攻
    GOAL_AWARD_ID_303 = 303,          //303峡谷单局团队输出
    GOAL_AWARD_ID_304 = 304,          //304峡谷单局个人输出
    GOAL_AWARD_ID_305 = 305,          //305峡谷累计人头
    GOAL_AWARD_ID_306 = 306,          //306峡谷累计助攻
    GOAL_AWARD_ID_307 = 307,          //307峡谷累计输出
    GOAL_AWARD_ID_308 = 308,          //308峡谷累计mvp
    GOAL_AWARD_ID_309 = 309,          //309峡谷无队友死亡击败对手人数
    GOAL_AWARD_ID_310 = 310,          //310峡谷胜利场数

    GOAL_AWARD_ID_401 = 401,          //401巢穴通关时间
    GOAL_AWARD_ID_402 = 402,          //402巢穴通关不死人数
    GOAL_AWARD_ID_403 = 403,          //403巢穴平均输出

    GOAL_AWARD_ID_501 = 501,          //501龙本通关时间
    GOAL_AWARD_ID_502 = 502,          //502龙本无伤通关人数
    GOAL_AWARD_ID_503 = 503,          //503龙本平均输出量
    GOAL_AWARD_ID_504 = 504,          //504龙本通关不死人数
    GOAL_AWARD_ID_505 = 505,          //505龙本通关零输出不死

    GOAL_AWARD_ID_601 = 601,          //601好友组队通关次数
    GOAL_AWARD_ID_602 = 602,          //602累计好友度
    GOAL_AWARD_ID_603 = 603,          //603微信手Q好友组队通关次数

    GOAL_AWARD_ID_701 = 701,          //701龙本累计助战次数
    GOAL_AWARD_ID_702 = 702,          //702巢穴累计助战次数

    GOAL_AWARD_ID_801 = 801,          //801家园料理等级
    GOAL_AWARD_ID_802 = 802,          //802家园驱赶小妖精次数
    GOAL_AWARD_ID_803 = 803,          //803家园钓鱼等级
    GOAL_AWARD_ID_804 = 804,          //804家园举办家宴次数

    GOAL_AWARD_ID_901 = 901,          //901大乱斗累计击杀

    GOAL_AWARD_ID_1001 = 1001,        //1001世界boss伤害名次
    GOAL_AWARD_ID_1002 = 1002,        //1002世界boss累计输出
    GOAL_AWARD_ID_1003 = 1003,        //1003世界boss累计次数

    GOAL_AWARD_ID_1101 = 1101,       //11龙本关卡不死通关
    GOAL_AWARD_ID_1102,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1103,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1104,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1105,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1106,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1107,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1108,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1109,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1110,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1111,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1112,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1113,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1114,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1115,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1116,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1117,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1118,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1119,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1120,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1121,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1122,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1123,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1124,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1125,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1126,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1127,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1128,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1129,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1130,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1131,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1132,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1133,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1134,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1135,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1136,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1137,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1138,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1139,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1140,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1141,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1142,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1143,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1144,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1145,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1146,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1147,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1148,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1149,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1150,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1151,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1152,              //11龙本关卡不死通关
    GOAL_AWARD_ID_1153,              //11龙本关卡不死通关
    GOAL_AWARD_ID_11xx,              //11龙本关卡不死通关END

    GOAL_AWARD_ID_1201 = 1201,       //12小龙本x分钟通关
    GOAL_AWARD_ID_1202,              //12小龙本x分钟通关
    GOAL_AWARD_ID_1203,              //12小龙本x分钟通关
    GOAL_AWARD_ID_1204,              //12小龙本x分钟通关
    GOAL_AWARD_ID_1205,              //12小龙本x分钟通关
    GOAL_AWARD_ID_1206,              //12小龙本x分钟通关
    GOAL_AWARD_ID_1207,              //12小龙本x分钟通关
    GOAL_AWARD_ID_1208,              //12小龙本x分钟通关
    GOAL_AWARD_ID_12xx,              //12小龙本x分钟通关END

    GOAL_AWARD_ID_1301 = 1301,       //13小龙本零输出不死
    GOAL_AWARD_ID_1302,              //13小龙本零输出不死
    GOAL_AWARD_ID_1303,              //13小龙本零输出不死
    GOAL_AWARD_ID_1304,              //13小龙本零输出不死
    GOAL_AWARD_ID_1305,              //13小龙本零输出不死
    GOAL_AWARD_ID_1306,              //13小龙本零输出不死
    GOAL_AWARD_ID_1307,              //13小龙本零输出不死
    GOAL_AWARD_ID_1308,              //13小龙本零输出不死
    GOAL_AWARD_ID_13xx,              //13小龙本零输出不死END

    GOAL_AWARD_ID_1401 = 1401,       //1401结婚关卡胜利次数
};

#endif
