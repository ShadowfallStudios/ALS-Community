# ALSV4_CPP
Performance optimized C++ Port of Advanced Locomotion System V4 for **Unreal Engine 4.25**

# Setting Up The Plugin

- Clone the repository if you're working on a C++ project, or download the latest release if you're on a BP project.
- Move ALSV4_CPP folder into your project's `Plugins` folder
- Add the lines below into your project's `DefaultIntput.ini`, below `[/Script/Engine.InputSettings]` tag:
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
- Add the lines below into your `DefaultEngine.ini`, below `[/Script/Engine.CollisionProfile]` tag:
```
+Profiles=(Name="ALS_Character",CollisionEnabled=QueryAndPhysics,bCanModify=True,ObjectTypeName="Pawn",CustomResponses=((Channel="Visibility",Response=ECR_Ignore),(Channel="Camera",Response=ECR_Ignore),(Channel="Grabbable",Response=ECR_Ignore),(Channel="Climbable",Response=ECR_Ignore)),HelpMessage="Custom collision settings for the capsule in the ALS_BaseCharacter.")
+Profiles=(Name="ALS_Prop",CollisionEnabled=QueryAndPhysics,bCanModify=True,ObjectTypeName="WorldDynamic",CustomResponses=((Channel="Visibility",Response=ECR_Ignore),(Channel="Camera",Response=ECR_Ignore),(Channel="Grabbable",Response=ECR_Ignore),(Channel="Climbable",Response=ECR_Ignore)),HelpMessage="Custom collision settings for the props")
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Block,bTraceType=True,bStaticObject=False,Name="Climbable")
+EditProfiles=(Name="Pawn",CustomResponses=((Channel="Grabbable",Response=ECR_Ignore)))
+EditProfiles=(Name="Spectator",CustomResponses=((Channel="Grabbable",Response=ECR_Ignore)))
+EditProfiles=(Name="InvisibleWall",CustomResponses=((Channel="Grabbable",Response=ECR_Ignore)))
+EditProfiles=(Name="InvisibleWallDynamic",CustomResponses=((Channel="Grabbable",Response=ECR_Ignore)))
+EditProfiles=(Name="Trigger",CustomResponses=((Channel="Grabbable",Response=ECR_Ignore)))
+EditProfiles=(Name="Ragdoll",CustomResponses=((Channel="Grabbable",Response=ECR_Ignore)))
+EditProfiles=(Name="UI",CustomResponses=((Channel="Grabbable",Response=ECR_Ignore),(Channel="Destructible",Response=ECR_Overlap)))
+EditProfiles=(Name="CharacterMesh",CustomResponses=((Channel="Grabbable",Response=ECR_Ignore)))
```
- Launch your project, and enable plugin content viewer as seen below. This will show contents of the ALS in your content browser:
![image](https://github.com/dyanikoglu/ALSV4_CPP/raw/master/Resources/Readme_Content_1.png)

## Known Issues & Missing Features
Known issues & missing features can be tracked under Github issues tab.

## License & Contribution
**Source code** is licensed under MIT license, and other developers are encouraged to fork the repository, open issues & pull requests to help the development.
