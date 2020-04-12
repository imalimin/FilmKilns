![Build status](https://onedrive.visualstudio.com/Design/_apis/build/status/fabric-mobile/fabric-android-CI-github?branchName=master)
[![GitHub release](https://img.shields.io/github/release/officedev/ui-fabric-android.svg)](https://github.com/OfficeDev/ui-fabric-android/releases/latest)

# [Office UI Fabric for Android](http://dev.office.com/fabric)

##### The Android UI framework for building experiences for Office and Office 365.

Office UI Fabric for Android is a native library that provides the Office UI experience for the Android platform. It contains information about colors and typography, as well as custom controls and customizations for platform controls, all from the official Fluent design language used in Office and Office 365 products.

## Contents

- [Colors and typography](#colors-and-typography)
- [Controls](#controls)
- [Install and use Office UI Fabric](#install-and-use-office-ui-fabric)
- [Demo app](#demo-app)
- [Contributing](#contributing)
- [License](#license)
- [Changelog](#changelog)

## Colors and typography

Office UI Fabric for Android provides [colors](OfficeUIFabric/src/main/res/values/colors.xml) and [typography](OfficeUIFabric/src/main/res/values/styles_font.xml) based on the Fluent design language.

## Controls

Office UI Fabric for Android includes an expanding library of controls written in Kotlin. These controls implement the Fluent design language and bring consistency across Office app experiences.

Some of the controls available include:
- AvatarView
- Button styles
- BottomSheet
- CalendarView
- CircularProgress styles
- DateTimePickerDialog
- Drawer
- ListItemView
- PeoplePickerView
- PersonaChipView
- PersonaListView
- PersonaView
- Snackbar
- TemplateView
- Tooltip

A full list of currently supported controls can be found here: [OfficeUIFabric](OfficeUIFabric/src/main/java/com/microsoft/officeuifabric).

## Install and use Office UI Fabric

### Requirements

API 21+

### 1. Using Gradle

- Our library is published through JCenter, so make sure the `jcenter()` repository has been added to your project level build.gradle file (which usually is automatic).

- Inside the dependency block in your build.gradle, add this line for the OfficeUIFabric library:
```gradle
dependencies {
    ...
    implementation 'com.microsoft.uifabric:OfficeUIFabric:$version'
    ... 
}
```

- Make sure you replace `$version` with the latest version of OfficeUIFabric.

### 2. Using Maven

- Add the OfficeUIFabric library as a dependency:
```xml
<dependency>
  <groupId>com.microsoft.uifabric</groupId>
  <artifactId>OfficeUIFabric</artifactId>
  <version>${version}</version>
</dependency>
```

- Make sure you replace `${version}` with the latest version of OfficeUIFabric.

### 3. Manual installation

- Download the latest changes from the [Office UI Fabric Android](https://github.com/OfficeDev/UI-Fabric-Android) repository.

- Follow [these instructions](https://developer.android.com/studio/projects/android-library) to build and output an AAR file from the OfficeUIFabric module, import the module to your project, and add it as a dependency. If you're having trouble generating an AAR file for the module, make sure you select it and run "Make Module 'OfficeUIFabric'" from the Build menu.

- Some components have dependencies you will need to manually add to your app if you are using this library as an AAR artifact because these dependencies do not get included in the output.
  - If using **PeoplePickerView**, include this dependency in your gradle file:  
    ```gradle
    implementation 'com.splitwise:tokenautocomplete:2.0.8'
    ```
  - If using **CalendarView** or **DateTimePickerDialog**, include this dependency in your gradle file:
    ```gradle
    implementation 'com.jakewharton.threetenabp:threetenabp:1.1.0'
    ```
  - Double check that these library versions correspond to the latest versions we implement in the OfficeUIFabric [build.gradle](OfficeUIFabric/build.gradle).

### Import and use the library

In code:
```kotlin
import com.microsoft.officeuifabric.persona.AvatarView
```

In XML:
```xml
<com.microsoft.officeuifabric.persona.AvatarView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    app:name="Mona Kane" />
```

## Demo app

Included in this repository is a demo of currently implemented controls. A full list of implemented controls available in the demo can be found here:  [Demos](OfficeUIFabric.Demo/src/main/java/com/microsoft/officeuifabricdemo/demos).

To see samples of all of our implemented controls and design language, run the [OfficeUIFabric.Demo](OfficeUIFabric.Demo) module in Android Studio.

## Contributing

Post bug reports, feature requests, and questions in [Issues](https://github.com/OfficeDev/UI-Fabric-Android/issues).

## Changelog

We use [GitHub Releases](https://github.com/blog/1547-release-your-software) to manage our releases, including the changelog between every release. You'll find a complete list of additions, fixes, and changes on the [Releases page](https://github.com/OfficeDev/UI-Fabric-Android/releases).

## License

All files on the Office UI Fabric for Android GitHub repository are subject to the MIT license. Please read the [LICENSE](LICENSE) file at the root of the project.

Usage of the logos and icons referenced in Office UI Fabric for Android is subject to the terms of the [assets license agreement](https://aka.ms/fabric-assets-license).
