#ifndef __FLOWEROP_H__
#define __FLOWEROP_H__

#include "socialop.h"

struct Flower;
class FlowerOp : public SocialOp
{
public:
	FlowerOp(UINT64 roleid);
	virtual ~FlowerOp();

	// flower
	void SendFlower(UINT64 send, UINT32 itemid, UINT32 count);
	Flower* GetFlowerInfo();
};

#endif