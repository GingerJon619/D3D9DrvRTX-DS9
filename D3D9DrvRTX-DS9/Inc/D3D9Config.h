#pragma once

//Make sure valid build config selected
#undef UTGLR_VALID_BUILD_CONFIG

#if defined(UNREAL_TOURNAMENT_OLDUNREAL)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_USES_ALPHABLEND 1
	#define UTGLR_NO_PALETTE_ALPHA_FIX 1
#elif defined(UNREAL_TOURNAMENT)
	#define UTGLR_VALID_BUILD_CONFIG 1
#elif defined(DEUS_EX)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_ALT_DECLARE_CLASS 1
	#define UTGLR_DEFINE_FTIME 1
	#define UTGLR_RENDERITERATOR_ACTOR_INIT 1
#elif defined(NERF_ARENA)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_USES_ALPHABLEND 0
	#define UTGLR_NO_DECALS 1
	#define UTGLR_ALT_DECLARE_CLASS 1
	#define UTGLR_DEFINE_FTIME 1
	#define UTGLR_ALT_FLUSH 1
	#define UTGLR_DEFINE_HACK_FLAGS 1
	#define UTGLR_OLD_POLY_CLASSES 1
	#define UTGLR_NO_DETAIL_TEX 1
	#define UTGLR_NO_ALLOW_PRECACHE 1
	#define UTGLR_NO_PLAYER_FLAG 1
	#define UTGLR_NO_SUPER_EXEC 1
#elif defined(RUNE)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_USES_ALPHABLEND 1
	#define UTGLR_DEFINE_FTIME 1
#elif defined(UNREAL_GOLD_OLDUNREAL)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_USES_ALPHABLEND 1
	#define UTGLR_HAS_DARK_LIGHT 1
	#define UTGLR_NO_TEXTURE_UNLOCK 1
	#define UTGLR_NO_PALETTE_ALPHA_FIX 1
#elif defined(UNREAL_GOLD)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_USES_ALPHABLEND 0
	#define UTGLR_NO_DECALS 1
	#define UTGLR_ALT_DECLARE_CLASS 1
	#define UTGLR_DEFINE_FTIME 1
	#define UTGLR_ALT_FLUSH 1
	#define UTGLR_DEFINE_HACK_FLAGS 1
	#define UTGLR_OLD_POLY_CLASSES 1
	#define UTGLR_NO_DETAIL_TEX 1
	#define UTGLR_NO_ALLOW_PRECACHE 1
	#define UTGLR_NO_SUPER_EXEC 1
	#define UTGLR_RENDERITERATOR_ACTOR_INIT 1
	#define UTGLR_NO_DESC_FLAGS 1
#elif defined(KLINGON_HONOR_GUARD)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_USES_ALPHABLEND 0
	#define UTGLR_NO_DECALS 1
	#define UTGLR_ALT_DECLARE_CLASS 1
	#define UTGLR_DEFINE_FTIME 1
	#define UTGLR_ALT_FLUSH 1
	#define UTGLR_DEFINE_HACK_FLAGS 1
	#define UTGLR_OLD_POLY_CLASSES 1
	#define UTGLR_NO_DETAIL_TEX 1
	#define UTGLR_NO_ALLOW_PRECACHE 1
	#define UTGLR_NO_SUPER_EXEC 1
	#define UTGLR_NO_RENDERITERATOR 1
	#define UTGLR_NO_DESC_FLAGS 1
	#define UTGLR_NO_LODMESH 1
	#define UTGLR_ALT_STATIC_CONSTRUCTOR 1
	#define UTGLR_NO_PLAYER_FLAG 1
	#define UTGLR_NO_TEXTURE_UNLOCK 1
	#define UTGLR_MAX_Y_RESOLUTION 1200
#elif defined(HARRY_POTTER_1)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_HP_ENGINE 1
	#define UTGLR_FORCE_RENDER_DEVICE 1
	#define UTGLR_HAS_DARK_LIGHT 1
#elif defined(HARRY_POTTER_2)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_HP_ENGINE 1
	#define UTGLR_HAS_DARK_LIGHT 1
#elif defined(BROTHER_BEAR)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_HP_ENGINE 1
	#define UTGLR_HAS_DARK_LIGHT 1
#elif defined(UNDYING)
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_NO_LODMESH 1
	#define UTGLR_NO_VECTOR_MEM 1
	#define UTGLR_HAS_DARK_LIGHT 1
	#define UTGLR_NO_PALETTE_ALPHA_FIX 1
#elif defined(DEEP_SPACE_NINE)
	// Star Trek: Deep Space Nine - The Fallen (v1.0, UE1 build 338)
	// Confirmed from Engine.dll / Core.dll / Render.dll export analysis:
	//   - HAS URenderIterator              → no UTGLR_NO_RENDERITERATOR
	//   - HAS ULodMesh                     → no UTGLR_NO_LODMESH
	//   - HAS UTexture::Lock(FTextureInfo&, DOUBLE, INT, URenderDevice*) + Unlock(FTextureInfo&)
	//     Lock() IS the info/acquire call for this build; NOT GetInfo().
	//     Engine.h declares Lock/Unlock; D3D9Render.cpp uses #elif DEEP_SPACE_NINE path.
	//   - HAS GUglyHackFlags in Core.dll   → no UTGLR_DEFINE_HACK_FLAGS (extern in Core.h)
	//   - appRequestExit(int) in Core.dll  → standard int-arg form
	//   - appSleep(float) in Core.dll      → standard float-arg form
	//   - No Flush(UBOOL) in URenderDevice → UTGLR_ALT_FLUSH: void Flush()
	//   - Old BSP zone: Model->Zones[]     → UTGLR_OLD_ZONE_CONNECTIVITY
	//   - Old FBspNode/FBspSurf layout     → UTGLR_OLD_POLY_CLASSES
	//   - 3-arg DECLARE_CLASS (no package) → UTGLR_ALT_DECLARE_CLASS
	//   - Build 338 static ctor pattern    → UTGLR_ALT_STATIC_CONSTRUCTOR
	//   - UTexture::AnimCur (not AnimCurrent) for animated frame tracking
	#define UTGLR_VALID_BUILD_CONFIG 1
	#define UTGLR_USES_ALPHABLEND 0
	#define UTGLR_NO_DECALS 1
	#define UTGLR_ALT_DECLARE_CLASS 1
	#define UTGLR_DEFINE_FTIME 1
	#define UTGLR_ALT_FLUSH 1
	#define UTGLR_OLD_POLY_CLASSES 1
	#define UTGLR_NO_DETAIL_TEX 1
	#define UTGLR_NO_ALLOW_PRECACHE 1
	#define UTGLR_NO_SUPER_EXEC 1
	#define UTGLR_NO_DESC_FLAGS 1
	#define UTGLR_ALT_STATIC_CONSTRUCTOR 1
	#define UTGLR_NO_PLAYER_FLAG 1
	#define UTGLR_OLD_ZONE_CONNECTIVITY 1
#else
	#define UTGLR_VALID_BUILD_CONFIG 0
#endif

#if !UTGLR_VALID_BUILD_CONFIG
	#error Valid build config not selected.
#endif
#undef UTGLR_VALID_BUILD_CONFIG

