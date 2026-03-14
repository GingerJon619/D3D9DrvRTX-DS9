# D3D9DrvRTX – Star Trek: Deep Space Nine: The Fallen Edition

RTX Remix path-tracing compatibility renderer for **Star Trek: Deep Space Nine – The Fallen** (v1.0, The Collective, 2000).

Built from [mmdanggg2's D3D9DrvRTX](https://github.com/mmdanggg2/D3D9DrvRTX), a custom Unreal Engine 1 render device optimized for NVIDIA RTX Remix.

---

## Engine Notes

DS9: The Fallen runs on **Unreal Engine 1 build 338**, the same era as Klingon Honor Guard. Key engine characteristics that influence this port:

| Feature | DS9 behaviour |
|---|---|
| Texture API | `GetInfo()` — no `Lock()`/`Unlock()` |
| BSP zone connectivity | `Model->Nodes->Zones[]` (old UE1 layout) |
| RenderIterator | Not present |
| `bOwnerNoSee` flag | Not present |
| `SupportsLazyTextures` | Not present |
| Unicode build | No |
| Config file | `DS9.ini` in `System\` |
| Weapon/viewmodel | `eventRenderOverlays` standard path |

---

## Requirements

- DS9: The Fallen (v1.0) installed
- [RTX Remix Runtime v0.6+](https://github.com/NVIDIAGameWorks/rtx-remix) — `d3d9.dll` and `.trex` folder in `System\`
- NVIDIA RTX GPU (Turing / Ampere / Ada recommended)

---

## Installation

1. Build `D3D9DrvRTX.dll` (see **Developer Setup** below).
2. Copy the following files into `<DS9 Install>\System\`:
   - `D3D9DrvRTX.dll`
   - `D3D9DrvRTX.int`
   - `DS9.ini` *(renderer settings)*
   - `D3D9DrvRTX_config.json` *(optional advanced config)*
   - RTX Remix `d3d9.dll` + `.trex\` folder
3. Edit `<DS9 Install>\System\DS9.ini` (the game's main ini, **not** the renderer ini of the same name — see note below) and change the render device under `[Engine.Engine]`:

```ini
[Engine.Engine]
GameRenderDevice=D3D9DrvRTX.D3D9RenderDevice
Render=D3D9DrvRTX.D3D9Render
```

> **INI naming note:** The renderer's own settings are stored in a section `[D3D9DrvRTX.D3D9RenderDevice]` inside the game's existing `DS9.ini`. No separate renderer ini file is needed; `StaticConfigName()` returns `"DS9"` so UE1 automatically reads/writes this section there.

4. Launch `DS9.exe`. The game should now render via D3D9 → RTX Remix path tracing.

---

## Settings

Settings live under `[D3D9DrvRTX.D3D9RenderDevice]` in `System\DS9.ini`:

| Key | Default | Description |
|---|---|---|
| `LightMultiplier` | `3500.0` | Global light brightness scalar |
| `LightRadiusDivisor` | `65.0` | Divisor applied before exponentiation |
| `LightRadiusExponent` | `0.55` | Exponent for light radius falloff |
| `NonSolidTranslucentHack` | `True` | Renders non-solid BSP geometry as translucent so lights shine through |
| `EnableSkyBoxRendering` | `True` | Renders skybox zone before main view |
| `EnableSkyBoxAnchors` | `True` | Emits a small anchor mesh for skybox attachment in Remix |
| `EnableHashTextures` | `False` | Replaces procedural textures with stable-hash equivalents (helps Remix asset replacement) |

### Light formula

```
brightness = light.brightness
           × pow(light.radius / LightRadiusDivisor, LightRadiusExponent)
           × LightMultiplier
           × (brightnessSlider × 2)
```

---

## Advanced Config (`D3D9DrvRTX_config.json`)

Per-level anchor and Remix config-var overrides are supported. See `D3D9DrvRTX_config_example.json` for DS9-specific level examples (Promenade, Terok Nor entry, etc.).

To enable Remix API config vars, add to `.trex\bridge.conf`:
```
exposeRemixApi = True
```

---

## Developer Setup

### Prerequisites
- Visual Studio 2022 with Windows SDK
- DirectX 9 SDK (June 2010)
- DS9: The Fallen installed

### SDK Headers

You need UE1 SDK headers extracted from the DS9 game DLLs (`Core.dll`, `Engine.dll`, `Render.dll`). These are the same format as other UE1 games. Place them at:

```
sdks/sdk_ds9_the_fallen/
  Core/
    Inc/   ← Core.h, UnObjBas.h, etc.
    Lib/   ← Core.lib
  Engine/
    Inc/   ← Engine.h, UnEngine.h, etc.
    Lib/   ← Engine.lib
  Render/
    Inc/   ← Render.h, UnRender.h
    Lib/   ← Render.lib
```

Tools like [UE Explorer](https://github.com/EliotVU/Unreal-Library) or [UEViewer](https://www.gildor.org/en/projects/umodel) can help extract export signatures. The import libs can be generated using `scripts/LibGen.py` against the game DLLs.

### Building

```bat
cd scripts
DS9_The_Fallen.bat   ← sets up SDK symlinks and preprocessor defines
```

Then open `D3D9Drv.sln` in Visual Studio and build **Release | Win32**.

The bat script:
1. Symlinks `sdk` → `sdks/sdk_ds9_the_fallen/`
2. Runs `SetProperties.py --debug-exe DS9.exe --no-unicode --game-define DEEP_SPACE_NINE`
3. Writes `gamename.h`

The `DEEP_SPACE_NINE` preprocessor define activates all DS9-specific code paths in `D3D9Config.h`.

---

## DS9-Specific Code Paths (`DEEP_SPACE_NINE` define)

| Guard | Behaviour |
|---|---|
| `UTGLR_NO_TEXTURE_UNLOCK` | Skips `Unlock()` after texture info fetch; uses `GetInfo()` |
| `UTGLR_ALT_DECLARE_CLASS` | Uses old-style `DECLARE_CLASS` without package arg |
| `UTGLR_DEFINE_FTIME` | Defines `FTime` as `DOUBLE` (not a typedef in this build) |
| `UTGLR_NO_RENDERITERATOR` | Skips `RenderIteratorClass` check (field absent) |
| `UTGLR_NO_PLAYER_FLAG` | Skips `bOwnerNoSee` visibility check (field absent) |
| `UTGLR_NO_LODMESH` | Disables LODMesh code paths |
| `UTGLR_ALT_STATIC_CONSTRUCTOR` | Uses `StaticConstructor(UClass*)` signature |
| `UTGLR_OLD_ZONE_CONNECTIVITY` | Reads `Model->Nodes->Zones[z].Connectivity` |
| `StaticConfigName()` | Returns `"DS9"` → reads/writes `DS9.ini` |

---

## Known Limitations / Investigation Areas

- **Decals**: Disabled (`UTGLR_NO_DECALS`). DS9 doesn't use the UE1 decal system.
- **Texture unlock**: DS9 uses `GetInfo()` which doesn't require `Unlock()`. If you see corrupted textures, check that `UTGLR_NO_TEXTURE_UNLOCK` is active.
- **SDK extraction**: Unlike KHG or UT, there is no public DS9 modding SDK. You must generate import libs from the shipping DLLs.
- **`getTextureFromInfo()`**: Uses the same `CacheID >> 32` object-index lookup as KHG. If textures appear wrong, verify that this is the correct layout for build 338.
- **Audio geometry**: `RenderAudioGeometry` is disabled (same as KHG); the game handles its own audio occlusion.

---

## Credits

- Original D3D9DrvRTX: [mmdanggg2](https://github.com/mmdanggg2/D3D9DrvRTX)
- Original D3D9Drv: [Chris Dohnal](https://www.cwdohnal.com/utglr/)
- RTX Remix: NVIDIA GameWorks
