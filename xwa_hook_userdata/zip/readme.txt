xwa_hook_userdata

This hook modifies where the pilot files are saved.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_userdata.dll and hook_userdata.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that reads and writes pilot files
At offset 1249B1, replace 6A0C with 6A0D.
At offset 1249CF, replace C60563C8B00000 with C60564C8B00000.
At offset 128036, replace E8B5000000 with E8F5FE0700.
At offset 128B2D, replace E8BEF5FFFF with E8F5FE0700.
At offset 138C2E, replace E8BDF4FEFF with E8FDF20600.
At offset 12813D, replace E8EE1F0000 with E8EEFD0700.
At offset 110B88, replace E803790100 with E8A3730900.
At offset 128477, replace E814000000 with E8B4FA0700.
At offset 128CA2, replace E8E9F7FFFF with E889F20700.
At offset 13D4FD, replace E88EAFFEFF with E82EAA0600.
At offset 170510, replace E87B7FFBFF with E81B7A0300.
At offset 170A0E, replace E87D7AFBFF with E81D750300.
At offset 12801C, replace E8DF050000 with E80FFF0700.
At offset 128A41, replace E8BAFBFFFF with E8EAF40700.

# To call the hook that sets the config settings
At offset 11FA42, replace 7527 with EB27.
At offset 120248, replace 7527 with EB27.
At offset 1236CA, replace 753B with EB3B.
At offset 123B19, replace E812660000 with E812440800.
At offset 123B82, replace BF80D6AB00 with E899430800.
At offset 13F9F1, replace E81A67FEFF with 9090909090.
At offset 170688, replace 775B with EB5B.

# To call the hook that sets the config presets
At offset 0FB7F5, replace 66813DC0538000CD000F95C0 with E826C70A0090909090909090.


*** Usage ***

The pilot files will be saved in a directory named "UserData\<ModName>\Pilot\"
The <ModName> is read from a text file named "modname.txt" situated at the root of XWA directory, next to xwingalliance.exe.

To start the game from a given mission, set the StartMissionIndex setting. It is the index of the start mission starting from 0.
For example, to start from the 10th mission, set StartMissionIndex = 9.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
