#pragma once

#include <d3d11.h>
#include <xnamath.h>

class Frustum
{
public:
    Frustum();
    ~Frustum();

    void Create(XMMATRIX view);

    bool CheckPoint(float x, float y, float z);
    bool CheckCube(float centerX, float centerY, float centerZ, float radius);
    bool CheckSphere(float centerX, float centerY, float centerZ, float radius);
    bool CheckRectangle(float centerX, float centerY, float centerZ, float sizeX, float sizeY, float sizeZ);

private:
    XMVECTOR _Planes[6];
};

