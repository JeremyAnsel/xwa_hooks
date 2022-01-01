xwa_hook_concourse

This hook permits to modify the concourse items.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_concourse.dll and hook_concourse.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that loads medal details
At offset 162EB3, replace 8B048D58446000 with 51E86750040058.
At offset 162F75, replace 8B3C85C8426000 with 50E8A54F04005F.
At offset 162F1B, replace E83090FFFF with E810500400.
At offset 162FD7, replace E8748FFFFF with E8544F0400.

# Skip the first 7 missions
At offset 138C4D, replace 743A with EB3A.
At offset 13928B, replace 7424 with EB24.
At offset 13A843, replace 7454 with EB54.
At offset 13A94D, replace 7426 with EB26.
At offset 13A98E, replace 7414 with EB14.
At offset 13AC90, replace 0F842D010000 with 90E92D010000.
At offset 13AEB7, replace 0F8410010000 with 90E910010000.
At offset 13B0B5, replace 0F842A020000 with 90E92A020000.
At offset 13B1C7, replace 0F8418010000 with 90E918010000.
At offset 13B36A, replace 7411 with EB11.
At offset 13BAD0, replace 7427 with EB27.
At offset 142D07, replace 744D with EB4D.
At offset 1447A7, replace 7425 with EB25.
At offset 1448A2, replace 7425 with EB25.
At offset 1449F0, replace 7424 with EB24.
At offset 15F68E, replace 740E with EB0E.
At offset 1604A3, replace 7421 with EB21.
At offset 160523, replace 7465 with EB65.
At offset 160CA6, replace 7421 with EB21.
At offset 160FFD, replace 7422 with EB22.
At offset 161093, replace 741C with EB1C.
At offset 16311A, replace 740C with EB0C.
At offset 163234, replace 7408 with EB08.
At offset 1639A8, replace 0F84B3000000 with 90E9B3000000.
At offset 163B26, replace 7408 with EB08.
At offset 163C19, replace 7408 with EB08.
At offset 164454, replace 7438 with EB38.
At offset 16539C, replace 7424 with EB24.
At offset 170B91, replace 7430 with EB30.
At offset 170C72, replace 7413 with EB13.

# To call the hook that plays custom movies on points level
At offset 110795, replace E8B6D3F1FF with E886770900.
At offset 17E1E7, replace E81493FDFF with E8349D0200.
At offset 02F512, replace 0F83CB070000 with 909090909090.
At offset 02F535, replace 3B2C95A8305B00720D with 55E8E58917005DEB0D.

# To call the hook that draws the concourse doors
At offset 139506, replace 68AE00000068E7000000 with E815EA0600EB7D909090.
At offset 1395BD, replace E80EEB0100 with E85EE90600.
At offset 13972B, replace E8A0E90100 with E8F0E70600.
At offset 1398B0, replace E81BE80100 with E86BE60600.
At offset 139A6C, replace E81FE60100 with E8AFE40600.
At offset 139BF2, replace E8D9E40100 with E829E30600.
At offset 139D6B, replace E860E30100 with E8B0E10600.

# To call the hook that defines medals
At offset 163563, replace 8B0CB5A04260008B17 with 5656E8B64904005A59.
At offset 163EA4, replace 8B14B5A04260008B07 with 5656E875400400585A.
At offset 16358D, replace 52E8BD89FFFF83C404 with 56E89D49040083C404.
At offset 1839CF, replace E87C85FDFF with E85C450200.
At offset 1635F8, replace E8D34AFFFF with E823490400.
At offset 163EF8, replace E863FFFCFF with E823400400.
At offset 15DAA4, replace 07 with 14.
At offset 15DA6E, replace 35 with 64.
At offset 15DBC5, replace 06 with 64.
At offset 163632, replace 06 with 64.
At offset 163CC5, replace 06 with 64.
At offset 163F03, replace 06 with 64.
At offset 16360C, replace 05A6030000 with 9090909090.
At offset 183AD2, replace 05A6030000 with 9090909090.
At offset 183C5E, replace 81C2A6030000 with 909090909090.
At offset 163614, replace E83789FFFF with E817490400.
At offset 183ADB, replace E87084FDFF with E850440200.
At offset 183C65, replace E8E682FDFF with E8C6420200.

# To call the hook that sets the battles and missions images count
At offset 15D16D, replace 6A35 with 6A7F.
At offset 15D183, replace 6A35 with 6A7F.
At offset 15D19C, replace 6A08 with 6A40.

# To call the hook that sets the rank points
At offset 02F4CD, replace 3B3495C0305B00 with 3B3495C0305B00.
At offset 02E7C2, replace 8B2C9DD42F5B00 with 8B2C9DD42F5B00.
At offset 02E841, replace F73C9DD42F5B00 with F73C9DD42F5B00.
At offset 02E943, replace 8B2C9DD42F5B00 with 8B2C9DD42F5B00.
At offset 02E9BF, replace F73C9DD42F5B00 with F73C9DD42F5B00.

# To call the hook that plays per-mission movies
At offset 176E88, replace E89341FEFF with E893100300.

# To call the hook that defines emails
At offset 1285F6, replace B801000000 with E835F90700.
At offset 128D30, replace B801000000 with E8FBF10700.
At offset 12811B, replace E8F0200700 with E810FE0700.
At offset 162670, replace 893D104B9F00 with E8AB58040090.


*** Usage ***

To set the medal details list, open "MedalDetailList.txt" and change the corresponding line to the desired value.
If the file does not exist, default values are used.
A line set to "null" means that the line is an empty string.
To set the medal decription, edit the "FamilyMedalsDescriptions.txt" file.
See MedalDetailList.txt and FamilyMedalsDescriptions.txt.

To set the custom cutscenes list that are played on points levels, edit the "CustomMoviesOnPointsList.txt" file.
The format of the file is a line per cutscene:
points level, cutscene name
See CustomMoviesOnPointsList.txt.

To define the doors positions, edit the "ConcourseDoors.txt" file.
See ConcourseDoors.txt.

To set the medals positions in the medals case, edit the "MedalsPositions.txt" file.
To set the medals descriptions, edit the "MedalsDescriptions.txt" file.
There can be up to 20 medals.
See MedalsPositions.txt and MedalsDescriptions.txt.
To set the position of the Kalidor Crescent, set the KalidorCrescentPositionX and KalidorCrescentPositionY settings in "hook_concourse.cfg".
To set the points of the Kalidor Crescent, edit the "KalidorCrescentPoints.txt" file.
To set the descriptions of the Kalidor Crescent, edit the "KalidorCrescentDescriptions.txt" file.
See KalidorCrescentPoints.txt and KalidorCrescentDescriptions.txt.

To set the points for the rank points, edit the "RankPoints.txt" file.
To set the points for the pilot rating points, edit the "PilotRatingPoints.txt" file.
See RankPoints.txt and PilotRatingPoints.txt.

To start in the concourse instead of the family room, before having completed the first 7 missions, set SkipFirst7Missions to 1 in hook_concourse.cfg.
See "hook_concourse.cfg"

Suppose that the mission is "[MissionDir]\[Mission].tie".
To play a per-misson briefing movie, create a movie named "Movies\[Mission]_briefroom.[ext]" where [ext] is "snm" or another supported movie format.
If a such movie doesn't exist, then "briefroom.[ext]" is played.

Emails count is now unlimited. The emails status are written to a file named "<PilotName>_emails.bin" where "<PilotName>" is the name of the pilot.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
