hook_joystick_ff

This hook permits to customize the joystick settings.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_joystick_ff.dll, XwaJoystickConfig.exe and XwaJoystickConfig.exe.config next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that selects the joystick ff device
At offset 0349D0, replace 83EC108B44241456 with E85B351700C20800.

# To call the hook that allows joystick small movements
;At offset 0338A0, replace 51 with C3.
At offset 0338A0, replace 5166A1C21C8C00 with E87B461700C390.
;At offset 10AE69, replace 7E22 with 9090.
;At offset 10AEAE, replace 7E2F with 9090.
;At offset 10AF00, replace 7E28 with 9090.
;At offset 10AF4B, replace 7E3E with 9090.
At offset 10AD61, replace 8915C4337700 with E8BAD1090090.

# To call the hook that selects the controller
At offset 1401B6, replace A01E699F00 with E8757D0600.
At offset 10ABF4, replace 752655E8445A0300 with 909055E834D30900.
At offset 10AE4B, replace 0F8586010000 with E8D0D0090090.

# To call the hook that defines the controller buttons
At offset 033A7E, replace B901000000BF91C8B000668B37 with 55E89C4417005DE9AF00000090.
;At offset 0ECBF6, replace E8756DF4FF with E825B30B00.

# To call the hook that defines the remap config menu
At offset 11E941, replace E80AD60300 with E8EA950800.
At offset 11E9E3, replace C705B82978000D000000891DA0297800 with E8389508009090909090909090909090.
At offset 11EBCF, replace 891DB8297800 with E84C93080090.

# To call the hook that defines the look move
At offset 0F8EB7, replace 668B0DC053800084C0A100A05A00 with 51E873F00A0059803D6B959E0000.


*** Usage ***

To configure the hook, use the "JoystickConfig.txt" file or use the "XwaJoystickConfig.exe" tool.

The format is:
JoystickFFDeviceIndex = device index
The default value is 0, which select the first device for force feedback.
When the value is -1, the last feedback device will be selected for force feedback.

EnableSmallMovement = value
The default value is 1, which enable small movement.
When the value is 0, small movement is disabled.

MainControllerIndex = controller index
Select the controller used for the main axis and buttons. The default value is 0.

YawControllerIndex = controller index
Select the controller used for yaw. The default value is 0.

YawControllerAxisIndex = axis index
Select the controller axis used for yaw. The default value is 0 (x axis).

InvertYaw = value
Invert yaw axis

PitchControllerIndex = controller index
Select the controller used for pitch. The default value is 0.

PitchControllerAxisIndex = axis index
Select the controller axis used for pitch. The default value is 1 (y axis).

InvertPitch = value
Invert pitch axis

ThrottleControllerIndex = controller index
Select the controller used for throttle. The default value is 0.

ThrottleControllerAxisIndex = axis index
Select the controller axis used for throttle. The default value is 2 (z axis).

InvertThrottle = value
Invert throttle axis

RudderControllerIndex = controller index
Select the controller used for rudder. The default value is 0.

RudderControllerAxisIndex = axis index
Select the controller axis used for rudder. The default value is 3 (r axis).

InvertRudder = value
Invert rudder axis

UsePovControllerAsButtons = value
The default value is 0, which enable continious movement for the POV buttons.
When the value is 1, the behavior of POV buttons is like regular buttons.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
