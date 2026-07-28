> 模板版本：v0.4.0

<p align="center">
  <h1 align="center"> <code>@astrocoders/react-native-selectable-text</code> </h1>
</p>
<p align="center">
    <a href="https://github.com/Astrocoders/react-native-selectable-text">
        <img src="https://img.shields.io/badge/platforms-iOS%20|%20Android%20|%20HarmonyOS-lightgrey.svg" alt="Supported platforms" />
    </a>
    <a href="https://github.com/Astrocoders/react-native-selectable-text/blob/master/LICENSE">
        <img src="https://img.shields.io/badge/license-MIT-green.svg" alt="License" />
    </a>
</p>

本项目基于 [@astrocoders/react-native-selectable-text](https://github.com/Astrocoders/react-native-selectable-text) 开发。

该第三方库的仓库在 Github，且支持直接从 npm 下载，新的包名为：`@react-native-ohos/react-native-selectable-text` 版本所属关系如下：

| 三方库名称 | 三方库版本 | 发布信息 | 支持RN版本 | Autolink | 编译API版本 | 社区基线版本 | npm地址 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| @react-native-ohos/react-native-selectable-text | ~1.6.0 | [Github Releases](https://github.com/react-native-oh-library/react-native-selectable-text/releases) | `0.82.*` | 是 | API12+ | 1.5.1 | [Npm Address](https://www.npmjs.com/package/@react-native-ohos/react-native-selectable-text) |
| @react-native-ohos/react-native-selectable-text | ~1.5.2 | [Github Releases](https://github.com/react-native-oh-library/react-native-selectable-text/releases) | `0.72.*/0.77.*`| 否 | API12+ | 1.5.1 | [Npm Address](https://www.npmjs.com/package/@react-native-ohos/react-native-selectable-text) |

## 1. 安装与使用

进入到工程目录并输入以下命令：

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

下面的代码展示了这个库的基本使用场景：

> [!WARNING] 使用时 import 的库名不变。

```js
import React from "react";
import { View, Alert } from "react-native";
import { SelectableText } from "react-native-selectable-text";

const App = () => {
  return (
    <View style={{ flex: 1, padding: 20 }}>
      <SelectableText
        value="这是一段可选择的文本，长按选择后会弹出自定义菜单。"
        menuItems={["复制", "翻译", "搜索"]}
        onSelection={({ eventType, content, selectionStart, selectionEnd }) => {
          Alert.alert(
            eventType,
            `选中: "${content}"\n范围: [${selectionStart}, ${selectionEnd}]`
          );
        }}
        style={{ fontSize: 16, color: "#333" }}
      />
    </View>
  );
};

export default App;
```
## 2. Link

|                                      | 是否支持autolink | RN框架版本 |
|--------------------------------------|-----------------|------------|
| 1.6.0                              |  Yes              |  0.82     |
| 1.5.2                              |  No              |  0.72/0.77     |

使用AutoLink的工程需要根据该文档配置，Autolink框架指导文档：https://gitcode.com/openharmony-sig/ohos_react_native/blob/master/docs/zh-cn/Autolinking.md

如您使用的版本支持 Autolink，并且工程已接入 Autolink，可跳过ManualLink配置。
<details>
  <summary>ManualLink: 此步骤为手动配置原生依赖项的指导</summary>

首先需要使用 DevEco Studio 打开项目里的 HarmonyOS 工程 `harmony`。

### 2.1. Overrides RN SDK

为了让工程依赖同一个版本的 RN SDK，需要在工程根目录的 `oh-package.json5` 添加 overrides 字段，指向工程需要使用的 RN SDK 版本。替换的版本既可以是一个具体的版本号，也可以是一个模糊版本，还可以是本地存在的 HAR 包或源码目录。

关于该字段的作用请阅读[官方说明](https://developer.huawei.com/consumer/cn/doc/harmonyos-guides-V5/ide-oh-package-json5-V5#zh-cn_topic_0000001792256137_overrides)

```json
{
  "overrides": {
    "@rnoh/react-native-openharmony": "^0.72.38" // ohpm 在线版本
    // "@rnoh/react-native-openharmony" : "./react_native_openharmony.har" // 指向本地 har 包的路径
    // "@rnoh/react-native-openharmony" : "./react_native_openharmony" // 指向源码路径
  }
}
```

### 2.2. 引入原生端代码

目前有两种方法：

- 通过 har 包引入；
- 直接链接源码。

方法一：通过 har 包引入（推荐）

> [!TIP] har 包位于三方库安装路径的 `harmony` 文件夹下。

打开 `entry/oh-package.json5`，添加以下依赖

```json
"dependencies": {
    "@rnoh/react-native-openharmony": "file:../react_native_openharmony",
    "@react-native-ohos/react-native-selectable-text": "file:../../node_modules/@react-native-ohos/react-native-selectable-text/harmony/selectable_text.har"
  }
```

点击右上角的 `sync` 按钮

或者在命令行终端执行：

```bash
cd entry
ohpm install
```

方法二：直接链接源码

> [!TIP] 如需使用直接链接源码，请参考[直接链接源码说明](./link-source-code.md)

### 2.3. 配置 CMakeLists 和引入 SelectableTextPackage

打开 `entry/src/main/cpp/CMakeLists.txt`，添加：

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

打开 `entry/src/main/cpp/PackageProvider.cpp`，添加：

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

### 2.4. 运行

点击右上角的 `sync` 按钮

或者在命令行终端执行：

```bash
cd entry
ohpm install
```

然后编译、运行即可。
</details>

## 3. 约束与限制

### 3.1. 兼容性

请到三方库相应的 Releases 发布地址查看 Release 配套的版本信息：[@react-native-ohos/react-native-selectable-text Releases](https://github.com/react-native-oh-library/react-native-selectable-text/releases)

本文档内容基于以下版本验证通过：

1. RNOH: 0.72.108; SDK: HarmonyOS-6.0.2.129 (API Version 22 Beta1); IDE: DevEco Studio 6.0.2.636; ROM: 6.0.0.129;
2. RNOH: 0.77.18; SDK: HarmonyOS-6.0.2.129 (API Version 22 Beta1); IDE: DevEco Studio 6.0.2.636; ROM: 6.0.0.129;
3. RNOH: 0.82.7; SDK: HarmonyOS-6.0.2.130 (API Version 22 Release); IDE: DevEco Studio 6.0.2.642; ROM: 6.0.0.328;

### 3.2. 编译运行API要求

> [!TIP] 当前三方库所有版本均已实现版本隔离，支持在 `API12+` 工程编译，及 `API12+` ROM运行。

> [!TIP] 以下功能依赖特定版本的API，使用 `低于API22版本的工程编译` 或 `低于API22版本的ROM运行` 均可能导致部分功能受限。
1. 本库引入[NODE_TEXT_EDIT_MENU_OPTIONS](https://developer.huawei.com/consumer/cn/doc/harmonyos-references/capi-native-node-h)，实现弹出自定义菜单功能，此API需要在支持`API22+`的工程编译，并在支持`API22+`的ROM上运行，方可生效。

## 4. 属性

> [!TIP] "Platform"列表示该属性在原三方库上支持的平台。

> [!TIP] "HarmonyOS Support"列为 yes 表示 HarmonyOS 平台支持该属性；no 则表示不支持；partially 表示部分支持。使用方法跨平台一致，效果对标 iOS 或 Android 的效果。

> [!TIP] onSelection和menuItems属性仅在API version 22开始支持。

| Name               | Description                                                                 | Type                                                                                      | Required | Platform    | HarmonyOS Support |
|--------------------|-----------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|----------|-------------|-------------------|
| value              | 文本内容                                                                    | string                                                                                    | yes      | iOS/Android | yes               |
| onSelection        | 用户点击选择菜单项时的回调(仅在API version 22开始支持)                                                  | ({ eventType: string, content: string, selectionStart: int, selectionEnd: int }) => void | no       | iOS/Android | yes               |
| menuItems          | 自定义菜单项数组(仅在API version 22开始支持)                                                            | array(string)                                                                             | no       | iOS/Android | yes               |
| style              | 应用于文本的额外样式                                                        | Object                                                                                    | no       | iOS/Android | yes               |
| highlights         | 需要高亮的文本范围数组，可包含 id                                           | array({ id: string, start: int, end: int })                                               | no       | iOS/Android | yes               |
| highlightColor     | 高亮背景颜色                                                                | string                                                                                    | no       | iOS/Android | yes               |
| onHighlightPress   | 用户点击高亮区域时的回调                                                    | (id: string) => void                                                                      | no       | iOS/Android | yes               |
| appendToChildren   | 追加到文本末尾的元素                                                        | ReactNode                                                                                 | no       | iOS/Android | yes               |
| TextComponent      | 用于渲染文本的自定义组件                                                    | ReactNode                                                                                 | no       | iOS/Android | yes               |
| textValueProp      | 传递给 TextComponent 的文本属性名，配合 TextComponent 使用，默认为 'children' | string                                                                                    | no       | iOS/Android | yes               |
| textComponentProps | 传递给 TextComponent 的额外属性                                             | object                                                                                    | no       | iOS/Android | yes               |

## 5. 遗留问题

暂无

## 6. 其他

暂无

## 7. 开源协议

本项目基于 [The MIT License (MIT)](https://github.com/Astrocoders/react-native-selectable-text/blob/master/LICENSE) ，请自由地享受和参与开源。
