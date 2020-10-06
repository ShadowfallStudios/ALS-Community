# Advanced Locomotion System V4 on C++
![image](https://github.com/dyanikoglu/ALSV4_CPP/raw/main/Resources/Readme_Content_2.gif)

Performance optimized & replicated C++ port of [Advanced Locomotion System V4](https://www.unrealengine.com/marketplace/en-US/product/advanced-locomotion-system-v1) for **Unreal Engine 4.25** with additional bug fixes.

## Features
- Converted into plugin structure (by [AHGS-Jens](https://github.com/AHGS-Jens)). You can immediately start using it in your project without doing any serious change in your project configuration.
- Source code is highly optimized while being ported from blueprints. For 100 characters in a scene, CPU time is decreased from ~17ms to ~10ms.
- Currently, only Windows & Linux are supported, and tested.
- Network performance optimized, and comes with full replication support (Big kudos to [senfkorn92](https://github.com/senfkorn92) for his work). ALSV4_CPP currently beats ALS V3 and other BP based V4 replication implementations on network performance:
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
- Clone the repository inside your project's `Plugins` folder, or download the source code zip file for latest release and extract it into your project's `Plugins` folder.
- Put `Config/DefaultInput.ini` from the plugin folder inside your project's config folder. If your project already have this .ini file, merge it into yours.
- Add the lines below into your `DefaultEngine.ini`, below `[/Script/Engine.CollisionProfile]` tag (Create the tag if it doesn't exist):
```
+Profiles=(Name="ALS_Character",CollisionEnabled=QueryAndPhysics,bCanModify=True,ObjectTypeName="Pawn",CustomResponses=((Channel="Visibility",Response=ECR_Ignore),(Channel="Camera",Response=ECR_Ignore),(Channel="Climbable",Response=ECR_Ignore)),HelpMessage="Custom collision settings for the capsule in the ALS_BaseCharacter.")
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Block,bTraceType=True,bStaticObject=False,Name="Climbable")
```
- Generate Visual Studio Project files, and compile your project. (Unreal Engine 4.25.2 & 4.25.3 update broke compiling distribution binaries for plugins, so you need to compile them yourself)
- Launch the project, and enable plugin content viewer as seen below. This will show contents of the ALS in your content browser:
![image](https://github.com/dyanikoglu/ALSV4_CPP/raw/main/Resources/Readme_Content_1.png)
- If you're getting problems with the plugin binaries, try compiling them yourself from the source.

## License & Contribution
**Source code** of the plugin is licensed under MIT license, and other developers are encouraged to fork the repository, open issues & pull requests to help the development.
