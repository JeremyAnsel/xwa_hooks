xwa_hook_time

This hook reduces the CPU usage and enables 60 FPS inflight.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=10&t=11338


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_time.dll and hook_time.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that reduces CPU usage
At offset 10D810, replace E82BF8FFFF8B with E81BA70900C3.
At offset 10D830, replace E80BF8FFFF8B with E8FBA60900C3.
At offset 13E328, replace 99F7FE with 33C040.
At offset 13EC26, replace F77C2404 with 33C04090.
At offset 13FED3, replace A142709F0085C0750EEBC1 with 6A30FF1574905A0090EB0C.
At offset 13DEEF, replace 0F844AFDFFFF with 909090909090.
At offset 13F262, replace 0F84C1FEFFFF with 909090909090.
At offset 0736E9, replace E8C20CFCFF with E832481300.
At offset 05BA86, replace 8A0D68DB8000 with E895C4140090.

# To call the hook that sets full FPS
At offset 10FBD2, replace 83FA08 with 83FA01.
At offset 10FBF6, replace 83F808 with 83F801.
At offset 10FC8A, replace 83FA08 with 83FA01.
At offset 10FCB2, replace 83F808 with 83F801.
At offset 58005, replace 83FA08 with 83FA04.
At offset 58027, replace 83F908 with 83F904.
At offset 0F739C, replace 730A with EB0A.
At offset 10FD86, replace 7D07 with EB07.
At offset 11034C, replace 7D07 with EB07.

# To call the hook that disables flush texture cache inflight
At offset 499E0, replace E808D21400 with 9090909090.
At offset 196B96, replace 8B5508 with EB5190.

# To call the hook that sets the animations speed
At offset 000EF5, replace 0F85310C0000 with 909090909090.
At offset 000F7D, replace 8D04408D0480 with E89E6F1A0090.
At offset 000F9C, replace 8D04408D0480 with E87F6F1A0090.
At offset 000FC2, replace 8D04408D0480 with E8596F1A0090.
At offset 001A33, replace 0F84D5000000 with E8E8641A0090.
At offset 001017, replace 25FFFF0000 with E8046F1A00.
At offset 00193A, replace 66817DFCDE00752B with E8E1651A00909090.

# To call the hook that returns time
At offset 10D040, replace FF25AC925A00 with E8EBAE0900C3.
At offset 11C4F8, replace FF25AC925A00 with E8430BFFFFC3.
At offset 10CC00, replace 8B3DAC925A00 with BF40DC500090.
At offset 10CC81, replace 8B35AC925A00 with BE40DC500090.
At offset 10A5AE, replace FF15AC925A00 with E88D2A000090.
At offset 191F1C, replace FF15AC925A00 with E81FB1F7FF90.
At offset 191F57, replace FF15AC925A00 with E8E4B0F7FF90.
At offset 192000, replace FF15AC925A00 with E83BB0F7FF90.
At offset 15E0E0, replace FF25F8905A00 with E84B9E0400C3.
At offset 13DBB2, replace FF15F8905A00 with E82905020090.
At offset 13DE7B, replace FF15F8905A00 with E86002020090.
At offset 13E17F, replace FF15F8905A00 with E85CFF010090.
At offset 13F10D, replace FF15F8905A00 with E8CEEF010090.
At offset 13F1E3, replace FF15F8905A00 with E8F8EE010090.

# To call the hook that updates ai functions
At offset 0F5C38, replace E843B5FAFF with E8E3220B00.


*** Usage ***

To define the duration of an animation frame for explosions, set the ExplosionAnimationTimeFrame setting in "hook_time.cfg". The setting is an integer value. The default value is 60.
To define the duration of an animation frame for small debris, set the SmallDebrisAnimationTimeFrame setting in "hook_time.cfg". The setting is an integer value. The default value is 60.
See "hook_time.cfg"

Press Ctrl+Q to speed up the time.
Press Ctrl+A to slow down the time.

The time multiplicator/divisor is written to *(float*)0x0781E60.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
