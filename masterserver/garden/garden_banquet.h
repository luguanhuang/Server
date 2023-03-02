#ifndef __GARDEN_BANQUET_H__
#define __GARDEN_BANQUET_H__
#include <string>
#include "garden/garden_define.h"
#include "role/switch.h"

#define GARDEN_BANQUET_TIME 1000
class Banquet : ITimer
{
public:
	Banquet();
	~Banquet();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
public:
	void Run();
	void Cancel();

	void SetBanquet(
		const UINT32 banquet_id,
		const std::vector<UINT64>& guests
		);
	void SetGardenID(const UINT64 garden_id)
	{
		garden_id_ = garden_id;
	}
public:
	HTIMER garden_timehandle_;

	UINT32 banquet_id_;
	UINT32 stage_counter_;
	UINT32 stage_id_;
	std::unordered_map<UINT32,UINT32> banquet_stage_;
	std::vector<KKSG::MapIntItem> guests_;
	INT64 garden_id_;

	bool runing;
};


class GardenBanquet : public IGardenSerializerInterface 
{
public:
	GardenBanquet(const UINT64 garden_id);
	virtual ~GardenBanquet() ;

	virtual bool IsChanged() ;
	virtual bool Load(const KKSG::GardenInfo& garden_info) ;
	virtual void Save(KKSG::GardenInfo& garden_info) ;
	virtual const std::string ToBinary() ;
	virtual void OnTimer();
	
public:

	// ¿ªÊ¼Ñç»á
	void ServeBanquet(const UINT32 banquet_id,const std::vector<UINT64>& guests);
	bool GetBanquetStatus()
	{
		return banquet_.runing;
	}

	UINT32 GetBanquetTimes()
	{
		return banquet_store_.banquet_times_;
	}
	
	UINT32 GetAwardTimesTSWK();
	void AddBanquetTimes();
private:		
	UINT64 garden_id_;
	Banquet banquet_;
	Switch swithc_;
	
	GardenBanquetStore banquet_store_;
};

#endif // __GARDEN_BANQUET_H__