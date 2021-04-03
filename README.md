# Advanced Locomotion System Community
![image](https://github.com/dyanikoglu/ALS-Community/raw/main/Resources/Readme_Content_2.gif)

Replicated and optimized community version of [Advanced Locomotion System V4](https://www.unrealengine.com/marketplace/en-US/product/advanced-locomotion-system-v1) for **Unreal Engine 4.26** with additional bug fixes.

## Supported Platforms
- Windows
- Linux

*Mac, Android, IOS, and console builds are not tested and supported at the moment. Use the plugin on those platforms with your own risk.*

## Features
- Based on latest marketplace release (V4) of Advanced Locomotion System
- Fully implemented in C++
- Full replication support with low bandwidth usage
- Plugin structure
- Highly optimized for production
- Mantling and debugging features are implemented as a separate plug-in/plug-out type actor component to reduce total overhead on base character class
- Improved footstep behavior with Niagara particle & decal support.
- Lots of bug fixes additional to marketplace version

## Known Issues & Discussion
- See [Issues](https://github.com/dyanikoglu/ALS-Community/issues) section for list of known issues
- See [Discussions](https://github.com/dyanikoglu/ALS-Community/discussions) section to discuss anything about the plugin, and ask questions. Please do not open an issue to ask questions about the plugin.

## Setting Up The Plugin
- **C++ project is a requirement. BP projects are currently not supported.**
- Clone the repository inside your project's `Plugins` folder, or download the latest release and extract it into your project's `Plugins` folder.
- Put `Config/DefaultInput.ini` from the plugin folder inside your project's config folder. If your project already have this .ini file, merge it into yours.
- Regenerate visual studio project files and build your project.
- Launch the project, and enable plugin content viewer as seen below. This will show contents of the plugin in your content browser:
![image](https://github.com/dyanikoglu/ALS-Community/raw/main/Resources/Readme_Content_1.png)

## License & Contribution
**Source code** of the plugin is licensed under MIT license, and other developers are encouraged to fork the repository, open issues & pull requests to help the development.
