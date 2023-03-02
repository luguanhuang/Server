#ifndef __FRIENDDEF_H__
#define __FRIENDDEF_H__

#define MAX_FRIEND_COUNT		( GetGlobalConfig().FriendMaxCount)
#define MAX_BLACK_COUNT			( GetGlobalConfig().BlackMaxCount)
#define MAX_APPLYFRIEND_COUNT	( GetGlobalConfig().ApplyMaxCount)
#define MAX_DEGREE		( GetGlobalConfig().FriendMaxDegree)
#define MAX_GIFTDEGREE	( GetGlobalConfig().GiftSendMinDegree)
#define MAX_GIFTRECEIVE	( GetGlobalConfig().GiftReceiveMaxTimes)
#define MAX_GIFTSEND	( GetGlobalConfig().GiftSendMaxTimes)

enum FriendErr
{
	FriendSuccess,

	FriendMax,
	FriendRepeated,
	FriendNotExist,

	BlackSuccess,

	InSelfBlackList,
	InOtherBlackList,
	BlackNotExist,
	BlackRepeated,
	BlackMax,
};


#endif
