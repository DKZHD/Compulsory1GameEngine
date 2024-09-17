#pragma once

#include <numeric>

enum class GameView
{
	Two_Dimensional,
	Three_Dimensional
};

class Core
{
public:
	static GameView GetGameView() { return mGameView; }
	static void SetGameView(GameView gv) { mGameView = gv; }
private:
	inline static GameView mGameView = GameView::Two_Dimensional;
};

struct fvec3
{
	float x = 0, y = 0, z = 0;
	fvec3(float x, float y, float z) : x(x), y(y),z(z){}

	fvec3& operator+(const fvec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	fvec3& operator+(const float& other)
	{
		x += other;
		y += other;
		z += other;
		return *this;
	}
	fvec3& operator-(const fvec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	fvec3& operator-(const float& other)
	{
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}
	fvec3& operator*(const fvec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	fvec3& operator*(const float& other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}
	fvec3& operator/(const fvec3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}
	fvec3& operator/(const float& other)
	{
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}
};


class RollingBall
{
public:
	float r;
	float m;
	fvec3 p;
	fvec3 v;
};

inline float dot(fvec3 vec1, fvec3 vec2)
{
	fvec3 v = vec1 * vec2;
	return v.x + v.y + v.z;
}

inline void detectCollision(RollingBall& b1, RollingBall& b2)
{
	fvec3 p0 = b1.p - b1.v;
	fvec3 q0 = b2.p - b2.v;
	fvec3 A = p0 - q0;
	fvec3 B = b1.p - b2.p;
	fvec3 AB = A * B;
	fvec3 AA = A * A;
	fvec3 BB = B * B;
	float d = b1.r + b2.r;
	fvec3 rot = AB * AB - BB * (AA - d * d);
	float t(-1.0f);
	//if(rot>=0.f)
	//{
	//	
	//}
	//if(BB > std::numeric_limits<float>::epsilon())
	//{
	//	t = t / BB;
	//}
}
inline std::pair<fvec3,fvec3> collisionResponse(RollingBall b1, RollingBall b2)
{
	fvec3 v0 = b1.v * (b1.m - b2.m) + b1.v * b1.m * 2;
	v0 = v0 / (b1.m + b1.m);

}