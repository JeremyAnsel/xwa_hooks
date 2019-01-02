xwa_hook_engine_sound

This hook enables setting engine sound and weapon sound behavior for any craft.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11746


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_engine_sound.dll next to xwingalliance.exe


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

# To call the hook that defines weapon sounds
At offset 03B5DE, replace 8DB8E8FEFFFF83FF190F870801000033D28A972CC34300FF2495FCC24300 with 8B56230FBF929500000050525551E83FC9160083C4105F5E5DC390909090.


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
WeaponSoundBehavior = behavior value
Use a value of 0 to not select a sound. For engine sounds, it means that no sound is played. For weapon sounds, it means that a default sound is played (based on weapon type).

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

Add a line with "WeaponSoundBehavior = value". The value is 0, CorellianTransport, TieFighter, Rebel, RebelTurbo, Empire, EmpireTurbo, Falcon, FalconTurbo, Ion or IonTurbo.
0 means that a default sound is played.
CorellianTransport means that Laser and LaserTurbo are replaced with FalconLaser or FalconLaserTurbo.
TieFighter means that the open weapon (307) sound is EmpireLaserChChChhh or that laser sounds (RebelLaser, RebelLaserTurbo, EmpireLaser, EmpireLaserTurbo, IonCannon, IonCannonTurbo) are replaced with EmpireLaserChChChhh.
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

For the tie fighters (TieFighter, TieInterceptor, TieBomber, TieAdvanced, TieDefender, TieBizarro, TieBigGun, TieWarheads, TieBomb, TieBooster), laser sounds are replaced with "EmpireLaserChChChhh.wav".
For the Corellian transports (CorellianTransport2, FamilyTransport, MilleniumFalcon2), "LaserRebel" is replaced with "FalconLaser.wav" and "LaserRebelTurbo" is replaced with "FalconLaserTurbo.wav".

See EmptySound.txt and Examples\*Sound.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
