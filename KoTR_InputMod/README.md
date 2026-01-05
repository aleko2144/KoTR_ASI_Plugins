# KoTR_InputMod (15.12.2025)
Version 0.05

A modification that expands the control options for the game "Hard Truck 2 / King of the Road" and adds the ability to steer using the mouse.

### A little about mouse settings ###
1) The linearity of steering with the mouse is set in the game settings using the "Steering sensitivity" parameter. The higher this parameter, the weaker the steering wheel rotation changes in positions close to the center.
2) The [MOUSE]->steering_coeff parameter sets the multiplier of the mouse displacement coefficient from the center of the game window. When the pointer is clearly in the center of the window, the coefficient is zero. When the pointer is located
at the window border, then the coefficient is either 1.0 or -1.0. Thus, steering_coeff amplifies the bias coefficient. For example, with steering_coeff = 1.5 the steering wheel will be turned all the way
1/2 the distance from the center of the window to its border, and so on.

### And some more comments ###
1) In the forced full gas/brake mode (AlwaysFull in ini), the ModifierKey modifier key works in the reverse mode, that is, it reduces the force of pressing the pedals.
2) If replacing function key assignments is enabled ([KEYBINDINGS]->UseModKeys), then exiting the current menu is only possible by pressing the ESC key.

### KoTR_InputMod.ini ###
`[MOD]`
* `Enabled` = 0/1 - on/off modification

`[MOUSE]`
* `UseForSteering` = 0/1 - on/off steering by mouse
* `steering_coeff` = 1.1 - mouse motion ratio (the higher the value, the less mouse movement is required to fully turn the steering wheel)

* `UseForGasBrake` = 0/1 - on/off gas/brake by mouse clicks
* `MouseKeyAccel`  = 0x1 - key code for gas
* `MouseKeyBrakes` = 0x2 - key code for brakes

* `AlwaysFull`     = 0/1 - on/off always full gas/brake (only works if parameters [MOUSE]->UseForGasBrake = 1 and [MODIFIERS]->UseModKeys = 1).

`[MODIFIERS]`
* `UseModKeys`  = 0/1 - on/off. replacing modifier keys
* `ModifierKey` = 0x11 - key code for the gas modifier, brake, headlight switch
* `SteerModKey` = 0x10 - steering modifier key code

`[KEYBINDINGS]`
* `UseModKeys`     = 0/1 - on/off. replacing function keys
* `KeyMenuHelp`    = 0x70 - key code for the "Keyboard" menu (F1 by default)
* `KeyMenuRating`  = 0x71 - key code for the "Rating" menu (F2 by default)
* `KeyMenuInfo`    = 0x72 - key code for the "Info" menu (F3 by default)
* `KeyMenuCompany` = 0x73 - key code for the "My Company" menu (F4 by default)
* `KeyRadio`       = 0x74 - radio key code (F5 by default)
* `KeyScreenshot`  = 0x75 - key code for taking a screenshot (F6 by default)

* `KeyFastSave`    = 0x77 - code for the game's fast save key (F8 by default)
* `KeyFastLoad`    = 0x78 - key code for fast loading save (F9 by default)

* `KeyPause`       = 0x13 - pause key code for entering cheat codes (default Pause/Break)