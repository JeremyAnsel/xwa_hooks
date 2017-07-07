xwa_hook_engine_sound

This hook enables setting engine sound and weapon sound behavior for any craft.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

1) add hook_engine_sound.dll to dinput_hooks.lst

2) place hook_engine_sound.dll next to xwingalliance.exe

3) edit xwingalliance.exe:

You can manually apply the modifications with an hexadecimal editor, or you can use XwaExePatcher to have a GUI.

# To call the hook that defines engine sounds
At offset 03C4F5, replace 8A543A02885424108B54241081E2FF000000 with 668B543A0281E2FFFF000089542410909090.
At offset 03C572, replace 8A5424108815B0D06300 with 8B5424108915B0D06300.
At offset 03C746, replace 25FF000000 with 9090909090.
At offset 03C507, replace 8D6AFF83FD40775633D28A9558D44300FF2495FCD34300 with 536A005652E80FBA160083C4045E5D5BEB509090909090.
At offset 03C74B, replace 4883F840773E33C98A88F8D44300 with 5650E8CEB7160083C4045EEB3790.
At offset 03CDFA, replace 4883F8400F87D500000033DB8A98C4DC4300 with 5650E81FB1160083C4045EE9CF0000009090.
At offset 05826F, replace 4983F9400F87CF00000033D28A9178964500 with 5051E8AAFC140083C40458E9C90000009090.

# To call the hook that defines weapon sounds
At offset 03B5DE, replace 8DB8E8FEFFFF83FF190F870801000033D28A972CC34300FF2495FCC24300 with 8B56230FBF929500000050525551E83FC9160083C4105F5E5DC390909090.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To define which sound is played, create a file named "FlightModels\[Model]Sound.txt";
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

Add a line with "EngineSoundFlyBy = value". The value is between 0 and 7:
- 0 means no sound
- 1 means FlyByXW or FlyByXW3D
- 2 means FlyByYW or FlyByYW3D
- 3 means FlyByAW or FlyByAW3D
- 4 means FlyByTI or FlyByTI3D
- 5 means FlyByAG or FlyByAG3D
- 6 means FlyByCort or FlyByCort3D
- 7 means FlyByCort or FlyByCort3D
- 8 means custom (first unused) or custom3D (second unused)

Add a line with "WeaponSoundBehavior = value". The value is 0, CorellianTransport or TieFighter.

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
