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

#ifndef HARMONY_SELECTABLE_TEXT_VIEW_SRC_MAIN_CPP_SELECTABLETEXTVIEWPACKAGE_H
#define HARMONY_SELECTABLE_TEXT_VIEW_SRC_MAIN_CPP_SELECTABLETEXTVIEWPACKAGE_H

#include "RNOH/Package.h"
#include "ComponentDescriptors.h"
#include "SelectableTextViewJSIBinder.h"
#include "SelectableTextViewNapiBinder.h"
#include "SelectableTextViewComponentInstance.h"
#include "SelectableTextViewEmitRequestHandler.h"

namespace rnoh {

class SelectableTextViewPackageComponentInstanceFactoryDelegate : public ComponentInstanceFactoryDelegate {
public:
    using ComponentInstanceFactoryDelegate::ComponentInstanceFactoryDelegate;
    ComponentInstance::Shared create(ComponentInstance::Context ctx) override {
        if (ctx.componentName == "RNCSelectableTextView") {
            return std::make_shared<SelectableTextViewComponentInstance>(std::move(ctx));
        }
        return nullptr;
    }
};

class SelectableTextViewPackage : public Package {
public:
    SelectableTextViewPackage(Package::Context ctx) : Package(ctx) {}

    ComponentInstanceFactoryDelegate::Shared createComponentInstanceFactoryDelegate() override {
        return std::make_shared<SelectableTextViewPackageComponentInstanceFactoryDelegate>();
    }

    std::vector<facebook::react::ComponentDescriptorProvider> createComponentDescriptorProviders() override {
        return {
            facebook::react::concreteComponentDescriptorProvider<
                facebook::react::RNCSelectableTextViewComponentDescriptor>(),
        };
    }

    ComponentJSIBinderByString createComponentJSIBinderByName() override {
        return {
            {"RNCSelectableTextView", std::make_shared<SelectableTextViewJSIBinder>()},
        };
    }

    ComponentNapiBinderByString createComponentNapiBinderByName() override {
        return {
            {"RNCSelectableTextView", std::make_shared<SelectableTextViewNapiBinder>()},
        };
    }

    EventEmitRequestHandlers createEventEmitRequestHandlers() override {
        return {
            std::make_shared<SelectableTextViewEmitRequestHandler>()
        };
    }
};

} // namespace rnoh
#endif