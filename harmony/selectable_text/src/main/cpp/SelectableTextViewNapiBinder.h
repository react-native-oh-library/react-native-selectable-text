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

#ifndef HARMONY_SELECTABLE_TEXT_VIEW_SRC_MAIN_CPP_SELECTABLETEXTNONENAPIBINDER_H
#define HARMONY_SELECTABLE_TEXT_VIEW_SRC_MAIN_CPP_SELECTABLETEXTNONENAPIBINDER_H
#include "RNOHCorePackage/ComponentBinders/ViewComponentNapiBinder.h"
#include "Props.h"

namespace rnoh {

class SelectableTextViewNapiBinder : public ViewComponentNapiBinder {
public:
    napi_value createProps(napi_env env, facebook::react::ShadowView const shadowView) override {
        napi_value napiViewProps = ViewComponentNapiBinder::createProps(env, shadowView);
        auto propsObjBuilder = ArkJS(env).getObjectBuilder(napiViewProps);

        if (auto props = std::dynamic_pointer_cast<const facebook::react::RNCSelectableTextViewProps>(shadowView.props)) {
            propsObjBuilder.addProperty("value", props->value);
            propsObjBuilder.addProperty("selectable", props->selectable);
            
            auto menuItems = std::vector<napi_value>();
            for (auto menuItem : props->menuItems) {
                auto matrixObjectBuilder = ArkJS(env).createObjectBuilder();
                matrixObjectBuilder.addProperty("menuItem", menuItem);
                menuItems.push_back(matrixObjectBuilder.build());
            }
            propsObjBuilder.addProperty("menuItems", ArkJS(env).createArray(menuItems));
            
            return propsObjBuilder.build();
        }
        return napiViewProps;
    };
};

} // namespace rnoh
#endif
