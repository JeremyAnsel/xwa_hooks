#pragma once

int BeginSceneHook(int* params);
int EndSceneHook(int* params);
int ExecuteBufferLockHook(int* params);
int ExecuteBufferAddVerticesHook(int* params);
int ExecuteBufferProcessVerticesHook(int* params);
int ExecuteBufferAddTrianglesHook(int* params);
int ExecuteBufferUnlockAndExecuteHook(int* params);
int ExecuteBufferTriangleInstructionCount1Hook(int* params);
int ExecuteBufferTriangleInstructionCount2Hook(int* params);
