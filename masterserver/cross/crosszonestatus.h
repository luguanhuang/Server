#ifndef _CROSS_ZONE_STATUS_H_
#define _CROSS_ZONE_STATUS_H_

class CrossZoneStatus
{
	CrossZoneStatus();
	~CrossZoneStatus();
	DECLARE_SINGLETON(CrossZoneStatus)

public:
	bool Init();
	void Uninit();

	bool IsCrossZoneUsable();
	void OnCrossZoneUnusable();

	void SetWorldStatus(bool status);
	void SetCrossGsNum(UINT32 num);

private:
	bool m_worldOk;
	UINT32 m_crossGsNum;
};

#endif