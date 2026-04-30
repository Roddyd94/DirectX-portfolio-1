#pragma once

namespace CollisionShape
{
    enum Type : uint8
    {
        AABB,
        OBB,
        Sphere,
        End
    };
}

namespace CollisionState
{
    enum Type : uint8
    {
        Exit,
        Enter,
        Stay,
        None,
    };
}

namespace CollisionResponse
{
    enum Type : uint8
    {
        Ignore,
        Block,
        Overlap,
        End,
    };
}

namespace ColliderType
{
    enum Type : uint8
    {
        Player,
        Monster,
        Boss,
        PlayerProjectile,
        MonsterProjectile,
        Item,
        End,
    };
}

struct AABB2D
{
    Vector2 min;
    Vector2 max;
};

struct OBB2D
{
    Vector2 axises[2];
    Vector2 center;
    Vector2 halfSize;
};

class CollisionSystem
{
public:
    static bool AABBToAABB(Weak<class AABBCollisionComponent> src,
      Weak<class AABBCollisionComponent>                      dest);
    static bool AABBToSphere(Weak<class AABBCollisionComponent> src,
      Weak<class SphereCollisionComponent>                      dest);
    static bool AABBToOBB(Weak<class AABBCollisionComponent> src,
      Weak<class OBBCollisionComponent>                      dest);
    static bool OBBToOBB(Weak<class OBBCollisionComponent> src,
      Weak<class OBBCollisionComponent>                    dest);
    static bool OBBToSphere(Weak<class OBBCollisionComponent> src,
      Weak<class SphereCollisionComponent>                    dest);
    static bool SphereToSphere(Weak<class SphereCollisionComponent> src,
      Weak<class SphereCollisionComponent>                          dest);

    static bool AABBToAABB(const AABB2D& src, const AABB2D& dest);
    static bool AABBToSphere(
      const AABB2D& src, Vector3 destCenter, float destRadius);
    static bool AABBToOBB(const AABB2D& src, const OBB2D& dest);
    static bool OBBToOBB(const OBB2D& src, const OBB2D& dest);
    static bool OBBToSphere(
      const OBB2D& src, Vector3 destCenter, float destRadius);
    static bool SphereToSphere(
      Vector3 srcCenter, float srcRadius, Vector3 destCenter, float destRadius);

private:
    static bool  ComputeAxisProjection(Vector2 centerLine,
       Vector2                                 srcAxis,
       float                                   srcAxisHalfSize,
       const OBB2D&                            destOBB);
    static OBB2D CreateOBB(const AABB2D& aabb);
};
