#ifndef _T3DMath_H_
#define _T3DMath_H_

/* INCLUDES */

#include <math.h>
#include <stdio.h>
#include <memory.h>

/* CONSTANTS */
const double T_PI = 3.14159265;
const double T_PI2 = 1.5707963;
const double T_2PI = 6.2831853;
const float  T_G = -32.174f; // ft/s^2
const float  T_EPSILON = 0.00001f;

/* DEFINES */

#define TFRONT    0
#define TBACK     1
#define TPLANAR   2
#define TCLIPPED  3
#define TCULLED   4
#define TVISIBLE  5

/* MACROS */

float _fabs(float f);

/* STRUCTS */


/* DECLARATIONS */

class TMatrix;
class TObb;
class TAabb;
class TPlane;
class TQuat;
class TPolygon;

/* CLASSES */

// Vector class
class __declspec(dllexport) TVector {
public:
    float x, y, z, w;       // coordinateset

    //---------------------------------------

    TVector(void) { x = 0, y = 0, z = 0, w = 1.0f; }
    TVector(float _x, float _y, float _z)
    {
        x = _x; y = _y; z = _z; w = 1.0f;
    }

    inline void  Set(float _x, float _y, float _z, float _w = 1.0f);
    inline float GetLength(void);                   // length
    inline float GetSqrLength(void) const;          // square length
    inline void  Negate(void);                      // vector mult -1
    inline void  Normalize(void);                   // normalize
    inline float AngleWith(TVector& v);          // angle in rad
    inline void  RotateWith(const TMatrix&);      // apply rotation part
    inline void  InvRotateWith(const TMatrix&);   // apply inverse rotation
    inline void  Difference(const TVector& v1,    // from v1 to v2
        const TVector& v2);
    void operator += (const TVector& v);          // operator +=
    void operator -= (const TVector& v);          // operator -=
    void operator *= (float f);                     // scale vector
    void operator /= (float f);                     // scale down
    void operator += (float f);                     // add scalar
    void operator -= (float f);                     // subtract scalar
    float     operator * (const TVector& v)const; // dot product
    TVector operator * (float f)const;            // scale vector
    TVector operator / (float f)const;            // scalar divide
    TVector operator + (float f)const;            // add scalar
    TVector operator - (float f)const;            // scale down
    TQuat   operator * (const TQuat& q)const; // vector * quaternion
    TVector operator * (const TMatrix& m)const; // vector-matrix product
    TVector operator + (const TVector& v)const; // addition
    TVector operator - (const TVector& v)const; // subtraction

    inline void Cross(const TVector& v1,          // cross product
        const TVector& v2);
};  // class
/*----------------------------------------------------------------*/


// Matrix class
class __declspec(dllexport) TMatrix {
public:
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;

    //---------------------------------------

    TMatrix(void) { /* nothing to do */; }

    inline void Identity(void);                       // identity matrix
    inline void RotaX(float a);                       // x axis
    inline void RotaY(float a);                       // y axis
    inline void RotaZ(float a);                       // z axis
    inline void Rota(const TVector& vc);            // x, y and z
    inline void Rota(float x, float y, float z);      // x, y and z
    inline void RotaArbi(const TVector& vcAxis, float a);
    inline void ApplyInverseRota(TVector* pvc);
    inline void Translate(float dx, float dy, float dz);
    inline void SetTranslation(TVector vc, bool EraseContent = false);
    inline TVector GetTranslation(void);

    inline void Billboard(TVector vcPos, TVector vcDir,
        TVector vcWorldUp = TVector(0, 1, 0));
    inline void LookAt(TVector vcPos, TVector vcLookAt,
        TVector vcWorldUp = TVector(0, 1, 0));

    inline void TransposeOf(const TMatrix& m);       // transpose m, save result in this
    inline void InverseOf(const TMatrix& m);         // invert m, save result in this

    TMatrix operator * (const TMatrix& m)const;    // matrix multiplication
    TVector operator * (const TVector& vc)const;   // matrix vector multiplication
}; // class
/*----------------------------------------------------------------*/


// Ray class
class __declspec(dllexport) TRay {
public:
    TVector m_vcOrig,  // ray origin
        m_vcDir;   // ray direction

//---------------------------------------

    TRay(void) { /* nothing to do */; }

    inline void Set(TVector vcOrig, TVector vcDir);
    inline void DeTransform(const TMatrix& _m); // move to matrixspace

    bool Intersects(const TVector& vc0, const TVector& vc1,
        const TVector& vc2, bool bCull,
        float* t);
    bool Intersects(const TVector& vc0, const TVector& vc1,
        const TVector& vc2, bool bCull,
        float fL, float* t);
    bool Intersects(const TPlane& plane, bool bCull,
        float* t, TVector* vcHit);
    bool Intersects(const TPlane& plane, bool bCull,
        float fL, float* t, TVector* vcHit);
    bool Intersects(const TAabb& aabb, float* t);
    bool Intersects(const TAabb& aabb, float fL, float* t);
    bool Intersects(const TObb& obb, float* t);
    bool Intersects(const TObb& obb, float fL, float* t);
}; // class
/*----------------------------------------------------------------*/


// Plane class
class __declspec(dllexport) TPlane {
public:
    TVector m_vcN,       // plane normal vector
        m_vcPoint;   // point on plane
    float     m_fD;        // distance to origin

    //---------------------------------------

    TPlane(void) { /* nothing to do */; }

    inline void  Set(const TVector& vcN, const TVector& vcP);
    inline void  Set(const TVector& vcN, const TVector& vcP, float fD);
    inline void  Set(const TVector& v0, const TVector& v1, const TVector& v2);
    inline float Distance(const TVector& vcPoint);
    inline int   Classify(const TVector& vcPoint);
    int   Classify(const TPolygon& Polygon);

    bool Clip(const TRay*, float, TRay*, TRay*);

    bool Intersects(const TVector& vc0, const TVector& vc1,
        const TVector& vc2);
    bool Intersects(const TPlane& plane, TRay* pIntersection);
    bool Intersects(const TAabb& aabb);
    bool Intersects(const TObb& obb);

}; // class
/*----------------------------------------------------------------*/


// Orientedbox class
class __declspec(dllexport) TObb {
public:
    float     fA0, fA1, fA2; // half axis length
    TVector vcA0, vcA1, vcA2; // box axis
    TVector vcCenter;         // centerpoint

    //---------------------------------------

    TObb(void) { /* nothing to do */; }

    inline void DeTransform(const TObb& obb,
        const TMatrix& m);

    bool Intersects(const TRay& Ray, float* t);
    bool Intersects(const TRay& Ray, float fL, float* t);
    bool Intersects(const TObb& Obb);
    bool Intersects(const TVector& v0,
        const TVector& v1,
        const TVector& v2);

    int  Cull(const TPlane* pPlanes, int nNumPlanes);

private:
    void ObbProj(const TObb& Obb, const TVector& vcV,
        float* pfMin, float* pfMax);
    void TriProj(const TVector& v0, const TVector& v1,
        const TVector& v2, const TVector& vcV,
        float* pfMin, float* pfMax);
}; // class
/*----------------------------------------------------------------*/

// Axisalignedbox class
class __declspec(dllexport) TAabb {
public:
    TVector vcMin, vcMax; // box extreme points
    TVector vcCenter;     // centerpoint

    //---------------------------------------

    TAabb(void) { /* nothing to do */; }
    TAabb(TVector vcMin, TVector vcMax);

    void Construct(const TObb* pObb);                  // build from obb
    int  Cull(const TPlane* pPlanes, int nNumPlanes);

    // normals pointing outwards
    void GetPlanes(TPlane* pPlanes);

    bool Contains(const TRay& Ray, float fL);
    bool Intersects(const TRay& Ray, float* t);
    bool Intersects(const TRay& Ray, float fL, float* t);
    bool Intersects(const TAabb& aabb);
    bool Intersects(const TVector& vc0);
}; // class
/*----------------------------------------------------------------*/


// Polygon class
class __declspec(dllexport) TPolygon {
    friend class TPlane;        // access for easier classifying

private:
    TPlane       m_Plane;    // plane which poly lies in

    int            m_NumP;     // number of points
    int            m_NumI;     // number of indices
    TAabb        m_Aabb;     // bounding box
    unsigned int   m_Flag;     // whatever you want it to be

    void CalcBoundingBox(void);

    //---------------------------------------

public:
    TPolygon(void);
    ~TPolygon(void);


    TVector* m_pPoints;  // list of points
    unsigned int* m_pIndis;   // index list

    void          Set(const TVector* pPoints, int nNumP,
        const unsigned int* pIndis, int nNumI);

    void          Clip(const TPlane& Plane,
        TPolygon* pFront,
        TPolygon* pBack);
    void          Clip(const TAabb& aabb);
    int           Cull(const TAabb& aabb);

    void          CopyOf(const TPolygon& Poly);

    void          SwapFaces(void);

    bool          Intersects(const TRay& Ray, bool, float* t);
    bool          Intersects(const TRay& Ray, bool, float fL, float* t);

    int           GetNumPoints(void) { return m_NumP; }
    int           GetNumIndis(void) { return m_NumI; }
    TVector* GetPoints(void) { return m_pPoints; }
    unsigned int* GetIndices(void) { return m_pIndis; }
    TPlane      GetPlane(void) { return m_Plane; }
    TAabb       GetAabb(void) { return m_Aabb; }
    unsigned int  GetFlag(void) { return m_Flag; }
    void          SetFlag(unsigned int n) { m_Flag = n; }

    // DEBUG ONLY
    void Print(FILE*);
}; // class
/*----------------------------------------------------------------*/


// a simple input no output stack for polygons
class TPolylist {
public:
    TPolylist(void);
    ~TPolylist(void);

    bool AddPolygon(const TPolygon&);
    void Reset(void);

    TPolygon* GetPolylist(void) { return m_pPolys; }
    unsigned int  GetNum(void) { return m_Num; }

private:
    TPolygon* m_pPolys;
    unsigned int  m_Num;
    unsigned int  m_Max;

    bool CheckMem(void);
}; // class
/*----------------------------------------------------------------*/


// Quaternion class
class __declspec(dllexport) TQuat {
public:
    float x, y, z, w;

    //---------------------------------------

    TQuat(void) { x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f; }
    TQuat(float _x, float _y, float _z, float _w)
    {
        x = _x; y = _y; z = _z; w = _w;
    }

    void  MakeFromEuler(float fPitch, float fYaw, float fRoll);
    void  Normalize();
    void  Conjugate(TQuat q);
    void  GetEulers(float* fPitch, float* fYaw, float* fRoll);
    void  GetMatrix(TMatrix* m);
    float GetMagnitude(void);


    void    operator /= (float f);
    TQuat operator /  (float f);

    void    operator *= (float f);
    TQuat operator *  (float f);

    TQuat operator *  (const TVector& v) const;

    TQuat operator *  (const TQuat& q) const;
    void    operator *= (const TQuat& q);

    void    operator += (const TQuat& q);
    TQuat operator +  (const TQuat& q) const;

    TQuat operator~(void) const { return TQuat(-x, -y, -z, w); }

    void Rotate(const TQuat& q1, const TQuat& q2);

    TVector Rotate(const TVector& v);

}; // class
/*----------------------------------------------------------------*/


// our bsp tree class for leavy non-solid bsp trees 
class TBspTree {
public:
    TBspTree(void);
    virtual ~TBspTree(void);

    void          BuildTree(const TPolygon*, unsigned int);
    void          TraverseBtF(TPolylist*, TVector, const TPlane*);
    void          TraverseFtB(TPolylist*, TVector, const TPlane*);

    TAabb       GetAabb(void) { return m_Aabb; }
    unsigned int  GetNumPolys(void) { return TBspTree::m_sNum; }

    bool          LineOfSight(const TVector&, const TVector&);
    bool          TestCollision(const TAabb&, TPlane*);
    bool          TestCollision(const TRay&, float, float*, TVector*);

private:
    TAabb       m_Aabb;        // bounding box
    TPlane      m_Plane;       // splitting plane
    TBspTree* m_pBack;       // backlist
    TBspTree* m_pFront;      // frontlist
    TBspTree* m_pRoot;       // root node
    TBspTree* m_pParent;     // parent node
    TPolygon* m_pPolys;      // if leaf node
    unsigned int  m_NumPolys;    // if leaf

    static unsigned int m_sNum;  // final poly count

    void CreateChilds(void);
    bool FindBestSplitter(void);
    void AddPolygon(const TPolygon&);
    void SetRelationships(TBspTree*, TBspTree*);
    void CalcBoundingBox(const TPolygon*, unsigned int);
    bool IsLeaf(void) { return (m_pFront == nullptr) && (m_pBack == nullptr); }
}; // class
/*----------------------------------------------------------------*/


// our basic octree class
class TOctree {
public:
    TOctree(void);
    virtual ~TOctree(void);

    void     BuildTree(const TPolygon*, unsigned int);

    void     Traverse(TPolylist*, TPolylist*, const TPlane*);

    TAabb  GetAabb(void) { return m_Aabb; }

    bool     GetFloor(const TVector&, float*, TPlane*);
    bool     TestCollision(const TAabb&, TPlane*);
    bool     TestCollision(const TRay&, float, float*);

private:
    TAabb       m_Aabb;        // bounding box
    TPolygon* m_pPolys;      // if leaf node
    unsigned int  m_NumPolys;    // if leaf
    TOctree* m_pChild[8];   // eight child objects
    TOctree* m_pRoot;       // root node
    TOctree* m_pParent;     // root node
    int           m_Pos;         // NE, NW, ...

    void CalcBoundingBox(const TPolygon*, unsigned int);
    void InitChildObject(int ChildID, TOctree* pParent);
    void ChopListToMe(TPolygon*, unsigned int);
    void CreateChilds(TOctree* pRoot);
    bool IntersectsDownwardsRay(const TVector&, float);
    void GetAabbAsPolygons(TPolylist*);

    bool IsLeaf(void) { return (m_pChild[0] == nullptr); }

    void SetBoundingBox(const TAabb& Aabb)
    {
        memcpy(&m_Aabb, &Aabb, sizeof(TAabb));
    }
}; // class
/*----------------------------------------------------------------*/


#endif 
