#pragma once
// UnRender.h – Deep Space Nine: The Fallen SDK (UE1 build 338)
// Full definitions from Render.dll export analysis.
// Classes: URender, FLightManagerBase, FSpanBuffer (full), FDynamicsCache.

#ifndef RENDER_API
#define RENDER_API __declspec(dllimport)
#endif

//=============================================================================
// Forward declarations
//=============================================================================
class  URender;
class  FLightManagerBase;
struct FRasterSpan;
struct FStampedPoint;
struct FDynamicsCache;
struct FDynamicItem;

//=============================================================================
// FRasterSpan
//=============================================================================
struct FRasterSpan
{
    FLOAT StartX,EndX;
};

//=============================================================================
// FSpanBuffer – full definition from Render.dll export analysis
//=============================================================================
class RENDER_API FSpanBuffer
{
public:
    struct FSpan { SWORD StartX,EndX; FSpan* Next; };

    FSpan**   Index;      // span index array [StartY..EndY]
    FMemStack* MemStack;  // allocator
    INT       StartY;
    INT       EndY;
    INT       ValidLines;
    UBOOL     bFlushed;

    FSpanBuffer(){}
    FSpanBuffer(const FSpanBuffer& Other, FMemStack& Mem);

    void  AllocIndex(SWORD StartY, SWORD EndY, FMemStack* Mem);
    void  AllocIndexForScreen(SWORD StartY, SWORD EndY, FMemStack* Mem);
    void  CopyIndexFrom(const FSpanBuffer& Other, FMemStack* Mem);
    UBOOL CopyFromRaster(FSpanBuffer& Dest, FLOAT StartY, FLOAT EndY, FRasterSpan* Raster);
    UBOOL CopyFromRasterUpdate(FSpanBuffer& Dest, FLOAT StartY, FLOAT EndY, FRasterSpan* Raster);
    void  MergeWith(const FSpanBuffer& Other);
    void  Release();
    void  GetValidRange(SWORD* MinX, SWORD* MaxX);
    UBOOL BoxIsVisible(INT X1,INT Y1,INT X2,INT Y2);
    void  AssertValid(const ANSICHAR* Tag);
    void  AssertEmpty(const ANSICHAR* Tag);
    void  AssertNotEmpty(const ANSICHAR* Tag);
    void  AssertGoodEnough(const ANSICHAR* Tag);
};

//=============================================================================
// FDynamicsCache
//=============================================================================
struct FDynamicsCache
{
    FDynamicItem* Items;
};

//=============================================================================
// FStampedPoint
//=============================================================================
struct FStampedPoint
{
    FVector Point;
    DWORD   Stamp;
};

//=============================================================================
// FScreenBounds (also declared in Engine.h – guard against double-def)
//=============================================================================
#ifndef FSCREENBOUNDS_DEFINED
#define FSCREENBOUNDS_DEFINED
struct FScreenBounds
{
    FLOAT MinX,MinY,MaxX,MaxY,MinZ;
};
#endif

//=============================================================================
// FLightManagerBase
//=============================================================================
class RENDER_API FLightManagerBase
{
public:
    virtual ~FLightManagerBase(){}
};

//=============================================================================
// URender
// Render.dll exports GRender (global URender*), GSceneMem, GDynMem, GLightManager.
// Sizeof(URender) == 184 for DS9 build 338 (matches KHG; verified same UE1 lineage).
//=============================================================================
class RENDER_API URender : public URenderBase
{
public:
    // Static data (exported as global statics from Render.dll)
    static INT*              DynLightLeaves;
    static INT*              DynLightSurfs;
    static INT               NumDynLightLeaves;
    static INT               NumDynLightSurfs;
    static INT               MaxLeafLights;
    static INT               MaxSurfLights;
    static FActorLink**      SurfLights;
    static FVolActorLink**   LeafLights;
    static DWORD             Stamp;
    static FDynamicsCache*   DynamicsCache;
    static FStampedPoint*    PointCache;

    // Virtual methods from Render.dll
    virtual void  Init(UEngine* Engine) override;
    virtual void  Destroy();
    virtual UBOOL Exec(const ANSICHAR* Cmd,FOutputDevice& Ar);
    virtual void  PreRender(FSceneNode* Frame);
    virtual void  PostRender(FSceneNode* Frame);
    virtual void  DrawWorld(FSceneNode* Frame);
    virtual void  DrawActor(FSceneNode* Frame,AActor* Actor);
    virtual void  DrawStats(FSceneNode* Frame);
    virtual void  DrawPaths(FSceneNode* Frame);
    virtual void  DrawBox(FSceneNode* Frame,FPlane Color,DWORD LineFlags,FVector Min,FVector Max);
    virtual void  DrawCircle(FSceneNode* Frame,FPlane Color,DWORD LineFlags,FVector& Origin,FLOAT Radius);
    virtual UBOOL BoundVisible(FSceneNode* Frame,FBox* Box,FSpanBuffer* SpanBuffer,FScreenBounds& Bounds);
    virtual UBOOL Project(FSceneNode* Frame,const FVector& V,FLOAT& ScreenX,FLOAT& ScreenY,FLOAT* Scale);
    virtual UBOOL Deproject(FSceneNode* Frame,INT ScreenX,INT ScreenY,FVector& V);
    virtual void  GetVisibleSurfs(UViewport* Viewport,TArray<INT>& VisibleSurfs);
    virtual FSceneNode* CreateMasterFrame(UViewport* Viewport,FVector CameraLocation,FRotator CameraRotation,FScreenBounds* Bounds);
    virtual FSceneNode* CreateChildFrame(FSceneNode* Frame,FSpanBuffer* SpanBuffer,ULevel* Level,INT iSurf,INT iZone,FLOAT Mirror,const FPlane& NearClip,const FCoords& Coords,FScreenBounds* Bounds);
    virtual void  FinishMasterFrame();
    virtual void  Precache(UViewport* Viewport);
    virtual void  GlobalLighting(UBOOL Realtime,AActor* Owner,FLOAT& Brightness,FPlane& Color);
    virtual AZoneInfo* GetFogZone(ULevel* Level,INT ZoneNum);
    virtual void  SetDistanceClipping(FSceneNode* Frame);
    virtual void  SetDistanceFogging(FSceneNode* Frame);

    // Non-virtual utility methods
    void OccludeBsp(FSceneNode* Frame);
    void OccludeFrame(FSceneNode* Frame);
    void DrawFrame(FSceneNode* Frame);
    void SetupDynamics(FSceneNode* Frame,AActor* Exclude);
    FDynamicItem*& Dynamic(INT iActor,INT IsDecoration);
    void LeafVolumetricLighting(FSceneNode* Frame,UModel* Model,INT iLeaf);
    INT  ClipBspSurf(INT iNode,FTransTexture**& Pts);
    INT  ClipDecal(FSceneNode* Frame,FDecal* Decal,UModel* Model,FBspSurf* Surf,FSavedPoly* Poly,FTransTexture**& Pts,INT& NumPts);
    void CleanupProjectedShadow(FDynamicSprite* Sprite);
    void DrawActorSprite(FSceneNode* Frame,FDynamicSprite* Sprite);
    void DrawMesh(FDynamicSprite* Sprite,FSceneNode* Frame,AActor* Owner,AActor* Base,FSpanBuffer* SpanBuffer,AZoneInfo* Zone,const FCoords& Coords,FVolActorLink* Volumetrics,FActorLink* Dynamics,DWORD ExtraFlags);
    void DrawLodMesh(FDynamicSprite* Sprite,FSceneNode* Frame,AActor* Owner,AActor* Base,FSpanBuffer* SpanBuffer,AZoneInfo* Zone,const FCoords& Coords,FVolActorLink* Volumetrics,FActorLink* Dynamics,DWORD ExtraFlags);
    void DrawProjectedShadow(FDynamicSprite* Sprite,FSceneNode* Frame);
    void PrepareProjectedShadow(FDynamicSprite* Sprite,FSceneNode* Frame,FTransTexture* Pts,FMeshWedge* Wedge);
    void VARARGS ShowStat(FSceneNode* Frame,const ANSICHAR* Fmt,...);

    static void StaticConstructor(UClass* clazz);
    static UClass* StaticClass();
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// Globals (exported from Render.dll)
//=============================================================================
extern RENDER_API URender*          GRender;    // ?GRender@@3PAVURender@@A
extern RENDER_API FMemStack         GSceneMem;  // ?GSceneMem@@3VFMemStack@@A
extern RENDER_API FMemStack         GDynMem;    // ?GDynMem@@3VFMemStack@@A
extern RENDER_API FLightManagerBase* GLightManager; // ?GLightManager@@3PAVFLightManagerBase@@A

//=============================================================================
// FMeshWedge – used in PrepareProjectedShadow
//=============================================================================
#ifndef FMESHWEDGE_DEFINED
#define FMESHWEDGE_DEFINED
struct FMeshWedge
{
    WORD  iVertex;
    FLOAT U,V;
};
#endif

