#ifndef _PROTO_WINDOW_H_
#define _PROTO_WINDOW_H_

#include <vector>

class ProtoWindow
{
public:
	ProtoWindow(UINT32 size = 1)
	:m_cur(0)
	{
		m_protos.resize(size);
	}
	void SetSize(UINT32 size)
	{
		if (size == 0)
		{
			return;
		}
		m_protos.resize(size);
	}

	void Push(UINT32 protoType)
	{
		m_protos[m_cur] = protoType;
		m_cur = (m_cur + 1) % m_protos.size();
	}

	void Get(std::vector<UINT32>& protos)
	{
		protos.resize(m_protos.size());
		for (size_t i = 0; i < m_protos.size(); ++i)
		{
			int index = (m_cur + 1 + i) % m_protos.size();
			protos[i] = m_protos[index];
		}
	}

private:
	UINT32 m_cur;
	std::vector<UINT32> m_protos;
};

#endif