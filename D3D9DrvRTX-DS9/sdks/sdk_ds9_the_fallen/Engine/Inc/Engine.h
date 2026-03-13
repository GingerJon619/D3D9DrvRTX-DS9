#pragma once
// Engine.h – Deep Space Nine: The Fallen SDK (UE1 build 338)
// Generated from Engine.dll + Render.dll export analysis.
// Provides: URenderDevice, URenderIterator, AActor, UTexture, UModel,
//           FBspNode, FBspSurf, FSceneNode, FTextureInfo, etc.

#ifndef ENGINE_API
#define ENGINE_API __declspec(dllimport)
#endif

#include "Core.h"

//=============================================================================
// Forward declarations
//=============================================================================
class  UEngine;
class  UClient;
class  UViewport;
class  URenderBase;
class  URenderDevice;
class  URenderIterator;
class  ULevel;
class  UModel;
class  UTexture;
class  UBitmap;
class  UPalette;
class  UPrimitive;
class  UMesh;
class  ULodMesh;
class  USound;
class  UMusic;
class  AActor;
class  APawn;
class  APlayerPawn;
class  ABrush;
class  AMover;
class  AZoneInfo;
class  ALevelInfo;
class  ASkyZoneInfo;
class  ANavigationPoint;
class  AInventory;           // weapon/inventory actor
class  APolyobj;             // DS9-specific brush poly object actor
struct FURL;
struct FBspSurf;
struct FSceneNode;
struct FTextureInfo;
struct FSurfaceInfo;
struct FSurfaceFacet;
struct FTransTexture;
struct FTransSample;
struct FMipmapBase;
class  FSpanBuffer;
struct FScreenBounds;
struct FDynamicSprite;
struct FActorLink;
struct FVolActorLink;
struct FPoly;
struct FSavedPoly;
struct FDecal;
struct FPointRegion;
struct FFogSurf;
struct FDynamicItem;
struct FCheckResult;
struct FLazyLoader;
struct HHitProxy;
class  UCanvas;
class  UPlayer;

//=============================================================================
// ETextureFormat
//=============================================================================
enum ETextureFormat
{
    TEXF_P8     = 0x00,
    TEXF_RGBA7  = 0x01,
    TEXF_RGB16  = 0x02,
    TEXF_DXT1   = 0x03,
    TEXF_RGB8   = 0x04,
    TEXF_RGBA8  = 0x05,
    TEXF_NODATA = 0x06,
    TEXF_DXT3   = 0x07,
    TEXF_DXT5   = 0x08,
    TEXF_L8     = 0x09,
    TEXF_G16    = 0x0A,
    TEXF_RRRGGGBBB = 0x0B,
};

//=============================================================================
// EPolyFlags – surface rendering flags
//=============================================================================
enum EPolyFlags
{
    PF_Invisible        = 0x00000001,
    PF_Masked           = 0x00000002,
    PF_Translucent      = 0x00000004,
    PF_NotSolid         = 0x00000008,
    PF_Environment      = 0x00000010,
    PF_ForceViewZone    = 0x00000010,
    PF_Semisolid        = 0x00000020,
    PF_Modulated        = 0x00000040,
    PF_FakeBackdrop     = 0x00000080,
    PF_TwoSided         = 0x00000100,
    PF_AutoUPan         = 0x00000200,
    PF_AutoVPan         = 0x00000400,
    PF_NoSmooth         = 0x00000800,
    PF_BigWavy          = 0x00001000,
    PF_SmallWavy        = 0x00002000,
    PF_Flat             = 0x00004000,
    PF_LowShadowDetail  = 0x00008000,
    PF_NoMerge          = 0x00010000,
    PF_CloudWavy        = 0x00020000,
    PF_DirtyShadows     = 0x00040000,
    PF_BrightCorners    = 0x00080000,
    PF_SpecialLit       = 0x00100000,
    PF_Gouraud          = 0x00200000,
    PF_NoBoundRejection = 0x00200000,
    PF_Unlit            = 0x00400000,
    PF_HighShadowDetail = 0x00800000,
    PF_Portal           = 0x04000000,
    PF_Mirrored         = 0x08000000,
    PF_Memorized        = 0x01000000,
    PF_Selected         = 0x02000000,
    PF_Highlighted      = 0x10000000,
    PF_FlatShaded       = 0x40000000,
    PF_EdProcessed      = 0x40000000,
    PF_EdCut            = 0x80000000,
    PF_Occlude          = 0x80000000,
    PF_RenderFog        = 0x40000000,
    PF_NoOcclude        = PF_Masked|PF_Translucent|PF_Invisible|PF_Modulated,
    PF_AllFlags         = 0xFFFFFFFF,
};

// Render lock flags
#define RLF_ClearScreen   1
#define RLF_RealTimeOnly  2

//=============================================================================
// ELightType – actor light type
//=============================================================================
enum ELightType
{
    LT_None=0, LT_Steady, LT_Pulse, LT_Blink, LT_Flicker, LT_Strobe,
    LT_BackLight, LT_SubtlePulse, LT_TexturePaletteOnce, LT_TexturePaletteLoop,
};

//=============================================================================
// FMipmapBase
//=============================================================================
struct FMipmapBase
{
    BYTE* DataPtr;     // pointer to raw texels
    INT   USize;       // width in texels
    INT   VSize;       // height in texels
    BYTE  UBits;       // log2(USize)
    BYTE  VBits;       // log2(VSize)
    FMipmapBase(){}
    FMipmapBase(BYTE UB,BYTE VB):DataPtr(nullptr),USize(1<<UB),VSize(1<<VB),UBits(UB),VBits(VB){}
};


//=============================================================================
// EDrawStyle – actor/poly style (STY_ values used by renderer)
//=============================================================================
enum EDrawStyle
{
    STY_None        = 0,
    STY_Normal      = 1,
    STY_Masked      = 2,
    STY_Translucent = 3,
    STY_Modulated   = 4,
    STY_AlphaBlend  = 5,
    STY_AlphaBlendZ = 6,
};

// Texture cache ID types
#define CID_RenderTexture 0x14

//=============================================================================
// FTextureInfo – filled by UTexture::Lock()
// Signature: Lock(FTextureInfo&, DOUBLE Time, INT LOD, URenderDevice*)
//=============================================================================
struct ENGINE_API FTextureInfo
{
    UTexture*    Texture;         // +0   source texture object
    QWORD        CacheID;         // +4   unique id (QWORD)
    DWORD        PolyFlags;       // +12  poly flags
    FPlane       ColorRemap;      // +16  colour remapping plane (16 bytes)
    FColor*      Palette;         // +32  pointer to 256-entry palette (for P8)
    FLOAT        UScale;          // +36  horizontal scale
    FLOAT        VScale;          // +40  vertical scale
    FLOAT        UPan;            // +44  u panning offset
    FLOAT        VPan;            // +48  v panning offset
    INT          USize;           // +52  texture width
    INT          VSize;           // +56  texture height
    INT          UClamp;          // +60  u clamping limit
    INT          VClamp;          // +64  v clamping limit
    INT          NumMips;         // +68  number of mip levels
    FMipmapBase* Mips[12];        // +72  mip level data pointers
    UBOOL        bRealtimeChanged;// +120 realtime texture changed flag
    BYTE         bRealtime;       // +124 realtime texture (always updating)
    BYTE         RenderInterface; // +125
    BYTE         bHighColorQuality;
    BYTE         bHighTextureQuality;
    BYTE         LOD;             // +127
    ETextureFormat Format;           // texture format (ETextureFormat)

    void Load();
    void Unload();
    void CacheMaxColor();
};

//=============================================================================
// FSurfaceFacet
//=============================================================================
struct FSurfaceFacet
{
    FCoords  Coords;              // world-space coords for this facet
    FCoords  MapCoords;           // texture mapping coords
    FSpanBuffer* Span;            // span buffer (may be null)
    FSavedPoly*  Polys;           // clipped polygon list
    FVector  MapUnlit;
};

//=============================================================================
// FTransform – transformed vertex (used by renderer)
// NOTE: Moved here from below so FTransSample can inherit from it.
//=============================================================================
struct FTransform
{
    FVector Point;      // transformed point
    FLOAT   RZ;         // 1/Z
    FLOAT   ScreenX;
    FLOAT   ScreenY;
    DWORD   Flags;      // clipping flags
};

//=============================================================================
// FTransSample / FTransTexture
//=============================================================================
struct FTransSample : public FTransform
{
    // Point is inherited from FTransform
    FLOAT   U,V;
    FLOAT   Fog;
    FPlane  Light;
};

struct FTransTexture : public FTransSample
{
    FLOAT   U2,V2;
};

//=============================================================================
// FPoly / FSavedPoly  (UTGLR_OLD_POLY_CLASSES layout — pre-UT436)
//=============================================================================
#define FPoly_MAX_VERTICES  16

struct ENGINE_API FPoly
{
    FVector Base;                 // base point
    FVector Normal;               // face normal
    FVector TextureU,TextureV;    // texture vectors
    FVector Vertex[FPoly_MAX_VERTICES]; // vertices
    DWORD   PolyFlags;
    ABrush* Actor;
    class UTexture* Texture;
    FName   ItemName;
    INT     NumVertices;
    INT     iLink;
    INT     iBrushPoly;
    FLOAT   LightMapScale;
    FLOAT   ShadowMapScale;
    SWORD   PanU;                 // U panning offset (pixels)
    SWORD   PanV;                 // V panning offset (pixels)
    DWORD   Unused[2];

    void Init();
    INT  Finalize(INT NoError);
    INT  CalcNormal();
    FLOAT Area();
    INT  Fix();
    void Reverse();
    INT  RemoveColinears();
    INT  IsCoplanar(const FPoly& P) const;
    INT  IsBackfaced(const FVector& Point) const;
    INT  Faces(const FPoly& P) const;
    INT  Split(const FVector& Normal,const FVector& Base,INT NoFix);
    INT  SplitInHalf(FPoly* OtherHalf);
    INT  SplitWithPlane(const FVector& PlaneBase,const FVector& PlaneNormal,FPoly* FrontPoly,FPoly* BackPoly,INT VeryPrecise) const;
    INT  SplitWithPlaneFast(const FPlane& Plane,FPoly* FrontPoly,FPoly* BackPoly) const;
    INT  SplitWithNode(const UModel* Model,INT iNode,FPoly* FrontPoly,FPoly* BackPoly,INT VeryPrecise) const;
    void Transform(const FModelCoords& Coords,const FVector& PreSubtract,const FVector& PostAdd,FLOAT Orientation);
};

struct FSavedPoly
{
    FSavedPoly*   Next;            // next poly in chain
    AActor*       Actor;
    INT           NumPts;
    FTransTexture* Pts[1];         // variable length — pointers to transformed verts
};

//=============================================================================
// FDecal
//=============================================================================
struct FDecal
{
    FVector     Vertices[4];
    AActor*     Actor;
    TArray<INT> Nodes;
};

//=============================================================================
// FBspNode – BSP tree node (build 338 layout, UTGLR_OLD_ZONE_CONNECTIVITY)
//=============================================================================
#define MAX_ZONES  64

struct FZoneProperties
{
    class AZoneInfo* ZoneActor;     // zone actor (may be null for default zone)
    QWORD            Connectivity;  // zone connectivity bit mask (64 bits)
    QWORD            Visibility;    // zone visibility bit mask
};

struct ENGINE_API FBspNode
{
    static const int ZONE_COUNT = 64; // Use MAX_ZONES #define for plain array sizes; ZONE_COUNT for FBspNode:: scope
    // Plane
    FPlane  Plane;                  // +0   splitting plane (16 bytes)
    // Child / leaf / surface indices
    DWORD   ZoneMask;               // +16  zones visible from this node
    INT     iVertPool;              // +20  index into vertex pool
    INT     iSurf;                  // +24  surface index (-1 = no surface)
    INT     iBack;                  // +28  back child (-1 = none)
    INT     iFront;                 // +32  front child (-1 = none)
    INT     iPlane;                 // +36  co-planar node chain (-1 = end)
    INT     iCollisionBound;        // +40
    INT     iRenderBound;           // +44
    BYTE    iZone[2];               // +48  zone index (back, front)
    BYTE    NumVertices;            // +50  verts in this node's polygon
    BYTE    NodeFlags;              // +51  node flags (NF_*)
    INT     iLeaf[2];               // +52  leaf indices (back, front)
    // Total: 60 bytes
};

// NodeFlags
#define NF_NotCsg        0x01
#define NF_ShootThrough  0x02
#define NF_NotVisBlocking 0x04
#define NF_PolyOccluded  0x08
#define NF_BoxOccluded   0x10
#define NF_BrightCorners 0x80


//=============================================================================
// FVert – vertex pool entry (one per polygon vertex)
//=============================================================================
struct FVert
{
    INT  pVertex;   // index into UModel::Points
    INT  iSide;     // which side of the polygon (-1 = none)
};

//=============================================================================
// FBspSurf – BSP surface (build 338 layout)
//=============================================================================
struct ENGINE_API FBspSurf
{
    UTexture*  Texture;       // +0   texture (may be null → no texture drawn)
    DWORD      PolyFlags;     // +4   poly flags
    INT        pBase;         // +8   point pool index of base point
    INT        vNormal;       // +12  vector pool index of surface normal
    INT        vTextureU;     // +16  vector pool index of U texture vector
    INT        vTextureV;     // +20  vector pool index of V texture vector
    INT        iLightMap;     // +24  lightmap index
    INT        iBrushPoly;    // +28  brush polygon index
    SWORD      iBrush;        // +32  brush actor index (-1 = world brush)
    SWORD      PanU;          // +34  U pan in pixels
    SWORD      PanV;          // +36  V pan in pixels
    SWORD      Unused;        // +38
    AActor*    Actor;         // +40  mover/actor owning this surface
    TArray<FDecal> Decals;   // +44  decal list  (skipped by UTGLR_NO_DECALS)
    TArray<INT> Nodes;        // +56  BSP nodes referencing this surface
    // Total size ~68 bytes
    FPoly*     Polys;         // +68  polygon list (UTGLR_OLD_POLY_CLASSES accessor)
};

//=============================================================================
// FPointRegion
//=============================================================================
struct FPointRegion
{
    AZoneInfo* Zone;
    INT        iLeaf;
    BYTE       ZoneNumber;
    FPointRegion():Zone(nullptr),iLeaf(-1),ZoneNumber(0){}
    FPointRegion(AZoneInfo* Z):Zone(Z),iLeaf(-1),ZoneNumber(0){}
    FPointRegion(AZoneInfo* Z,INT Leaf,BYTE ZN):Zone(Z),iLeaf(Leaf),ZoneNumber(ZN){}
};


//=============================================================================
// FScreenBounds
//=============================================================================
#ifndef FSCREENBOUNDS_DEFINED
#define FSCREENBOUNDS_DEFINED
struct FScreenBounds
{
    FLOAT MinX,MinY,MaxX,MaxY;
    FLOAT MinZ;
};
#endif

//=============================================================================
// FBspDrawList – render draw list entry
//=============================================================================
struct FBspDrawList
{
    FBspDrawList* Next;   // next in draw list
    INT           iSurf;  // surface index into UModel::Surfs
    INT           iZone;  // zone number this draw came from
};

//=============================================================================
// FDynamicItem / FActorLink / FVolActorLink / FDynamicSprite
//=============================================================================
struct FDynamicItem
{
    FDynamicItem* Next;
};

struct FActorLink
{
    AActor*      Actor;
    FActorLink*  Next;
};

struct FVolActorLink
{
    AActor*      Actor;
    FVolActorLink* Next;
    FLOAT        Weight;
};

struct FDynamicSprite : public FDynamicItem
{
    FSpanBuffer*   Span;
    AActor*        Actor;
    FDynamicSprite* RenderNext;
    // Additional render-time fields…
};

//=============================================================================
// FFogSurf
//=============================================================================
struct FFogSurf
{
    FTextureInfo* FogMap;
    FPlane        FogColor;
    FLOAT         FogDistance;
    FLOAT         FogDensity;
};


struct FSurfaceInfo  // ENGINE_API removed: DS9 lib ctor signature mismatches; use inline ctor
{
    DWORD        PolyFlags;     // polygon flags
    FPlane       FlatColor;     // flat shading color
    ULevel*      Level;         // level this surface belongs to
    FTextureInfo* Texture;      // diffuse texture (may be null)
    FTextureInfo* LightMap;     // light map
    FTextureInfo* MacroTexture; // macro texture
    FTextureInfo* DetailTexture;// detail texture
    FTextureInfo* FogMap;       // fog map
    FSurfaceInfo() : PolyFlags(0), Level(nullptr), Texture(nullptr), LightMap(nullptr),
        MacroTexture(nullptr), DetailTexture(nullptr), FogMap(nullptr) {}
};

//=============================================================================
// FSceneNode
//=============================================================================
struct ENGINE_API FSceneNode
{
    // Viewport
    UViewport*   Viewport;          // rendering viewport
    FSpanBuffer* Span;              // span buffer
    // Camera
    FCoords      Coords;            // world-to-camera transform
    FCoords      Uncoords;          // camera-to-world transform
    FVector      ViewOrigin;        // camera origin in world
    FPlane       NearClip;
    FDynamicSprite* Sprite;       // actor sprite list
    FBspDrawList*   Draw[2];      // BSP draw lists
    FMemStack*      VectorMem;    // vector scratch mem
    INT             XB,X,XR,YB,Y,YR;  // clip boundaries / viewport pixel size
    // Projection
    FLOAT        FX,FY;             // screen dimensions FLOAT
    FLOAT        FX2,FY2;           // half screen dims
    FLOAT        ProjZ;             // projection Z (FOV-based)
    FLOAT        Zoom;
    // Frame hierarchy
    FSceneNode*  Parent;
    FSceneNode*  Sibling;
    FSceneNode*  Child;
    ULevel*      Level;
    INT          iSurf;             // surface being portaled through
    FLOAT        Mirror;            // ±1 mirroring
    // Zone
    INT          ZoneNumber;
    AZoneInfo*   ViewZone;
    // Misc
    DWORD        DrawFlags;
    INT          Recursion;
    FLOAT        RealTime;
    // Actor lists
    FDynamicSprite* DynSprites;
    FActorLink*    OwnedActors;

    FVector      RProj;             // reciprocal projection vector (Z = tan(HalfFOV))

    void ComputeRenderCoords(FVector& CamOrigin, FRotator& CamRot);
    void ComputeRenderSize();
    BYTE* Screen(INT X,INT Y);
};

//=============================================================================
// UBitmap / UPalette / UTexture
//=============================================================================
class ENGINE_API UBitmap : public UObject
{
public:
    BYTE         Format;        // ETextureFormat
    UPalette*    Palette;
    BYTE         UBits,VBits;
    INT          USize,VSize;
    FLOAT        Scale;
    INT          UClamp,VClamp;
    FColor       MipZero;
    FColor       MaxColor;
    DOUBLE       LastUpdateTime;
    static UClass* StaticClass();
};

class ENGINE_API UPalette : public UObject
{
public:
    TArray<FColor> Colors;
    BYTE  BestMatch(FColor Col,INT First);
    static UClass* StaticClass();
};

class ENGINE_API UTexture : public UBitmap
{
public:
    // Animated texture chain — DS9 build 338 uses AnimCur (not AnimCurrent)
    UTexture*    AnimCur;       // current animation frame (used by D3D9Render.cpp)
    UTexture*    AnimNext;      // next frame in animation sequence
    FLOAT        MinFrameRate,MaxFrameRate;
    FLOAT        Accumulator;

    // Surface properties
    DWORD        PolyFlags;     // default poly flags
    BYTE         BumpMap;       // bump map index
    BYTE         DetailTexture; // detail texture index
    BYTE         MacroTexture;  // macro texture index
    BYTE         bHighColorQuality;
    BYTE         bHighTextureQuality;
    BYTE         bRealtime;
    BYTE         bParametric;
    BYTE         bRealtimeChanged;
    FLOAT        DrawScale;
    FLOAT        LODSet;

    // DS9 texture API — Lock() is the info-acquire call (not GetInfo())
    virtual void Lock(FTextureInfo& TextureInfo, DOUBLE Time, INT LOD, URenderDevice* RenDev);
    virtual void Unlock(FTextureInfo& TextureInfo);
    virtual UTexture* Get(DOUBLE Time);      // returns current animation frame
    virtual FMipmapBase* GetMip(INT i);
    virtual void Init(INT InUSize,INT InVSize);
    virtual void Clear(DWORD ClearFlags);
    virtual void Click(DWORD PolyFlags,FLOAT X,FLOAT Y);
    virtual void Tick(FLOAT DeltaSeconds);
    virtual void ConstantTimeTick();
    virtual void MousePosition(DWORD Buttons,FLOAT X,FLOAT Y);
    virtual DWORD GetExtraTexturesPtr(UTexture*** List);
    virtual void Update(DOUBLE Time);

    INT  GetNumMips()  const;
    INT  DefaultLOD()  const;

    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// UPrimitive
//=============================================================================
class ENGINE_API UPrimitive : public UObject
{
public:
    FBox BoundingBox;
    FSphere BoundingSphere;
    static UClass* StaticClass();
    virtual FBox GetRenderBoundingBox(const AActor* Owner,UBOOL Exact);
    virtual FSphere GetRenderBoundingSphere(const AActor* Owner);
    virtual FBox GetCollisionBoundingBox(const AActor* Owner) const;
    virtual UBOOL LineCheck(FCheckResult& Result,AActor* Owner,FVector End,FVector Start,FVector Extent,DWORD ExtraNodeFlags);
    virtual UBOOL PointCheck(FCheckResult& Result,AActor* Owner,FVector Location,FVector Extent,DWORD ExtraNodeFlags);
};

//=============================================================================
// UMesh / ULodMesh
//=============================================================================
// FMeshFace/FMeshWedge - UE1 mesh face/wedge types
// FMeshWedge is also defined in UnRender.h; guard here
struct FMeshFace { WORD iWedge[3]; WORD MaterialIndex; };
#ifndef FMESHWEDGE_DEFINED
#define FMESHWEDGE_DEFINED
struct FMeshWedge { WORD iVertex; FLOAT U, V; };
#endif  // FMESHWEDGE_DEFINED
struct FMeshTri  { WORD iVertex[3]; BYTE MatIndex; BYTE zFlags; FLOAT U[3], V[3]; };
struct FMeshUV   { FLOAT U, V; };
struct FMeshMaterial_Stub {};  // placeholder; full def below

class ENGINE_API UMesh : public UPrimitive
{
public:
    TArray<UTexture*>  Textures;       // skins array
    TArray<FVector>  Verts;
    TArray<FMeshTri> Tris;
    INT              FrameVerts;
    virtual void GetFrame(FVector* Verts, INT Size, const FCoords& Coords, AActor* Owner) {}
    TArray<FVector>  Normals;
    INT              FrameCount;
    INT              VertCount;
    FLOAT            Scale;
    FVector          Origin;
    FRotator         RotOrigin;
    FLOAT            AnimMinRate;
    FLOAT            DefaultAnim;
    static UClass* StaticClass();
    virtual UTexture* GetTexture(INT Count,AActor* Owner);
};

//=============================================================================
// FMeshMaterial — moved here so ULodMesh::Materials can reference it
//=============================================================================
struct FMeshMaterial
{
    DWORD PolyFlags;
    INT   TextureIndex;
};

class ENGINE_API ULodMesh : public UMesh
{
public:
    INT                ModelVerts;     // count of model (non-special) vertices
    TArray<FMeshFace>  SpecialFaces;   // special-render faces
    // GetFrame: 4-arg (UNREAL_GOLD), 5-arg/INT (standard LOD), 5-arg/FSceneNode* (cloth/skel)
    virtual void GetFrame(FVector* Verts, INT Size, const FCoords& Coords, AActor* Owner) override {}
    virtual void GetFrame(FVector* Verts, INT Size, const FCoords& Coords, AActor* Owner, INT NumVerts) {}
    virtual void GetFrame(FVector* Verts, INT Size, const FCoords& Coords, AActor* Owner, FSceneNode* Frame) {}
    TArray<WORD>          CollapsePointThus;
    TArray<WORD>          FaceLevel;
    TArray<FMeshFace>     Faces;          // LOD mesh faces
    TArray<WORD>          CollapseWedgeThus;
    TArray<FMeshWedge>    Wedges;         // LOD mesh wedges (vertex + UV)
    TArray<FMeshMaterial> Materials;      // material/texture slots
    TArray<WORD>          Specials;
    INT                   SpecialVerts;   // count of special (attachment) vertices
    FLOAT         LODMinVerts;
    FLOAT         LODStrength;
    FLOAT         LODMorph;
    FLOAT         LODZDisplace;
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// UPolys – polygon list container (UModel::Polys points to one of these)
//=============================================================================
class ENGINE_API UPolys : public UObject
{
public:
    TArray<FPoly> Element;       // the actual polygon array
    static UClass* StaticClass();
};

//=============================================================================
// UModel
//=============================================================================
class ENGINE_API UModel : public UPrimitive
{
public:
    // Public arrays
    TArray<FVector>      Vectors;
    TArray<FVector>      Points;
    TArray<FBspNode>     Nodes;        // BSP nodes
    TArray<FBspSurf>     Surfs;        // BSP surfaces
    TArray<FVert>        Verts;        // vertex pool (index→FVert)
    TArray<INT>          LeafHulls;
    TArray<struct FLeaf> Leaves;
    // Zone data — UTGLR_OLD_ZONE_CONNECTIVITY: accessed as Model->Zones[i]
    FZoneProperties      Zones[MAX_ZONES];
    INT                  NumZones;
    INT                  RootOutside;
    INT                  Linked;
    // Lightmap
    TArray<struct FLightMapIndex> LightMap;
    TArray<BYTE>         LightBits;
    TArray<FBox>         Bounds;
    TArray<INT>          LeafVolumes;
    ABrush*              Owner;
    INT                  MoverLink;
    UPolys*              Polys;        // brush polygon array (for mover geometry)

    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;

    UBOOL PotentiallyVisible(INT iLeaf1,INT iLeaf2);
    FLightMapIndex* GetLightMapIndex(INT iSurf);
    void  BuildBound();
    void  EmptyModel(UBOOL EmptySurfs,UBOOL EmptyPolys);
    BYTE  FastLineCheck(FVector Start,FVector End);
    void  ModifySurf(INT Index,UBOOL UpdateMaster);
    FPointRegion PointRegion(AZoneInfo* Zone,FVector Point) const;
};

//=============================================================================
// AActor
//=============================================================================
class ENGINE_API AActor : public UObject
{
public:
    // --- Critical fields referenced by D3D9DrvRTX ---
    // Location / orientation
    FVector      Location;
    FVector      Velocity;
    FVector      Acceleration;
    FRotator     Rotation;
    FVector      OldLocation;
    FVector      ColLocation;
    // Mesh
    UMesh*       Mesh;
    FLOAT        DrawScale;
    FVector      PrePivot;
    FRotator     AmbientSound;
    // Region
    FPointRegion Region;
    // Texture / drawing
    UTexture*    Texture;
    UTexture*    Skin;          // skin override
    ELightType   LightType;
    BYTE         LightBrightness;
    BYTE         LightHue;
    BYTE         LightSaturation;
    BYTE         LightRadius;
    BYTE         LightCone;
    BYTE         LightPeriod;
    BYTE         LightPhase;
    BYTE         LightEffect;
    // Rendering flags
    BYTE         Style;         // render style (ERenderStyle)
    BYTE         bHidden;
    BYTE         bShadowCast;
    BYTE         bUnlit;
    BYTE         bNoSmooth;
    BYTE         bMeshCurvy;
    BYTE         bParticles;
    BYTE         bRandomFrame;
    BYTE         bMeshEnviroMap;
    BYTE         bSelected;
    BYTE         bEdLocked;
    BYTE         bEdShouldSnap;
    BYTE         bEdSnap;
    BYTE         bTempEditor;
    BYTE         bDeleteMe;
    BYTE         bTicked;
    BYTE         bLightChanged;
    BYTE         bDynamicLight;
    BYTE         bTimerLoop;
    BYTE         AmbientGlow;
    BYTE         Fatness;
    FLOAT        ScaleGlow;
    FLOAT        VisibilityRadius;
    FLOAT        VisibilityHeight;
    FLOAT        AmbientScale;
    // Collision
    FLOAT        CollisionRadius;
    FLOAT        CollisionHeight;
    UPrimitive*  Primitive;
    BYTE         bCollideActors;
    BYTE         bCollideWorld;
    BYTE         bBlockActors;
    BYTE         bBlockPlayers;
    BYTE         bProjTarget;
    BYTE         bSpecialLit;
    BYTE         bActorShadows;
    BYTE         bCorona;
    // Owner / level
    AActor*      Owner;
    FName        Tag;
    FName        Event;
    AActor*      Target;
    AActor*      Instigator;
    ULevel*      Level;         // owning level; also accessed as XLevel
    FLOAT        LifeSpan;
    // Animation
    FName        AnimSequence;
    FLOAT        AnimFrame;
    FLOAT        AnimRate;
    FLOAT        TweenRate;
    FLOAT        AnimLast;
    FLOAT        AnimMinRate;
    FLOAT        OldAnimRate;
    INT          AnimBlend;
    // Misc
    FName        InitialState;
    FName        Group;
    FVector      AttachTag;
    INT          StandingCount;
    INT          MiscNumber;
    INT          LatentByte;
    INT          LatentInt;
    FLOAT        LatentFloat;
    AActor*      LatentActor;
    AActor*      Deleted;

    // --- Rendering members (present on all actors in DS9 build 338) ---
    BYTE         DrawType;        // EDrawType: how the actor is drawn
    BYTE         bHiddenEd;       // hidden in editor
    BYTE         bOnlyOwnerSee;   // only owner can see this actor
    BYTE         bOwnerNoSee;     // owner cannot see this actor
    UTexture*    MultiSkins[8];   // per-LOD skin overrides
    UClass*      RenderIteratorClass;  // render iterator class (for custom rendering)
    URenderIterator* RenderInterface;  // active render iterator
    ULevel*      XLevel;          // alias for Level (same pointer, different name)

    // Methods used by renderer
    virtual void GetViewRotation(FRotator& Rot);
    virtual FVector GetCylinderExtent() const;
    virtual UTexture* GetSkin(INT Index);
    virtual UPrimitive* GetPrimitive() const;
    virtual ULevel* GetLevel() const;
    virtual APlayerPawn* GetPlayerPawn() const;
    virtual void BeginTouch(AActor* Other);
    virtual void EndTouch(AActor* Other,UBOOL NoNotifySelf);
    virtual FLOAT GetNetPriority(AActor* Other,FLOAT Time,FLOAT Lag);

    UBOOL IsIn(UObject* Outer) const;
    UBOOL IsInZone(const AZoneInfo* Zone) const;
    UBOOL IsOwnedBy(const AActor* Other) const;
    FLOAT LifeFraction();              // non-const version in Engine.lib
    FLOAT LifeFraction() const { return const_cast<AActor*>(this)->LifeFraction(); }
    AActor* GetTopOwner();

    // Script events
    void eventRenderOverlays(UCanvas* Canvas);

    static UClass* StaticClass();
};

//=============================================================================
// APawn
//=============================================================================
class ENGINE_API APawn : public AActor
{
public:
    APlayerPawn* Player;
    FLOAT        Health;
    FName        ReducedDamagePct;
    AInventory*  SelectedItem;
    AInventory*  Weapon;          // currently held weapon
    ANavigationPoint* MoveTarget;
    ANavigationPoint* RouteCache[16];
    FVector      Destination;
    FVector      Focus;
    FLOAT        MoveTimer;
    FLOAT        GroundSpeed;
    FLOAT        WaterSpeed;
    FLOAT        AirSpeed;
    FLOAT        AccelRate;
    FLOAT        JumpZ;
    FLOAT        MaxStepHeight;
    FLOAT        EyeHeight;
    FLOAT        BaseEyeHeight;
    FLOAT        MinHitWall;
    FLOAT        Noise1Loudness;
    FLOAT        Noise2Loudness;
    FVector      Noise1Spot;
    FVector      Noise2Spot;
    FLOAT        SightRadius;
    FLOAT        PeripheralVision;
    FLOAT        HearingThreshold;
    FLOAT        Aggressiveness;
    AActor*      Enemy;
    BYTE         bIsPlayer;
    BYTE         bCanJump;
    BYTE         bCanWalk;
    BYTE         bCanSwim;
    BYTE         bCanFly;
    BYTE         bCanOpenDoors;
    BYTE         bCanDoSpecial;
    BYTE         bDrowning;
    BYTE         bLOSflag;
    BYTE         bFromWall;
    BYTE         bHunting;
    BYTE         bAvoidLedges;
    BYTE         bStopAtLedges;
    BYTE         bJumpOffPawn;
    BYTE         bShootSpecial;
    BYTE         bAutoActivate;
    BYTE         bIsHuman;
    BYTE         bIsFemale;
    BYTE         bIsMultiSkinned;

    void ShowSelf();
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// APlayerPawn
//=============================================================================
class ENGINE_API APlayerPawn : public APawn
{
public:
    UPlayer*     Player;
    FLOAT        CurrentTime;           // viewport current time
    DWORD        ShowFlags;             // viewport show flags (SHOW_*)
    BYTE         Style;
    FLOAT        LifeFraction;          // DEPRECATED field — LifeFraction() method on AActor is preferred
    UBOOL        bBehindView;           // camera is behind the player
    AActor*      ViewTarget;            // camera view target (null = self)
    UBOOL        bDeleteMe;
    UViewport*   Viewport;
    FLOAT        FovAngle;              // field of view in degrees
    FLOAT        DemoViewer;
    FLOAT        ShadowScale;
    UTexture*    FlashTexture;
    FVector      FlashScale;
    FPlane       FlashFog;
    FLOAT        DesiredFlashFog;
    FLOAT        DesiredFlashScale;
    FVector      WeaponLoc;
    FRotator     WeaponRot;

    UBOOL CanSee(const AActor* Other);
    UBOOL IsActorVisible(const AActor* Other);
    UBOOL IsSurfVisible(const FBspNode* Node,INT iSurf,const FBspSurf* Surf);
    UBOOL IsZoneVisible(INT ZoneNum);
    UBOOL RecomputeLighting();
    UBOOL ClearScreen();
    INT   GetViewZone(INT bReflect,const UModel* Model);
    void  SetPlayer(UPlayer* P);

    void eventPlayerCalcView(AActor*& ViewActor,FVector& CameraLocation,FRotator& CameraRotation);
    void eventPostRender(UCanvas* Canvas);
    void eventPreRender(UCanvas* Canvas);
    void eventPlayerTick(FLOAT DeltaSeconds);

    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// ABrush / AMover
//=============================================================================
class ENGINE_API ABrush : public AActor
{
public:
    DWORD    CsgOper;
    UModel*  Brush;
    FLOAT    MainScale;
    FLOAT    PostScale;
    FLOAT    TempScale;
    FCoords  CsgTransform;
    FCoords  InvCsgTransform;
    DWORD    PolyFlags;
    DWORD    NotPolyFlags;
    ABrush*  BrushColor;

    FLOAT       BuildCoords(FModelCoords* Coords,FModelCoords* Uncoords);
    FCoords     ToLocal()  const;
    FCoords     ToWorld()  const;
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

class ENGINE_API AMover : public ABrush
{
public:
    BYTE    MoverEncroachType;
    BYTE    MoverGlideType;
    BYTE    bUseShortestPath;
    BYTE    bDamageSpecial;
    FLOAT   DamageThreshold;
    INT     NumKeys;
    INT     WorldRaytraceKey;
    INT     BrushRaytraceKey;
    INT     MoveTime;
    FLOAT   StayOpenTime;
    FLOAT   OtherTime;
    INT     EncroachDamage;
    BYTE    bTriggerOnceOnly;
    BYTE    bSlave;
    BYTE    bReversed;
    BYTE    bDelaying;
    BYTE    KeyNum;
    BYTE    PrevKeyNum;
    FVector BasePos,OldPos,OldPrePivot;
    FVector KeyPos[24];
    FRotator BaseRot,OldRot;
    FRotator KeyRot[24];
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// AInfo / AZoneInfo / ALevelInfo / ASkyZoneInfo
//=============================================================================
class ENGINE_API AInfo : public AActor
{
public:
    static UClass* StaticClass();
};

class ENGINE_API AZoneInfo : public AInfo
{
public:
    FLOAT  TexUPanSpeed;
    FLOAT  TexVPanSpeed;
    UTexture* EnvironmentMap;
    FLOAT    KillZ;
    FLOAT    KillZDamagePerSec;
    FLOAT    ZoneGravity;
    FVector  ZoneVelocity;
    FLOAT    ZoneGroundFriction;
    FLOAT    ZoneFluidFriction;
    FLOAT    ZoneTerminalVelocity;
    FName    ZoneTag;
    DWORD    ZonePlayerEvent;
    INT      ZonePlayerCount;
    INT      NumCarcasses;
    INT      DamagePerSec;
    FName    DamageType;
    FName    PainTime;
    FName    EntrySound;
    FName    ExitSound;
    class ASkyZoneInfo* SkyZone;
    FPlane   AmbientBrightness;
    FPlane   AmbientHue;
    FPlane   AmbientSaturation;
    BYTE     bWaterZone;
    BYTE     bFogZone;
    BYTE     bKillZone;
    BYTE     bNeutralZone;
    BYTE     bGravityZone;
    BYTE     bPainZone;
    BYTE     bDestructive;
    BYTE     bNoInventory;
    BYTE     bMoveProjectiles;
    BYTE     bReverbZone;
    BYTE     bRaytraceReverb;
    BYTE     SpeedOfSound;
    FPlane   MasterGain;
    INT      CutoffHz;
    BYTE     Delay[6];
    BYTE     Gain[6];
    BYTE     LensFlare[12];
    FLOAT    LensFlareOffset[12];
    FLOAT    LensFlareScale[12];
    UTexture* SkyTexture;
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

class ENGINE_API ALevelInfo : public AZoneInfo
{
public:
    FLOAT    TimeDilation;
    FLOAT    TimeSeconds;
    ANSICHAR Title[64];
    ANSICHAR Author[32];
    INT      IdealPlayerCount;
    INT      HubStackLevel;
    BYTE     LevelEnterText[64];
    FName    DefaultGameType;
    FName    Song;
    UTexture* Screenshot;
    FString  VisibleGroups;
    BYTE     bNoCheating;
    BYTE     bAllowFOV;
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
    FString Pauser;
};


class ENGINE_API ASkyZoneInfo : public AZoneInfo
{
public:
    FRotator RotationRate;   // sky rotation speed
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

class  UFont;           // forward decl for UCanvas

//=============================================================================
// UCanvas
//=============================================================================
class ENGINE_API UCanvas : public UObject
{
public:
    UViewport* Viewport;
    FLOAT      OrgX,OrgY;
    FLOAT      ClipX,ClipY;
    FLOAT      CurX,CurY,CurZ;
    FLOAT      Z;
    BYTE       Style;
    FLOAT      SpaceX,SpaceY;
    FLOAT      FontScaleX,FontScaleY;
    UFont*     Font;
    FPlane     DrawColor;

    virtual void Init(UViewport* InViewport);
    virtual void Update(FSceneNode* Frame);
    virtual void SetClip(INT X,INT Y,INT Width,INT Height);
    virtual void DrawIcon(UTexture* Tex,FLOAT X,FLOAT Y,FLOAT XL,FLOAT YL,FSpanBuffer* Span,FLOAT Z,FPlane Color,FPlane Fog,DWORD Style);
    virtual void DrawTile(UTexture* Tex,FLOAT X,FLOAT Y,FLOAT XL,FLOAT YL,FLOAT U,FLOAT V,FLOAT UL,FLOAT VL,FSpanBuffer* Span,FLOAT Z,FPlane Color,FPlane Fog,DWORD Style);
    void eventReset();
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// UViewport
//=============================================================================

class ENGINE_API USubsystem : public UObject
{
public:
    static UClass* StaticClass();
};


class ENGINE_API UPlayer : public UObject
{
public:
    APlayerPawn* Actor;
    FString      ConfiguredInternetSpeed;
    INT          CurrentNetSpeed;
    static UClass* StaticClass();
};

class ENGINE_API UViewport : public UPlayer
{
public:
    DWORD  ExtraPolyFlags;
    UClient*         Client;
    URenderDevice*   RenDev;
    FVector          FlashScale;
    FPlane           FlashFog;
    UCanvas*         Canvas;
    APlayerPawn*     Actor;         // owning player pawn (APlayerPawn in DS9)
    FLOAT            CurrentTime;
    DWORD            Blitflags;
    INT              SizeX,SizeY;
    INT              ColorBytes;
    BYTE             Fullscreen;
    UBOOL         FullscreenOnly;
    UBOOL         PrefersDeferredLoad;
    INT           HitX, HitY, HitXL, HitYL;  // selection hit-test region
    virtual HWND  GetWindow() { return nullptr; }
    virtual UBOOL ResizeViewport(DWORD BlitType, INT NewX, INT NewY, INT NewColorBytes=0) { return 1; }

    UBOOL IsOrtho();           // non-const in Engine.lib
    UBOOL IsOrtho() const { return const_cast<UViewport*>(this)->IsOrtho(); }
    UBOOL IsRealtime() const;
    UBOOL IsWire()     const;
    BYTE  ByteID();
    void  Lock(FPlane FlashScale,FPlane FlashFog,FPlane ScreenClear,DWORD RenderLockFlags,BYTE* HitData,INT* HitSize);
    void  ReadInput(FLOAT DeltaSeconds);
    void  PushHit(const HHitProxy& Hit,INT Size);
    void  PopHit(UBOOL bForce);
    void  ExecMacro(const ANSICHAR* Filename,FOutputDevice& Ar);
    void  ExecuteHits(const struct FHitCause& Cause,BYTE* HitData,INT HitCount);
    void  DumpScreen(ANSICHAR* Filename,INT Format);
    UClient* GetOuterUClient() const;
    UBOOL Exec(const ANSICHAR* Cmd,FOutputDevice& Ar) override;
    void  Serialize(FArchive& Ar) override;
    void  Destroy() override;

    static UClass* StaticClass();
    static UClass PrivateStaticClass;
};

//=============================================================================
// ULevel
//=============================================================================
// FURL – level URL (Map field is the map name)
struct FURL
{
    FString Map;       // map name (e.g. "Entry")
    FString Options;
    FString Portal;
    INT     Port;
    UBOOL   bValid;
    FURL() : Port(0), bValid(1) {}
};

//=============================================================================
// FMovingBrushTrackerBase – dynamic surface (mover) tracker
//=============================================================================
class ENGINE_API FMovingBrushTrackerBase
{
public:
    virtual UBOOL SurfIsDynamic(INT iSurf) = 0;
    virtual ~FMovingBrushTrackerBase() {}
};

//=============================================================================
// ULevel
//=============================================================================
class ENGINE_API ULevel : public UObject
{
public:
    UTexture*                DefaultTexture;
    FMovingBrushTrackerBase* BrushTracker;    // dynamic surface tracker (movers)
    UTexture*                EnvironmentMap;
    TArray<AActor*>          Actors;
    UModel*                  Model;
    ALevelInfo*              Info;
    FURL                     URL;             // level URL (URL.Map = map name)
    FLOAT                    TimeSeconds;     // current level time in seconds
    ABrush* Brush();
    AZoneInfo* GetZoneActor(INT ZoneNum);
    ALevelInfo* GetLevelInfo();
    void CleanupDestroyed(UBOOL bForce);
    void CompactActors();
    void Destroy() override;
    INT  GetActorIndex(AActor* Actor);
    void SetActorZone(AActor* Actor,UBOOL bTest,UBOOL bForce) {}

    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// UClient
//=============================================================================
class ENGINE_API UClient : public UObject
{
public:
    FLOAT    Brightness;          // gamma/brightness setting (0.5 = normal)
    INT      WindowedColorBits;
    INT      FullscreenColorBits;
    UEngine* Engine;
    static UClass* StaticClass();
};

//=============================================================================
// URenderBase
//=============================================================================


class ENGINE_API URenderBase : public USubsystem
{
public:
    virtual void Init(UEngine* Engine);

    static UClass* StaticClass();
    static UClass PrivateStaticClass;
};

//=============================================================================
// URenderDevice
// Vtable layout (Engine.dll build 338):
//   vtblPad0  = Init
//   vtblPad1  = SetRes
//   vtblPad2  = Exit
//   vtblPad3  = Flush          (void; UTGLR_ALT_FLUSH — no UBOOL param)
//   vtblPad4  = Lock
//   vtblPad5  = Unlock
//   vtblPad6  = DrawComplexSurface
//   vtblPad7  = DrawGouraudPolygon
//   Named exports: Draw2DClippedLine, Draw3DLine, DrawStats, EndFlash,
//                  Exec, Placeholder, PrecacheTexture, SetSceneNode
//=============================================================================
class ENGINE_API URenderDevice : public URenderBase
{
public:
    UViewport*   Viewport;  // associated viewport
    // ======================================================================
    // Pure virtual interface (= vtblPad0-7 in DS9 Engine.dll)
    // ======================================================================
    virtual UBOOL Init(UViewport* Viewport,INT NewX,INT NewY,INT NewColorBytes,UBOOL Fullscreen) = 0; // vtblPad0
    virtual UBOOL SetRes(INT NewX,INT NewY,INT NewColorBytes,UBOOL Fullscreen) = 0;                   // vtblPad1
    virtual void  Exit() = 0;                                                                          // vtblPad2
    virtual void  Flush() = 0;                                                                         // vtblPad3  (ALT_FLUSH: no AllowPrecache param)
    virtual UBOOL Lock(FPlane FlashScale,FPlane FlashFog,FPlane ScreenClear,DWORD RenderLockFlags,BYTE* HitData,INT* HitSize) = 0; // vtblPad4
    virtual void  Unlock(UBOOL Blit) = 0;                                                              // vtblPad5
    virtual void  DrawComplexSurface(FSceneNode* Frame,FSurfaceInfo& Surface,FSurfaceFacet& Facet) = 0; // vtblPad6
    virtual void  DrawGouraudPolygon(FSceneNode* Frame,FTextureInfo& Info,FTransTexture** Pts,INT NumPts,DWORD PolyFlags,FSpanBuffer* Span) = 0; // vtblPad7

    // ======================================================================
    // Non-pure virtuals with default implementations in Engine.dll
    // ======================================================================
    virtual void DrawTile(FSceneNode* Frame,FTextureInfo& Info,FLOAT X,FLOAT Y,FLOAT XL,FLOAT YL,FLOAT U,FLOAT V,FLOAT UL,FLOAT VL,FSpanBuffer* Span,FLOAT Z,FPlane Color,FPlane Fog,DWORD PolyFlags);
    virtual void Draw2DLine(FSceneNode* Frame,FPlane Color,DWORD LineFlags,FVector P1,FVector P2);
    virtual void Draw2DPoint(FSceneNode* Frame,FPlane Color,DWORD LineFlags,FLOAT X1,FLOAT Y1,FLOAT X2,FLOAT Y2,FLOAT Z);
    virtual void ClearZ(FSceneNode* Frame);
    virtual void PushHit(const BYTE* Data,INT Count);
    virtual void PopHit(INT Count,UBOOL bForce);
    virtual void GetStats(TCHAR* Result);
    virtual void ReadPixels(FColor* Pixels);
    virtual void EndFlash();
    virtual void SetSceneNode(FSceneNode* Frame);
    virtual void PrecacheTexture(FTextureInfo& Info,DWORD PolyFlags);
    virtual void DrawStats(FSceneNode* Frame);
    virtual void Draw2DClippedLine(FSceneNode* Frame,FPlane Color,DWORD LineFlags,FVector P1,FVector P2);
    virtual void Draw3DLine(FSceneNode* Frame,FPlane Color,DWORD LineFlags,FVector P1,FVector P2);
    virtual void Placeholder();

    // Optional interface (for fog / gouraud variants — default no-ops)
    virtual void DrawFogSurface(FSceneNode* Frame,FFogSurf& FogSurf){}
    virtual void PreDrawFogSurface(){}
    virtual void PostDrawFogSurface(){}
    virtual void PreDrawGouraud(FSceneNode* Frame,FLOAT FogDistance,FPlane FogColor){}
    virtual void PostDrawGouraud(FLOAT FogDistance){}
    virtual INT  MaxVertices(){ return 0xFFFF; }
    virtual void DrawTriangles(FSceneNode* Frame,FTextureInfo& Info,FTransTexture** Pts,INT NumPts,USHORT* Indices,INT NumIdx,DWORD PolyFlags,FSpanBuffer* Span){}
    virtual UBOOL SupportsTextureFormat(ETextureFormat Fmt){ return 0; }
    virtual void ReadPixels(FColor* Pixels,UBOOL bGammaCorrectOutput){}
    virtual void ShutdownAfterError(){}

    // Config
    UBOOL SpanBased,Fullscreen,SupportsFogMaps,SupportsDistanceFog;
    UBOOL SupportsTC,HighDetailActors,Coronas,ShinySurfaces,VolumetricLighting;
    UBOOL SupportsLazyTextures;
    UBOOL FullscreenOnly;
    UBOOL PrefersDeferredLoad;
    INT   FrameBufferDepth;
    INT   DetailMax;
    UBOOL PrecacheOnFlip;

    virtual void StaticConstructor();
    virtual UBOOL Exec(const ANSICHAR* Cmd,FOutputDevice& Ar);

    static UClass* StaticClass();
    static UClass PrivateStaticClass;
};

//=============================================================================
// URenderIterator – DS9 has this (confirmed export)
//=============================================================================
class ENGINE_API URenderIterator : public UObject
{
public:
    FSceneNode* Observer;  // renamed from APlayerPawn*
    INT          MaxItems;
    INT          Index;

    virtual void   Init(FSceneNode* InFrame);
    virtual void   UnInit() {}
    virtual void   First();
    virtual void   Next();
    virtual UBOOL  IsDone();
    virtual AActor* CurrentItem();

    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// UEngine
//=============================================================================
class ENGINE_API UEngine : public USubsystem
{
public:
    URenderDevice*   GRenDev;
    URenderBase*     Render;
    UClient*         Client;
    ULevel*          GLevel;
    ULevel*          GEntry;
    INT              TickCycles,GameCycles,ClientCycles;
    FLOAT            MaxTickRate;
    INT              CurrentTickRate;
    static UClass* StaticClass();
};

//=============================================================================
// USubsystem (base for UEngine, URenderBase, etc.)
//=============================================================================
//=============================================================================
// FCheckResult / HHitProxy helpers
//=============================================================================
struct FCheckResult
{
    FVector   Location;
    FVector   Normal;
    AActor*   Actor;
    UPrimitive* Primitive;
    FLOAT     Time;
    INT       Item;
    BYTE      BoneName;
    FCheckResult* GetNext() const;
};

struct HHitProxy { DWORD Type; };

//=============================================================================
// Globals (exported from Engine.dll)
//=============================================================================
extern ENGINE_API FName ENGINE_DetailChange;

ENGINE_API class FMovingBrushTrackerBase* GNewBrushTracker(ULevel* Level);

//=============================================================================
// UPlayer (base for UViewport)
//=============================================================================
//=============================================================================
// UFont
//=============================================================================
class ENGINE_API UFont : public UObject
{
public:
    TArray<struct FFontPage> Pages;
    static UClass* StaticClass();
};

//=============================================================================
// EDrawType
//=============================================================================
enum EDrawType
{
    DT_None         = 0,
    DT_Sprite       = 1,
    DT_Mesh         = 2,
    DT_Brush        = 3,
    DT_RopeSegment  = 4,
    DT_SpriteAnimOnce = 5,  // sprite that plays animation once then hides
    DT_Particles    = 6,
    DT_StaticMesh   = 7,
    DT_SkeletalMesh = 8,
    DT_ParticleSystem=9,
    DT_Emitter      = 10,
};

//=============================================================================
// EBlitType (UViewport blit modes)
//=============================================================================
enum EBlitType
{
    BLIT_None         = 0x00,
    BLIT_Direct3D     = 0x08,
    BLIT_HardwarePaint= 0x10,
    BLIT_Fullscreen   = 0x20,
};

//=============================================================================
// ELockRenderFlags
//=============================================================================
enum ELockRenderFlags
{
    LOCKR_ClearScreen  = 1,
    LOCKR_LightDiminish= 2,
};

//=============================================================================
// UViewport additions
//=============================================================================
inline FString& operator+=(FString& A, const FString& B) { return A; }
class ENGINE_API UStructProperty : public UProperty { public: UStructProperty(ECppProperty e, INT o, const TCHAR* Cat, DWORD f, UStruct* s) : UProperty(e,o,Cat,f) {} };
// Show flags (viewport->Actor->ShowFlags)
#define SHOW_Backdrop        0x00000001
#define SHOW_Actors          0x00000002
#define SHOW_PlayerCtrl      0x00000004
#define SHOW_ActorIcons      0x00000008
#define SHOW_Brush           0x00000010
#define SHOW_StandardView    0x00000020
#define SHOW_StaticMeshes    0x00000040
#define SHOW_Volumes         0x00000080

// Draw2DLine / Draw3DLine line flags
#define LINE_None            0x00000000
#define LINE_Transparent     0x00000001
#define LINE_DepthCued       0x00000002

//=============================================================================
// AInventory – base class for all inventory/weapon actors
//=============================================================================
class ENGINE_API AInventory : public AActor
{
public:
    AInventory* Inventory;      // next inventory item
    UMesh*      ThirdPersonMesh;  // 3rd-person weapon mesh
    FLOAT       ThirdPersonScale; // 3rd-person mesh draw scale
    UBOOL       bSteadyFlash3rd;  // steady muzzle flash in 3rd person
    UBOOL       bToggleSteadyFlash;
    UBOOL       bSteadyToggle;
    UMesh*      MuzzleFlashMesh;
    void eventInvCalcView();    // script event for weapon view calc
    static UClass* StaticClass();
};

