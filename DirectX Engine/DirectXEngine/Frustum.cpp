#include "Frustum.h"
#include "ScreenManager.h"
#include "Constants.h"

Frustum::Frustum()
{
}


Frustum::~Frustum()
{
}

void Frustum::Create(XMMATRIX view)
{
    float zMin, r;
    XMMATRIX matrix, proj;

    proj = Screen::Instance()->GetPerspectiveMatrix();

    // Calculate the minimum Z distance in the frustum.
    zMin = -proj._43 / proj._33;
    r = ScreenConstants::ScreenDepth / (ScreenConstants::ScreenDepth - zMin);
    proj._33 = r;
    proj._43 = -r * zMin;

    //create frustum matrix
    matrix = XMMatrixMultiply(view, proj);

    //calculate near plane of frustum
    _Planes[0] = XMVectorSet(
        matrix._14 + matrix._13, matrix._24 + matrix._23,
        matrix._34 + matrix._33, matrix._44 + matrix._43
    );
    _Planes[0] = XMPlaneNormalize(_Planes[0]);

    //calculate far plane of frustum
    _Planes[1] = XMVectorSet(
        matrix._14 - matrix._13, matrix._24 - matrix._23,
        matrix._34 - matrix._33, matrix._44 - matrix._43
    );
    _Planes[1] = XMPlaneNormalize(_Planes[1]);

    //calculate left plane of frustum
    _Planes[2] = XMVectorSet(
        matrix._14 + matrix._11, matrix._24 + matrix._21,
        matrix._34 + matrix._31, matrix._44 + matrix._41
    );
    _Planes[2] = XMPlaneNormalize(_Planes[2]);

    //calculate right plane of frustum
    _Planes[3] = XMVectorSet(
        matrix._14 - matrix._11, matrix._24 - matrix._21,
        matrix._34 - matrix._31, matrix._44 - matrix._41
    );
    _Planes[3] = XMPlaneNormalize(_Planes[3]);

    //calculate top plane of frustum
    _Planes[4] = XMVectorSet(
        matrix._14 - matrix._12, matrix._24 - matrix._22,
        matrix._34 - matrix._32, matrix._44 - matrix._42
    );
    _Planes[4] = XMPlaneNormalize(_Planes[4]);

    //calculate bottom plane of frustum
    _Planes[5] = XMVectorSet(
        matrix._14 + matrix._12, matrix._24 + matrix._22,
        matrix._34 + matrix._32, matrix._44 + matrix._42
    );
    _Planes[5] = XMPlaneNormalize(_Planes[5]);

    
}

bool Frustum::CheckPoint(float x, float y, float z)
{
    //check if point is inside planes
    for (int i = 0; i < 6; i++) {
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet(x, y, z, 1.0f))) < 0.0f) {
            return false;
        }
    }
    return true;
}

bool Frustum::CheckCube(float centerX, float centerY, float centerZ, float radius)
{
    //check if any one point of cube is inside frustum
    for (int i = 0; i < 6; i++) {
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX - radius), (centerY - radius), (centerZ - radius), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX + radius), (centerY - radius), (centerZ - radius), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX - radius), (centerY + radius), (centerZ - radius), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX + radius), (centerY + radius), (centerZ - radius), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX - radius), (centerY - radius), (centerZ + radius), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX + radius), (centerY - radius), (centerZ + radius), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX - radius), (centerY + radius), (centerZ + radius), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX + radius), (centerY + radius), (centerZ + radius), 1.0f))) >= 0.0f) {
            continue;
        }
        return false;
    }
    return true;
}

bool Frustum::CheckSphere(float centerX, float centerY, float centerZ, float radius)
{
    //check to see if radius of sphere is inside frustum
    for (int i = 0; i < 6; i++) {
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet(centerX, centerY, centerZ, 1.0f))) < -radius) {
            return false;
        }
    }
    return true;
}

bool Frustum::CheckRectangle(float centerX, float centerY, float centerZ, float sizeX, float sizeY, float sizeZ)
{
    //check to see if any 6 planes of rectangle are in view frustum
    for (int i = 0; i < 6; i++) {
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX - sizeX), (centerY - sizeY), (centerZ - sizeZ), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX + sizeX), (centerY - sizeY), (centerZ - sizeZ), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX - sizeX), (centerY + sizeY), (centerZ - sizeZ), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX + sizeX), (centerY + sizeY), (centerZ - sizeZ), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX - sizeX), (centerY - sizeY), (centerZ + sizeZ), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX + sizeX), (centerY - sizeY), (centerZ + sizeZ), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX - sizeX), (centerY + sizeY), (centerZ + sizeZ), 1.0f))) >= 0.0f) {
            continue;
        }
        if (XMVectorGetX(XMPlaneDotCoord(_Planes[i], XMVectorSet((centerX + sizeX), (centerY + sizeY), (centerZ + sizeZ), 1.0f))) >= 0.0f) {
            continue;
        }
        return false;
    }
    return true;
}
