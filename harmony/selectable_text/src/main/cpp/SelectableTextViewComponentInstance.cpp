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

#include "SelectableTextViewComponentInstance.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include "RNOHCorePackage/ComponentInstances/TextComponentInstance.h"

namespace rnoh {

SelectableTextViewComponentInstance::SelectableTextViewComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
    mSelectableTextNode.setAlignment(ARKUI_ALIGNMENT_TOP);
}

void SelectableTextViewComponentInstance::onChildInserted(
    ComponentInstance::Shared const& childComponentInstance, std::size_t index) {
    CppComponentInstance::onChildInserted(childComponentInstance, index);
    mSelectableTextNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
}

void SelectableTextViewComponentInstance::onChildRemoved(
    ComponentInstance::Shared const& childComponentInstance) {
    CppComponentInstance::onChildRemoved(childComponentInstance);
    mSelectableTextNode.removeChild(childComponentInstance->getLocalRootArkUINode());
}

SelectableTextViewNode &SelectableTextViewComponentInstance::getLocalRootArkUINode() { 
    return mSelectableTextNode; 
}

void SelectableTextViewComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    CppComponentInstance::onPropsChanged(props);
    
    mSelectableTextNode.setSelectable(props->selectable);
    
    mSelectableTextNode.bindCustomSelectionMenu(
        props->menuItems,
        [this](const std::string& eventType, 
               const std::string& content, 
               int selStart, int selEnd) {
            this->handleMenuItemClicked(eventType, content, selStart, selEnd);
        }
    );
}

void SelectableTextViewComponentInstance::handleMenuItemClicked(
    const std::string& menuItemName,
    const std::string& selectedContent,
    int selectionStart,
    int selectionEnd) {
    
    if (m_eventEmitter) {
        facebook::react::SelectableTextEventEmitter::SelectContent event{
            menuItemName,
            selectedContent,
            selectionStart,
            selectionEnd
        };
        m_eventEmitter->onSelection(event);
    }
}

} // namespace rnoh
