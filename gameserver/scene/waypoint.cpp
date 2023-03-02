#include "pch.h"
#include "waypoint.h"
#include "util.h"
#include <tinyxml2.h> 
#include "unit/unit.h"
#include "scene.h"
#include "entity/XObject.h"


bool WayPoint::LoadData( const std::string& filePath)
{
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLError error = doc.LoadFile(filePath.c_str());

	if (error != tinyxml2::XML_SUCCESS)
	{
		LogError("load WayPoint file %s get XMLError code: %d", filePath.c_str(), error);
		return false;
	}

	m_pointData.clear();
	m_next.clear();
	tinyxml2::XMLElement* root = doc.RootElement();

	if(root)
	{

		const tinyxml2::XMLElement *elem = root->FirstChildElement("point");
		while(elem)
		{
			int index = -1;
			const tinyxml2::XMLAttribute * attr = elem->FindAttribute("index");
			if(attr)
			{
				index = attr->IntValue();	
			}

			attr = elem->FindAttribute("pos");
			if(attr)
			{
				std::vector<std::string> svec = Split(attr->Value() , ":");
				if(3 == (int)svec.size())
				{
					Vector3 vtmp;
					vtmp.x = (float)std::atof(svec[0].c_str());
					vtmp.y = (float)std::atof(svec[1].c_str());
					vtmp.z = (float)std::atof(svec[2].c_str());
					m_pointData.push_back(vtmp);
				}
			}

			elem = elem->NextSiblingElement("point");
		}
		if(m_pointData.empty())
		{
			SSError << "waypoint config error next = 0 , fileName = " << m_name << END;
		}
	}

	return true;
}

bool WayPoint::HavePoint( int index )
{
	if(index >=0 && index < (int)m_pointData.size())
	{
		return true;
	}
	return false;
}

Vector3 WayPoint::GetPoint( int index )
{
	if(index >=0 && index < (int)m_pointData.size())
	{
		return m_pointData[index];
	}

	return Vector3::zero;
}

bool WayPoint::OnIndexPoint( const Vector3& myPoint , int index )
{
	if(HavePoint(index))
	{
		if(Vector3::Distance(myPoint , m_pointData[index]) < 0.5)
		{
			return true;
		}
	}
	return false;
}

int WayPoint::GetClosePointIndex( const Vector3& pos ,Unit* unit)
{
	if(NULL == unit || NULL == unit->GetCurrScene())
	{
		return 0;
	}

	Scene* scene = unit->GetCurrScene();

	int ret = -1;
	float dist = 0;
	//取最近点
	for(size_t i = 0 ; i < m_pointData.size(); ++i)
	{
		float dtmp = Vector3::Distance(pos , m_pointData[i]);
		if(-1 == ret || dtmp < dist)
		{
			ret = i;
			dist = dtmp;
		}
	}

	//如果最近点不可达 取最近可达点
	if(HavePoint(ret) && !scene->CheckMoveReach(unit , pos , m_pointData[ret]-pos))
	{
		ret = -1;
		dist = 0;
		for(size_t i = 0 ; i < m_pointData.size(); ++i)
		{
			if(scene->CheckMoveReach(unit , pos , m_pointData[i]-pos))
			{
				float dtmp = Vector3::Distance(pos , m_pointData[i]);
				if(-1 == ret || dtmp < dist)
				{
					ret = i;
					dist = dtmp;
				}
			}
		}
		if(-1 == ret)
		{
			SSInfo << "waypoint find close point error my fileName = " << m_name << 
				" pos = " << pos.x << " " << pos.y << " " << pos.z << END;
		}
	}
	return (ret < 0 ? 0 : ret);
}

int WayPoint::GetLastPointIndex()
{
	int ret = (int(m_pointData.size()) - 1);
	return (ret < 0 ? 0 : ret);
}

int WayPoint::GetNextIndex( int fromIndex , int toIndex )
{
	if(m_next.empty())
	{
		SSError << "waypont next empty name = " << m_name << END;
		if(fromIndex == toIndex)
		{
			return fromIndex;
		}
		else if(fromIndex < toIndex)
		{
			return fromIndex + 1;
		}
		else if(fromIndex > toIndex)
		{
			return fromIndex - 1;
		}
	}
	else
	{
		int I = (fromIndex * m_pointData.size() + toIndex);
		if(I < (int)m_next.size())
		{
			return m_next[I];
		}
	}
	return fromIndex;
}

void WayPoint::CorrectPathIndex( int &stIndex , int destIndex , Unit *unit )
{
	if(NULL == unit || NULL == unit->GetCurrScene() || NULL == unit->GetXObject())
	{
		return ;
	}

	Scene* scene = unit->GetCurrScene();
	int nextIndex = GetNextIndex(stIndex , destIndex); 
	const Vector3& myPos = unit->GetXObject()->GetPosition_p();
	if(nextIndex != stIndex && HavePoint(nextIndex))
	{
		if( scene->CheckMoveReach(unit , myPos, m_pointData[nextIndex] - myPos ))
		{
			stIndex = nextIndex;
		}
	}
}

float SqDist(const Vector3& stPos , const Vector3& ndPos)
{
	return (stPos.x - ndPos.x)*(stPos.x - ndPos.x) + (stPos.z - ndPos.z)*(stPos.z - ndPos.z);
}

void WayPoint::CreateNavData(Unit* unit)
{
	if(!m_next.empty())
	{
		return ;
	}

	const int N = 100;
	const int n = (int)m_pointData.size();
	if(0 == n || n > N)
	{
		SSWarn << "data size error size = " << n << " fileName = " << m_name << END;	
		return ;
	}
	if(NULL == unit || NULL == unit->GetCurrScene())
	{
		SSError << "pointer null " << END;
		return ;
	}

	Scene* scene = unit->GetCurrScene();

	const float FINF = 1e6; 

	float dist[N][N];
	int pass[N][N];
	int next[N][N];
	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < n; ++j)
		{
			Vector3 stPos = GetPoint(i);
			Vector3 ndPos = GetPoint(j);
			if(scene->CheckMoveReach(unit , stPos , ndPos - stPos))
			{
				dist[i][j] = SqDist(stPos , ndPos);
			}else
			{
				//SSWarn << " can't reach i = " << i << " " << j << END;
				dist[i][j] = FINF * 2;
			}
			pass[i][j] = i;
			next[i][j] = i;
		}
	}

	//floyd
	for(int k = 0; k < n; ++k)
	{
		for(int i = 0; i < n; ++i)
		{
			for(int j = 0; j < n; ++j)
			{
				if(dist[i][j] > dist[i][k] + dist[k][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					pass[i][j] = k;
				}
			}
		}
	}

	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < n; ++j)
		{
			int l = i , r = j;

			if(dist[i][j] >= FINF)
			{
				r = i;
				float ftmp = FINF;
				for(int k = 0; k < n; ++k)
				{
					if(dist[i][k] < FINF)
					{
						r = k;
					}

				}
			}
			int count = 0;
			while(count < n)
			{
				if(dist[l][r] >= FINF)
				{
					SSError << "pass error fileName = " << m_name << " l = " << l << " r = " << r << END;
				}
				if(l == r)
				{
					next[i][j] = l;
					break;
				}
				if(pass[l][r] == l || pass[l][r] == r)
				{
					next[i][j] = r;
					break;
				}
				r = pass[l][r];
				++count;
			}
			if(count >= n)
			{
				SSWarn << " error fileName = " << m_name << " i = " << i << " j = " << j << END;
			}
		}
	}

	m_next.clear();
	m_next.resize(n*n);
	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < n; ++j)
		{
			m_next[i*n+j] = (BYTE)next[i][j];
		}
	}
}

