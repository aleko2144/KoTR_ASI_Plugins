## KoTR_ModuleLoader (11.03.2024)
Version 1.06</br>
King of The Road *.b3d / *.wav loader.

### ModuleLoader.ini:
```
[COMMON]
DisplayConsole = on/off - display console window

common_path  = COMMON\\ - common.b3d path
cabines_path = COMMON\\ - cabines.b3d path
trucks_path  = COMMON\\ - trucks.b3d path

env_path  = ENV\\  - gameworld b3d files path
menv_path = MENV\\ - multiplayer circuit path
mtb_path  = MTB\\  - multiplayer truckball path
mtr_path  = MTR\\  - multiplayer desert path

ignore_texture_settings = on/off - if enabled, the game will use only <name>.res files (<name>2/3.res will ignored)
```

#### B3D load section:
```
[FILE_<index>]
path = path to b3d file (with two backslashes, e.g. "vehicles\\")
name = b3d file name (e.g. "Fiat")
```
The index starts from 1. The b3d and res files of the same name will be loaded, as well as if there is an ovl.

#### Single RES load section:
```
[RESOURCE_<index>]
path = path to RES file (with two backslashes, e.g. "COMMON\\")
name = RES file name (e.g. "vehicle_msk")
```
This section is needed to load res files that are not related to any b3d - for example, separate sets of msk-textures for in-game menus.

#### WAV load section:
```
[SOUND_<index>]
path = path to WAV file (with two backslashes including extension, e.g. "data\\effects\\tire_blow.wav")
name = in-game sound name (e.g. "tireBlowSound")
```

#### Include another ini-list with files to load:
```
[INCLUDE]
list_1 = vehicles\\common_load_list.lst
list_2 = vehicles\\vehicles_load_list.lst
list_3 = data\\sounds_load_list.lst
list_4 = data\\objects_load_list.lst
```

### Features
Loaded *.res must must not contain sound files. Otherwise, the game will crash when the gameworld finishes loading.
