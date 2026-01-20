import {
    HostComponent,
    ViewProps,
} from "react-native";

import codegenNativeComponent from "react-native/Libraries/Utilities/codegenNativeComponent"
import { Int32, DirectEventHandler, WithDefault } from "react-native/Libraries/Types/CodegenTypes";

export interface SelectableTextProps extends ViewProps {
    value?: string;
    selectable?: boolean;
    menuItems?: string[];
    onSelection?: DirectEventHandler<{
        eventType: string;
        content: string;
        selectionStart: Int32;
        selectionEnd: Int32;
    }>;
}

export default codegenNativeComponent<SelectableTextProps>("RNCSelectableTextView") as HostComponent<SelectableTextProps>;
