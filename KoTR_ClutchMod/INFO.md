# KoTR ClutchMod (26.12.2025)
Version 0.3

A modification that adds support for steering wheels with clutch pedals and gearboxes to the game "Hard Truck 2 / King of the Road".

### Clutch adjustment procedure ###
1. Connect the device to the computer, calibrate if necessary.
2. In the KoTR_ClutchMod.ini file, set the printJoyInfo = 1 parameter and start the game.
3. In the game, adjust the axes of the steering wheel, gas and brake pedals.
4. Release all pedals and place the steering wheel in the center position.
5. The text will be displayed in the upper left part of the game window:

>Device ID: 0</br>
>Axes/Buttons/POVs/Sliders: 4/24/1/0</br>
>Axes: 1.=0 2.=0 3.=-1000 4.=0 5.=0 6.=-1000</br>
>clutchAxisValue=1</br>

You need to smoothly press the clutch pedal and follow the values of the line “Axes: 1...”. One of the parameters will begin to change.
When it becomes clear which parameter is being changed, you will need to release the pedal all the way and write the parameter value (for example, -1000) to KoTR_ClutchMod.ini->[CLUTCH]->min.
Then press the pedal all the way and write the parameter value (for example, 1000) in KoTR_ClutchMod.ini->[CLUTCH]->max.
You also need to write down the pedal axis number in KoTR_ClutchMod.ini->[CLUTCH]->axisID (the axis number is the number with a dot before the parameter value in the console window).

Next, you need to save the modified ini file and restart the game. After starting, you also need to switch to the console window and smoothly press/release the pedal, monitoring the clutchAxisValue line.
When the pedal is fully released, the value of this parameter should be 1, when the pedal is fully pressed - 0. If the reverse values are displayed (0 when the pedal is released, 1 when the pedal is pressed), then in the file
KoTR_ClutchMod.ini in the [CLUTCH] section you need to set invert = 0 (or 1 if there was a zero value before).

6. After completing the clutch pedal settings, you should close the game and turn off the data output. To do this, in KoTR_ClutchMod.ini in the [MOD] section set printJoyInfo = 0.
7. This completes the clutch adjustment.

### Procedure for setting up the gearbox ###
If the steering wheel is equipped with a gearbox, then it can be used in the game using this modification.

1. Launch the system application “Setting up USB game controllers” (it can be found in the Start menu, it is built into Windows).
2. In the list of devices, select a steering wheel with a gearbox and open the button testing menu (depending on the specific device model).
3. Engage first gear. An indicator with a number inside will light up in the window - this is the number of the button that is currently pressed (and the first gear button is pressed).
In the KoTR_ClutchMod.ini file in the [GEARBOX] section in the joy_gear_1 parameter, enter the value of this button (for example, 17).

4. Repeat the third step for 2nd, 3rd, 4th, 5th, 6th gears, as well as reverse.
5. Assign neutral gear to any convenient button on the device and also write it in KoTR_ClutchMod.ini.
6. Assign a button to the gear range switch (joy_gear_range in the ini file; not only a button, but even a two-position switch mounted on the gearbox housing will do).
7. If desired, you can set the autoNeutralSet parameter to 1 - then neutral gear will be turned on automatically when no other gear is selected. Otherwise, you will have to turn on the neutral manually.
The neutral button is active even if autoNeutralSet = 1.

Important! The maximum allowed button index in the ini file is 32. More buttons on the device are not supported by the game "Hard Truck 2/King of the Road".

### Features ###
1) In cases where any gear other than neutral is selected, the driving speed is less than 3.6 km/h, and the clutch is only partially depressed, the vehicle brake is automatically applied.
This was done on purpose. Otherwise, the car will begin to spontaneously roll forward/backward with acceleration.

2) Shifting into gear without the clutch being depressed at least halfway is not allowed! In this case, the gear will not engage, and shifting gears using the “gear+” or “gear-” buttons may not work correctly.
If the truck is stationary and the ability to upshift is lost, you should engage reverse, and then first gear (it should engage). As a last resort, you can use the keyboard keys to
turning on specific gears, after which the ability to use the “gear+” and “gear-” buttons should return.

3) Press/switch the gear range button when the clutch is depressed, but a gear has not yet been selected. That is, immediately after pressing the clutch pedal. In other cases, the modification will not track the range switching. Each time it is switched, a message about the current gear range (1-6 or 6-12) is displayed in the console.

4) If a gear is selected on the device that is not on the current game car, then neutral will be engaged.

### KoTR_ClutchMod.ini ###
`[MOD]`
* `Enabled` = 0/1 - on/off. modification
* `printJoyInfo` = 0/1 - on/off. output information to the console

`[STEERING]`
* `cabSteerMaxDeg` = 450 - the maximum angle of rotation of the interior steering wheel in one direction (in the game by default 90)

`[CLUTCH]`
* `axisID` = 2 - number of the device axis used for the clutch pedal
* `min` = 0 - minimum value on the axis
* `max` = 1000 - maximum value on the axis
* `invert` = 0/1 - on/off clutch pedal inversion
* `interp` = 0.02 - coefficient of inertia of engine speed when pressing the clutch (the higher, the faster the engine speed will drop)

`[GEARBOX]`
* `enabled` = 1 - on/off use of external gearbox

* `joy_gear_R` = 23 - reverse button
* `joy_gear_N` = 3 - neutral button
* `joy_gear_1` = 17 - 1st/7th gear button
* `joy_gear_2` = 18 - 2nd/8th gear button
* `joy_gear_3` = 19 - 3rd/9th gear button
* `joy_gear_4` = 20 - 4th/10th gear button
* `joy_gear_5` = 21 - 5th/11th gear button
* `joy_gear_6` = 22 - 6th/12th gear button
* `joy_gear_range` = 24 - gear range switch button (1-6, 6-12)

* `autoNeutralSet` = 0/1 - on/off automatic activation of neutral gear if no other gear is selected
