#include "Plane.h"

class TerrainPlane : public Plane
{
public:
	TerrainPlane(Graphics* graphics) : Plane(graphics) {}
	void Make(float width, float depth, UINT m, UINT n, const char* heightmap);

private:
	float* ReadHeightMap(UINT m, UINT n, const char* heightmap);
};
