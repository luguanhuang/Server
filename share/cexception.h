#ifndef _H_Exception_H__
#define _H_Exception_H__


class CException
{
public: 
    CException()
	{
	}

    explicit CException(const char* pszWhat)
		:m_strWhat(pszWhat)
	{
	}

    virtual ~CException()
	{
	}

    const char* what() const
    {
        return m_strWhat.c_str();
    }

protected:
	std::string m_strWhat;
};


#endif