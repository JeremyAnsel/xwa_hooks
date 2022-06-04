xwa_hook_engine_sound

This hook enables setting engine sound and weapon sound behavior for any craft.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11746


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_engine_sound.dll next to xwingalliance.exe

To setup Sfx_EngineWash, copy the following files to WAVE\Sfx_EngineWash\
From WAVE\FE_HIGH_RES\
Copy files:
EngineWash.wav
EngineWashSD.wav

To setup Sfx_TakeOff, copy the following files to WAVE\Sfx_TakeOff\
From WAVE\FE_HIGH_RES\
Copy files:
LandingGearUp.wav
MfTakeOff.wav

To setup Sfx_EngineSlowing, copy the following files to WAVE\Sfx_EngineSlowing\
From WAVE\FE_HIGH_RES\
Copy files:
LandingGearDown.wav

To setup Sfx_CanopyOpening, copy the following files to WAVE\Sfx_CanopyOpening\
From WAVE\FE_HIGH_RES\
Copy files:
LandingGearDown.wav

To setup Sfx_FlyBy, copy the following files to WAVE\Sfx_FlyBy\
From WAVE\FE_HIGH_RES\
Copy files:
FlyByAG.wav
FlyByAW.wav
FlyByCORT.wav
FlyByTI.wav
FlyByXW.wav
FlyByYW.wav

To setup Sfx_Interior, copy the following files to WAVE\Sfx_Interior\
From WAVE\FE_HIGH_RES\
Copy files:
EngineIntAG.wav
EngineIntAW.wav
EngineIntCORT.wav
EngineIntFALC.wav
EngineIntTI.wav
EngineIntXW.wav
EngineIntYW.wav

To setup Sfx_Weapon, copy the following files to WAVE\Sfx_Weapon\
From WAVE\FE_HIGH_RES\
Copy files:
EmpireLaser.wav
EmpireLaserChChChhh.wav
EmpireLaserStarship.wav
EmpireLaserTurbo.wav
FalconLaser.wav
FalconLaserTurbo.wav
IonCannon.wav
IonCannonStarship.wav
IonCannonTurbo.wav
RebelLaser.wav
RebelLaserStarship.wav
RebelLaserTurbo.wav


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that replaces mission sounds
At offset 0DA120, replace 8B44240C8B4C2408 with E80BDE0C00C39090.

# To call the hook that defines engine sounds
At offset 03C4F5, replace 8A543A02885424108B54241081E2FF000000 with 668B543A0281E2FFFF000089542410909090.
At offset 03C572, replace 8A5424108815B0D06300 with 8B5424108915B0D06300.
At offset 03C746, replace 25FF000000 with 9090909090.
At offset 03C507, replace 8D6AFF83FD40775633D28A9558D44300FF2495FCD34300 with 536A005652E80FBA160083C4045E5D5BEB509090909090.
At offset 03C74B, replace 4883F840773E33C98A88F8D44300 with 5650E8CEB7160083C4045EEB3790.
At offset 03CDFA, replace 4883F8400F87D500000033DB8A98C4DC4300 with 5650E81FB1160083C4045EE9CF0000009090.
At offset 05826F, replace 4983F9400F87CF00000033D28A9178964500 with 5051E8AAFC140083C40458E9C90000009090.
At offset 039725, replace 7515 with 9090.
At offset 039733, replace 7726 with EB26.
At offset 03CC6F, replace 3D89000000741A3D8A000000 with 50E8BBB216008BF858EB1B90.
At offset 03CD3C, replace 52E80EEF090083C404 with E8DFB11600EB369090.
At offset 05ABD0, replace 6683F93A74266683F941 with E84BD31400EB20909090.
At offset 05B2E4, replace E8A700FEFF with E837CC1400.
At offset 05B489, replace E802FFFDFF with E892CA1400.

# To call the hook that defines weapon sounds
At offset 03B5DE, replace 8DB8E8FEFFFF83FF190F870801000033D28A972CC34300FF2495FCC24300 with 8B56230FBF929500000050525551E83FC9160083C4105F5E5DC390909090.

# To call the hook that defines hyper sounds
At offset 00326A, replace E821810300 with E8B14C1A00.
At offset 003353, replace 7528 with 9090.
At offset 003375, replace E816800300 with E8A64B1A00.


*** Usage ***

# mission sounds
Suppose that the mission is "[MissionDir]\[Mission].tie".

To replace sounds for a given mission, create a file named "[MissionDir]\[Mission]_Sounds.txt" or create a section named "[Sounds]" in "[MissionDir]\[Mission].ini".

If a "Sounds.txt" file doesn't exist, no sound is replaced.
The format is a line per sound: "Wave\SoundA.wav = Wave\NewSoundA.wav"
See "Sounds.txt".

# craft and weapon sounds
Suppose that the craft is "FlightModels\[Model].opt".

To define which sound is played, create a file named "FlightModels\[Model]Sound.txt" or create a section named "[Sound]" in "FlightModels\[Model].ini".
The format is:
EngineSoundInterior = interior sound type
EngineSoundFlyBy = flyby sound type
EngineSoundWash = wash sound type
EngineSoundTakeOff = take off sound type
WeaponSoundBehavior = behavior value
Use a value of 0 to not select a sound.
For interior and flyby sounds, it means that no sound is played.
For wash sounds, it means that the default sound is played.
For take off sounds, it means that the default sound is played.
For weapon sounds, it means that a default sound is played (based on weapon type).

Add a line whith "EngineSoundInterior = value". The value is between 0 and 7:
- 0 means no sound
- 1 means EngineIntXW
- 2 means EngineIntYW
- 3 means EngineIntAW
- 4 means EngineIntTI
- 5 means EngineIntAG
- 6 means EngineIntCort
- 7 means EngineIntFalc

Add a line with "EngineSoundFlyBy = value". The value is between 0 and 8, 11 and 18, 21 and 28:
- 0 means no sound
- 1 means FlyByXW or FlyByXW3D
- 2 means FlyByYW or FlyByYW3D
- 3 means FlyByAW or FlyByAW3D
- 4 means FlyByTI or FlyByTI3D
- 5 means FlyByAG or FlyByAG3D
- 6 means FlyByCort or FlyByCort3D
- 7 means FlyByCort or FlyByCort3D
- 8 means custom (first unused/FlyBySlave) or custom3D (second unused/FlyBySlave3D)
- 11 means FlyByXW
- 12 means FlyByYW
- 13 means FlyByAW
- 14 means FlyByTI
- 15 means FlyByAG
- 16 means FlyByCort
- 17 means FlyByCort
- 18 means custom (first unused/FlyBySlave)
- 21 means FlyByXW3D
- 22 means FlyByYW3D
- 23 means FlyByAW3D
- 24 means FlyByTI3D
- 25 means FlyByAG3D
- 26 means FlyByCort3D
- 27 means FlyByCort3D
- 28 means custom3D (second unused/FlyBySlave3D)

Add a line with "EngineSoundWash = value". The value is between 1 and 2:
- 1 means EngineWash
- 2 means EngineWashSD

Add a line with "EngineSoundTakeOff = value". The value is between 1 and 2:
- 1 means LandingGearUp
- 2 means MfTakeOff

Add a line with "WeaponSoundBehavior = value". The value is 0, CorellianTransport, TieFighter, TieFighterWithIon, Rebel, RebelTurbo, Empire, EmpireTurbo, Falcon, FalconTurbo, Ion or IonTurbo.
0 means that a default sound is played.
CorellianTransport means that Laser and LaserTurbo are replaced with FalconLaser or FalconLaserTurbo.
TieFighter means that the open weapon (307) sound is EmpireLaserChChChhh or that laser sounds (RebelLaser, RebelLaserTurbo, EmpireLaser, EmpireLaserTurbo, IonCannon, IonCannonTurbo) are replaced with EmpireLaserChChChhh.
TieFighterWithIon means that the open weapon (307) sound is EmpireLaserChChChhh or that laser sounds (RebelLaser, RebelLaserTurbo, EmpireLaser, EmpireLaserTurbo) are replaced with EmpireLaserChChChhh or ion lasers (IonCannon, IonCannonTurbo) are not replaced.
Rebel means that the open weapon (307) sound is RebelLaser or that RebelLaser and RebelLaserTurbo are replaced with RebelLaser.
RebelTurbo means that the open weapon (307) sound is RebelLaserTurbo or that RebelLaser and RebelLaserTurbo are replaced with RebelLaserTurbo.
Empire means that the open weapon (307) sound is EmpireLaser or that EmpireLaser and EmpireLaserTurbo are replaced with EmpireLaser.
EmpireTurbo means that the open weapon (307) sound is EmpireLaserTurbo or that EmpireLaser and EmpireLaserTurbo are replaced with EmpireLaserTurbo.
Falcon means that the open weapon (307) sound is FalconLaser or that RebelLaser, RebelLaserTurbo, EmpireLaser and EmpireLaserTurbo are replaced with FalconLaser.
FalconTurbo means that the open weapon (307) sound is FalconLaserTurbo or that RebelLaser, RebelLaserTurbo, EmpireLaser and EmpireLaserTurbo are replaced with FalconLaserTurbo.
Ion means that the open weapon (307) sound is IonCannon or that IonCannon and IonCannonTurbo are replaced with IonCannon.
IonTurbo means that the open weapon (307) sound is IonCannonTurbo or that IonCannon and IonCannonTurbo are replaced with IonCannonTurbo.

If the file does not exist, default values are used:

Interior sound:
type 1:
- ModelIndex_001_0_0_Xwing
- ModelIndex_004_0_3_Bwing
- ModelIndex_014_0_13_Z_95
- ModelIndex_015_0_14_R41
- ModelIndex_031_0_30_SlaveOne
type 2:
- ModelIndex_002_0_1_Ywing
- ModelIndex_011_0_10_ToscanFighter
- ModelIndex_023_0_22_CloakshapeFighter
type 3:
- ModelIndex_003_0_2_Awing
- ModelIndex_010_0_9_IrdFighter
- ModelIndex_013_0_12_Twing
- ModelIndex_027_0_26_Piggyback
type 4:
- ModelIndex_005_0_4_TieFighter
- ModelIndex_006_0_5_TieInterceptor
- ModelIndex_007_0_6_TieBomber
- ModelIndex_008_0_7_TieAdvanced
- ModelIndex_009_0_8_TieDefender
type 5:
- ModelIndex_012_0_11_MissileBoat
- ModelIndex_016_0_15_AssaultGunboat
- ModelIndex_024_0_23_RazorFighter
- ModelIndex_025_0_24_PlanetaryFighter
- ModelIndex_029_0_28_PreybirdFighter
type 6:
- ModelIndex_017_0_16_SkiprayBlastBoat
- ModelIndex_058_0_45_CorellianTransport2
- ModelIndex_065_0_52_FamilyTransport
type 7:
- ModelIndex_026_0_25_SupaFighter
- ModelIndex_032_0_31_SlaveTwo
- ModelIndex_059_0_46_MilleniumFalcon2

FlyBy sound:
type 1:
- ModelIndex_001_0_0_Xwing
- ModelIndex_004_0_3_Bwing
- ModelIndex_014_0_13_Z_95
- ModelIndex_015_0_14_R41
- ModelIndex_031_0_30_SlaveOne
type 2:
- ModelIndex_002_0_1_Ywing
- ModelIndex_011_0_10_ToscanFighter
- ModelIndex_023_0_22_CloakshapeFighter
type 3:
- ModelIndex_003_0_2_Awing
- ModelIndex_010_0_9_IrdFighter
- ModelIndex_013_0_12_Twing
- ModelIndex_027_0_26_Piggyback
type 4:
- ModelIndex_005_0_4_TieFighter
- ModelIndex_006_0_5_TieInterceptor
- ModelIndex_007_0_6_TieBomber
- ModelIndex_008_0_7_TieAdvanced
- ModelIndex_009_0_8_TieDefender
type 5:
- ModelIndex_012_0_11_MissileBoat
- ModelIndex_016_0_15_AssaultGunboat
- ModelIndex_024_0_23_RazorFighter
- ModelIndex_025_0_24_PlanetaryFighter
- ModelIndex_029_0_28_PreybirdFighter
- ModelIndex_043_0_32_Tug
- ModelIndex_044_0_33_CombatUtilityVehicle
- ModelIndex_045_0_34_HeavyLifter
- ModelIndex_050_0_37_Shuttle
- ModelIndex_051_0_38_EscortShuttle
- ModelIndex_052_0_39_StormtrooperTransport
- ModelIndex_053_0_40_AssaultTransport
- ModelIndex_054_0_41_EscortTransport
- ModelIndex_055_0_42_SystemPatrolCraft
- ModelIndex_056_0_43_ScoutCraft
type 6:
- ModelIndex_017_0_16_SkiprayBlastBoat
- ModelIndex_058_0_45_CorellianTransport2
- ModelIndex_065_0_52_FamilyTransport
type 7:
- ModelIndex_026_0_25_SupaFighter
- ModelIndex_032_0_31_SlaveTwo
- ModelIndex_059_0_46_MilleniumFalcon2

Wash sound:
type 2:
- ModelIndex_137_0_91_Interdictor2
- ModelIndex_138_0_92_VictoryStarDestroyer2
- ModelIndex_139_0_93_ImperialStarDestroyer2
- ModelIndex_140_0_94_SuperStarDestroyer
type 1:
other crafts

Take Off sound:
type 2:
- ModelIndex_058_0_45_CorellianTransport2
- ModelIndex_059_0_46_MilleniumFalcon2
- ModelIndex_065_0_52_FamilyTransport
type 1:
other crafts

For the tie fighters (TieFighter, TieInterceptor, TieBomber, TieAdvanced, TieDefender, TieBizarro, TieBigGun, TieWarheads, TieBomb, TieBooster), laser sounds are replaced with "EmpireLaserChChChhh.wav".
For the Corellian transports (CorellianTransport2, FamilyTransport, MilleniumFalcon2), "LaserRebel" is replaced with "FalconLaser.wav" and "LaserRebelTurbo" is replaced with "FalconLaserTurbo.wav".

To define which sound is played when entering hyperspace, set the "HyperSoundBehavior" value.
A value of -1 means that the sound is based on the player Iff.
A value of 0 means that the "HyperStartReb" sound is played.
A value of 1 means that the "HyperStartImp" sound is played.
A value of 2 means that the "HyperStartImp" and the "HyperZoomImp" sounds are played.


See EmptySound.txt and Examples\*Sound.txt.


For Interior sounds, when the type is 0, sounds defined in "Wave\Sfx_Interior.lst" are used if the lst file exists. The sounds are indexed by the crafts model index.
For FlyBy sounds, when the type is 0, sounds defined in "Wave\Sfx_FlyBy.lst" are used if the lst file exists. The sounds are indexed by the crafts model index.
For EngineWash sounds, when the type is 0, sounds defined in "Wave\Sfx_EngineWash.lst" are used if the lst file exists. The sounds are indexed by the crafts model index.
For TakeOff sounds, when the type is 0, sounds defined in "Wave\Sfx_TakeOff.lst" are used if the lst file exists. The sounds are indexed by the crafts model index.
For Engine Slowing sounds, sounds defined in "Wave\Sfx_EngineSlowing.lst" are used if the lst file exists. The sounds are indexed by the crafts model index.
For Canopy Opening sounds, sounds defined in "Wave\Sfx_CanopyOpening.lst" are used if the lst file exists. The sounds are indexed by the crafts model index.

For weapon sounds, when the behavior is empty, sounds defined in "Wave\Sfx_Weapon.lst" are used if the lst file exists.
For each craft, there are 10 items. The last item is for the open weapon.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
