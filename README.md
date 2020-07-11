# Advanced Locomotion System V4 on C++
![image](https://github.com/dyanikoglu/ALSV4_CPP/raw/master/Resources/Readme_Content_2.gif)

Performance optimized C++ Port of [Advanced Locomotion System V4](https://www.unrealengine.com/marketplace/en-US/product/advanced-locomotion-system-v1) for **Unreal Engine 4.25+**

## Features
- Converted into plugin structure. You can immediately start using it in your project without doing any serious change in your project configuration.
- Source code is highly optimized while being ported from blueprints. For 100 characters in a scene, CPU time is decreased from ~17ms to ~10ms.
- Suitable for a network-performance friendly replication support. Replication implementation is still in progress, and currently beats ALS V3 and other BP based replicated V4 implementations on network performance:
```
Advanced Locomotion System V3 (BP):
Total outgoing bandwidth is 686KB over 60 seconds (30 seconds playing server, 30 on client).
Character Actor Data sent over network - 43KB
'ReplicatedMovement' Data sent over network - 19KB
Highest RPC Data sent over network - 90KB
Overall Property Size - 46KB
Overall RPC Size - 201KB

Advanced Locomotion System V4 (BP):
Total outgoing bandwidth is  2.7MB over 60 seconds (30 seconds playing server, 30 on client).
Character Actor Data sent over network - 116KB
'ReplicatedMovement' Data sent over network - 45KB
Highest RPC Data sent over network - 536KB
Overall Property Size - 209KB
Overall RPC Size - 1.7MB

Advanced Locomotion System V4 (C++):
Total outgoing bandwidth is 618KB over 60 seconds (30 seconds playing server, 30 on client).
Character Actor Data sent over network - 38KB
'ReplicatedMovement' Data sent over network - 14KB
Highest RPC Data sent over network - 48KB
Overall Property Size - 41.1KB
Overall RPC Size - 91.1KB

Netprofiler benchmark by @Scragnog
```

## Setting Up The Plugin
- Clone the repository if you're working on a C++ project, or download the latest release if you're on a BP project.
- Move ALSV4_CPP folder into your project's `Plugins` folder
- Add the lines below into your project's `DefaultInput.ini`, below `[/Script/Engine.InputSettings]` tag:
```
+ActionMappings=(ActionName="JumpAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=SpaceBar)
+ActionMappings=(ActionName="JumpAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_FaceButton_Bottom)
+ActionMappings=(ActionName="StanceAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=LeftAlt)
+ActionMappings=(ActionName="SprintAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=LeftShift)
+ActionMappings=(ActionName="StanceAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_FaceButton_Right)
+ActionMappings=(ActionName="SprintAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_LeftThumbstick)
+ActionMappings=(ActionName="WalkAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=LeftControl)
+ActionMappings=(ActionName="WalkAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_RightShoulder)
+ActionMappings=(ActionName="AimAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=RightMouseButton)
+ActionMappings=(ActionName="AimAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_LeftTrigger)
+ActionMappings=(ActionName="SelectRotationMode_1",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=One)
+ActionMappings=(ActionName="SelectRotationMode_2",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Two)
+ActionMappings=(ActionName="SelectRotationMode_1",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_DPad_Left)
+ActionMappings=(ActionName="SelectRotationMode_2",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_DPad_Right)
+ActionMappings=(ActionName="CameraAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=C)
+ActionMappings=(ActionName="CameraAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_RightThumbstick)
+ActionMappings=(ActionName="RagdollAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=X)
+ActionMappings=(ActionName="RagdollAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_Special_Left)
+ActionMappings=(ActionName="CycleOverlayUp",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=MouseScrollUp)
+ActionMappings=(ActionName="CycleOverlayDown",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=MouseScrollDown)
+ActionMappings=(ActionName="CycleOverlayUp",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_DPad_Up)
+ActionMappings=(ActionName="CycleOverlayDown",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_DPad_Down)
+ActionMappings=(ActionName="OpenOverlayMenu",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Q)
+ActionMappings=(ActionName="OpenOverlayMenu",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_LeftShoulder)
+ActionMappings=(ActionName="UseAction",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=E)
+AxisMappings=(AxisName="MoveForward/Backwards",Scale=1.000000,Key=W)
+AxisMappings=(AxisName="MoveRight/Left",Scale=1.000000,Key=D)
+AxisMappings=(AxisName="LookUp/Down",Scale=-1.000000,Key=MouseY)
+AxisMappings=(AxisName="LookLeft/Right",Scale=1.000000,Key=MouseX)
+AxisMappings=(AxisName="MoveForward/Backwards",Scale=-1.000000,Key=S)
+AxisMappings=(AxisName="MoveRight/Left",Scale=-1.000000,Key=A)
+AxisMappings=(AxisName="MoveForward/Backwards",Scale=1.000000,Key=Gamepad_LeftY)
+AxisMappings=(AxisName="MoveRight/Left",Scale=1.000000,Key=Gamepad_LeftX)
+AxisMappings=(AxisName="LookUp/Down",Scale=1.000000,Key=Gamepad_RightY)
+AxisMappings=(AxisName="LookLeft/Right",Scale=1.000000,Key=Gamepad_RightX)
```
- Add the lines below into your `DefaultEngine.ini`, below `[/Script/Engine.CollisionProfile]` tag (Create the tag if it doesn't exist):
```
+Profiles=(Name="ALS_Character",CollisionEnabled=QueryAndPhysics,bCanModify=True,ObjectTypeName="Pawn",CustomResponses=((Channel="Visibility",Response=ECR_Ignore),(Channel="Camera",Response=ECR_Ignore),(Channel="Climbable",Response=ECR_Ignore)),HelpMessage="Custom collision settings for the capsule in the ALS_BaseCharacter.")
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Block,bTraceType=True,bStaticObject=False,Name="Climbable")
```
- Launch your project, and enable plugin content viewer as seen below. This will show contents of the ALS in your content browser:
![image](https://github.com/dyanikoglu/ALSV4_CPP/raw/master/Resources/Readme_Content_1.png)

## License & Contribution
**Source code** of the plugin is licensed under MIT license, and other developers are encouraged to fork the repository, open issues & pull requests to help the development.
