xwa_hook_briefing

This hook creates briefing questions/answers and texts.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_briefing.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that creates briefing
At offset 1641C1, replace E86A170000 with E85A3D0400.
At offset 1651AA, replace E8A16DFFFF with E8812D0400.
At offset 16528B, replace E8C06CFFFF with E85A3D0400.
At offset 164A70, replace E89BC1FDFF with E8AB340400.
At offset 170CCD, replace 6850EC5700 with E84E720300.
At offset 0D3D31, replace E8AA110000 with E8FA410D00.

# To call the hook that creates text briefing
At offset 0x143013, replace 6800100000 with 6800000100.
At offset 0x1647FF, replace 6800100000 with 6800000100.
At offset 0x16DA5D, replace 6800100000 with 6800000100.
At offset 0x16F16A, replace 6800100000 with 6800000100.
At offset 0x176C64, replace 6800100000 with 6800000100.
At offset 0x17E78B, replace 6800100000 with 6800000100.
At offset 0x17E7B5, replace 6800100000 with 6800000100.
At offset 0x1477B5, replace E8162AFEFF with E866070600.
At offset 0x147BA8, replace E82326FEFF with E873030600.
At offset 0x1832B7, replace E8146FFAFF with E8644C0200.


*** Usage ***

Suppose that the mission is "[MissionDir]\[Mission].tie".

To override the briefings mission data, create a file named "[MissionDir]\[Mission]BriefingQuestions.txt" or create a section named "[BriefingQuestions]" in "[MissionDir]\[Mission].ini".

The format is:

[BriefingQuestions]
OfficerImage = frontres\tour\squad5.cbm
OfficerImagePositionX = 100
OfficerImagePositionY = 150
BackQuestion = Back
DoneQuestion = Done
Question00 = Question 0
Answer00 = Answer 0
AnswerWave00 = Wave\Frontend\B0M1\B000102.wav
Officer00 = frontres\tour\squad6.cbm
OfficerPositionX00 = 100
OfficerPositionY00 = 150
Question01 = Question 1
Answer01 = Answer 1
Question02 = Question 2
Answer02 = Answer 2
Question03 = Question 3
Answer03 = Answer 3

You can have up to 100 questions/answers.
The "DoneQuestion" is used to exit the briefing and go to the hangar.

To override the victory debriefings mission data, create a file named "[MissionDir]\[Mission]DebriefingVictoryQuestions.txt" or create a section named "[DebriefingVictoryQuestions]" in "[MissionDir]\[Mission].ini".

The format is:

[DebriefingVictoryQuestions]
OfficerImage = frontres\tour\squad5.cbm
OfficerImagePositionX = 100
OfficerImagePositionY = 150
BackQuestion = Back
DoneQuestion = Done
Question00 = Question 0
Answer00 = Answer 0
AnswerWave00 = Wave\Frontend\B0M1\B000102.wav
Officer00 = frontres\tour\squad6.cbm
OfficerPositionX00 = 100
OfficerPositionY00 = 150
Goals00 =
GoalsMethod00 =
Question01 = Question 1
Answer01 = Answer 1
Question02 = Question 2
Answer02 = Answer 2
Question03 = Question 3
Answer03 = Answer 3

You can have up to 100 questions/answers.
The "DoneQuestion" is used to exit the debriefing.
The format for GoalsXX is X_Y_Z separated with ',' or ';'. X is the fg inddex. Y is the goal index. Z is the state of the goal.
GoalsMethodXX can be "All" or "AtLeastOne". Default is "All".

To override the defeat debriefings mission data, create a file named "[MissionDir]\[Mission]DebriefingDefeatQuestions.txt" or create a section named "[DebriefingDefeatQuestions]" in "[MissionDir]\[Mission].ini".

The format is:

[DebriefingDefeatQuestions]
OfficerImage = frontres\tour\squad5.cbm
OfficerImagePositionX = 100
OfficerImagePositionY = 150
BackQuestion = Back
DoneQuestion = Done
Question00 = Question 0
Answer00 = Answer 0
AnswerWave00 = Wave\Frontend\B0M1\B000102.wav
Officer00 = frontres\tour\squad6.cbm
OfficerPositionX00 = 100
OfficerPositionY00 = 150
Goals00 =
GoalsMethod00 =
Question01 = Question 1
Answer01 = Answer 1
Question02 = Question 2
Answer02 = Answer 2
Question03 = Question 3
Answer03 = Answer 3

You can have up to 100 questions/answers.
The "DoneQuestion" is used to exit the debriefing.
The format for GoalsXX is X_Y_Z separated with ',' or ';'. X is the fg inddex. Y is the goal index. Z is the state of the goal.
GoalsMethodXX can be "All" or "AtLeastOne". Default is "All".

## Text

For answers you can use the '$' char to insert line break.

## BriefingText

To override the text for the briefing, create a file named "[MissionDir]\[Mission]BriefingText.txt" or create a section named "[BriefingText]" in "[MissionDir]\[Mission].ini".

The format is:

[BriefingText]
Description = ...
WinDebriefing = ...
LostDebriefing = ...

The limit is 65535 characters for each string. You can use the '$' char to insert line break.

## Translations

You can define translations in "xwa.ini" at the root of the xwa directory.

The format is:

[MissionPathWithoutExtension_BriefingQuestions]
BackQuestion =
DoneQuestion =
Question00 =
Answer00 =
AnswerWave00 =

[MissionPathWithoutExtension_DebriefingVictoryQuestions]
BackQuestion =
DoneQuestion =
Question00 =
Answer00 =
AnswerWave00 =

[MissionPathWithoutExtension_DebriefingDefeatQuestions]
BackQuestion =
DoneQuestion =
Question00 =
Answer00 =
AnswerWave00 =


*** Credits ***

- Jérémy Ansel (JeremyaFr)
