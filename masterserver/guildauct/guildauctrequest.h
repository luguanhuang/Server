#ifndef __GUILDAUCTREQUEST_H__
#define __GUILDAUCTREQUEST_H__

namespace KKSG
{
	class GuildAuctReqArg;
	class GuildAuctReqRes;
}
class GABaseData;

class CRole;

class GuildAuctRequest
{
public:
	GuildAuctRequest();
	GuildAuctRequest(CRole* role);

	int HandleActList(int acttype, KKSG::GuildAuctReqRes& roRes);

	int HandleItemList(int itemtype, KKSG::GuildAuctReqRes& roRes);

	int HandleBuyAuctPre(UINT64 UID, UINT32 curAuctPrice, UINT32& price, KKSG::GuildAuctReqRes& roRes);

	int HandleBuyAuctAft(UINT64 UID, KKSG::GuildAuctReqRes& roRes);

	int HandleBuyNowPre(UINT64 UID, UINT32& price);

	int HandleBuyNowAft(UINT64 UID);

	void ClearLock(UINT64 UID); 

	int HandleAuctGuildHistory(KKSG::GuildAuctReqRes& roRes);

	int HandleAuctWorldHistory(KKSG::GuildAuctReqRes& roRes);

	void AuctFailMailBack(UINT64 roleID, UINT32 itemID,UINT32 price);

	void AuctSuccessMail(UINT64 roleID, UINT32 itemID, UINT32 itemCount, bool isBuyNow);

	void AuctProfitMail(UINT64 roleID, UINT64 GuildID, int actType, UINT32 price);

	void AuctProfitFailMail(UINT64 roleID,UINT32 act_type);


	void HandleCommonData(const KKSG::GuildAuctReqArg& roArg, KKSG::GuildAuctReqRes& roRes);

private:

	void BuyNowGuildNotice(GABaseData* baseData);
	void AuctFailNotify(GABaseData* baseData);

	CRole* m_pRole;
};

#endif