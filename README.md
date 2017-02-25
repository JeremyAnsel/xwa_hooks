# XWA hooks

This repository contains hooks created for X-Wing Alliance.

## xwa_hook_main

This is the main dll hook that is used to call other hooks. A hook is a way to inject code into the exe.

## xwa_hook_backdrops

This hook adds starfield backdrops to skirmish mission.

## xwa_hook_d3dinfos_textures

This hook increases the D3DTextures count limit. When there are more than 1624 textures, the game outputs "Not enough D3DTextures" and uses the last created texture. This hook fixes that.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=10&t=11274

## xwa_hook_hangars

This hook permits to customize the hangars. The XWA's engine uses two hangar models, Hangar.opt and FamilyBase.opt. If you wanted to use a custom hangar, you had to replace one of them. You can now choose which hangar to use, customize the objects and the camera positions, for a specific craft or for a specific mission.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11197

## xwa_hook_lightmaps

This hook replaces a static buffer by a dynamic one in lightmaps loading.

## xwa_hook_pilot

This hook enables pilot meshes animation for any craft.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11695

## xwa_hook_sfoils

This hook enables S-Foils for any craft.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11693

## xwa_hook_time

This hook reduces the CPU usage of the game. A side effect is that the framerate is a bit reduced.

See http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=10&t=11338
