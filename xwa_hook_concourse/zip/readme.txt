xwa_hook_concourse

This hook permits to modify the concourse items.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_concourse.dll, hook_concourse_net.dll and hook_concourse.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that loads medal details
At offset 162EB3, replace 8B048D58446000 with 51E86750040058.
At offset 162F75, replace 8B3C85C8426000 with 50E8A54F04005F.
At offset 163B52, replace A1C05E9F00 with E8C9430400.
At offset 162F3D, replace 0F84FC000000 with E8DE4F040090.
At offset 15CF8A, replace E84103F7FF with E8A1AF0400.
;At offset 162E5C, replace A1C05E9F00 with E8BF500400.
;At offset 16303F, replace 8B74241C with 83C444C3.
At offset 15DA40, replace 568D442408 with E8DBA40400.
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
At offset 13DCC2, replace 8B154114A200 with E859A2060090.
At offset 13DD08, replace 891D8DC0A100 with E813A2060090.
At offset 13A002, replace E8A9120600 with 33C0909090.
At offset 13A011, replace E89A120600 with 33C0909090.
At offset 13A024, replace 83C2372BD7 with E8F7DE0600.
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
;At offset 15DA6E, replace 35 with 64.
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
At offset 15D160, replace 686A3B0000 with E8BBAD0400.

# To call the hook that sets the rank points
At offset 02F4CD, replace 3B3495C0305B00 with 3B3495C0305B00.
At offset 02E7C2, replace 8B2C9DD42F5B00 with 8B2C9DD42F5B00.
At offset 02E841, replace F73C9DD42F5B00 with F73C9DD42F5B00.
At offset 02E943, replace 8B2C9DD42F5B00 with 8B2C9DD42F5B00.
At offset 02E9BF, replace F73C9DD42F5B00 with F73C9DD42F5B00.

# To call the hook that plays per-mission movies
At offset 176E88, replace E89341FEFF with E893100300.
At offset 1840C8, replace E8536FFDFF with E8533E0200.
At offset 176C44, replace E827A5FBFF with E8D7120300.
At offset 176ECD, replace E89EA2FBFF with E84E100300.

# To call the hook that defines emails
At offset 1285F6, replace B801000000 with E835F90700.
At offset 128D30, replace B801000000 with E8FBF10700.
At offset 12811B, replace E8F0200700 with E810FE0700.
At offset 162670, replace 893D104B9F00 with E8AB58040090.

# To call the hook that generates concourse starfield
At offset 1394CB, replace E8800B0000 with E860EA0600.
At offset 1394FE, replace E84D0C0000 with E82DEA0600.

# To call the hook that loads the front planet image
At offset 13936A, replace 0F8495000000 with E8B1EB060090.
At offset 139446, replace E8553C0200 with E8D5EA0600.

# To call the hook that draws the combat doors
At offset 13AB57, replace E80493FFFF with E8C4D30600.
At offset 13AC45, replace E81692FFFF with E8D6D20600.
At offset 13B3D9, replace E8828AFFFF with E842CB0600.
At offset 13B49D, replace E8BE89FFFF with E87ECA0600.
At offset 13AFE3, replace E8A8D00100 with E838CF0600.
At offset 13ADDC, replace E8AFD20100 with E83FD10600.
At offset 13B2FB, replace E890CD0100 with E820CC0600.

# To call the hook that plays the pod movie
At offset 172443, replace E8D88BFEFF with E8D85A0300.
At offset 13B549, replace E8D2FA0100 with E8D2C90600.
At offset 142CF9, replace E822830100 with E822520600.
At offset 1724EE, replace E87DECFBFF with E82D5A0300.

# To call the hook that removes panels
At offset 137C50, replace A10F7F9F00 with E8DB020700.

# To call the hook that draws hd concourse
At offset 15DC11, replace B8FFFFFF7F with E81AA30400.
At offset 136EB1, replace E87A32FFFF with E87A100700.
At offset 131487, replace E8B4540000 with E8A46A0700.
At offset 133E65, replace E8D62A0000 with E8C6400700.
At offset 134875, replace E8C6200000 with E8B6360700.
At offset 131755, replace E8E6510000 with E8D6670700.
At offset 132095, replace E8A6480000 with E8965E0700.
At offset 132FF5, replace E846390000 with E8364F0700.
At offset 132CC5, replace E8763C0000 with E866520700.
At offset 1557E2, replace E8C9280000 with E839270500.
At offset 15AE04, replace E897BBFFFF with E827D10400.
At offset 158190, replace 8B4424048B4808 with E88BFD04009090.
At offset 15838F, replace 8B45088B4808 with E88CFB040090.
At offset 1584D6, replace 8B45088B4808 with E845FA040090.
At offset 168BB5, replace E8269EFDFF with E866F30300.
At offset 168C78, replace E8639DFDFF with E8A3F20300.
At offset 168C0F, replace E87C9CFDFF with E80CF30300.
At offset 168CBF, replace E8CC9BFDFF with E85CF20300.
At offset 1420A1, replace C745F800080000 with E87A5E06009090.
At offset 1649A6, replace A0BCC7B000 with E885350400.
At offset 11C5D6, replace E8B5250200 with E845B90800.
At offset 167076, replace E8651F0000 with E8A50E0400.
At offset 158A13, replace E8B8030000 with E818F50400.
At offset 17457B, replace E8B0BBFCFF with E8B0390300.
At offset 172580, replace 391DEC3F78000F8463020000 with E89B59030090909090909090.
At offset 158FCF, replace E8BCF0FFFF with E84CEF0400.
At offset 14383A, replace 391DB03078000F8411010000 with E8E146060090909090909090.
At offset 1495AC, replace E8DF2D0000 with E86FE90500.

# To call the hook that defines interactive positions
At offset 15F6CA, replace E8C189FFFF with E851880400.
At offset 15F798, replace E8F388FFFF with E883870400.
At offset 15F876, replace E81588FFFF with E8A5860400.
At offset 15F9E0, replace E8AB86FFFF with E83B850400.
At offset 15F631, replace E82A48FDFF with E8FA880400.
At offset 163FFD, replace E85EFEFCFF with E82E3F0400.
At offset 16403A, replace E821FEFCFF with E8F13E0400.
At offset 1632B7, replace 0F841D010000 with E8644C040090.
At offset 1633C3, replace 833A17740D with E8584B0400.
At offset 163CC8, replace A18EDBAE00 with E863420400
At offset 1633E0, replace 0F844F020000 with E83B4B040090.
At offset 1634EF, replace 833A097410 with E82C4A0400.
At offset 163AD8, replace A1EED8AE00 with E853440400.
At offset 131584, replace 8038007518 with E897690700.
At offset 131735, replace 7C038D41FF with E8E6670700.

# To call the hook that draws hd text
At offset 155F5C, replace 8B04BD69BC9F00 with E8BF1F05009090.
At offset 156553, replace BEFB7F9F00 with E8C8190500.
At offset 1572EF, replace 8B048569BC9F00 with E82C0C05009090.
At offset 157329, replace 8B348569BC9F00 with E8F20B05009090.
At offset 156F4B, replace 8B3C8569BC9F00 with E8D00F05009090.
At offset 156D41, replace 8B34AD69BC9F00 with E8DA1105009090.
At offset 156B51, replace 8B2CB569BC9F00 with E8CA1305009090.
At offset 1569D1, replace 8B349D69BC9F00 with E84A1505009090.
At offset 156883, replace 8B348569BC9F00 with E8981605009090.
At offset 156743, replace 8B348569BC9F00 with E8D81705009090.
At offset 1565D5, replace 8B348569BC9F00 with E8461905009090.
At offset 11C91A, replace C744241C322E3032 with E801B60800909090.

# To call the hook that draws review briefing
At offset 1310C6, replace B8EF010000B909000000 with E8556E0700EB50909090.
At offset 13103D, replace E84E700200 with E8DE6E0700.
At offset 130FEA, replace E8A1700200 with E8316F0700.
At offset 130DC1, replace E8CA720200 with E85A710700.
At offset 130F00, replace E80B580200 with E81B700700.
At offset 130F8E, replace E87D570200 with E88D6F0700.
At offset 130FB8, replace E853570200 with E8636F0700.

# To call the hook that draws the mission selection screen
At offset 147BCF, replace 7507BBE6000000 with E84C0306009090.

# To call the hook that draws the battlezoom and kalidorzoom
At offset 183BC8, replace E89302FBFF with E853430200.
At offset 183C41, replace E81A02FBFF with E8DA420200.
At offset 183CD3, replace E8B844FDFF with E848420200.
At offset 1839B1, replace E8AA04FBFF with E86A450200.
At offset 183A3B, replace E85047FDFF with E8E0440200.
At offset 183AB9, replace E8A203FBFF with E862440200.
At offset 183B59, replace E8C22FFDFF with E8C2430200.
At offset 183A56, replace E8452FFDFF with E8C5440200.
At offset 183CED, replace E8AE2CFDFF with E82E420200.

# To call the hook that plays the ds brief movie
At offset 1683A5, replace E8C68DFCFF with E876FB0300.
At offset 164269, replace E812D2FCFF with E8B23C0400.
At offset 1678A4, replace E8879DFCFF with E877060400.
At offset 1679B2, replace E8A9C4FCFF with E869050400.
At offset 167CF0, replace E86BC1FCFF with E82B020400.

# To call the hook that plays the squad logo movie
At offset 176D70, replace E8FBA3FBFF with E8AB110300.
At offset 17697F, replace E8FCAAFBFF with E89C150300.
At offset 1771D7, replace E854A4FBFF with E8440D0300.
At offset 1771C8, replace E893CCFBFF with E8530D0300.
At offset 177323, replace E838CBFBFF with E8F80B0300.

# To call the hook that draws multi
At offset 170E52, replace E819DFFCFF with E8C9700300.
At offset 12A87F, replace E88C3B0100 with E89CD60700.

# To call the hook that draws the view cutscenes config menu
At offset 12656E, replace E81D1B0300 with E8AD190800.

# To call the hook that draws the medals ceremony screen
At offset 17E89C, replace A1CC4B7800 with E87F960200.


*** Usage ***

To set the medal details list, open "MedalDetailList.txt" and change the corresponding line to the desired value.
If the file does not exist, default values are used.
A line set to "null" means that the line is an empty string.
To set the medal drawing order, edit the "MedalDetailListOrder.txt" file.
To set the medal decription, edit the "FamilyMedalsDescriptions.txt" file.
See MedalDetailList.txt and FamilyMedalsDescriptions.txt.
The medal images are in "Frontend.dat". The first 100 images are from 15900-100 to 15900-199. The next images up to 255 are from 15900-2000 to 15900-2155.

To set the custom cutscenes list that are played on points levels, edit the "CustomMoviesOnPointsList.txt" file.
The format of the file is a line per cutscene:
points level, cutscene name
See CustomMoviesOnPointsList.txt.

To set the custom movies list that are played on pilot rank promotion, add it to the "RankCustomMovies.txt" file.
To set the custom movies list that are played on pilot rating promotion, add it to the "PilotRatingCustomMovies.txt" file.

To define the planet and the doors positions, edit the "ConcourseDoors.txt" file.
See ConcourseDoors.txt.
When a value is greater than 8000 it is divided by 10.
For examples:
- 1 means 1
- 10 means 10
- 8000 means 0.0 (8000-8000)/10
- 8100 means 10.0 (8100-8000)/10
- 8205 means 20.5 (8205-8000)/10

Suppose that the mission is "[MissionDir]\[Mission].tie".
To override the mission settings, create a file named "[MissionDir]\[Mission]_concourse.txt" or create a section named "[concourse]" in "[MissionDir]\[Mission].ini".
To define the front planet image index, set "FrontPlanetIndex = value". value is the planet index as defined in the "FrontPlanet.dat" file. If the setting is not defined or is -1 then the default game behavior is used.
To define the position of the front planet, set "FrontPlanetPositionX = valueX" and "FrontPlanetPositionY = valueY". When valueX or valueY is -1 then the default behavior is used.

To define animations for the concourse, edit the "ConcourseAnimations.txt" file.
The format is:
roomName, positionX, positionY, datGroupId, delayMin, delayMax, groupName, missionIds
roomName, positionX, positionY, positionWidth, positionHeight, datGroupId, delayMin, delayMax, groupName, missionIds
Each line defines an animation.
The valid values for roomName are: concourse.
When positionWidth or positionHeight is 0 then the dimensions of the animation file are used.
The datGroupId is the id of the image group in a dat file. The custom dat files are defined in the "Resdata.txt" file.
The delay between animations is random from delayMin to delayMax.
Only 1 animation per groupName is rendered at the same time. If the group name starts with a '+' character then the animation is looped until the room is reloaded.
To define the animations on a per-mission basis, set the missionIds setting. It is a space separated list. If an Id is -1 then the animation is played for all missions. If an Id begins with ~ then the animation is not played for the mission.

To define a sound for an animation,
- copy the sound .waw file in the "Sfx" directory
- reference the file in "Sfx\Sfx.lst"
The key for the sound is "concourse_anim_" followed by the image id. For example, if the id is 32002 then the key is "concourse_anim_32002".

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
To play a per-misson ceremony movie, create a movie named "Movies\[Mission]_ceremony.[ext]" where [ext] is "snm" or another supported movie format.
If a such movie doesn't exist, then "ceremony.[ext]" is played.

To play a random movie, create a "Movies\Movies.txt" file.
The format for a movie, the format is:
MissionName_MovieName = MovieA, MovieB, MovieC
or
MovieName = MovieA, MovieB, MovieC
To replace the start image, name it MovieA_start.cbm
To replace the end image, name it MovieA_end.cbm

Emails count is now unlimited. The emails status are written to a file named "<PilotName>_emails.bin" where "<PilotName>" is the name of the pilot.

To define which movie is played before entering the film, multi or single rooms, set the "FilmPodMovie", "MultiPodMovie" and "SinglePodMovie" settings. The default value is "pod".

To draw or not draw the panels in the simulator set the "DrawPanelsInSimulator" setting.
To draw or not draw the panels in the Tour of Duty briefing set the "DrawPanelsInTourOfDutyBriefing" setting.
To draw or not draw the panels in the Tour of Duty debriefing set the "DrawPanelsInTourOfDutyDebriefing" setting.
To draw or not draw the panels in the family monitor set the "DrawPanelsInFamilyMonitor" setting.
To draw or not draw the panels in the film room set the "DrawPanelsInFilmRoom" setting.
See "hook_concourse.cfg"

To enable or disable the hd concourse feature, set the "HDConcourseEnabled" setting in "hook_concourse.cfg".
Suppose that the dat file is "Resdata\[DatFileName].dat".
To replace sd images with hd ones, place them in "Resdata\[DatFileName]_hd.dat".
Suppose that the cbm file is "FrontRes\[FrontResSubDirectory\[CbmFileName].cbm".
To replace sd images with hd ones, place them in "FrontRes\[FrontResSubDirectory\[CbmFileName]_hd.dat" or in "FrontRes\[FrontResSubDirectory\[CbmFileName]_hd.cbm". The dat file is read before the cbm file.
To replace the text font, set the "HDConcourseTextFont" setting. The default value is "consolas".

To define whether the aspect ratio for the items on a background are wide or 4:3, edit the "ConcourseBackgroundsWide.txt" file.
The format is:
RelativePath = value
RelativePath is the path of the vanilla file.
The format is "RelativePath.cbm" for cbm images and "RelativePath.dat-GroupId-ImageId" for dat images.
value is 0 or 1
By default, the value for the loading screen is 0.
For examples:
; loading screen background
FRONTRES\CONFIG\DPMED.CBM = 0
; concourse background
RESDATA\FRONTEND.dat-15001-3100 = 1

To replace the dsbrief animation with a webm video, the filename is "Resdata\dsbrief.webm".
To replace the techdoor animation with a webm video, the filename is "Resdata\techdoor.webm".

To replace the squadlogo animation with a webm video, the filename is "Resdata\squad%d.webm".
The settings file is "Resdata\squad%d.txt". The format is:
PositionX = integer value
PositionY = integer value
Width = integer value
Height = integer value

# View configuration

Suppose that the craft is "FlightModels\[Model].opt".

To define the view configuration settings, create a file named "FlightModels\[Model]ViewConfiguration.txt" or create a section named "[ViewConfiguration]" in "FlightModels\[Model].ini".
The format is:
SkirmishOptScale = 1.0
SkirmishOptAnglePitch = 110.0
SkirmishOptAngleYaw = 225.0
SkirmishOptAngleRoll = 0.0
SkirmishOptPositionX = 0.5
SkirmishOptPositionY = 0.0
SkirmishOptPositionZ = 0.25

If the file does not exist, default values from "hook_concourse.cfg" or the "[hook_concourse]" section of "Hooks.ini" are used.

# Medal case test

To test the medal case after mission ceremony, set these settings in "hook_concourse.cfg" or the "[hook_concourse]" section of "Hooks.ini".

; 0 (disabled) or 1 (enabled)
TestMedalCase_Overwrite = 1

; Duration in seconds
TestMedalCase_Duration = 10

; BattleZoom medal index or 0 to not show
TestMedalCase_BattleZoomMedalIndex = 1

; KalidorZoom medal index or 0 to not show
TestMedalCase_KalidorZoomMedalIndex = 1


*** Credits ***

- Jérémy Ansel (JeremyaFr)
