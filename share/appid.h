#ifndef _H_APP_ID_H__
#define _H_APP_ID_H__


enum EAppTag
{
	APP_TAG_NORMAL = 0,
	APP_TAG_TIYAN,
};

class CAppConfig
{
	CAppConfig():m_nAppTag(APP_TAG_NORMAL){}
	~CAppConfig(){}
	DECLARE_SINGLETON(CAppConfig);
public:
	bool Init();
	void Uninit();

	EAppTag	GetTag() const { return m_nAppTag; }
private:
	EAppTag	m_nAppTag;
};


//TiYanTest
#define TiYan_MobileQQ_App_ID  "1106046974"							//手Q AppID
#define TiYan_MobileQQ_App_Key "nVjLiPdNdoRSyaRq"					//手Q AppKey
#define TiYan_WeChat_App_ID  "wx352ac21da072fd0f"					//微信AppID
#define TiYan_WeChat_App_Key  "38e2a3ffbc9e57c299e10ef66e8f03f7"	//微信Appkey

#define TiYan_PayAppKeyTestAnd "2q6GiLCvKmygI0jF8km4y4UEHXcNmcv4" //安卓沙箱
#define TiYan_PayAppKeyAnd "cTkXr1POZunO1q3IsLAq3dWJRK98jjLT" //安卓现网
#define TiYan_PayAppKeyTestIos "ZHG2qcTlTHaEn0i1vJVzwmD50U49N4Ku" //ios沙箱
#define TiYan_PayAppKeyIos	"LwZetAtEhqGG3vbq3mVbRQ0o4VBRiJC5" //ios现网

#define TiYan_MidasAndroidAppid "1450011659"
#define TiYan_MidasIosAppid "1450012594"
#define TiYan_PayBuyGoodsAndroidAppid "1450012097" //体验服没有给id
#define TiYan_PayBuyGoodsIosAppid "1450012098" //体验服没有给id

#define TiYan_PayBuyGoodsAppKeyTestAnd "jcBilfMaABwoskPvH9JEvOFb4zeiu612" //直购安卓沙箱（体验服没有给）
#define TiYan_PayBuyGoodsAppKeyAnd "SPYUreXOrjOdg61qTR9C78Kalo7nIts8" //直购安卓现网（体验服没有给）
#define TiYan_PayBuyGoodsAppKeyTestIos "iCzXm245Ilegsm7Nw4s0y1BbAzR2mEy3" //直购ios沙箱（体验服没有给）
#define TiYan_PayBuyGoodsAppKeyIos	"MwOOnpxlkgwwYEhjD2xWz8kjyyQ2jEba" //直购ios现网（体验服没有给）


//Normal
#define Normal_MobileQQ_App_ID  "1105309683"						//手Q AppID
#define Normal_MobileQQ_App_Key "xa0seqAScOhSsgrm"					//手Q AppKey
#define Normal_WeChat_App_ID  "wxfdab5af74990787a"					//微信AppID
#define Normal_WeChat_App_Key  "6dea891b19634f98e78d27edc74125bf"	//微信Appkey

#define Normal_PayAppKeyTestAnd "XdZzmzMxZNA8s3ilJldYEAd2iJdgo3wp" //安卓沙箱
#define Normal_PayAppKeyAnd "XSMwjzY2Bg4R9q0YShAjaZRE8LDJjJdJ" //安卓现网
#define Normal_PayAppKeyTestIos "qLfZKlnLvvpT1tclqn00SeSWuU6R9lfR" //ios沙箱
#define Normal_PayAppKeyIos	"7tuC44Qf3W8S2mBVUZdepGHo2pinO6u6" //ios现网

#define Normal_MidasAndroidAppid "1450007228" //充值安卓appid
#define Normal_MidasIosAppid "1450007239" //充值ios appid
#define Normal_PayBuyGoodsAndroidAppid "1450012097" //直购安卓appid
#define Normal_PayBuyGoodsIosAppid "1450012098" //直购ios appid

#define Normal_PayBuyGoodsAppKeyTestAnd "jcBilfMaABwoskPvH9JEvOFb4zeiu612" //直购安卓沙箱
#define Normal_PayBuyGoodsAppKeyAnd "SPYUreXOrjOdg61qTR9C78Kalo7nIts8" //直购安卓现网
#define Normal_PayBuyGoodsAppKeyTestIos "iCzXm245Ilegsm7Nw4s0y1BbAzR2mEy3" //直购ios沙箱
#define Normal_PayBuyGoodsAppKeyIos	"MwOOnpxlkgwwYEhjD2xWz8kjyyQ2jEba" //直购ios现网


//RealUse
#define MobileQQ_App_ID				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_MobileQQ_App_ID			:	Normal_MobileQQ_App_ID)		//手Q AppID
#define MobileQQ_App_Key			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_MobileQQ_App_Key			:	Normal_MobileQQ_App_Key)	//手Q AppKey
#define WeChat_App_ID				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_WeChat_App_ID			:	Normal_WeChat_App_ID)		//微信AppID
#define WeChat_App_Key				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_WeChat_App_Key			:	Normal_WeChat_App_Key)		//微信Appkey

#define PayAppKeyTestAnd			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayAppKeyTestAnd			:	Normal_PayAppKeyTestAnd)	//安卓沙箱
#define PayAppKeyAnd				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayAppKeyAnd				:	Normal_PayAppKeyAnd)		//安卓现网
#define PayAppKeyTestIos			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayAppKeyTestIos			:	Normal_PayAppKeyTestIos)	//ios沙箱
#define PayAppKeyIos				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayAppKeyIos				:	Normal_PayAppKeyIos)		//ios现网

#define MidasAndroidAppid			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_MidasAndroidAppid		:	Normal_MidasAndroidAppid)	//充值安卓appid
#define MidasIosAppid				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_MidasIosAppid			:	Normal_MidasIosAppid)		//充值ios appid
#define PayBuyGoodsAndroidAppid		(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAndroidAppid	:	Normal_PayBuyGoodsAndroidAppid)		//直购安卓appid
#define PayBuyGoodsIosAppid			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsIosAppid		:	Normal_PayBuyGoodsIosAppid)			//直购ios appid

#define PayBuyGoodsAppKeyTestAnd	(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAppKeyTestAnd :	Normal_PayBuyGoodsAppKeyTestAnd)	//直购安卓沙箱
#define PayBuyGoodsAppKeyAnd		(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAppKeyAnd		:	Normal_PayBuyGoodsAppKeyAnd)		//直购安卓现网
#define PayBuyGoodsAppKeyTestIos	(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAppKeyTestIos :	Normal_PayBuyGoodsAppKeyTestIos)	//直购ios沙箱
#define PayBuyGoodsAppKeyIos		(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAppKeyIos		:	Normal_PayBuyGoodsAppKeyIos)		//直购ios现网


//心悦大R代充只有ios的;
#define XinYueRPayIosAppid "1450012203"
#define XinYueRPayAppkeyTestIos "Q5LbE1RgsiELuOeSahGqATqLS4Cwvcjs"
#define XinYueRPayAppkeyIos "XuFwwOOZMrK76p1znzyh5Uf3gwJu7njO"

#define XinGeAccessIdIOS 2296009683							// 信鸽ios access_id
#define XinGeAccessIdAnd 2196009683							// 信鸽android access_id
#define XinGeSecretKeyIOS "xa0seqAScOhSsgrm"					// 信鸽ios secret_key
#define XinGeSecretKeyAnd "xa0seqAScOhSsgrm"					// 信鸽android secret_key

#endif