hook_joystick_ff

This hook permits to select the joystick device with force feedback.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_joystick_ff.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that selects the joystick ff device
At offset 0349D0, replace 83EC108B44241456 with E85B351700C20800.

# To call the hook that allows joystick small movements
At offset 0338A0, replace 51 with C3.
At offset 10AE69, replace 7E22 with 9090.
At offset 10AEAE, replace 7E2F with 9090.
At offset 10AF00, replace 7E28 with 9090.
At offset 10AF4B, replace 7E3E with 9090.


*** Usage ***

To select a joystick with force feedback, set JoystickDeviceIndex in "hook_joystick_ff.cfg".

The format is:
JoystickDeviceIndex = device index

The default value is 0, which select the first device.
When the value is -1, the last device will be selected.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
