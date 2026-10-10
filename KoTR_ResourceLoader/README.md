## KoTR_ResourceLoader (10.10.2026)
Version 1.07</br>
King of The Road *.b3d / *.ovl / *.res / *.wav loader (ex ModuleLoader).

The loader scans the "mods" directory and its subfolders for *.lst files containing files to load. The "mods" directory must be located in the root of the game folder.

### KoTR_ResourceLoader.ini:
```
[COMMON]
DisplayConsole = on/off - display console window
WriteLog       = on/off - write load info into KoTR_ResourceLoader.log file

common_path  = COMMON\\ - common.b3d path
cabines_path = COMMON\\ - cabines.b3d path
trucks_path  = COMMON\\ - trucks.b3d path

env_path  = ENV\\  - gameworld b3d files path
menv_path = MENV\\ - multiplayer circuit path
mtb_path  = MTB\\  - multiplayer truckball path
mtr_path  = MTR\\  - multiplayer desert path

ignore_texture_settings = on/off - if enabled, the game will use only <name>.res files (<name>2/3.res will ignored).
Enabled by default.
```

### Any *.lst-file:
#### B3D load section:
```
[MODULE_<index>]
path = path to b3d file (with two backslashes, e.g. "vehicles\\")
name = b3d file name (e.g. "Fiat")
```
The b3d and res files of the same name will be loaded, as well as if there is an ovl.

#### Single RES load section:
```
[RESOURCE_<index>]
path = path to RES file (with two backslashes, e.g. "COMMON\\")
name = RES file name (e.g. "vehicle_msk")
```
This section is needed to load res files that are not related to any b3d - for example, separate sets of msk-textures for in-game menus.

Note: The file loaded from the [RESOURCE] section must have a name like <name>16.res. For example, if "vehicle_msk" is specified, the file must be named "vehicle_msk16.res". This is a game requirement.

#### WAV load section:
```
[SOUND_<index>]
path = path to WAV file (with two backslashes including extension, e.g. "data\\effects\\tire_blow.wav")
name = in-game sound name (e.g. "tireBlowSound")
```

### Features
* Loaded *.res must must not contain sound files. Otherwise, the game will crash when the gameworld finishes loading.
* File/folder paths are specified relative to the location of the LST file. Empty path parameters are allowed for the [MODULE] and [RESOURCE] sections (if the file being loaded is located in the same directory as the LST file).
* Each section is numbered starting from one.