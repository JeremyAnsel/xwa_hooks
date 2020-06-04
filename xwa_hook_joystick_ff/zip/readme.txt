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
At offset 0338A0, replace 51 with C3.


*** Usage ***

To select a joystick with force feedback, set JoystickDeviceIndex in "hook_joystick_ff.cfg".

The format is:
JoystickDeviceIndex = device index

The default value is 0, which select the first device.
When the value is -1, the last device will be selected.


*** Credits ***

- Jérémy Ansel (JeremyaFr)