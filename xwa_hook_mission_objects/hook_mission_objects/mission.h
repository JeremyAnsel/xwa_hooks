#pragma once

int MissionObjectsHook(int* params);
int MissionObjectsStatsHook(int* params);
int CraftTurretHook(int* params);

int LaserShootHook(int* params);
int LaserShoot2Hook(int* params);
int WarheadShootHook(int* params);

int SetXwaCraftsHook(int* params);
int ClearCraftDataHook(int* params);
int CloneObjectHook(int* params);
int ComputeCraftAnglesHook(int* params);
int SetRotationAngle1Hook(int* params);
int SetRotationAngle2Hook(int* params);
int SetL00491EB0Angle1Hook(int* params);
int SetL00491EB0Angle2Hook(int* params);
int SetL00497610Angle1Hook(int* params);
int SetL00497610Angle2Hook(int* params);
int SetL0040D410Angle1Hook(int* params);
int SetL0040D410Angle2Hook(int* params);
int SetL0043F8E0Angle1Hook(int* params);
int SetL0043F8E0Angle2Hook(int* params);

int ReadTurretPositionYHook(int* params);
int ReadTurretPositionXHook(int* params);
int ReadTurretPositionZHook(int* params);
int ReadTurretOrientationHook(int* params);
int ReadL00507B20HasTurretHook(int* params);
int ReadL004C3EF0HasTurretHook(int* params);

int TurretIndex2TransformHook(int* params);
int TurretIndex2BlockedHook(int* params);

int ObjectProfileHook(int* params);
int AddObjectProfileHook(int* params);
int BriefingObjectProfileHook(int* params);
int RenderOptObjectProfileHook(int* params);
