/*=============================================================================
    DS9Compat.cpp — Linker stubs for DS9 (Star Trek: The Fallen) build.

    Problem:
        URenderDevice is ENGINE_API (__declspec(dllimport)).  MSVC emits the
        vtable for ENGINE_API classes inside the exporting DLL (Engine.dll).
        When building the vtable for the derived class UD3D9RenderDevice, the
        linker must resolve every inherited slot — including those that ARE
        overridden in the derived class — because it needs a base-class thunk
        for virtual dispatch through a URenderDevice* pointer.

        The optional no-op virtuals were originally written as inline {} bodies
        in Engine.h.  With __declspec(dllimport), MSVC treats inline bodies on
        member functions as declarations, not definitions — the actual symbol is
        expected to be exported from the DLL.  Engine.dll was shipped without
        exporting these symbols, causing LNK2001 / LNK2019.

    Solution:
        1. Strip the inline bodies from Engine.h (leave bare declarations).
        2. Define them here, outside any DLL-import boundary, so they link
           directly into D3D9DrvRTX.dll.

        UD3D9RenderDevice overrides every one of these, so at runtime the
        derived implementations are always called; these stubs only satisfy
        the linker's vtable-resolution pass.

    Note on StaticConstructor:
        URenderDevice::StaticConstructor IS exported by Engine.dll
        (?StaticConstructor@URenderDevice@@QAEXXZ) so it is NOT defined here.
=============================================================================*/

// D3D9RenderDevice.h -> <d3d9.h> -> <windows.h> must come first so that
// DebugBreak(), va_start/va_end etc. are available when Core.h is parsed.
#include "D3D9RenderDevice.h"
#include "D3D9DrvRTX.h"

// C4273: "inconsistent dll linkage"
// URenderDevice is ENGINE_API (dllimport), but these methods are not exported
// from Engine.dll.  We define them here so the linker can build the vtable.
// UD3D9RenderDevice overrides all of them, so these stubs are never called
// at runtime.  The warning is expected and harmless.
#pragma warning(push)
#pragma warning(disable: 4273)

// ---------------------------------------------------------------------------
// URenderDevice optional-interface stubs
// ---------------------------------------------------------------------------

void URenderDevice::DrawFogSurface(FSceneNode* /*Frame*/, FFogSurf& /*FogSurf*/)
{
}

void URenderDevice::PreDrawFogSurface()
{
}

void URenderDevice::PostDrawFogSurface()
{
}

void URenderDevice::PreDrawGouraud(FSceneNode* /*Frame*/, FLOAT /*FogDistance*/, FPlane /*FogColor*/)
{
}

void URenderDevice::PostDrawGouraud(FLOAT /*FogDistance*/)
{
}

INT URenderDevice::MaxVertices()
{
    return 0xFFFF;
}

void URenderDevice::DrawTriangles(
    FSceneNode*     /*Frame*/,
    FTextureInfo&   /*Info*/,
    FTransTexture** /*Pts*/,
    INT             /*NumPts*/,
    USHORT*         /*Indices*/,
    INT             /*NumIdx*/,
    DWORD           /*PolyFlags*/,
    FSpanBuffer*    /*Span*/)
{
}

UBOOL URenderDevice::SupportsTextureFormat(ETextureFormat /*Fmt*/)
{
    return 0;
}

// Only the two-arg overload is missing from Engine.dll exports.
// The single-arg ReadPixels(FColor*) is exported and resolved via Engine.lib.
void URenderDevice::ReadPixels(FColor* /*Pixels*/, UBOOL /*bGammaCorrectOutput*/)
{
}

void URenderDevice::ShutdownAfterError()
{
}

#pragma warning(pop)

// ---------------------------------------------------------------------------
// UD3D9RenderDevice::PrecacheTexture
//
// In D3D9RenderDevice.cpp this is wrapped in:
//   #if !KLINGON_HONOR_GUARD && !DEEP_SPACE_NINE
// ...so it's compiled out for the DS9 build. Define it here instead.
// The implementation is identical to the non-DS9 version.
// ---------------------------------------------------------------------------
void UD3D9RenderDevice::PrecacheTexture(FTextureInfo& Info, DWORD PolyFlags)
{
    guard(UD3D9RenderDevice::PrecacheTexture);
    SetTextureNoPanBias(0, Info, PolyFlags);
    unguard;
}
