# XWA hooks

This repository contains hooks created for X-Wing Alliance.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11772

## xwa_hook_main

This is the main dll hook that is used to call other hooks. A hook is a way to inject code into the exe.

### xwa_hook_main_no_dinput

If you use a custom Dinput.dll, it will conflict with this hook. In this case, use xwa_hook_main_no_dinput instead of xwa_hook_main.

## xwa_hook_backdrops

This hook adds starfield backdrops to skirmish mission and permits to replace .dat files for a given mission.

## xwa_hook_countermeasures

This hook fixes countermeasures count when reloading.

## xwa_hook_d3dinfos_textures

This hook increases the D3DTextures count limit. When there are more than 1624 textures, the game outputs "Not enough D3DTextures" and uses the last created texture. This hook fixes that.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=10&t=11274

## xwa_hook_engine_sound

This hook enables setting engine sound and weapon sound behavior for any craft.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11746

## xwa_hook_hangars

This hook permits to customize the hangars.
The XWA's engine uses two hangar models, Hangar.opt and FamilyBase.opt. If you wanted to use a custom hangar, you had to replace one of them. You can now choose which hangar to use, customize the objects and the camera positions, for a specific craft or for a specific mission.
You can also define which crafts appear in the craft selection menu.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11197

## xwa_hook_hull_icon

This hook enables setting hull icon for any craft.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11761

## xwa_hook_interdictor

This hook permits to enable interdictor feature for any craft.

## xwa_hook_lightmaps

This hook replaces a static buffer by a dynamic one in lightmaps loading.

## xwa_hook_mission_objects

This hook permits to replace mission objects and to enable crafts turrets.

## xwa_hook_mission_tie

This hook permits to override the data of mission files.

## xwa_hook_opt_limit

This hook removes the opt limit of 512 vertices per mesh.

## xwa_hook_pilot

This hook enables pilot meshes animation for any craft.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11695

## xwa_hook_res1200

This hook replaces a static buffer with a dynamic one in initializing 2d buffer. It fixes a crash when resolution height > 1200 is used.

## xwa_hook_resolution

This hook displays the screen resolution in the video options screen.

## xwa_hook_sfoils

This hook enables S-Foils for any craft.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11693

## xwa_hook_shield

This hook permits to define shield recharge rates for any craft.

See https://www.xwaupgrade.com/phpBB3/viewtopic.php?f=33&t=12125

## xwa_hook_slam

This hook enables the SLAM system when the K key pressed.

See http://www.xwaupgrade.com/phpBB3/viewtopic.php?f=9&t=12050

## xwa_hook_time

This hook reduces the CPU usage and enables 60 FPS inflight.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=10&t=11338 and http://www.xwaupgrade.com/phpBB3/viewtopic.php?f=9&t=12016

## xwa_hook_weapon_color

This hook permits to define weapon color for any craft.

See http://www.xwaupgrade.com/phpBB3/viewtopic.php?p=156215#p156215

## xwa_hook_weapon_rate

This hook permits to define weapon decharge and recharge rates for any craft.

## xwa_hook_windowed

This hook enables windowed mode.

See http://www.xwaupgrade.com/phpBB3/viewtopic.php?f=10&t=12061

## xwa_hook_wingmen_voices

This hook enables setting wingmen voices for imperial IFF.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11770
