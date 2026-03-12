#pragma once


// UE1 placement New
#define New(Mem,Class) new(Mem)Class
// Assert macro
#ifndef check
#define check(expr) { if(!(expr)) {} }
#endif
// Core.h – Deep Space Nine: The Fallen SDK (UE1 build 338)
// Generated from Core.dll export analysis + standard UE1 sources.

#ifndef CORE_API
#define CORE_API __declspec(dllimport)
#endif

#pragma warning(disable:4244 4251 4275 4291 4324 4355 4996)

#include <stddef.h>   // offsetof
#include <string.h>   // strlen, memcpy …
#include <math.h>     // sqrtf etc.
#include <float.h>

//=============================================================================
// Primitive types
//=============================================================================
typedef unsigned char      BYTE;
typedef unsigned short     WORD;
typedef unsigned long      DWORD;
typedef int                INT;
typedef unsigned int       UINT;
typedef float              FLOAT;
typedef double             DOUBLE;
typedef long long          SQWORD;
typedef unsigned long long QWORD;
typedef int                UBOOL;
typedef char               ANSICHAR;
typedef unsigned short     USHORT;
typedef short              SWORD;

// DS9 is a non-Unicode build; TCHAR == char.
// Guard so we don't conflict if windows.h or the VS project already defines it.
#ifndef TCHAR
typedef ANSICHAR TCHAR;
#endif
#ifndef TEXT
#define TEXT(s) s
#endif
#ifndef TCHAR_TO_ANSI
#define TCHAR_TO_ANSI(s) (s)
#define ANSI_TO_TCHAR(s) (s)
#endif

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

// Calling conventions — guard against Windows SDK redefinitions
#ifndef FASTCALL
#define FASTCALL __fastcall
#endif
#ifndef VARARGS
#define VARARGS  __cdecl
#endif
#ifndef CDECL
#define CDECL    __cdecl
#endif
#ifndef STDCALL
#define STDCALL  __stdcall
#endif
#ifndef DLLIMPORT
#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)
#endif

//=============================================================================
// Utility macros
//=============================================================================
#define ARRAY_COUNT(a)     (sizeof(a)/sizeof((a)[0]))
#define STRUCT_OFFSET(s,m) ((INT)(size_t)&((s*)0)->m)
#define appErrorf(...)     appError(appSprintf(__VA_ARGS__))

// NOTE: CPP_PROPERTY is defined in D3D9RenderDevice.h (needs UD3D9RenderDevice).

//=============================================================================
// Guard macros
//=============================================================================
#define guard(f)         { static const char __FUNC_NAME__[] = #f; try {
#define unguard          } catch(...) { throw; } }
#define unguardf(m)      } catch(...) { throw; } }
#define guardSlow(f)     guard(f)
#define unguardSlow      unguard
#define unguardfSlow(m)  unguardf(m)

//=============================================================================
// Forward declarations (needed before any class body)
//=============================================================================
class FArchive;
class FOutputDevice;
class FExec;
class FName;
class FString;
class UObject;
class UClass;
class UField;
class UStruct;
class UFunction;
class UState;
class UPackage;
class USubsystem;
class ULinkerLoad;
class FConfigCache;
class FMemStack;
class FFileManager;
struct FFrame;
struct FNameEntry;

//=============================================================================
// EName – well-known names
//=============================================================================
enum EName
{
    NAME_None=0, NAME_All=4,
    NAME_DevNet=7, NAME_DevPath=8, NAME_DevNetTraffic=9, NAME_DevAudio=10,
    NAME_DevLoad=11, NAME_DevSave=12, NAME_DevGarbage=13, NAME_DevKill=14,
    NAME_DevReplace=15, NAME_DevUI=16, NAME_DevSound=17, NAME_DevCompile=18,
    NAME_DevBind=19, NAME_DevBsp=20, NAME_DevGame=21, NAME_DevAnim=22,
    NAME_DevExec=23, NAME_DevFilter=24, NAME_DevStats=25, NAME_DevTick=26,
    NAME_Object=100, NAME_Actor=101, NAME_Class=102, NAME_Function=106,
    NAME_Package=107, NAME_Struct=109, NAME_State=112,
    NAME_Log=200, NAME_Init=201, NAME_Exit=202, NAME_Cmd=203,
    NAME_Warning=205, NAME_Error=210, NAME_Critical=208,
    NAME_Destroyed=300, NAME_Config=400, NAME_Input=401,
    NAME_COUNT,
};
enum EFindName { FNAME_Find, FNAME_Add, FNAME_Intrinsic };
enum EInternal { EC_InternalUseOnly };

//=============================================================================
// FNameEntry / FName
// IMPORTANT: methods inside a CORE_API class must NOT carry CORE_API
// themselves — MSVC C2487 fires when dllimport is double-applied.
//=============================================================================
//=============================================================================
// FArray / TArray
//=============================================================================
class CORE_API FArray
{
public:
    void* Data;
    INT   ArrayNum;
    INT   ArrayMax;

    FArray() : Data(nullptr), ArrayNum(0), ArrayMax(0) {}
    INT  Num()              const { return ArrayNum; }
    bool IsValidIndex(INT i) const { return i >= 0 && i < ArrayNum; }
    void*       GetData()         { return Data; }
    const void* GetData()   const { return Data; }

    void Add(INT n, INT ElementSize);
    void AddZeroed(INT n, INT ElementSize);
    void Insert(INT i, INT n, INT ElementSize);
    void InsertZeroed(INT i, INT n, INT ElementSize);
    void Remove(INT i, INT n, INT ElementSize);
    void Empty(INT Slack, INT ElementSize);
    void Shrink(INT ElementSize);
    void Realloc(INT ElementSize);
    INT  CountBytes(FArchive& Ar, INT ElementSize);
};

template<class T>
class TArray : public FArray
{
public:
    TArray()  { Data = nullptr; ArrayNum = ArrayMax = 0; }
    ~TArray() {}

    T&       operator()(INT i)         { return ((T*)Data)[i]; }
    const T& operator()(INT i)   const { return ((const T*)Data)[i]; }
    T&       Last(INT c = 0)           { return ((T*)Data)[ArrayNum-1-c]; }

    INT  Add(INT n = 1)          { INT i = ArrayNum; FArray::Add(n, sizeof(T)); return i; }
    INT  AddZeroed(INT n = 1)    { INT i = ArrayNum; FArray::AddZeroed(n, sizeof(T)); return i; }
    INT  AddItem(const T& t)     { INT i = Add(); (*this)(i) = t; return i; }
    void Remove(INT i, INT n=1)  { FArray::Remove(i, n, sizeof(T)); }
    void Empty(INT Slack = 0)    { FArray::Empty(Slack, sizeof(T)); }
    void Shrink()                { FArray::Shrink(sizeof(T)); }

    T*       begin()       { return (T*)Data; }
    T*       end()         { return (T*)Data + ArrayNum; }
    const T* begin() const { return (const T*)Data; }
    const T* end()   const { return (const T*)Data + ArrayNum; }
};

template<class K, class V> class TMap {};
template<class K, class V> class TMultiMap {};

struct FNameEntry { INT Index; DWORD Flags; ANSICHAR Name[1]; };

class CORE_API FName
{
public:
    INT Index;

    static TArray<FNameEntry*> Names;
    static TArray<INT>         Available;
    static INT                 Initialized;

    FName()                            : Index(NAME_None) {}
    FName(EName E)                     : Index(E) {}
    FName(const ANSICHAR* N, EFindName FT = FNAME_Add);

    static void        StaticInit();
    static void        StaticExit();
    static FNameEntry* GetEntry(INT i);
    static INT         GetMaxNames();
    static INT         GetInitialized();
    static void        Hardcode(FNameEntry* AutoName);
    static const ANSICHAR* SafeString(EName E);
    static INT         SafeSuppressed(EName E);

    INT   GetIndex() const { return Index; }
    DWORD GetFlags() const;
    void  SetFlags(DWORD F) const;
    void  ClearFlags(DWORD F) const;
    INT   IsValid() const;

    const ANSICHAR* operator*() const
    {
        FNameEntry* E = (Index >= 0 && Index < Names.Num()) ? Names(Index) : nullptr;
        return E ? E->Name : "None";
    }
    bool operator==(FName O)  const { return Index == O.Index; }
    bool operator!=(FName O)  const { return Index != O.Index; }
    bool operator==(EName E2) const { return Index == (INT)E2; }
};


//=============================================================================
// FString
//=============================================================================
class CORE_API FString
{
public:
    TArray<ANSICHAR> Data;

    FString() {}
    FString(const ANSICHAR* s)
    {
        if (s) { INT l = (INT)strlen(s)+1; Data.Add(l); memcpy(Data.GetData(), s, l); }
    }

    bool operator==(const FString& O) const { return true; }
    bool operator!=(const FString& O) const { return false; }
    FString& operator+=(const ANSICHAR* s) { return *this; }
    FString& operator+=(const FString& s)  { return *this; }
    const ANSICHAR* operator*() const { return Data.Num() ? (ANSICHAR*)Data.GetData() : ""; }
    INT  Len()     const { return Data.Num() ? Data.Num()-1 : 0; }
    bool IsEmpty() const { return Len() == 0; }

    FString Left(INT n)  const;
    FString Right(INT n) const;
    FString Mid(INT s, INT n = 0x7fffffff) const;
    INT     InStr(const FString& S, INT FromEnd = 0) const;
    INT     InStr(const ANSICHAR* S, INT FromEnd = 0) const;
    FString Caps()  const;
    FString Locs()  const;
    void    Empty();
    void    Shrink();
    INT     Split(const FString& In, FString* L, FString* R, INT Search = 0) const;
    static FString Printf(const ANSICHAR* Fmt, ...);
    TArray<ANSICHAR>& GetCharArray() { return Data; }
};

//=============================================================================
// Math types
// Note: methods inside CORE_API structs/classes must NOT carry CORE_API.
//=============================================================================
struct CORE_API FVector
{
    // X/Y/Z with R/G/B aliases
    union { struct { FLOAT X,Y,Z; }; struct { FLOAT R,G,B; }; };
    FVector() {}
    FVector(FLOAT x, FLOAT y, FLOAT z) : X(x), Y(y), Z(z) {}

    FVector operator+(const FVector& v) const { return {X+v.X, Y+v.Y, Z+v.Z}; }
    FVector operator-(const FVector& v) const { return {X-v.X, Y-v.Y, Z-v.Z}; }
    FVector operator*(FLOAT f)          const { return {X*f,   Y*f,   Z*f};   }
    FVector operator/(FLOAT f)          const { FLOAT r=1.f/f; return {X*r,Y*r,Z*r}; }
    FVector operator*(const FVector& v) const { return {X*v.X, Y*v.Y, Z*v.Z}; }
    FVector operator-()                 const { return {-X,-Y,-Z}; }
    FVector& operator+=(const FVector& v){ X+=v.X; Y+=v.Y; Z+=v.Z; return *this; }
    FVector& operator-=(const FVector& v){ X-=v.X; Y-=v.Y; Z-=v.Z; return *this; }
    FVector& operator*=(FLOAT f)         { X*=f; Y*=f; Z*=f; return *this; }
    FVector& operator/=(FLOAT f)         { FLOAT r=1.f/f; X*=r; Y*=r; Z*=r; return *this; }
    bool operator==(const FVector& v)const{ return X==v.X&&Y==v.Y&&Z==v.Z; }
    bool operator!=(const FVector& v)const{ return !(*this==v); }
    FLOAT operator|(const FVector& v) const { return X*v.X+Y*v.Y+Z*v.Z; }  // dot
    FVector operator^(const FVector& v) const                               // cross
        { return {Y*v.Z-Z*v.Y, Z*v.X-X*v.Z, X*v.Y-Y*v.X}; }

    FLOAT SizeSquared()   const { return X*X+Y*Y+Z*Z; }
    FLOAT SizeSquared2D() const { return X*X+Y*Y; }
    bool  IsZero()        const { return X==0.f && Y==0.f && Z==0.f; }
    FLOAT& Component(INT i) { return (&X)[i]; }

    // Bodies defined after appSqrt/appFabs are available (below)
    FLOAT  Size()          const;
    FLOAT  Size2D()        const;
    bool   IsNearlyZero()  const;

    FLOAT  Normalize();
    FVector SafeNormal()   const;
    FVector UnsafeNormal() const;
    void    AddBounded(const FVector& V, FLOAT Radius);
    FVector BoundToCube(FLOAT R);
    FVector GridSnap(const FVector& Grid) const;
    FVector Projection() const;
    struct FRotator Rotation();
    void FindBestAxisVectors(FVector& A1, FVector& A2) const;
    FVector TransformPointBy(const struct FCoords& C)   const;
    FVector TransformVectorBy(const struct FCoords& C)  const;
    FVector MirrorByPlane(const struct FPlane& P)       const;
    FVector MirrorByVector(const FVector& N)            const;
    FVector C3DTransformPointBy(const struct FCoords& C) const;
};

struct CORE_API FPlane : public FVector
{
    FLOAT W;
    FPlane() {}
    FPlane(const FVector& N, FLOAT d)           : FVector(N),        W(d) {}
    FPlane(FLOAT x, FLOAT y, FLOAT z, FLOAT w) : FVector(x,y,z),    W(w) {}
    FLOAT PlaneDot(const FVector& P) const { return X*P.X+Y*P.Y+Z*P.Z-W; }
    FPlane Flip() const { return {-X,-Y,-Z,-W}; }
    FPlane TransformPlaneByOrtho(const struct FCoords& C) const;
};

struct CORE_API FRotator
{
    INT Pitch, Yaw, Roll;
    FRotator() {}
    FRotator(INT p, INT y, INT r) : Pitch(p), Yaw(y), Roll(r) {}
    bool IsZero() const { return Pitch==0 && Yaw==0 && Roll==0; }
    FVector  Vector();
    FRotator Add(INT dp, INT dy, INT dr);
    FRotator AddBounded(INT dp, INT dy, INT dr);
    FRotator Reduce() const;
    FRotator GridSnap(const FRotator& G) const;
};

struct CORE_API FCoords
{
    FCoords& operator/=(const FLOAT S) { XAxis/=S; YAxis/=S; ZAxis/=S; return *this; }
    FCoords& operator*=(const FLOAT S) { XAxis*=S; YAxis*=S; ZAxis*=S; return *this; }
    FCoords& operator/=(const FRotator& R);   // unrotate (inline impl below)
    FCoords& operator*=(const FCoords& C);    // compose frames (inline impl below)
    FVector Origin, XAxis, YAxis, ZAxis;
    FCoords() {}
    FCoords(const FVector& O, const FVector& X, const FVector& Y, const FVector& Z)
        : Origin(O), XAxis(X), YAxis(Y), ZAxis(Z) {}
    explicit FCoords(const FVector& O)
        : Origin(O), XAxis(1,0,0), YAxis(0,1,0), ZAxis(0,0,1) {}
    FCoords  Inverse()        const;
    FCoords  Transpose()      const;
    FCoords  MirrorByPlane(const FPlane& P)   const;
    FCoords  MirrorByVector(const FVector& V) const;
    FRotator OrthoRotation()  const;
};

struct FModelCoords
{
    FCoords PointXform, VectorXform;
    FModelCoords() {}
    FModelCoords(const FCoords& P, const FCoords& V) : PointXform(P), VectorXform(V) {}
};

struct CORE_API FQuat   { FLOAT X,Y,Z,W; FQuat(){} FQuat(FLOAT x,FLOAT y,FLOAT z,FLOAT w):X(x),Y(y),Z(z),W(w){} };
struct CORE_API FMatrix { FLOAT M[4][4]; FMatrix(){} };

struct CORE_API FBox
{
    FVector Min, Max;
    BYTE    IsValid;
    FBox() : IsValid(0) {}
    FBox(const FVector& mn, const FVector& mx) : Min(mn), Max(mx), IsValid(1) {}

    FBox& operator+=(const FVector& V)
    {
        if (!IsValid) { Min = Max = V; IsValid = 1; }
        else
        {
            // Use ternary to avoid relying on ::min/::max from <algorithm>
            Min.X = V.X < Min.X ? V.X : Min.X;
            Min.Y = V.Y < Min.Y ? V.Y : Min.Y;
            Min.Z = V.Z < Min.Z ? V.Z : Min.Z;
            Max.X = V.X > Max.X ? V.X : Max.X;
            Max.Y = V.Y > Max.Y ? V.Y : Max.Y;
            Max.Z = V.Z > Max.Z ? V.Z : Max.Z;
        }
        return *this;
    }
    FBox operator+(const FVector& V) const { FBox r=*this; r+=V; return r; }
    FBox operator+(const FBox& B)    const { FBox r=*this; if(B.IsValid){r+=B.Min;r+=B.Max;} return r; }
    FVector&       GetExtrema(INT i)       { return (&Min)[i]; }
    const FVector& GetExtrema(INT i) const { return (&Min)[i]; }
    FBox ExpandBy(FLOAT W)   const;
    FBox TransformBy(const FCoords& C) const;
};

struct CORE_API FSphere : public FPlane
{
    FSphere() {}
    FSphere(const FVector& O, FLOAT r) : FPlane(O, r) {}
};

struct CORE_API FColor
{
    BYTE R, G, B, A;
    DWORD TrueColor() const { return ((DWORD)R<<16)|((DWORD)G<<8)|B; }
    FColor() {}
    FColor(BYTE r, BYTE g, BYTE b)         : R(r), G(g), B(b), A(255) {}
    FColor(BYTE r, BYTE g, BYTE b, BYTE a) : R(r), G(g), B(b), A(a)   {}
    explicit FColor(const FPlane& P);
    bool operator==(const FColor& C) const { return R==C.R&&G==C.G&&B==C.B&&A==C.A; }
    bool operator!=(const FColor& C) const { return !(*this==C); }
    FColor Brighten(INT Amount);
    INT    Brightness()  const;
    FLOAT  FBrightness() const;
    WORD   HiColor555()  const;
    WORD   HiColor565()  const;
};

struct CORE_API FGuid
{
    DWORD A, B, C, D;
    FGuid() : A(0), B(0), C(0), D(0) {}
    FGuid(DWORD a, DWORD b, DWORD c, DWORD d) : A(a), B(b), C(c), D(d) {}
    bool operator==(const FGuid& O) const { return A==O.A&&B==O.B&&C==O.C&&D==O.D; }
    bool operator!=(const FGuid& O) const { return !(*this==O); }
    ANSICHAR* String() const;
};

//=============================================================================
// FMemStack / FMemMark
//=============================================================================
struct FTaggedMemory { BYTE* Next; };

class CORE_API FMemStack
{
public:
    BYTE* Push(INT AllocSize, INT Align=4);
public:
    BYTE*          Top;
    BYTE*          End;
    FTaggedMemory* TopChunk;
    FTaggedMemory* UnusedChunks;

    FMemStack() : Top(nullptr), End(nullptr), TopChunk(nullptr), UnusedChunks(nullptr) {}
    void  Init(INT DefaultChunkSize);
    void  Exit();
    BYTE* PushBytes(INT AllocSize, INT Align);
    void  Tick();
    void  Purge();
    INT   GetByteCount()     const;
    INT   GetAllocatedSize() const;

    template<class T>
    T* PushArray(INT Count) { return (T*)PushBytes(Count*sizeof(T), alignof(T)); }
};

class CORE_API FMemMark
{
public:
    FMemStack*     MemPtr;   // pointer so move is possible
    BYTE*          Top;
    FTaggedMemory* SavedChunk;
    FMemMark() : MemPtr(nullptr), Top(nullptr), SavedChunk(nullptr) {}
    FMemMark(FMemStack& InMem);
    FMemMark(FMemMark&& O) : MemPtr(O.MemPtr), Top(O.Top), SavedChunk(O.SavedChunk) { O.MemPtr=nullptr; }
    FMemMark& operator=(FMemMark&& O) { MemPtr=O.MemPtr; Top=O.Top; SavedChunk=O.SavedChunk; O.MemPtr=nullptr; return *this; }
    void Pop();
    // Legacy ref accessor for code that uses Mem
    FMemStack& GetMem() { return *MemPtr; }
};

// Legacy: some code uses mark.Mem directly — provide compatibility via alias
#define FMemMark_Mem (*MemPtr)


//=============================================================================
// FArchive
//=============================================================================
class FLazyLoader;
class CORE_API FArchive
{
public:
    UBOOL ArIsLoading, ArIsSaving, ArIsTrans, ArIsNet, ArIsPersistent;
    INT   ArVer, ArNetVer;
    UBOOL ArForEdit, ArForClient, ArForServer, ArIsError;

    FArchive()
        : ArIsLoading(0), ArIsSaving(0), ArIsTrans(0), ArIsNet(0),
          ArIsPersistent(0), ArVer(0), ArNetVer(0),
          ArForEdit(1), ArForClient(1), ArForServer(1), ArIsError(0) {}
    virtual ~FArchive() {}

    virtual void  Serialize(void* V, INT L)             {}
    virtual void  SerializeBits(void* V, INT Bits)      {}
    virtual void  SerializeInt(DWORD& V, DWORD Max)     {}
    virtual INT   Tell()                    { return -1; }
    virtual INT   TotalSize()               { return -1; }
    virtual void  Seek(INT Pos)             {}
    virtual UBOOL AtEnd()                   { return 0;  }
    virtual void  Flush()                   {}
    virtual UBOOL Close()                   { return !ArIsError; }
    virtual UBOOL GetError()                { return ArIsError;  }
    virtual INT   GetLibVersion()           { return 0; }
    virtual void  SetLibVersion(INT)        {}
    virtual void  Precache(INT Hint)        {}
    virtual void  Preload(UObject* O)       {}
    virtual void  CountBytes(DWORD,DWORD)   {}
    virtual INT   MapName(FName*)           { return 0; }
    virtual INT   MapObject(UObject*)       { return 0; }
    virtual void  AttachLazyLoader(FLazyLoader*) {}
    virtual void  DetachLazyLoader(FLazyLoader*) {}
    virtual void  String(ANSICHAR* S, INT MaxLen) {}

    void ByteOrderSerialize(void* V, INT Length);

    UBOOL IsLoading()    const { return ArIsLoading;    }
    UBOOL IsSaving()     const { return ArIsSaving;     }
    UBOOL IsNet()        const { return ArIsNet;        }
    UBOOL IsError()      const { return ArIsError;      }
    UBOOL IsPersistent() const { return ArIsPersistent; }
    UBOOL IsTrans()      const { return ArIsTrans;      }
    UBOOL ForClient()    const { return ArForClient;    }
    UBOOL ForServer()    const { return ArForServer;    }
    UBOOL ForEdit()      const { return ArForEdit;      }
    INT   Ver()          const { return ArVer;          }
    INT   NetVer()       const { return ArNetVer;       }

    FArchive& operator<<(BYTE&   B) { Serialize(&B,1); return *this; }
    FArchive& operator<<(WORD&   W) { Serialize(&W,2); return *this; }
    FArchive& operator<<(DWORD&  D) { Serialize(&D,4); return *this; }
    FArchive& operator<<(INT&    I) { Serialize(&I,4); return *this; }
    FArchive& operator<<(FLOAT&  F) { Serialize(&F,4); return *this; }
    FArchive& operator<<(DOUBLE& D) { Serialize(&D,8); return *this; }
    FArchive& operator<<(FName&  N);
    FArchive& operator<<(UObject*& O);
    FArchive& operator<<(FVector& V) { return *this << V.X << V.Y << V.Z; }
};

//=============================================================================
// FOutputDevice / FExec
//=============================================================================
class CORE_API FOutputDevice
{
public:
    virtual void Serialize(const ANSICHAR* Data, EName MsgType) = 0;
    virtual ~FOutputDevice() {}
    void Log(EName MsgType, const ANSICHAR* Str);
    void Log(const ANSICHAR* Str);
    void Log(const FString& Str);
    void Log(EName MsgType, const FString& Str);
    void VARARGS Logf(EName MsgType, const ANSICHAR* Fmt, ...);
    void VARARGS Logf(const ANSICHAR* Fmt, ...);
};

class CORE_API FExec
{
public:
    virtual UBOOL Exec(const ANSICHAR* Cmd, FOutputDevice& Ar) { return 0; }
    virtual ~FExec() {}
};

//=============================================================================
// Object flags
//=============================================================================
typedef DWORD EObjectFlags;
#define RF_Transactional  0x00000001
#define RF_Unreachable    0x00000002
#define RF_Public         0x00000004
#define RF_TagImp         0x00000008
#define RF_TagExp         0x00000010
#define RF_Transient      0x00004000
#define RF_Standalone     0x00080000
#define RF_Native         0x04000000
#define RF_Marked         0x08000000
#define RF_NoFlags        0x00000000
#define RF_AllFlags       0xFFFFFFFF
// Class flags
typedef DWORD EClassFlags;
#define CLASS_Abstract    0x00000001
#define CLASS_Compiled    0x00000002
#define CLASS_Config      0x00000004
#define CLASS_Transient   0x00000008
#define CLASS_NoExport    0x00000100
#define CLASS_Placeable   0x00000200

//=============================================================================
// UObject
// Rule: virtual functions are fine; non-virtual methods inside CORE_API class
// must NOT carry CORE_API (would double-annotate __declspec(dllimport)).
//=============================================================================
class CORE_API UObject : public FExec
{
public:
    // Virtual table — must match Core.dll vtable order exactly
    virtual void  ProcessEvent(class UFunction* F, void* P, void* R = nullptr);
    virtual void  ProcessState(FLOAT dt);
    virtual UBOOL ProcessRemoteFunction(class UFunction* F, void* P, struct FFrame* S);
    virtual void  Modify();
    virtual void  PostLoad();
    virtual void  Destroy();
    virtual void  Serialize(FArchive& Ar);
    virtual UBOOL IsPendingKill()  { return 0; }
    virtual FString GetStateName() const;
    virtual void  InitExecution();
    virtual void  ShutdownAfterError();
    virtual void  PostEditChange();
    virtual void  CallFunction(struct FFrame& Stack, void* Result, class UFunction* F);
    virtual UBOOL ScriptConsoleExec(const ANSICHAR* Cmd, FOutputDevice& Ar, UObject* Exec);
    virtual void  Register();
    virtual void  LanguageChange();
    virtual void  IncrementLogs()    {}
    virtual void  IncrementWarnings() {}
    virtual UBOOL Exec(const ANSICHAR* Cmd, FOutputDevice& Ar);

    // Object data layout
    EObjectFlags ObjectFlags;
    INT          Index;
    UObject*     HashNext;
    struct FStateFrame* StateFrame;
    ULinkerLoad* _Linker;
    INT          _LinkerIndex;
    UObject*     Outer;
    DWORD        _Pad0;
    FName        Name;
    UClass*      Class;

    // Static data
    static TArray<UObject*>  GObjObjects;
    static TArray<INT>       GObjAvailable;
    static TArray<UObject*>  GObjLoaded;
    static TArray<UObject*>  GObjRoot;
    static TArray<UObject*>  GObjLoaders;
    static UPackage*         GObjTransientPkg;
    static ANSICHAR          GObjCachedLanguage[32];
    static ANSICHAR          GLanguage[64];
    static INT               GObjInitialized;
    static INT               GObjNoRegister;
    static INT               GObjBeginLoadCount;
    static INT               GObjRegisterCount;

    // Static methods — these are free-standing, so no CORE_API issue:
    // They are exported from Core.dll and called as regular function pointers,
    // BUT since they're inside a CORE_API class, the class annotation covers them.
    // We just declare without redundant CORE_API here.
    static UObject*    StaticAllocateObject(UClass*, UObject*, FName, EObjectFlags, UObject* = nullptr, FOutputDevice* = nullptr, UObject* = nullptr);
    static UObject*    StaticConstructObject(UClass*, UObject*, FName, EObjectFlags, UObject* = nullptr, FOutputDevice* = nullptr);
    static UObject*    StaticFindObject(UClass*, UObject*, const ANSICHAR*, UBOOL = 0);
    static UObject*    StaticFindObjectChecked(UClass*, UObject*, const ANSICHAR*, UBOOL = 0);
    static UObject*    StaticLoadObject(UClass*, UObject*, const ANSICHAR*, const ANSICHAR*, DWORD, class UPackageMap*);
    static UClass*     StaticLoadClass(UClass*, UObject*, const ANSICHAR*, const ANSICHAR*, DWORD, class UPackageMap*);
    static void        StaticInit();
    static void        StaticExit();
    static void        StaticTick();
    static void        StaticShutdownAfterError();
    static void        CollectGarbage(EObjectFlags KeepFlags);
    static void        BeginLoad();
    static void        EndLoad();
    static void        ProcessRegistrants();
    static INT         GetInitialized();
    static UObject*    GetIndexedObject(INT i);
    static UPackage*   GetTransientPackage();
    static UPackage*   CreatePackage(UObject* InOuter, const ANSICHAR* PkgName);
    static UPackage*   LoadPackage(UObject* InOuter, const ANSICHAR* Filename, DWORD LoadFlags);
    static ULinkerLoad* GetPackageLinker(UObject*, const ANSICHAR*, DWORD, class UPackageMap*, FGuid*);
    static void        ResetLoaders(UObject*, UBOOL, UBOOL);
    static const ANSICHAR* GetLanguage();
    static void        SetLanguage(const ANSICHAR*);
    static UBOOL       StaticExec(const ANSICHAR* Cmd, FOutputDevice& Ar);
    static UClass*     StaticClass();
    static const ANSICHAR* StaticConfigName() { return "Object"; }

    // Non-virtual instance methods
    void AddToRoot();
    void RemoveFromRoot();
    void SetFlags(EObjectFlags F)    { ObjectFlags |= F; }
    void ClearFlags(EObjectFlags F)  { ObjectFlags &= ~F; }
    EObjectFlags GetFlags()    const { return ObjectFlags; }
    INT          GetIndex()    const { return Index; }
    UClass*      GetClass()    const { return Class; }
    UObject*     GetOuter()    const { return Outer; }
    FName        GetFName()    const { return Name; }
    const ANSICHAR* GetName()  const { return *Name; }
    const ANSICHAR* GetFullName(ANSICHAR* Buf = nullptr) const;
    const ANSICHAR* GetPathName(UObject* StopOuter = nullptr, ANSICHAR* Buf = nullptr) const;
    UBOOL IsA(UClass* SomeBase)   const;
    UBOOL IsIn(UObject* SomeOuter) const;
    UBOOL IsValid() const;
    ULinkerLoad* GetLinker();
    INT   GetLinkerIndex();
    void  LoadConfig(UBOOL Propagate = 0, UClass* Cls = nullptr, const ANSICHAR* ForceSec = nullptr);
    void  SaveConfig(EObjectFlags Flags = RF_Public, const ANSICHAR* Filename = nullptr);
    void  LoadLocalized(UBOOL Propagate = 0, UClass* Cls = nullptr);
    void  ParseParms(const ANSICHAR* Parms);
    void  eventBeginState();
    void  eventEndState();
    void  VARARGS debugs(INT Depth, const ANSICHAR* Fmt, ...);
};

//=============================================================================
// DECLARE_CLASS macros — DS9 uses ALT form (3 args, no package)
//=============================================================================
#define DECLARE_BASE_CLASS(TClass,TSuperClass,TStaticFlags) \
public: \
    typedef TSuperClass Super; \
    typedef TClass ThisClass; \
    static UClass* StaticClass(); \
    static const ANSICHAR* StaticConfigName(); \
    void* operator new(size_t, UObject* Outer, FName Name=NAME_None, EObjectFlags Flags=RF_NoFlags) \
        { return StaticAllocateObject(StaticClass(),Outer,Name,Flags); } \
    void* operator new(size_t, EInternal*) { return nullptr; } \
    static void InternalConstructor(void* X) { new((EInternal*)X) TClass(); } \
    static UClass PrivateStaticClass;

#define DECLARE_CLASS(TClass,TSuperClass,TStaticFlags) \
    DECLARE_BASE_CLASS(TClass,TSuperClass,TStaticFlags)


//=============================================================================
// Implementation macros
//=============================================================================
#define IMPLEMENT_CLASS(TClass)     UClass TClass::PrivateStaticClass;     UClass* TClass::StaticClass() { return &PrivateStaticClass; }     const ANSICHAR* TClass::StaticConfigName() { return #TClass; }

#define IMPLEMENT_PACKAGE(pkg)     extern "C" { int pkg##_impl = 0; }

// Helpers
template<class T>
T* ConstructObject(UClass* Cls, UObject* Outer, FName Name = NAME_None, EObjectFlags Flags = RF_NoFlags)
{ return (T*)UObject::StaticConstructObject(Cls,Outer,Name,Flags); }

template<class T>
T* FindObject(UObject* Outer, const ANSICHAR* Name, UBOOL ExactClass = 0)
{ return (T*)UObject::StaticFindObject(T::StaticClass(),Outer,Name,ExactClass); }

template<class T>
T* Cast(UObject* O) { return O && O->IsA(T::StaticClass()) ? (T*)O : nullptr; }

//=============================================================================
// UField / UStruct / UState / UClass / UFunction / UPackage
//=============================================================================
class CORE_API UField : public UObject
{
public:
    UField* Next;
    UField* HashNext;
    static UClass* StaticClass();
    virtual void  AddCppProperty(class UProperty* P) {}
    virtual void  Bind() {}
    virtual UClass* GetOwnerClass() { return nullptr; }
    virtual INT   GetPropertiesSize() { return 0; }
    virtual UBOOL MergeBools() { return 0; }
    virtual void  PostLoad()  override;
    virtual void  Register()  override;
    virtual void  Serialize(FArchive& Ar) override;
    static void   InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

class CORE_API UStruct : public UField
{
public:
    UField*            Children;
    BYTE*              Script;
    INT                ScriptSize, TextPos, Line, PropertiesSize;
    class UTextBuffer* ScriptText;
    UField*            FriendlyNext;
    static UClass* StaticClass();
    virtual void  Serialize(FArchive& Ar) override;
    virtual void  Destroy() override;
    virtual UStruct* GetInheritanceSuper() { return nullptr; }
    virtual ANSICHAR* GetNameCPP();
    virtual INT  GetPropertiesSize() override { return PropertiesSize; }
    virtual void SetPropertiesSize(INT S) { PropertiesSize = S; }
    virtual void Link(FArchive& Ar, UBOOL Props);
    virtual void AddCppProperty(class UProperty* P) override;
    virtual void CleanupDestroyed(BYTE* Mem) {}
    virtual void SerializeBin(FArchive& Ar, BYTE* Data) {}
    virtual void SerializeTaggedProperties(FArchive& Ar, BYTE* Data, UClass* DefaultsClass) {}
    UBOOL IsChildOf(const UStruct* SomeBase) const;
    UStruct* GetSuperStruct() const;
    static void InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

class CORE_API UState : public UStruct
{
public:
    QWORD  ProbeMask, IgnoreMask;
    WORD   LabelTableOffset, StateFlags;
    UField* VfHash[64];
    static UClass* StaticClass();
    static UClass  PrivateStaticClass;
};

class CORE_API UClass : public UState
{
public:
    EClassFlags ClassFlags;
    FGuid       ClassGuid;
    INT         ClassReclassifyVersion;
    UObject*    DefaultObject;
    void*       ClassConstructor;
    void*       ClassStaticConstructor;
    UClass*     ClassWithin;
    FName       ClassConfigName;
    TArray<struct FDependency> Dependencies;
    TArray<FName>  PackageImports;
    TArray<BYTE>   Defaults;
    TArray<UField*> NetFields;
    static UClass* StaticClass();
    UClass* GetSuperClass() const;
    UObject* GetDefaultObject() { return DefaultObject; }
    class AActor* GetDefaultActor();
    UPackage* GetOuterUPackage() const;
    UBOOL IsA(const UClass* SomeBase) const;
    void  AddDependency(UClass* Dep, UBOOL Deep);
    static UClass PrivateStaticClass;
};

class CORE_API UFunction : public UStruct
{
public:
    DWORD FunctionFlags;
    WORD  iNative, RepOffset;
    BYTE  NumParms;
    WORD  ParmsSize, ReturnValueOffset;
    void* Func;
    static UClass* StaticClass();
    static void    InternalConstructor(void* X);
    static UClass  PrivateStaticClass;
};

class CORE_API UPackage : public UObject
{
public:
    void*  DllHandle;
    UBOOL  bDirty;
    static UClass* StaticClass();
    void*  GetDllExport(const ANSICHAR* ExportName, UBOOL Checked);
    static void   InternalConstructor(void* X);
    static UClass PrivateStaticClass;
};

//=============================================================================
// FFrame
//=============================================================================
struct FFrame
{
    UStruct*  Node;
    UObject*  Object;
    BYTE*     Code;
    BYTE*     Locals;
    FFrame*   PreviousFrame;
    struct FOutParmRec* OutParms;
    FFrame() {}
    FFrame(UObject* O) : Object(O), Code(nullptr), Locals(nullptr),
                         PreviousFrame(nullptr), OutParms(nullptr) {}
};

//=============================================================================
// FConfigCache / GConfig
//=============================================================================
class CORE_API FConfigCache
{
public:
    virtual ~FConfigCache() {}
    virtual UBOOL GetString(const ANSICHAR* Sec, const ANSICHAR* Key, ANSICHAR* Val, INT Size, const ANSICHAR* File = nullptr) = 0;
    virtual UBOOL GetString(const ANSICHAR* Sec, const ANSICHAR* Key, FString& Val, const ANSICHAR* File = nullptr) = 0;
    virtual UBOOL GetInt   (const ANSICHAR* Sec, const ANSICHAR* Key, INT&    Val, const ANSICHAR* File = nullptr) = 0;
    virtual UBOOL GetFloat (const ANSICHAR* Sec, const ANSICHAR* Key, FLOAT&  Val, const ANSICHAR* File = nullptr) = 0;
    virtual UBOOL GetBool  (const ANSICHAR* Sec, const ANSICHAR* Key, UBOOL&  Val, const ANSICHAR* File = nullptr) = 0;
    virtual void  SetString(const ANSICHAR* Sec, const ANSICHAR* Key, const ANSICHAR* Val, const ANSICHAR* File = nullptr) = 0;
    virtual void  SetInt   (const ANSICHAR* Sec, const ANSICHAR* Key, INT    Val, const ANSICHAR* File = nullptr) = 0;
    virtual void  SetFloat (const ANSICHAR* Sec, const ANSICHAR* Key, FLOAT  Val, const ANSICHAR* File = nullptr) = 0;
    virtual void  SetBool  (const ANSICHAR* Sec, const ANSICHAR* Key, UBOOL  Val, const ANSICHAR* File = nullptr) = 0;
    virtual void  Flush(UBOOL Read, const ANSICHAR* File = nullptr) = 0;
    virtual void  UnloadFile(const ANSICHAR* File) = 0;
    virtual UBOOL GetStr(const ANSICHAR* Sec, const ANSICHAR* Key, FString& Val) { return 0; }
    virtual const ANSICHAR* GetStr(const ANSICHAR* Sec, const ANSICHAR* Key) { return nullptr; }
};

//=============================================================================
// Globals (exported from Core.dll)
//=============================================================================
extern CORE_API FOutputDevice*  GLog;
extern CORE_API FOutputDevice*  GError;
extern CORE_API FOutputDevice*  GNull;
extern CORE_API FConfigCache*   GConfig;
extern CORE_API FMemStack       GMem;
extern CORE_API UObject*        GAutoRegister;
extern CORE_API INT             GIsEditor;
extern CORE_API INT             GIsRunning;
extern CORE_API DOUBLE          GSecondsPerCycle;
extern CORE_API DWORD           GUglyHackFlags;  // ?GUglyHackFlags@@3KA

//=============================================================================
// Global functions (exported from Core.dll)
// Free functions are fine with CORE_API — only member functions in CORE_API
// classes cause C2487.
//=============================================================================
CORE_API void      VARARGS appError(const ANSICHAR* Msg, ...);
CORE_API DOUBLE    appSeconds();
CORE_API DOUBLE    appSecondsSlow();
CORE_API void      appSleep(FLOAT Seconds);              // ?appSleep@@YAXM@Z
CORE_API void      appRequestExit(INT Immediate = 0);    // ?appRequestExit@@YAXH@Z
CORE_API ANSICHAR* VARARGS appSprintf(const ANSICHAR* Fmt, ...);
CORE_API void*     appMalloc(INT Size, const ANSICHAR* Tag);
CORE_API void      appFree(void* Ptr);
CORE_API void*     appRealloc(void* Ptr, INT NewSize, const ANSICHAR* Tag);
CORE_API void      appMemzero(void* Dest, INT Count);
CORE_API void      appMemcpy(void* Dest, const void* Src, INT Count);
CORE_API void      appMemmove(void* Dest, const void* Src, INT Count);
CORE_API INT       appMemcmp(const void* A, const void* B, INT Count);
CORE_API INT       appStrcmp(const ANSICHAR* A, const ANSICHAR* B);
CORE_API INT       appStricmp(const ANSICHAR* A, const ANSICHAR* B);
CORE_API ANSICHAR* appStrcpy(ANSICHAR* Dest, const ANSICHAR* Src);
CORE_API ANSICHAR* appStrcat(ANSICHAR* Dest, const ANSICHAR* Src);
CORE_API INT       appStrlen(const ANSICHAR* S);
CORE_API UBOOL     appLoadFileToArray(TArray<BYTE>& Result, const ANSICHAR* Filename, FFileManager* FM = nullptr);
CORE_API UBOOL     appSaveArrayToFile(const TArray<BYTE>& Array, const ANSICHAR* Filename, FFileManager* FM = nullptr);

//=============================================================================
// Math helpers (inline — no dllimport needed)
//=============================================================================
inline FLOAT appFloor(FLOAT f) { return floorf(f); }
inline FLOAT appCeil(FLOAT f)  { return ceilf(f);  }
inline FLOAT appFabs(FLOAT f)  { return fabsf(f);  }
inline FLOAT appSqrt(FLOAT f)  { return sqrtf(f);  }
inline FLOAT appSin(FLOAT f)   { return sinf(f);   }
inline FLOAT appCos(FLOAT f)   { return cosf(f);   }
inline FLOAT appAsin(FLOAT f)  { return asinf(f);  }
inline FLOAT appAcos(FLOAT f)  { return acosf(f);  }
inline FLOAT appAtan(FLOAT f)  { return atanf(f);  }
inline FLOAT appAtan2(FLOAT y, FLOAT x) { return atan2f(y, x); }

inline INT   Clamp(INT v, INT lo, INT hi)          { return v<lo?lo:(v>hi?hi:v); }
inline FLOAT Clamp(FLOAT v, FLOAT lo, FLOAT hi)    { return v<lo?lo:(v>hi?hi:v); }
inline FLOAT Lerp(FLOAT A, FLOAT B, FLOAT T)       { return A+(B-A)*T; }
inline FLOAT Square(FLOAT f)                       { return f*f; }

//=============================================================================
// FVector inline implementations needing appSqrt / appFabs
//=============================================================================
inline FLOAT FVector::Size()        const { return appSqrt(X*X+Y*Y+Z*Z); }
inline FLOAT FVector::Size2D()      const { return appSqrt(X*X+Y*Y); }
inline bool  FVector::IsNearlyZero()const { return appFabs(X)<1e-4f && appFabs(Y)<1e-4f && appFabs(Z)<1e-4f; }

//=============================================================================
// FFileManager stub
//=============================================================================
class CORE_API FFileManager { public: virtual ~FFileManager() {} };

//=============================================================================
// Math globals and utility functions
//=============================================================================
struct FCoords;  // forward decl for FMath
struct FMath
{
    FLOAT CosTab[16384];
    FLOAT SinTab[16384];
    FLOAT SqrtTab[16384];
    FCoords UnitCoords;  // identity coordinate system
};
extern CORE_API FMath GMath;

template<class T>
T* NewZeroed(FMemStack& Mem) { T* p = (T*)Mem.Push(sizeof(T),alignof(T)); memset(p,0,sizeof(T)); return p; }

inline INT  appRound(FLOAT F)  { return (INT)(F + 0.5f); }
inline void debugf(const ANSICHAR* Fmt, ...) {}
inline void debugf(EName N, const TCHAR* Fmt, ...) {}

//=============================================================================
// Property system stubs (UE1 reflection — minimally needed for StaticConstructor)
//=============================================================================
enum ECppProperty { EC_CppProperty = 0 };

// CPF_ property flags
enum { CPF_Config = 0x00000020 };

class CORE_API UProperty : public UObject
{
public:
    UProperty() {}
    UProperty(ECppProperty, INT Offset, const TCHAR* Category, DWORD PropFlags) {}
};
class CORE_API UBoolProperty  : public UProperty
{
public:
    UBoolProperty(ECppProperty e, INT o, const TCHAR* Cat, DWORD f) : UProperty(e,o,Cat,f) {}
};
class CORE_API UIntProperty   : public UProperty
{
public:
    UIntProperty(ECppProperty e, INT o, const TCHAR* Cat, DWORD f) : UProperty(e,o,Cat,f) {}
};
class CORE_API UFloatProperty : public UProperty
{
public:
    UFloatProperty(ECppProperty e, INT o, const TCHAR* Cat, DWORD f) : UProperty(e,o,Cat,f) {}
};
class CORE_API UByteProperty  : public UProperty
{
public:
    UByteProperty(ECppProperty e, INT o, const TCHAR* Cat, DWORD f) : UProperty(e,o,Cat,f) {}
};

#define CPP_PROPERTY(f) EC_CppProperty, offsetof(ThisClass, f)
inline const TCHAR* appFromAnsi(const ANSICHAR* s) { return (const TCHAR*)s; }

//=============================================================================
// PI constant — must be defined BEFORE FCoords operator implementations
//=============================================================================
#ifndef PI
constexpr FLOAT PI = 3.14159265358979323846f;
#endif
#ifndef ARRAY_COUNT
#define ARRAY_COUNT(arr) (sizeof(arr)/sizeof((arr)[0]))
#endif

//=============================================================================
// FCoords /= FRotator  and  *= FCoords — inline implementations
//=============================================================================
inline FCoords& FCoords::operator/=(const FRotator& R) {
    FLOAT SR = appSin((FLOAT)R.Roll  * (PI/32768.f));
    FLOAT SP = appSin((FLOAT)R.Pitch * (PI/32768.f));
    FLOAT SY = appSin((FLOAT)R.Yaw   * (PI/32768.f));
    FLOAT CR = appCos((FLOAT)R.Roll  * (PI/32768.f));
    FLOAT CP = appCos((FLOAT)R.Pitch * (PI/32768.f));
    FLOAT CY = appCos((FLOAT)R.Yaw   * (PI/32768.f));
    // Build rotation matrix, then compose its transpose (inverse) into *this
    FCoords rot;
    rot.Origin = FVector(0,0,0);
    rot.XAxis  = FVector(CP*CY,          CP*SY,          -SP   );
    rot.YAxis  = FVector(SR*SP*CY-CR*SY, SR*SP*SY+CR*CY,  SR*CP);
    rot.ZAxis  = FVector(CR*SP*CY+SR*SY, CR*SP*SY-SR*CY,  CR*CP);
    FCoords inv;
    inv.Origin = FVector(0,0,0);
    inv.XAxis  = FVector(rot.XAxis.X, rot.YAxis.X, rot.ZAxis.X);
    inv.YAxis  = FVector(rot.XAxis.Y, rot.YAxis.Y, rot.ZAxis.Y);
    inv.ZAxis  = FVector(rot.XAxis.Z, rot.YAxis.Z, rot.ZAxis.Z);
    *this *= inv;
    return *this;
}
inline FCoords& FCoords::operator*=(const FCoords& C) {
    Origin = Origin.TransformPointBy(C);
    XAxis  = XAxis.TransformVectorBy(C);
    YAxis  = YAxis.TransformVectorBy(C);
    ZAxis  = ZAxis.TransformVectorBy(C);
    return *this;
}

//=============================================================================
// Min / Max / Abs templates
//=============================================================================
template<class T> inline T Min(T a, T b) { return a < b ? a : b; }
template<class T> inline T Max(T a, T b) { return a > b ? a : b; }
template<class T> inline T Abs(T a)      { return a < T(0) ? -a : a; }

//=============================================================================
// Timing macros (no-ops in stub SDK)
//=============================================================================
#define clockFast(x)
#define unclockFast(x)

//=============================================================================
// GET_VARARGS
//=============================================================================
#include <stdarg.h>
#ifndef GET_VARARGS
#define GET_VARARGS(msg, len, fmt) \
    do { va_list _va; va_start(_va, fmt); vswprintf((wchar_t*)(msg),(len),(const wchar_t*)(fmt),_va); va_end(_va); } while(0)
#endif

//=============================================================================
// GET_COLOR_DWORD
//=============================================================================
#ifndef GET_COLOR_DWORD
#define GET_COLOR_DWORD(c) (((DWORD)(c).A<<24)|((DWORD)(c).R<<16)|((DWORD)(c).G<<8)|((DWORD)(c).B))
#endif

//=============================================================================
// _WORD alias
//=============================================================================
#ifndef _WORD
typedef WORD _WORD;
#endif

//=============================================================================
// LocalizeError / ParseCommand stubs
//=============================================================================
inline const TCHAR* LocalizeError(const TCHAR* Msg, const TCHAR* Pkg=nullptr, const TCHAR* Lang=nullptr) { return Msg; }
inline bool ParseCommand(const TCHAR** Stream, const TCHAR* Match) {
    if (!Stream || !*Stream || !Match) return false;
    const TCHAR* s = *Stream;
    while (*s == L' ') s++;
    const TCHAR* m = Match;
    while (*m && ((*s == *m) ||
                  (*s >= L'a' && *s <= L'z' && (TCHAR)(*s-32) == *m) ||
                  (*s >= L'A' && *s <= L'Z' && (TCHAR)(*s+32) == *m)))
        { s++; m++; }
    if (*m == 0 && (*s == 0 || *s == L' ')) { while (*s == L' ') s++; *Stream = s; return true; }
    return false;
}

//=============================================================================
// appToUnicode
//=============================================================================
inline const wchar_t* appToUnicode(const TCHAR* s) { return (const wchar_t*)s; }

//=============================================================================
// Cast<T> template — UObject type-safe downcast
//=============================================================================
template<class T, class F>
inline T* Cast(F* obj) {
    if (!obj) return nullptr;
    if (obj->IsA(T::StaticClass())) return static_cast<T*>(obj);
    return nullptr;
}
template<class T, class F>
inline const T* Cast(const F* obj) {
    if (!obj) return nullptr;
    if (obj->IsA(T::StaticClass())) return static_cast<const T*>(obj);
    return nullptr;
}

//=============================================================================
// VectorMem (used in UTGLR paths not guarded by UTGLR_NO_VECTOR_MEM)
//=============================================================================
#if !defined(UTGLR_NO_VECTOR_MEM)
extern CORE_API FMemStack VectorMem;
#endif

//=============================================================================
// New<T>(FMemStack&, INT Count=1) — typed stack allocator
// The UE1 macro  #define New(Mem,Class) new(Mem)Class  on line 5 fires when
// the token "New" is immediately followed by "(". We undef it around the
// template *definition* only; call sites that write  New<T>(Mem)  are safe
// because the preprocessor doesn't invoke function-like macros unless "New"
// is directly followed by "(" (the "<" breaks the match).
//=============================================================================
#pragma push_macro("New")
#undef New
template<class T>
inline T* New(FMemStack& Mem, INT Count = 1) {
    return (T*)Mem.Push(Count * (INT)sizeof(T), (INT)alignof(T));
}
#pragma pop_macro("New")
