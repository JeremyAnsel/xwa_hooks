xwa_hook_backdrops

This hook adds starfield backdrops to skirmish mission and permits to replace .dat files for a given mission.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_backdrops.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that adds starfield backdrops
At offset 17A3D9, replace 33DB85C08944241C0F8E4D0300008B74241469F63E0E0000 with 8D5C24145350E83CDB020083C408E9480300009090909090.

# To call the hook that loads mission
At offset 10F74A, replace E81154F0FF with E8E1870900.
At offset 11091F, replace E83C42F0FF with E80C760900.

# To call the hook that switches hyper buoy
At offset 008D51, replace E8EAE30800 with E8CAF11900.
At offset 005440, replace 8A0DF40D9100 with E8DB2A1A0090.
At offset 008D40, replace E89BF30800 with E8DBF11900.

# To call the hook that sets the backdrop scale
At offset 0165C0, replace D9442418D80DAC935A00E881341800 with E86B19190090909090909090909090.

# To call the hook that loads mission backdrops
At offset 015F14, replace 890DE0AC8000 with E80720190090.


*** Usage ***

Suppose that the mission is "[MissionDir]\[Mission].tie".

To replace dat images, create a file named "[MissionDir]\[Mission]_Resdata.txt" or create a section named "[Resdata]" in "[MissionDir]\[Mission].ini" and list your new dat files.
The format of this file is the same as "Resdata.txt".

To override the mission data, create a file named "[MissionDir]\[Mission].txt" or create a section named "[mission_tie]" in "[MissionDir]\[Mission].ini".
The format is a line per replacement.

To define the origin region for backdrops, the format is: "backdrop", backdrop fg, "from_region", region index.
The backdrop fg starts from 0.
The region index starts from 0.

To define the destination region name for buoys, the format is: "buoy", buoy fg, "region_name", region name.
The buoy fg starts from 0.

Defining arrival delay for backdrops is now supported.

To define the scale of the backdrops, edit the "Resdata\BackdropScales.txt" file.
You can also define the scales on a per-mission basis. Edit "[MissionDir]\[Mission]_BackdropScales.txt" or create a section named "[BackdropScales]" in "[MissionDir]\[Mission].ini".
If the per-mission file contains a setting named "ScalesFileName = value", for example "ScalesFileName = BackdropScalesFile.txt", then the value is the name of a file in the "Resdata" directory. When this setting exists and contains a file name, then the hook will read this file.
If no scale is defined for a backdrop then the global scale value is used.
The default scale value in vanilla game is 256.
To use higher sized backdrops use a smaller value.

To use backdrops between 104 and 255, the dat group id is between 6304 and 6455.

*** Credits ***

- Jérémy Ansel (JeremyaFr)
