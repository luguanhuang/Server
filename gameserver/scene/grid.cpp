#include "pch.h"
#include "grid.h"
#include "config.h"
#include <queue>
#include <math.h>
#include "table/globalconfig.h"

Grid::Grid()
{
	m_Data = NULL;

	m_step = 0;
	m_MaxOffset = 0;

	m_viewlenx = 0;
	m_viewlenz = 0;

	memset(&m_header, 0, sizeof(m_header));
}

Grid::~Grid()
{
	if (m_Data != NULL)
	{
		delete []m_Data;
	}
}

static std::string TranslateName(const char *fileName)
{
	std::string newName;
	int i;
	for (i = 0; fileName[i] != 0; ++i)
	{
		if (fileName[i] == '/')
		{
			newName += '_';
		}
		else
		{
			newName += fileName[i];
		}
	}

	newName += ".txt";
	return newName;
}

int Grid::view_side = 0;

bool Grid::LoadFile(const std::string &fileName)
{
	view_side = GetGlobalConfig().ViewGridLen;

	const size_t headSize = sizeof(GridHeader);
	std::string data = GSConfig::Instance()->ReadFileContent(fileName.c_str());
	size_t n = data.size();
	
	if (n <= headSize)
	{
		LogError("read grid file [%s] failed!, header size error: %d - %d", fileName.c_str(), n, headSize);
		return false;
	}

	memcpy(&m_header, data.data(), headSize);
	n -= headSize;

	// view grid
	m_viewlenx = (m_header.width * m_header.side)/view_side;
	if(m_viewlenx * view_side < m_header.width * m_header.side) ++m_viewlenx;
	m_viewlenz = (m_header.height * m_header.side)/view_side;
	if(m_viewlenz * view_side < m_header.height * m_header.side) ++m_viewlenz;

	int size = m_header.count;
	int buffSize = size * sizeof(int);

	int* pIdxData   = new int[size];
	short* pValueData = new short[size];

	memcpy(pIdxData, data.data() + headSize, buffSize);

	n -= buffSize;

	if ((int)n < 0 || n % 2)
	{
		LogError("read grid file [%s] failed! read data error: %d - %d", fileName.c_str(), n, buffSize);

		delete [] pIdxData;
		delete [] pValueData;

		return false;
	}

	short* value_buffer = new short[n >> 1];
	memcpy(value_buffer, data.data() + headSize + buffSize, n);

	int cursor = 0; size_t readIdx = 0;
	for(int i = 0; i < size; i++)
	{
		if(readIdx + 1 > (n >> 1)) break;

		short s = value_buffer[readIdx++];
		if (s < 0)
		{
			pValueData[cursor++] = -100;

			if(i + 1 < size)
			{
				if (s == -32768/*short.MinValue*/)
					s = 0;
				else
					s = (short)-s;

				pValueData[cursor++] = s;
				i++;
			}
		}
		else
			pValueData[cursor++] = s;
	}

	delete [] value_buffer;

	if (readIdx != (n >> 1))
	{
		LogError("read grid file [%s] failed! read data error: %d - %d", fileName.c_str(), readIdx, n >> 1);

		delete [] pIdxData;
		delete [] pValueData;

		return false;
	}
	else
	{
		m_MaxOffset = pIdxData[size - 1];

		if(m_MaxOffset == 0)
		{
			//can walk anywhere
			m_Data = new short[1];
			m_Data[0] = pValueData[0];
		}
		else
		{
			m_Data = new short[m_MaxOffset];

			int s = 1;
			for(int i = 0; i < m_MaxOffset; i++)
			{
				if(i == pIdxData[s])
					++s;

				assert(s < size);
				assert(i < pIdxData[s]);

				m_Data[i] = pValueData[s - 1];
			}
		}

		delete [] pIdxData;
		delete [] pValueData;

		int memorycost = m_MaxOffset * sizeof(short);
		if(memorycost >> 20)
			LogInfo("grid file [%s] is loaded, memory using [%dMB]", fileName.c_str(), memorycost >> 20);
		else
			LogInfo("grid file [%s] is loaded, memory using [%dKB]", fileName.c_str(), memorycost >> 10);

		return true;
	}
}

bool Grid::TryGetHeight(float x, float z, float& y)
{
	if(0 == m_MaxOffset) return true;

	int offset = GetCellOffset(x, z);

	if(offset >= 0 && offset < m_MaxOffset)
	{
		y = m_Data[offset] / 100.0f;
		y = y < 0 ? y : y + 0.02f;
		return true;
	}
	else
		return false;
}

bool Grid::IsCellBlocked(int gx, int gz)
{
	if (gx < 0 || gx >= m_header.width)
	{
		return true;
	}

	if (gz < 0 || gz >= m_header.height)
	{
		return true;
	}

	int offset = GetCellOffset(gz, gx);
	return GetHeight(offset) < 0;
}

float Grid::GetHeight(int key)
{
	if(key >= 0 && key < m_MaxOffset)
	{
		//for server add safe height 2cm.
		float r = m_Data[key] / 100.0f;
		return r < 0 ? r : r + 0.02f;
	}
	else
		return -1;
}

bool Grid::IsCellBlocked(const Vector3 &pos)
{
	int gz, gx;
	GetCellGrid(pos.x, pos.z, gz, gx);
	return IsCellBlocked(gx, gz);
}

static int isign(int i)
{
	if (i == 0)
		return 0;

	return i > 0 ? 1 : -1;
}

// 直线光栅化算法，用来计算走直线会碰到的格子
static void Bresenham(int xs, int zs, int xe, int ze, Grid *pGrid)
{
	int x = xs;
	int z = zs;
	int dx = abs(xe - xs);
	int dz = abs(ze - zs);
	int sx = isign(xe - xs);
	int sz = isign(ze - zs);
	bool swap = false;
	if (dz > dx)
	{
		int t = dx;
		dx = dz;
		dz = t;
		swap = true;
	}

	pGrid->BlockTestBegin();

	int err = 2 * dz - dx;
	for (int i = 1; i <= dx; ++i)
	{
		if (err >= 0)
		{
			if (swap)
			{
				x += sx;
			}
			else
			{
				z += sz;
			}
			err -= 2*dx;
		}

		if (swap)
		{
			z += sz;
		}
		else
		{
			x += sx;
		}

		err += 2*dz;

		if (pGrid->IsCellBlocked(x, z))
		{
			pGrid->BlockTestEnd((i-1)/(float)dx);
			return ;
		}
	}

	pGrid->BlockTestEnd(1.0f);
}

float Grid::LineBlockTest(const Vector3 &start, const Vector3 &end)
{
	int startGZ, startGX, endGZ, endGX;
	GetCellGrid(start.x, start.z, startGZ, startGX);
	GetCellGrid(end.x, end.z, endGZ, endGX);
	
	if (startGX == endGX && startGZ == endGZ)
	{
		return 1;
	}

	Bresenham(startGX, startGZ, endGX, endGZ, this);
	return m_step;
}

void Grid::BlockTestBegin()
{
	m_step = 0;
}

void Grid::BlockTestEnd(float step)
{
	m_step = step;
}

float Grid::GetHeight(float x, float z)
{
	int offset = GetCellOffset(x, z);
	return GetHeight(offset);
}

struct PathNode
{
	double rank;
	int step;
	int x, z;
	PathNode *prev;

	int ID() { return (x << 16)|z; }
};

struct NodeCMP
{
	bool operator()(PathNode *a, PathNode *b) const
	{
		return a->rank > b->rank;
	}
};

bool Grid::FindPath(const Vector3 &start, const Vector3 &target, int maxstep, std::vector<Vector3> &waypoint)
{
	int blockCount = 0;
	int startx, startz, targetx, targetz;

	GetCellGrid(start.x, start.z, startz, startx);
	GetCellGrid(target.x, target.z, targetz, targetx);
	if (startz == targetz &&
		startx == targetx)
	{
		waypoint.push_back(target);
		return true;
	}

	if (IsCellBlocked(targetx, targetz))
	{
		return false;
	}

	std::unordered_set<unsigned> Seen;
	std::priority_queue<PathNode *, std::vector<PathNode *>, NodeCMP> Queue;
	std::list<PathNode> Nodes;
	
	Nodes.push_back(PathNode());
	PathNode &root = Nodes.front();
	root.x = startx;
	root.z = startz;
	root.step = 0;
	root.rank = 0;
	root.prev = NULL;
	Queue.push(&root);

	bool success = false;
	PathNode tryNode;

	int MovePatterns[][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	int N = sizeof(MovePatterns)/sizeof(MovePatterns[0]);

	while (maxstep > 0 && !Queue.empty())
	{
		--maxstep;
		PathNode *node = Queue.top();
		Queue.pop();

		for (int i = 0; i < N; ++i)
		{
			tryNode = *node;
			tryNode.step += 1;
			tryNode.prev = node;
			tryNode.x += MovePatterns[i][0];
			tryNode.z += MovePatterns[i][1];

			if (tryNode.x == targetx && tryNode.z == targetz)
			{
				Nodes.push_back(tryNode);
				float scenex, scenez;
				int backx=0, backz=0;
				int keep = 0;
				PathNode *node = &Nodes.back();
				while (node != NULL)
				{
					GetSceneXY(scenex, scenez, node->z, node->x);

					if (waypoint.empty())
					{
						waypoint.push_back(Vector3(scenex, 0, scenez));
					}
					else
					{
						if (backx == node->x && abs(backz-node->z) == 1)
						{
							if (keep == 1)
							{
								waypoint.back().z = scenez;
							}
							else
							{
								keep = 1;
								waypoint.push_back(Vector3(scenex, 0, scenez));
							}
						}
						else if (backz == node->z && abs(backx-node->x) == 1)
						{
							if (keep == 2)
							{
								waypoint.back().x = scenex;
							}
							else
							{
								keep = 2;
								waypoint.push_back(Vector3(scenex, 0, scenez));
							}
						}
						else
						{
							keep = 0;
							waypoint.push_back(Vector3(scenex, 0, scenez));
						}
					}

					backx = node->x;
					backz = node->z;
					
					node = node->prev;
				}

				LogInfo("search path seen %d points, block %d", Seen.size(), blockCount);
				return true;
			}

			if (!IsCellBlocked(tryNode.x, tryNode.z))
			{
				double dx = tryNode.x-targetx;
				double dy = tryNode.z-targetz;
				tryNode.rank = sqrt(dx*dx + dy*dy) + 0.5 * tryNode.step;
				auto it = Seen.find(tryNode.ID());
				if (it == Seen.end())
				{
					Nodes.push_back(tryNode);
					Queue.push(&Nodes.back());
					Seen.insert(tryNode.ID());;
				}
			}
			else
			{
				++blockCount;
			}
		}
	}

	LogInfo("search path seen %d points, block %d", Seen.size(), blockCount);
	return false;
}

static std::unordered_map<std::string, Grid *> GridCache;

Grid * Grid::GetGrid(const std::string &fileName)
{
	auto it = GridCache.find(fileName);
	if (it != GridCache.end())
	{
		return it->second;
	}

	Grid *pGrid = new Grid();
	if (!pGrid->LoadFile(fileName))
	{
		delete pGrid;
		return NULL;
	}

	GridCache[fileName] = pGrid;
	return pGrid;
}

void Grid::DestoryAllGrid()
{
	for (auto i = GridCache.begin(); i != GridCache.end(); ++i)
	{
		delete i->second;
	}

	GridCache.clear();
}

