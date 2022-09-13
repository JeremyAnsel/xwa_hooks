==================================================================================
X-Wing Alliance Tour/Combat Engagements hook for Multiplayer by Justagai (v 2.1.2)
==================================================================================

===============================
CHANGELOG
===============================

2.1.2

- Added config option to change IFF colors to green for friendlies and red for enemies

2.1.1

- Fixed MFD Wingmen issue

2.1
- Fixed a major bug where not all missions were listed on the load screen
- Removed Dinput.dll, tactical officer and wingmen voices hooks (download them seperately)

2.0a
- Tactical Officer Hook now included

Bugs fixed/Features added:
- Combat and Racing buttons are now seperate and in different positions
- Craft selection has been fixed for both hosts and clients
- Load button on load screen is finally fixed
- Delete button removed from load screen unless its Skirmish
- Setting buttons are realigned properly
- IFF 4/Orange is now orange in the Craft and message lists
- Craft Selection can now be like XvT if the proper options are selected in the mission FG
- New quit message shown if player is to be captured during a mission
- Config now has three seperate options to change combat sim background, music and setting bar
- Hangar in Multiplayer now acts like the hangar in XvT (which is pop in and pop out with a wave loss)
- You can allow the AI to take over your craft with "Shift+M"
- You can now cycle regions if spectating in Multiplayer using shift+M(previous) and M(next) keys
- Server update rate options are now 8, 29 and 59
- Tour briefings in story mode will not use the custom background provided by the hook
- Target box color now changes to the target IFF
- Arrows in battle menus have returned and are fully functional


Features removed:
- Tactical officer code has been removed so that it can be placed in its own hook
- Mission description button removed

1.3
- Updated patch to work with Steam XWA Vanilla

1.2.1

- Updated Combat missions to make use of different tactical officers
- Included hook_tourmultiplayer.cfg
- Disabling xvt combat simulator background also restores original music

1.2

- Tactical Officers 3-7 can now be loaded as well as having different tactical officers between Team 1 and Team 2
- Included XvT Imperial Tactical officers
- Clients on the roster screen will now see the correct FG list
- Fixed bug where the mission list scroll bar would move to the bottom in Skirmish mode
- Added Mission Description button to UI
- Imperial Wingmen voices have been removed from my hook and are now controlled by Jeremy's wingmen voices hook
- Combat Engagements mode now has the correct UI buttons on the roster screen
- Fixed a bug where the wingmen menu would be grayed out if the player had multiple waves
- Fixed bug where Skirmish mode was loaded on the roster after exiting a Tour mission in Multiplayer

1.1

- Can disable the xvt combat simulator background in hook_tourmultiplayer.cfg
- Readme updated

1.0

- Patch is now a dll hook
- A ton of visual bugs fixed
- Allows waves/craft jumping for players for any gamemode set from mission file
- New pilots can now access Training Exercises even if they haven't completed the first campaign mission
- Removed white targeting boxes around friendlies in Skirmish 
- Changed winning team melee targeting box color from purple to dark blue
- "m" key will allow players to cycle regions if they are out of play (out of waves or pressed q and space)
- Changed color of targeting box to white
- Changed color of Camera targeting box to blueish white
- Changed IFF 4 (Imperial Traitor) color from red to orange
- Changed Player targeting box/waypoint color of IFF 4 (Imperial Traitor) to orange
- Changed Player targeting box/waypoint color of IFF 1 (Imperial) from orange to a bright red (like in XvT)
- Changed Player targeting box/waypoint color of IFF 5 (Purple) from yellow to purple
- Changed standard waypoint color from red to white
- FG names on roster are now correct (only for the host, clients currently still see the incorrect names)
- Selecting waves for a FG as host in a gamemode other than Skirmish no longer sets it to 0
- Scroll button no longer goes to the bottom of the screen when clicking on the load button
- Removed mission select arrows (they are buggy and are made obsolete with the load button anyway)
- Combat simulator now has a different background
- Music in combat simulator has been changed
- Tour button is now called "Training Exercises" button with a different look
- AI rank is no longer shown on roster unless its Skirmish
- S-foils are removed from the Assault Gunboat, Missile Boat and Skipray Blastboat (Compatible with JeremyFr's sfoils hook, sfoils will still work on those ships)
- Internet mode is now off by default when selecting/loading into a TCP/IP game
- Server update rate is now 30 by default, can be set to 8 by selecting "low" in the network settings
- Allows Imperial wingmen to talk (code from my wingmen patch is implemented with a few fixes, not compatible with JeremyFr's wingmen hook)
- Temporary fix for hangar in multiplayer (will have a better solution later)
- Fixed bug in Combat Engagement mission 2 where the autopilot takes over

===============================
WHAT IT DOES
===============================
- This patch allows you to use the built in campaign/tour mode within XWA to play the story campaign with other players, or fly custom campaigns
- also adds/fixes a ton of other improvements

===============================
FEATURES
===============================

- Allows Combat Engagements mode to be selected
- Buttons are added to access these modes and load missions
- Fixes briefing crash issue when loading multiplayer missions in singleplayer
- Adds the singleplayer XWA campaign that can be played with a total of 8 players
- Adds some Combat Engagement missions from Balance of Power
- Changes a few visuals to make it easier to pick out targets
- Compatible with XWAUP/DSUCP
- and much more

===============================
HOW TO USE
===============================
- Move all the contents to your XWA folder. Overwrite if asked.

- To load a mission, click the load button in the combat simulator and then double click on the desired mission.

- If you wish to have the original Combat Simulator background, set "xvtsimulatorbackground" to "= 0" in hook_tourmultiplayer.cfg or change it to "= 1" to turn it back on.


===============================
TACTICAL OFFICERS
===============================
- Has been removed and will be placed in its own hook.

===============================
NOTES/ISSUES
===============================

Missions
--------------
- Missions with a "*" in the MISSION.LST file must have the previous mission completed before moving on in the campaign. Basically the "*" notes progression.

- Missions that are added to the MISSION.LST file without a "*" can be loaded regardless if they were completed or not.

- No longer do you need to set the mission type to Skirmish with a mission editor to have multiple waves/craft jumping. Waves/Craft jumping is allowed in all mission types. I actually recommend that you don't set it to Skirmish since it does other things to all FGs in a mission (such as adding 200% shields to fighters/shuttles by default).

- Final Death star mission does not work in multiplayer at this point.



Multiplayer
--------------
- It is recommended that all players have the hook installed. Blue bars may appear otherwise. 

- If you installed my Muliplayer exe tour patch, revert the patch, then install the hook.

- XWA installations must be almost exactly the same between players to avoid most desync errors. So if the XWAUP is installed for example then every player must have it installed with the same checkboxes the host did during the install.

- The hangar is fully fixed so it is ok to use.


General
--------------
- Difficulty setting does affect Combat engagements. It has the same effects like that in Tour mode. The effects are as follows:

Easy: Buffs the team with the higher player rank. Buffs AI rank by two ranks. Debuffs the team by one AI rank with lower ranked players.
I do not recommend using Easy to play combat engagements unless all players are on one team and you want it easy.

Medium: Essentially plays like "Neutral" in XvT. AI rank is whatever is set in the mission by default. 

Hard: Like the "autobalance" setting in XvT. Game will buff the AI rank of the team with the lower player rank by one. May or may not debuff team with the higher rank players.

===============================
Q & A (Troubleshooting)
===============================

Q: I'm getting a blue bar at the start of the mission in multiplayer!
A: The game is syncing all players when the mission first starts. If it continues for longer than 30 seconds, then check to make sure that your XWA installations are the same. I recommend uploading your XWA folder to the cloud (Google Drive, Onedrive, Dropbox, etc) and have other players download it if you are having trouble solving this issue.

Q: The game says I need to put in a CD when I click on Combat Engagements!
A: Make sure there is a folder called "COMBAT" is in your XWA folder with a mission.lst and missions in it.

Q: I'm on the map and out of waves but I can't see the other players!
A: You're most likely viewing a region where the other players aren't. Press the "m" or "Shift+m" key to cycle regions until you find your fellow players.

Q: I get a blue bar when I'm playing the last Death Star mission with other players!
A: The last Death Star mission is not flyable in multiplayer yet (and it may never be, so don't get your hopes up).

Q: I'm seeing some weird stuff going on in the roster!
A: If you installed my Multiplayer exe patch before, you will have to revert it.

Q: The game says it couldn't load the dinput hook!
A: Check to see if your anti-virus is blocking it. You may have to make an exception.





===============================
CONTACT
===============================

My email is justagai101@gmail.com

You may also contact me on the XWAUP forums.

For those who are looking for pilots to play mutliplayer with:

XvT/XWA Multiplayer/Pilot general hangout Discord Server (all pilots welcome) https://discord.gg/feSxXh8


===============================
CREDITS
===============================

- JeremyFr for without his work on the hooks this wouldn't have been possible
- All testers who helped test this


Have a nice flight! And may the Force be with you!

- Justagai