#ifndef __XBITSET_H__
#define __XBITSET_H__

template <int N>
class BitSet
{
	enum 
	{ 
		BUFFSIZE = (N+7)/8,
	};

public:

	BitSet()
	{
		memset(m_bits, 0, BUFFSIZE);
	}

	bool check(unsigned int i) const
	{
		unsigned int offset = i / 8;
		unsigned int mask = (1 << (i%8));
		if(offset >= BUFFSIZE)
		{
			return false;
		}

		return (m_bits[offset] & mask) != 0;
	}

	bool operator[](unsigned int i) const
	{
		return check(i);
	}

	void Set(unsigned int i, bool flag)
	{
		unsigned int offset = i / 8;
		unsigned int mask = (1 << (i%8));
		if(offset >= BUFFSIZE)
		{
			return;
		}

		if (flag)
		{
			m_bits[offset] |= mask;
		}
		else
		{
			m_bits[offset] &= ~mask;
		}
	}

	void SetAll()
	{
		for (int i = 0; i < BUFFSIZE; ++i)
		{
			m_bits[i] = 0xff;
		}
	}

	std::string ToString()
	{
		return std::string((char *)m_bits, BUFFSIZE);
	}

	void FromString(const std::string &s)
	{
		unsigned int n = s.size();
		unsigned int copyLen = (n > BUFFSIZE) ? BUFFSIZE : n;
		memcpy(m_bits, s.data(), copyLen);
	}

private:

	unsigned char m_bits[BUFFSIZE];
};


#endif // __BITSET_H__