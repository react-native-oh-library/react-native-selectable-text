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

#ifndef SMART_SRC_MAIN_CPP_SELECTABLETEXTVIEWEMITREQUESTHANDLER_H
#define SMART_SRC_MAIN_CPP_SELECTABLETEXTVIEWEMITREQUESTHANDLER_H

#include <glog/logging.h>
#include "EventEmitters.h"
#include "RNOH/ArkJS.h"
#include "RNOH/EventEmitRequestHandler.h"

namespace rnoh {

    class SelectableTextViewEmitRequestHandler : public EventEmitRequestHandler {
    public:
        void handleEvent(EventEmitRequestHandler::Context const &ctx) override {
            ArkJS arkJs(ctx.env);
            auto eventName = ctx.eventName;
            auto eventEmitter =
                ctx.shadowViewRegistry->getEventEmitter<facebook::react::SelectableTextEventEmitter>(ctx.tag);
            if (eventEmitter == nullptr) {
                return;
            }
          if (eventName == "onSelection") {
                std::string eventType = arkJs.getString(arkJs.getObjectProperty(ctx.payload, "eventType"));
                std::string content = arkJs.getString(arkJs.getObjectProperty(ctx.payload, "content"));
                int selectionStart = arkJs.getInteger(arkJs.getObjectProperty(ctx.payload, "selectionStart"));
                int selectionEnd = arkJs.getInteger(arkJs.getObjectProperty(ctx.payload, "selectionEnd"));
                facebook::react::SelectableTextEventEmitter::SelectContent SelectContent{eventType, content, selectionStart, selectionEnd};
            
                eventEmitter->onSelection(SelectContent);
            } 
        }
    };

} // namespace rnoh
#endif