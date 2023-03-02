#ifndef __GRID_H__
#define __GRID_H__

#include "math/XVector3.h"


struct GridHeader
{
	int height;
	int width;
	float xmin;
	float zmin;
	float xmax;
	float zmax;
	float side;
	int count;
};

class Grid
{
	Grid();
	~Grid();

	bool LoadFile(const std::string &fileName);

public:
	
	static Grid *GetGrid(const std::string &fileName);
	static void DestoryAllGrid();
	
	bool IsCellBlocked(const Vector3 &pos);
	bool IsCellBlocked(int gx, int gz);
	float LineBlockTest(const Vector3 &start, const Vector3 &end);

	bool TryGetHeight(float x, float z, float& y);
	float GetHeight(float x, float z);

	void BlockTestBegin();
	void BlockTestEnd(float step);

	bool FindPath(const Vector3 &start, const Vector3 &target, int maxstep, std::vector<Vector3> &waypoint);

	inline void GetCellGrid(float x, float z, int &gz, int &gx)
	{
		gx = (int)((x - m_header.xmin) / m_header.side);
		gz = (int)((z - m_header.zmin) / m_header.side);
	}

	inline int GetCellOffset(float x, float z)
	{
		int gx = (int)((x - m_header.xmin) / m_header.side);
		int gz = (int)((z - m_header.zmin) / m_header.side);

		return gz * m_header.width + gx;
	}

	inline int GetCellOffset(int gz, int gx)
	{
		return gz * m_header.width + gx;
	}

	inline void GetSceneXY(float &x, float &z, int gz, int gx)
	{
		x = gx * m_header.side + m_header.xmin;
		z = gz * m_header.side + m_header.zmin;
	}
	// get view grid by pos
	inline int GetViewGridOffset(float x, float z)
	{
		int vx = (int)((x - m_header.xmin) / view_side);
		int vz = (int)((z - m_header.zmin) / view_side);
		return vz * m_viewlenx + vx;
	}

	// get view grid by pos
	inline void GetViewGridXY(float x, float z, int& gx, int& gz)
	{
		gx = (int)((x - m_header.xmin) / view_side);
		gz = (int)((z - m_header.zmin) / view_side);
	}

	inline void GetViewGridScale(int x, int z, int scale[])
	{
		scale[0] = x > 0 ? x - 1 : x;
		scale[1] = x < m_viewlenx ? x + 1 : x;
		scale[2] = z > 0 ? z - 1 : z;
		scale[3] = z < m_viewlenz ? z + 1 : z;
	}

	// get all view grids by center view grid
	inline void GetAllViewGrid(int viewoffset, int allviewoffset[])
	{
		if (viewoffset > m_viewlenx*m_viewlenz)
		{
			return;
		}
		int index = 1;
		int vx = viewoffset % m_viewlenx;
		int vz = (viewoffset - vx)/m_viewlenx;
		// x - 1, z + 1
		TryPushViewGrid(vx-1, vz+1, allviewoffset, index);
		// x , z + 1
		TryPushViewGrid(vx, vz+1, allviewoffset, index);
		// x + 1, z + 1
		TryPushViewGrid(vx+1, vz+1, allviewoffset, index);
		// x - 1, z
		TryPushViewGrid(vx-1, vz, allviewoffset, index);
		// x + 1, z
		TryPushViewGrid(vx+1, vz, allviewoffset, index);
		// x - 1, z - 1
		TryPushViewGrid(vx-1, vz-1, allviewoffset, index);
		// x , z - 1
		TryPushViewGrid(vx, vz-1, allviewoffset, index);
		// x + 1, z - 1
		TryPushViewGrid(vx+1, vz-1, allviewoffset, index);
		return;
	}

	inline bool IsInvalidViewGrid(int vx, int vz)
	{
		return (vx >= 0 && vx < m_viewlenx && vz >= 0 && vz < m_viewlenz);
	}

	inline void TryPushViewGrid(int vx, int vz, int allviewoffset[], int& index)
	{
		if (IsInvalidViewGrid(vx, vz))
		{
			allviewoffset[index] = (vz * m_viewlenx + vx);
			++index;
		}
	}

	inline int GetViewLenx() { return m_viewlenx; }
	inline int GetViewLenz() { return m_viewlenz; }

private:
	float GetHeight(int key);

private:
	GridHeader m_header;
	short *m_Data;

	// view grid
	int m_viewlenx;
	int m_viewlenz;

	float  m_step;
	int m_MaxOffset;

	static int view_side;
};

#endif // __GRID_H__