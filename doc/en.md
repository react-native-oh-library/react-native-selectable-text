> Template version: v0.4.0

<p align="center">
  <h1 align="center"> <code>@astrocoders/react-native-selectable-text</code> </h1>
</p>

This project is based on [@astrocoders/react-native-selectable-text](https://github.com/Astrocoders/react-native-selectable-text).

The repository of this third-party library is on Github and is now available for direct download from npm, the new package name is: `@react-native-ohos/react-native-selectable-text`, The version correspondence details are as follows:

| Name | Version | Release Information | Supported RN Version | Supported Autolink | Compile API Version | Community Baseline Version | npm Address |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| @react-native-ohos/react-native-selectable-text | ~1.6.0 | [Github Releases](https://github.com/react-native-oh-library/react-native-selectable-text/releases) | `0.82.*` | Yes | API12+ | 1.5.1 | [Npm Address](https://www.npmjs.com/package/@react-native-ohos/react-native-selectable-text) |
| @react-native-ohos/react-native-selectable-text | ~1.5.2 | [Github Releases](https://github.com/react-native-oh-library/react-native-selectable-text/releases) | `0.72.*/0.77.*` | No | API12+ | 1.5.1 | [Npm Address](https://www.npmjs.com/package/@react-native-ohos/react-native-selectable-text) |

## 1. Installation and Usage

Go to the project directory and execute the following instruction:

<!-- tabs:start -->

#### **npm**

```bash
npm install @react-native-ohos/react-native-selectable-text
```

#### **yarn**

```bash
yarn add @react-native-ohos/react-native-selectable-text
```

<!-- tabs:end -->

The following code demonstrates a basic usage scenario of this library:

> [!WARNING] The import library name remains unchanged during use.

```js
import React from "react";
import { View, Alert } from "react-native";
import { SelectableText } from "react-native-selectable-text";

const App = () => {
  return (
    <View style={{ flex: 1, padding: 20 }}>
      <SelectableText
        value="This is a selectable text. Custom menu will pop up after long press selection."
        menuItems={["Copy", "Translate", "Search"]}
        onSelection={({ eventType, content, selectionStart, selectionEnd }) => {
          Alert.alert(
            eventType,
            `Selected: "${content}"\nRange: [${selectionStart}, ${selectionEnd}]`
          );
        }}
        style={{ fontSize: 16, color: "#333" }}
      />
    </View>
  );
};

export default App;
```
## 2. Manual Link

|                                      | Supports Autolink | RN Framework Version |
|--------------------------------------|-----------------|------------|
| 1.6.0                              |  Yes              |  0.82     |
| 1.5.2                              |  No              |  0.72/0.77     |

Projects using AutoLink need to be configured according to this document, AutoLink framework guide.：https://gitcode.com/openharmony-sig/ohos_react_native/blob/master/docs/zh-cn/Autolinking.md

If the version you are using supports Autolink and the project has integrated Autolink, you can skip the ManualLink configuration.

<details>
  <summary>ManualLink: This step provides guidance for manually configuring native dependencies.</summary>

Open the `harmony` directory of the HarmonyOS project in DevEco Studio.

### 2.1 Overrides RN SDK

To ensure the project relies on the same version of the RN SDK, you need to add an `overrides` field in the project's root `oh-package.json5` file, specifying the RN SDK version to be used. The replacement version can be a specific version number, a semver range, or a locally available HAR package or source directory.

For more information about the purpose of this field, please refer to the [official documentation](https://developer.huawei.com/consumer/en/doc/harmonyos-guides-V5/ide-oh-package-json5-V5#en-us_topic_0000001792256137_overrides).

```json
{
  "overrides": {
    "@rnoh/react-native-openharmony": "^0.72.38" // ohpm version
    // "@rnoh/react-native-openharmony" : "./react_native_openharmony.har" // a locally available HAR package
    // "@rnoh/react-native-openharmony" : "./react_native_openharmony" // source code directory
  }
}
```

### 2.2 Introducing Native Code

Currently, two methods are available:

- Use the HAR file.
- Directly link to the source code。

Method 1 (recommended): Use the HAR file.

> [!TIP] The HAR file is stored in the `harmony` directory in the installation path of the third-party library.

Open `entry/oh-package.json5` file and add the following dependencies:

```json
"dependencies": {
    "@rnoh/react-native-openharmony": "file:../react_native_openharmony",
    "@react-native-ohos/react-native-selectable-text": "file:../../node_modules/@react-native-ohos/react-native-selectable-text/harmony/selectable_text.har"
  }
```

Click the `sync` button in the upper right corner.

Alternatively, run the following instruction on the terminal:

```bash
cd entry
ohpm install
```

Method 2: Directly link to the source code.

> [!TIP] For details, see [Directly Linking Source Code](./link-source-code.md).

### 2.3 Configuring CMakeLists and Introducing SelectableTextPackage

Open `entry/src/main/cpp/CMakeLists.txt` and add the following code:

```diff
project(rnapp)
cmake_minimum_required(VERSION 3.4.1)
set(RNOH_APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
+ set(OH_MODULES "${CMAKE_CURRENT_SOURCE_DIR}/../../../oh_modules")
set(RNOH_CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../react-native-harmony/harmony/cpp")

# RNOH_BEGIN: manual_package_linking_1
+ add_subdirectory("${OH_MODULES}/@react-native-ohos/react-native-selectable-text/src/main/cpp" ./selectable-text)
# RNOH_END: manual_package_linking_1

# RNOH_BEGIN: manual_package_linking_2
+ target_link_libraries(rnoh_app PUBLIC rnoh_selectable_text)
# RNOH_END: manual_package_linking_2
```

Open `entry/src/main/cpp/PackageProvider.cpp` and add the following code:

```diff
#include "RNOH/PackageProvider.h"
#include "generated/RNOHGeneratedPackage.h"
+ #include "SelectableTextViewPackage.h"

using namespace rnoh;

std::vector<std::shared_ptr<Package>> PackageProvider::getPackages(Package::Context ctx) {
    return {
        std::make_shared<RNOHGeneratedPackage>(ctx),
+       std::make_shared<SelectableTextViewPackage>(ctx),
    };
}
```

## 2.4 Running

Click the `sync` button in the upper right corner.

Alternatively, run the following instruction on the terminal:

```bash
cd entry
ohpm install
```

Then build and run the code.
</details>

## 3. Constraints

### 3.1 Compatibility

Please check the version matching information in the Releases page of the third-party library: [@react-native-ohos/react-native-selectable-text Releases](https://github.com/react-native-oh-library/react-native-selectable-text/releases)

The content of this document has been verified using the following versions:

1. RNOH: 0.72.108; SDK: HarmonyOS-6.0.2.129 (API Version 22 Beta1); IDE: DevEco Studio 6.0.2.636; ROM: 6.0.0.129;
2. RNOH: 0.77.18; SDK: HarmonyOS-6.0.2.129 (API Version 22 Beta1); IDE: DevEco Studio 6.0.2.636; ROM: 6.0.0.129;
3. RNOH: 0.82.7; SDK: HarmonyOS-6.0.2.130 (API Version 22 Release); IDE: DevEco Studio 6.0.2.642; ROM: 6.0.0.328;

### 3.2 API Requirements for Compilation and Runtime

> [!TIP] All versions of this third-party library have achieved version isolation and support compilation in `API12+` projects, as well as running on `API12+` ROMs.

> [!TIP] The following features rely on specific API versions. Compiling in projects `lower than API 22` or running on ROMs `lower than API 22` may result in limited functionality.

1. This library introduces [NODE_TEXT_EDIT_MENU_OPTIONS](https://developer.huawei.com/consumer/cn/doc/harmonyos-references/capi-native-node-h) to implement the custom popup menu functionality. This API requires compilation in a project supporting `API22+` and must run on a ROM supporting `API22+` to take effect.

## 4. Properties

| Name               | Description                                                                 | Type                                                                                      | Required | Platform    | HarmonyOS Support |
|--------------------|-----------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|----------|-------------|-------------------|
| value              | Text content                                                                    | string                                                                                    | yes      | iOS/Android | yes               |
| onSelection        | Callback when the user clicks a selection menu item (Supported starting from API version 22)                                                 | ({ eventType: string, content: string, selectionStart: int, selectionEnd: int }) => void | no       | iOS/Android | yes               |
| menuItems          | Array of custom menu items (Supported starting from API version 22)                                                           | array(string)                                                                             | no       | iOS/Android | yes               |
| style              | Additional style applied to text                                                        | Object                                                                                    | no       | iOS/Android | yes               |
| highlights         | Array of text ranges to highlight, can include id                                           | array({ id: string, start: int, end: int })                                               | no       | iOS/Android | yes               |
| highlightColor     | Highlight background color                                                                | string                                                                                    | no       | iOS/Android | yes               |
| onHighlightPress   | Callback when user clicks a highlighted area                                                    | (id: string) => void                                                                      | no       | iOS/Android | yes               |
| appendToChildren   | Elements appended to the end of the text                                                        | ReactNode                                                                                 | no       | iOS/Android | yes               |
| TextComponent      | Custom component used to render text                                                    | ReactNode                                                                                 | no       | iOS/Android | yes               |
| textValueProp      | The text property name passed to TextComponent, used with TextComponent, defaults to 'children' | string                                                                                    | no       | iOS/Android | yes               |
| textComponentProps | Additional properties passed to TextComponent                                             | object                                                                                    | no       | iOS/Android | yes               |

## 5. Known Issues

None

## 6. Others

None

## 7. License

This project is based on [The MIT License (MIT)](https://github.com/Astrocoders/react-native-selectable-text/blob/master/LICENSE), please freely enjoy and participate in open source.
