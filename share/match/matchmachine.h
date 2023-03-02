#ifndef __MATCHMACHINE_H__
#define __MATCHMACHINE_H__

/*
	���⣺һ������������Ϊx������һЩ����<=x�Ķ��飬��ϳ������������顣
	���������NP�ģ�û�ж���ʽʱ���㷨��
	���ƵĽ����������̰��ȥƥ�䡣

	������Ϊ4�Ķ��飬������ 4��3+1��2+2��2+1+1��1+1+1+1 ��ɣ�
	������Ϊ5�Ķ��飬������ 5��4+1��3+1��3+1+1��2+2+1��2+1+1+1��1+1+1+1+1 ��ɡ�
	̰�Ĳ��Ծ�������ʹ���˶�Ķ��飬��Ϊ���ٵĶ�������и����ѡ��
	����������С�ڵ���6������£��ֹ�ģ��û���ҳ������������ŵķ�����

	����������Ϊ7������:4+1+1+1��3+3+1��4�Ķ�����1����3�Ķ�����6����1�Ķ�����3����
	��������ʹ��4+1+1+1�������ֻ�����һ���������顣
	���ʹ��3+3+1,�Ϳ������3���������顣
*/

enum MatchMachineType
{
	MatchMachineType_Unknown,
	MatchMachineType_HeroBattle,
	MatchMachineType_MobaBattle,
};

class MatchMachine
{
public:
	MatchMachine();
	~MatchMachine();
	void Init(UINT32 num, MatchMachineType type = MatchMachineType_Unknown);	

	void Clear();

	bool AddMatchUnit(UINT64 uid, UINT32 num);
	bool DelMatchUnit(UINT64 uid);

	///> ƥ��ɹ���result�еľ�ֱ��ɾ����
	void DoMatch(std::vector<std::vector<UINT64>>& result);
	
	bool GMChangeNum(UINT32 num);

	void PrintLog();

private:
	std::set<std::pair<time_t, UINT64>>* _GetUnitSet(UINT64 uid);

	static void _BuildMatchTemplate();
	static void _DfsBuild(UINT32 dep, UINT32 val, std::vector<std::vector<UINT32>>& temp, std::vector<UINT32>& vec);

private:
	UINT32 m_number;
	MatchMachineType m_type;

	std::vector<std::set<std::pair<time_t, UINT64>>> m_units;
	std::map<UINT64, time_t> m_units2time;

	static std::unordered_map<UINT32, std::vector<std::vector<UINT32>>> m_stMatchTemplate;
};



#endif