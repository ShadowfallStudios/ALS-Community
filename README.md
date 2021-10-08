# Advanced Locomotion System Community
![image](https://github.com/dyanikoglu/ALS-Community/raw/main/Resources/Readme_Content_2.gif)

Replicated and optimized community version of [Advanced Locomotion System V4](https://www.unrealengine.com/marketplace/en-US/product/advanced-locomotion-system-v1) for **Unreal Engine 4.27** with additional bug fixes.

<p align="center">
  <a href="https://discord.gg/wYYMHFu"><img src="https://i.imgur.com/LP9bZQj.png"></a>
</p>

## Supported Platforms
- Windows
- Linux

*Mac, Android, IOS, and console builds are not tested and supported at the moment. Use the plugin on those platforms with your own risk.*

## Features
- Based on latest marketplace release (V4) of Advanced Locomotion System
- Fully implemented in C++
- Full replication support with low bandwidth usage (Ragdoll replication is in experimental state)
- Plugin structure
- Highly optimized for production
- Mantling and debugging features are implemented as a separate plug-in/plug-out type actor component to reduce total overhead on base character class
- Improved footstep behavior with Niagara particle & decal support.
- Lots of bug fixes additional to marketplace version

## Known Issues & Discussion
- See [Issues](https://github.com/dyanikoglu/ALS-Community/issues) section for list of known issues
- See [Discussions](https://github.com/dyanikoglu/ALS-Community/discussions) section to discuss anything about the plugin, and ask questions. Please do not open an issue for questions.

## Setting Up The Plugin
- **C++ project is a requirement. BP projects are currently not supported.**
- Clone the repository inside your project's `Plugins` folder, or download the latest release and extract it into your project's `Plugins` folder.
- Put `Config/DefaultInput.ini` from the plugin folder inside your project's config folder. If your project already have this .ini file, merge it into yours.
- Regenerate visual studio project files and build your project.
- Launch the project, and enable plugin content viewer as seen below. This will show contents of the plugin in your content browser:
![image](https://github.com/dyanikoglu/ALS-Community/raw/main/Resources/Readme_Content_1.png)

## Functions To Override In Blueprints
- There are a few functions you will need to override in blueprints. You will also need to update these functions for every new overlay state you add. These are commented in the C++ code, but are listed here for quick reference as well.
- In your player character blueprint: **GetGetUpAnimation** & **GetRollAnimation**
- In the MantleComponent: **GetMantleAsset**

## License & Contribution
**Source code** of the plugin is licensed under MIT license, and other developers are encouraged to fork the repository, open issues & pull requests to help the development.
