#include "pch.h"

#include "CollisionSystem.h"

#include "AABBCollisionComponent.h"
#include "OBBCollisionComponent.h"
#include "SphereCollisionComponent.h"

bool CollisionSystem::AABBToAABB(
  Weak<AABBCollisionComponent> src, Weak<AABBCollisionComponent> dest)
{
    Ptr<AABBCollisionComponent> srcCollider  = Lock<AABBCollisionComponent>(src);
    Ptr<AABBCollisionComponent> destCollider = Lock<AABBCollisionComponent>(dest);

    if (!srcCollider || !destCollider)
        return false;

    return AABBToAABB(srcCollider->GetBox(), destCollider->GetBox());
}

bool CollisionSystem::AABBToSphere(
  Weak<AABBCollisionComponent> src, Weak<SphereCollisionComponent> dest)
{
    Ptr<AABBCollisionComponent>   srcCollider  = Lock<AABBCollisionComponent>(src);
    Ptr<SphereCollisionComponent> destCollider = Lock<SphereCollisionComponent>(dest);

    if (!srcCollider || !destCollider)
        return false;

    return AABBToSphere(
      srcCollider->GetBox(), destCollider->GetWorldPosition(), destCollider->GetRadius());
}

bool CollisionSystem::AABBToOBB(Weak<AABBCollisionComponent> src, Weak<OBBCollisionComponent> dest)
{
    Ptr<AABBCollisionComponent> srcCollider  = Lock<AABBCollisionComponent>(src);
    Ptr<OBBCollisionComponent>  destCollider = Lock<OBBCollisionComponent>(dest);

    if (!srcCollider || !destCollider)
        return false;

    return AABBToOBB(srcCollider->GetBox(), destCollider->GetBox());
}

bool CollisionSystem::OBBToOBB(Weak<OBBCollisionComponent> src, Weak<OBBCollisionComponent> dest)
{
    Ptr<OBBCollisionComponent> srcCollider  = Lock<OBBCollisionComponent>(src);
    Ptr<OBBCollisionComponent> destCollider = Lock<OBBCollisionComponent>(dest);

    if (!srcCollider || !destCollider)
        return false;

    return OBBToOBB(srcCollider->GetBox(), destCollider->GetBox());
}

bool CollisionSystem::OBBToSphere(
  Weak<OBBCollisionComponent> src, Weak<SphereCollisionComponent> dest)
{
    Ptr<OBBCollisionComponent>    srcCollider  = Lock<OBBCollisionComponent>(src);
    Ptr<SphereCollisionComponent> destCollider = Lock<SphereCollisionComponent>(dest);

    if (!srcCollider || !destCollider)
        return false;

    return OBBToSphere(
      srcCollider->GetBox(), destCollider->GetWorldPosition(), destCollider->GetRadius());
}

bool CollisionSystem::SphereToSphere(
  Weak<SphereCollisionComponent> src, Weak<SphereCollisionComponent> dest)
{
    Ptr<SphereCollisionComponent> srcCollider  = Lock<SphereCollisionComponent>(src);
    Ptr<SphereCollisionComponent> destCollider = Lock<SphereCollisionComponent>(dest);

    if (!srcCollider || !destCollider)
        return false;

    return SphereToSphere(srcCollider->GetWorldPosition(), srcCollider->GetRadius(),
      destCollider->GetWorldPosition(), destCollider->GetRadius());
}

bool CollisionSystem::AABBToAABB(Rect src, Rect dest)
{
    if (src.left > dest.right)
        return false;
    if (src.bottom > dest.top)
        return false;
    if (src.right < dest.left)
        return false;
    if (src.top < dest.bottom)
        return false;

    return true;
}

bool CollisionSystem::AABBToSphere(Rect src, Vector3 destCenter, float destRadius)
{
    float foundX = std::clamp(destCenter.x, src.left, src.right);
    float foundY = std::clamp(destCenter.y, src.bottom, src.top);

    float distX = destCenter.x - foundX;
    float distY = destCenter.y - foundY;

    float foundDistSquare = (distX * distX) + (distY * distY);
    float radiusSquare    = destRadius * destRadius;

    if (foundDistSquare < radiusSquare)
        return true;

    return false;
}

bool CollisionSystem::AABBToOBB(Rect src, const OBB2D& dest)
{
    OBB2D srcObb = CreateOBB(src);
    return OBBToOBB(srcObb, dest);
}

bool CollisionSystem::OBBToOBB(const OBB2D& src, const OBB2D& dest)
{
    Vector2 centerLine = src.center - dest.center;

    if (!ComputeAxisProjection(centerLine, src.axises[AxisType::X], src.halfSize.x, dest))
        return false;

    if (!ComputeAxisProjection(centerLine, src.axises[AxisType::Y], src.halfSize.y, dest))
        return false;

    if (!ComputeAxisProjection(centerLine, dest.axises[AxisType::X], dest.halfSize.x, src))
        return false;

    if (!ComputeAxisProjection(centerLine, dest.axises[AxisType::Y], dest.halfSize.y, src))
        return false;

    return true;
}

bool CollisionSystem::OBBToSphere(const OBB2D& src, Vector3 destCenter, float destRadius)
{
    Vector2 convertCenter;
    convertCenter.x = destCenter.x;
    convertCenter.y = destCenter.y;

    Vector2 centerLine = convertCenter - src.center;
    Vector2 axis       = centerLine;
    axis.Normalize();

    float centerProj    = std::abs(centerLine.Dot(axis));
    float obbProjOnAxis = std::abs(axis.Dot(src.axises[AxisType::X])) * src.halfSize.x
                        + std::abs(axis.Dot(src.axises[AxisType::Y])) * src.halfSize.y;

    if (centerProj > destRadius + obbProjOnAxis)
        return false;

    float projX = std::abs(centerLine.Dot(src.axises[AxisType::X]));
    if (projX > destRadius + src.halfSize.x)
        return false;

    float projY = std::abs(centerLine.Dot(src.axises[AxisType::Y]));
    if (projY > destRadius + src.halfSize.y)
        return false;

    return true;
}

bool CollisionSystem::SphereToSphere(
  Vector3 srcCenter, float srcRadius, Vector3 destCenter, float destRadius)
{
    return srcCenter.Distance(destCenter) < srcRadius + destRadius;
}

bool CollisionSystem::ComputeAxisProjection(
  Vector2 centerLine, Vector2 srcAxis, float srcAxisHalfSize, const OBB2D& destOBB)
{
    float centerProjectionDist = std::abs(centerLine.Dot(srcAxis));
    float destProjectionDist
      = std::abs(srcAxis.Dot(destOBB.axises[AxisType::X])) * destOBB.halfSize.x
      + std::abs(srcAxis.Dot(destOBB.axises[AxisType::Y])) * destOBB.halfSize.y;

    float overlap = srcAxisHalfSize + destProjectionDist - centerProjectionDist;
    if (overlap <= 0)
        return false;

    return true;
}

OBB2D CollisionSystem::CreateOBB(Rect aabb)
{
    OBB2D result;

    result.center = (aabb.leftBottom + aabb.rightTop) * .5f;

    result.axises[AxisType::X].x = 1.f;
    result.axises[AxisType::X].y = 0.f;

    result.axises[AxisType::Y].x = 0.f;
    result.axises[AxisType::Y].y = 1.f;

    result.halfSize = (aabb.rightTop - aabb.leftBottom) * 0.5f;

    return result;
}
