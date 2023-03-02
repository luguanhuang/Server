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
#define TiYan_MobileQQ_App_ID  "1106046974"							//��Q AppID
#define TiYan_MobileQQ_App_Key "nVjLiPdNdoRSyaRq"					//��Q AppKey
#define TiYan_WeChat_App_ID  "wx352ac21da072fd0f"					//΢��AppID
#define TiYan_WeChat_App_Key  "38e2a3ffbc9e57c299e10ef66e8f03f7"	//΢��Appkey

#define TiYan_PayAppKeyTestAnd "2q6GiLCvKmygI0jF8km4y4UEHXcNmcv4" //��׿ɳ��
#define TiYan_PayAppKeyAnd "cTkXr1POZunO1q3IsLAq3dWJRK98jjLT" //��׿����
#define TiYan_PayAppKeyTestIos "ZHG2qcTlTHaEn0i1vJVzwmD50U49N4Ku" //iosɳ��
#define TiYan_PayAppKeyIos	"LwZetAtEhqGG3vbq3mVbRQ0o4VBRiJC5" //ios����

#define TiYan_MidasAndroidAppid "1450011659"
#define TiYan_MidasIosAppid "1450012594"
#define TiYan_PayBuyGoodsAndroidAppid "1450012097" //�����û�и�id
#define TiYan_PayBuyGoodsIosAppid "1450012098" //�����û�и�id

#define TiYan_PayBuyGoodsAppKeyTestAnd "jcBilfMaABwoskPvH9JEvOFb4zeiu612" //ֱ����׿ɳ�䣨�����û�и���
#define TiYan_PayBuyGoodsAppKeyAnd "SPYUreXOrjOdg61qTR9C78Kalo7nIts8" //ֱ����׿�����������û�и���
#define TiYan_PayBuyGoodsAppKeyTestIos "iCzXm245Ilegsm7Nw4s0y1BbAzR2mEy3" //ֱ��iosɳ�䣨�����û�и���
#define TiYan_PayBuyGoodsAppKeyIos	"MwOOnpxlkgwwYEhjD2xWz8kjyyQ2jEba" //ֱ��ios�����������û�и���


//Normal
#define Normal_MobileQQ_App_ID  "1105309683"						//��Q AppID
#define Normal_MobileQQ_App_Key "xa0seqAScOhSsgrm"					//��Q AppKey
#define Normal_WeChat_App_ID  "wxfdab5af74990787a"					//΢��AppID
#define Normal_WeChat_App_Key  "6dea891b19634f98e78d27edc74125bf"	//΢��Appkey

#define Normal_PayAppKeyTestAnd "XdZzmzMxZNA8s3ilJldYEAd2iJdgo3wp" //��׿ɳ��
#define Normal_PayAppKeyAnd "XSMwjzY2Bg4R9q0YShAjaZRE8LDJjJdJ" //��׿����
#define Normal_PayAppKeyTestIos "qLfZKlnLvvpT1tclqn00SeSWuU6R9lfR" //iosɳ��
#define Normal_PayAppKeyIos	"7tuC44Qf3W8S2mBVUZdepGHo2pinO6u6" //ios����

#define Normal_MidasAndroidAppid "1450007228" //��ֵ��׿appid
#define Normal_MidasIosAppid "1450007239" //��ֵios appid
#define Normal_PayBuyGoodsAndroidAppid "1450012097" //ֱ����׿appid
#define Normal_PayBuyGoodsIosAppid "1450012098" //ֱ��ios appid

#define Normal_PayBuyGoodsAppKeyTestAnd "jcBilfMaABwoskPvH9JEvOFb4zeiu612" //ֱ����׿ɳ��
#define Normal_PayBuyGoodsAppKeyAnd "SPYUreXOrjOdg61qTR9C78Kalo7nIts8" //ֱ����׿����
#define Normal_PayBuyGoodsAppKeyTestIos "iCzXm245Ilegsm7Nw4s0y1BbAzR2mEy3" //ֱ��iosɳ��
#define Normal_PayBuyGoodsAppKeyIos	"MwOOnpxlkgwwYEhjD2xWz8kjyyQ2jEba" //ֱ��ios����


//RealUse
#define MobileQQ_App_ID				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_MobileQQ_App_ID			:	Normal_MobileQQ_App_ID)		//��Q AppID
#define MobileQQ_App_Key			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_MobileQQ_App_Key			:	Normal_MobileQQ_App_Key)	//��Q AppKey
#define WeChat_App_ID				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_WeChat_App_ID			:	Normal_WeChat_App_ID)		//΢��AppID
#define WeChat_App_Key				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_WeChat_App_Key			:	Normal_WeChat_App_Key)		//΢��Appkey

#define PayAppKeyTestAnd			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayAppKeyTestAnd			:	Normal_PayAppKeyTestAnd)	//��׿ɳ��
#define PayAppKeyAnd				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayAppKeyAnd				:	Normal_PayAppKeyAnd)		//��׿����
#define PayAppKeyTestIos			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayAppKeyTestIos			:	Normal_PayAppKeyTestIos)	//iosɳ��
#define PayAppKeyIos				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayAppKeyIos				:	Normal_PayAppKeyIos)		//ios����

#define MidasAndroidAppid			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_MidasAndroidAppid		:	Normal_MidasAndroidAppid)	//��ֵ��׿appid
#define MidasIosAppid				(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_MidasIosAppid			:	Normal_MidasIosAppid)		//��ֵios appid
#define PayBuyGoodsAndroidAppid		(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAndroidAppid	:	Normal_PayBuyGoodsAndroidAppid)		//ֱ����׿appid
#define PayBuyGoodsIosAppid			(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsIosAppid		:	Normal_PayBuyGoodsIosAppid)			//ֱ��ios appid

#define PayBuyGoodsAppKeyTestAnd	(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAppKeyTestAnd :	Normal_PayBuyGoodsAppKeyTestAnd)	//ֱ����׿ɳ��
#define PayBuyGoodsAppKeyAnd		(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAppKeyAnd		:	Normal_PayBuyGoodsAppKeyAnd)		//ֱ����׿����
#define PayBuyGoodsAppKeyTestIos	(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAppKeyTestIos :	Normal_PayBuyGoodsAppKeyTestIos)	//ֱ��iosɳ��
#define PayBuyGoodsAppKeyIos		(CAppConfig::Instance()->GetTag() == APP_TAG_TIYAN ? TiYan_PayBuyGoodsAppKeyIos		:	Normal_PayBuyGoodsAppKeyIos)		//ֱ��ios����


//���ô�R����ֻ��ios��;
#define XinYueRPayIosAppid "1450012203"
#define XinYueRPayAppkeyTestIos "Q5LbE1RgsiELuOeSahGqATqLS4Cwvcjs"
#define XinYueRPayAppkeyIos "XuFwwOOZMrK76p1znzyh5Uf3gwJu7njO"

#define XinGeAccessIdIOS 2296009683							// �Ÿ�ios access_id
#define XinGeAccessIdAnd 2196009683							// �Ÿ�android access_id
#define XinGeSecretKeyIOS "xa0seqAScOhSsgrm"					// �Ÿ�ios secret_key
#define XinGeSecretKeyAnd "xa0seqAScOhSsgrm"					// �Ÿ�android secret_key

#endif