using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XwaJoystickConfig
{
    internal static class JoystickKeysResources
    {
        public static readonly string JoystickTxt = @"
!KEY_NONE!0 None No function
!KEY_A!97 A Target attacker of target
!KEY_B!98 B Beam weapon on/off
!KEY_C!99 C Fire countermeasure
!KEY_E!101 E Cycle through fighters targeting you
!KEY_F!102 F Toggle turret automatic firing
!KEY_G!103 G Toggle pilot/turret position
!KEY_I!105 I Target nearest incoming warhead
!KEY_J!106 J Jump to new craft if available
!KEY_K!107 K Toggle SLAM
!KEY_L!108 L Toggle padlock view on/off
!KEY_M!109 M Map mode
!KEY_N!110 N Target next hyper buoy
!KEY_O!111 O Target nearest objective craft
!KEY_P!112 P Target nearest player craft
!KEY_Q!113 Q Quit mission
!KEY_R!114 R Target nearest fighter
!KEY_S!115 S Cycle shield settings
!KEY_T!116 T Next target
!KEY_U!117 U Target newest craft
!KEY_V!118 V Toggle S-Foil
!KEY_W!119 W Cycle weapon settings
!KEY_X!120 X Cycle firing settings
!KEY_Y!121 Y Previous target
!KEY_Z!122 Z Toggle laser convergence
!KEY_1!49 1 Wingman command screen option 1
!KEY_2!50 2 Wingman command screen option 2
!KEY_3!51 3 Wingman command screen option 3
!KEY_4!52 4 Wingman command screen option 4
!KEY_5!53 5 Wingman command screen option 5
!KEY_6!54 6 Wingman command screen option 6
!KEY_7!55 7 Wingman command screen option 7
!KEY_8!56 8 Wingman command screen option 8
!KEY_9!57 9 Wingmain command screen option 9
!KEY_SHIFT_1!33 Shift-1 Custom Taunt #1
!KEY_SHIFT_2!64 Shift-2 Custom Taunt #2
!KEY_SHIFT_3!35 Shift-3 Custom Taunt #3
!KEY_SHIFT_4!36 Shift-4 Custom Taunt #4
!KEY_SHIFT_A!65 Shift-A Assign target to wingmen
!KEY_SHIFT_B!66 Shift-B Signal re-supply ship
!KEY_SHIFT_C!67 Shift-C Order wingmen to cover you
!KEY_SHIFT_D!68 Shift-D Dock with targeted craft
!KEY_SHIFT_E!69 Shift-E Order targeted craft to evade
!KEY_SHIFT_G!71 Shift-G Order waiting craft to go
!KEY_SHIFT_H!72 Shift-H Order targeted craft to go home
!KEY_SHIFT_I!73 Shift-I Order wingmen to ignore target
!KEY_SHIFT_P!80 Shift-P Pick up targeted craft
!KEY_SHIFT_R!82 Shift-R Release carried object.
!KEY_SHIFT_S!83 Shift-S Call for reinforcements
!KEY_SHIFT_W!87 Shift-W Order craft to stop/wait
!KEY_ALT_B!129 Alt-B Cycle brightness settings
!KEY_ALT_C!130 Alt-C Clear CMD
!KEY_ALT_D!131 Alt-D Cycle graphics detail settings
!KEY_ALT_E!132 Alt-E Eject
!KEY_ALT_J!137 Alt-J Flyby camera mode
!KEY_ALT_N!141 Alt-N Missile camera mode
!KEY_ALT_P!143 Alt-P Pause game
!KEY_ALT_S!146 Alt-S Toggle system messages on/off
!KEY_ALT_U!148 Alt-U Target camera mode
!KEY_ALT_V!149 Alt-V Displays game version #
!KEY_ALT_1!155 Alt-1 Pick target in sight
!KEY_ALT_2!156 Alt-2 Fire weapon
!KEY_ALT_3!157 Alt-3 Roll/Target ship in sights
!KEY_PAD_2!180 Pad-2 Virtual cockpit look down
!KEY_PAD_4!182 Pad-4 Virtual cockpit look left
!KEY_PAD_6!184 Pad-6 Virtual cockpit look right
!KEY_PAD_8!186 Pad-8 Virtual cockpit look up
!KEY_F1!195 F1 Target next friendly craft
!KEY_F2!196 F2 Target next neutral craft
!KEY_F3!197 F3 Target next enemy craft
!KEY_F4!198 F4 Target previous enemy craft
!KEY_F5!199 F5 Select target preset #1 as target
!KEY_F6!200 F6 Select target preset #2 as target
!KEY_F7!201 F7 Select target preset #3 as target
!KEY_F8!202 F8 Adjust beam recharge rate
!KEY_F9!203 F9 Adjust laser recharge rate 
!KEY_F10!204 F10 Adjust shield recharge rate
!KEY_F11!205 F11 Use power redirect preset #1
!KEY_F12!206 F12 Use power redirect preset #2
!KEY_SHIFT_F1!207 SHIFT-F1 Target previous friendly craft
!KEY_SHIFT_F2!208 SHIFT-F2 Target previous neutral craft
!KEY_SHIFT_F3!209 SHIFT-F3 Target previous enemy craft
!KEY_SHIFT_F5!211 SHIFT-F5 Save target in target preset #1
!KEY_SHIFT_F6!212 SHIFT-F6 Save target in target preset #2
!KEY_SHIFT_F7!213 SHIFT-F7 Save target in target preset #3
!KEY_SHIFT_F9!215 SHIFT-F9 Transfer shield energy to lasers
!KEY_SHIFT_F10!216 SHIFT-F10 Transfer laser energy to shields
!KEY_SHIFT_F11!217 SHIFT_F11 Save power redirect preset #1
!KEY_SHIFT_F12!218 SHIFT_F12 Save power redirect preset #2
!KEY_TAB!9 Tab Bring up wingman command screen in MFD
!KEY_ESCAPE!27 Escape Options screen
!KEY_SPACE!32 Space Confirm critical orders
!KEY_ENTER!13 Enter Match targeted craft's speed
!KEY_DELETE!169 Delete Toggle left MFD
!KEY_PAGEDOWN!173 PageDown Toggle right MFD
!KEY_END!171 End Toggle CMD
!KEY_HOME!170 Home Center indicators
!KEY_INSERT!168 Insert Toggle Left Sensor/Shield Indicator
!KEY_PAGEUP!172 PageUp Toggle Right Sensor/Beam Indicator
!KEY_BACKSPACE!8 Backspace Full throttle
!KEY_EQUAL!61 =(Equal) Increase throttle
!KEY_MINUS!45 -(Minus) Decrease throttle
!KEY_FOWARD_SLASH!92 \ Zero throttle
!KEY_LEFT_BRACKET!91 [ 1/3 throttle
!KEY_RIGHT_BRACKET!93 ] 2/3 throttle
!KEY_SEMICOLON!59 ;(Semicolon) Transfer shield energy to lasers
!KEY_APOSTROPHE!39 '(Apostrophe) Transfer laser energy to shields
!KEY_QUOTES!34 ""(Quotes) Transfer all laser energy to shields
!KEY_PERIOD!46 .(Period) Toggle cockpit on/off
!KEY_COMMA!44 ,(Comma) Cycle through target's components
!KEY_LESS_THAN!60 < Reverse cycle through target's components
!KEY_PAD_MINUS!189 Pad-/ External camera mode
!KEY_PAD_STAR!190 Pad-* Reposition camera in external mode
!KEY_SCROLL_LOCK!175 Scroll-Lock Toggle mouse look mode
!KEY_CTRL_L!282 Ctrl-L Toggle Landing Gears
";
    }
}
