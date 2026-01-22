/**
 * MIT License
 *
 * Copyright (C) 2026 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "RNOH/arkui/NativeNodeApi.h"
#include "SelectableTextViewNode.h"
#include <arkui/native_node.h>
#include <arkui/native_type.h>
#include <arkui/native_interface.h>
#include <hilog/log.h>
#include <dlfcn.h>
#include <deviceinfo.h>
#include <info/application_target_sdk_version.h>

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_DOMAIN 0x0001
#define LOG_TAG "RNST"

#define RNST_LOGE(...) ((void)OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_DOMAIN, LOG_TAG, __VA_ARGS__))

namespace rnoh {

SelectableTextViewNode::SelectableTextViewNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_STACK)),
      m_onMenuItemClick(nullptr),
      m_childTextNode(nullptr) {
}

SelectableTextViewNode::~SelectableTextViewNode() {
}

void SelectableTextViewNode::setSelectable(bool selectable) {
    m_selectable = selectable;
    
    if (m_childTextNode != nullptr) {
        ArkUI_NumberValue copyOptionValue = {.i32 = selectable ? ARKUI_COPY_OPTIONS_LOCAL_DEVICE : ARKUI_COPY_OPTIONS_NONE};
        ArkUI_AttributeItem copyOptionItem = {&copyOptionValue, 1, nullptr, nullptr};
        NativeNodeApi::getInstance()->setAttribute(m_childTextNode, NODE_TEXT_COPY_OPTION, &copyOptionItem);
    }
}

void SelectableTextViewNode::bindCustomSelectionMenu(
    const std::vector<std::string>& menuItems,
    MenuItemClickCallback onMenuItemClick) {
    
    m_onMenuItemClick = onMenuItemClick;
    m_menuItemNames = menuItems;
    
    if (m_childTextNode != nullptr) {
        bindMenuToChildNode();
    }
}

void SelectableTextViewNode::bindMenuToChildNode() {
    if (m_childTextNode == nullptr) {
        return;
    }

#if defined(OH_CURRENT_API_VERSION) && OH_CURRENT_API_VERSION >= 22
    if (OH_GetSdkApiVersion() < 22) {
        RNST_LOGE("Skip API 22 logic: Current Device SDK %{public}d < 22", OH_GetSdkApiVersion());
        return;
    }    
    
    // 使用 dlsym 动态加载所有 API 22+ 函数
    void* lib = dlopen("libace_ndk.z.so", RTLD_LAZY);
    if (lib == nullptr) {
        RNST_LOGE("libace_ndk.z.so not available");
        return;
    }
    
    // 定义函数指针类型并加载
    auto fnCreate = (void* (*)())dlsym(lib, "OH_ArkUI_TextEditMenuOptions_Create");
    auto fnDispose = (void (*)(void*))dlsym(lib, "OH_ArkUI_TextEditMenuOptions_Dispose");
    auto fnRegisterCreate = (void (*)(void*, void*, void (*)(void*, void*)))
        dlsym(lib, "OH_ArkUI_TextEditMenuOptions_RegisterOnCreateMenuCallback");
    auto fnRegisterClick = (void (*)(void*, void*, bool (*)(const void*, int32_t, int32_t, void*)))
        dlsym(lib, "OH_ArkUI_TextEditMenuOptions_RegisterOnMenuItemClickCallback");
    
    if (!fnCreate || !fnRegisterCreate || !fnRegisterClick) {
        RNST_LOGE("API 22+ menu functions not available");
        dlclose(lib);
        return;
    }
    
    void* editMenuOptions = fnCreate();
    if (editMenuOptions == nullptr) {
        RNST_LOGE("OH_ArkUI_TextEditMenuOptions_Create failed");
        dlclose(lib);
        return;
    }
    
    // 注册创建菜单回调
    fnRegisterCreate(editMenuOptions, this, [](void* items, void* userData) {
        auto* node = static_cast<SelectableTextViewNode*>(userData);
        if (!node) return;
        
        void* lib2 = dlopen("libace_ndk.z.so", RTLD_LAZY);
        if (!lib2) return;
        
        auto fnClear = (void (*)(void*))dlsym(lib2, "OH_ArkUI_TextMenuItemArray_Clear");
        auto fnCreateItem = (void* (*)())dlsym(lib2, "OH_ArkUI_TextMenuItem_Create");
        auto fnSetContent = (void (*)(void*, const char*))dlsym(lib2, "OH_ArkUI_TextMenuItem_SetContent");
        auto fnSetId = (void (*)(void*, int32_t))dlsym(lib2, "OH_ArkUI_TextMenuItem_SetId");
        auto fnInsert = (void (*)(void*, void*, int32_t))dlsym(lib2, "OH_ArkUI_TextMenuItemArray_Insert");
        
        if (fnClear) fnClear(items);
        
        for (size_t i = 0; i < node->m_menuItemNames.size(); i++) {
            void* menuItem = fnCreateItem ? fnCreateItem() : nullptr;
            if (menuItem) {
                if (fnSetContent) fnSetContent(menuItem, node->m_menuItemNames[i].c_str());
                if (fnSetId) fnSetId(menuItem, 10000 + static_cast<int32_t>(i));
                if (fnInsert) fnInsert(items, menuItem, static_cast<int32_t>(i));
            }
        }
        dlclose(lib2);
    });
    
    // 注册菜单项点击回调
    fnRegisterClick(editMenuOptions, this,
        [](const void* item, int32_t start, int32_t end, void* userData) -> bool {
            auto* node = static_cast<SelectableTextViewNode*>(userData);
            if (!node) return false;
            
            void* lib2 = dlopen("libace_ndk.z.so", RTLD_LAZY);
            auto fnGetId = lib2 ? (void (*)(const void*, int32_t*))dlsym(lib2, "OH_ArkUI_TextMenuItem_GetId") : nullptr;
            
            int32_t itemId = 0;
            if (fnGetId) fnGetId(item, &itemId);
            if (lib2) dlclose(lib2);
            
            int32_t menuIndex = itemId - 10000;
            
            if (menuIndex >= 0 && static_cast<size_t>(menuIndex) < node->m_menuItemNames.size()) {
                if (node->m_onMenuItemClick) {
                    std::string selectedContent;
                    std::string value;
                    
                    if (node->m_childTextNode) {
                        const ArkUI_AttributeItem* contentItem = 
                            NativeNodeApi::getInstance()->getAttribute(node->m_childTextNode, NODE_TEXT_CONTENT);
                        if (contentItem && contentItem->string) {
                            value = contentItem->string;
                        }
                    }
                    
                    if (start >= 0 && end > start && !value.empty()) {
                        int utf16Index = 0;
                        size_t byteStart = 0, byteEnd = 0, i = 0;
                        
                        while (i < value.length() && utf16Index < end) {
                            unsigned char c = value[i];
                            size_t utf8CharLen = 1;
                            int utf16Units = 1;
                            
                            if ((c & 0x80) == 0) { utf8CharLen = 1; utf16Units = 1; }
                            else if ((c & 0xE0) == 0xC0) { utf8CharLen = 2; utf16Units = 1; }
                            else if ((c & 0xF0) == 0xE0) { utf8CharLen = 3; utf16Units = 1; }
                            else if ((c & 0xF8) == 0xF0) { utf8CharLen = 4; utf16Units = 2; }
                            
                            if (utf16Index == start || (utf16Index < start && utf16Index + utf16Units > start)) {
                                byteStart = i;
                            }
                            i += utf8CharLen;
                            utf16Index += utf16Units;
                            if (utf16Index >= end) { byteEnd = i; break; }
                        }
                        
                        if (byteEnd > byteStart && byteEnd <= value.length()) {
                            selectedContent = value.substr(byteStart, byteEnd - byteStart);
                        }
                    }
                    
                    node->m_onMenuItemClick(node->m_menuItemNames[menuIndex], selectedContent, start, end);
                }
                return true;
            }
            return false;
        });
    
    ArkUI_AttributeItem menuItem = {nullptr, 0, nullptr, editMenuOptions};
    auto result = NativeNodeApi::getInstance()->setAttribute(m_childTextNode, NODE_TEXT_EDIT_MENU_OPTIONS, &menuItem);
    
    if (result != 0) {
        RNST_LOGE("NODE_TEXT_EDIT_MENU_OPTIONS failed: %{public}d", result);
        if (fnDispose) fnDispose(editMenuOptions);
    }
    
    dlclose(lib);
 #endif   
}

void SelectableTextViewNode::onChildNodeInserted(ArkUI_NodeHandle childNode) {
    // 检查子节点类型，只对 Text 节点绑定菜单
    int32_t nodeType = OH_ArkUI_NodeUtils_GetNodeType(childNode);
    if (nodeType != ARKUI_NODE_TEXT) {
        RNST_LOGE("Child node is not a Text node (type: %{public}d), menu binding skipped", nodeType);
        return;
    }
    
    m_childTextNode = childNode;
    
    ArkUI_NumberValue copyOptionValue = {.i32 = m_selectable ? ARKUI_COPY_OPTIONS_LOCAL_DEVICE : ARKUI_COPY_OPTIONS_NONE};
    ArkUI_AttributeItem copyOptionItem = {&copyOptionValue, 1, nullptr, nullptr};
    NativeNodeApi::getInstance()->setAttribute(childNode, NODE_TEXT_COPY_OPTION, &copyOptionItem);
    
    if (m_selectable) {
        bindMenuToChildNode();
    }
}

void SelectableTextViewNode::insertChild(ArkUINode &child, std::size_t index) {
    ArkUI_NodeHandle childHandle = child.getArkUINodeHandle();
    maybeThrow(NativeNodeApi::getInstance()->insertChildAt(m_nodeHandle, childHandle, index));
    onChildNodeInserted(childHandle);
}

void SelectableTextViewNode::removeChild(ArkUINode &child) {
    maybeThrow(NativeNodeApi::getInstance()->removeChild(m_nodeHandle, child.getArkUINodeHandle()));
    if (child.getArkUINodeHandle() == m_childTextNode) {
        m_childTextNode = nullptr;
    }
}

} // namespace rnoh