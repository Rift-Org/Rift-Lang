/////////////////////////////////////////////////////////////
///                                                       ///
///     ██████╗ ██╗███████╗████████╗                      ///
///     ██╔══██╗██║██╔════╝╚══██╔══╝                      ///
///     ██████╔╝██║█████╗     ██║                         ///
///     ██╔══██╗██║██╔══╝     ██║                         ///
///     ██║  ██║██║██║        ██║                         ///
///     ╚═╝  ╚═╝╚═╝╚═╝        ╚═╝                         ///
///     * RIFT CORE - The official compiler for Rift.     ///
///     * Copyright (c) 2024, Rift-Org                    ///
///     * License terms may be found in the LICENSE file. ///
///                                                       ///
/////////////////////////////////////////////////////////////

#pragma once

#pragma mark - Forward Declarations

#define __DEFAULT_FORWARD_1(class_name) __DEFAULT_FORWARD(class_name)
#define __DEFAULT_FORWARD_2(class1, class2) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2)
#define __DEFAULT_FORWARD_3(class1, class2, class3) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2) __DEFAULT_FORWARD(class3)
#define __DEFAULT_FORWARD_4(class1, class2, class3, class4) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2) __DEFAULT_FORWARD(class3) __DEFAULT_FORWARD(class4)
#define __DEFAULT_FORWARD_5(class1, class2, class3, class4, class5) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2) __DEFAULT_FORWARD(class3) __DEFAULT_FORWARD(class4) __DEFAULT_FORWARD(class5)
#define __DEFAULT_FORWARD_6(class1, class2, class3, class4, class5, class6) __DEFAULT_FORWARD(class1) __DEFAULT_FORWARD(class2) __DEFAULT_FORWARD(class3) __DEFAULT_FORWARD(class4) __DEFAULT_FORWARD(class5) __DEFAULT_FORWARD(class6)

#define __SELECT_FORWARD(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#define __DEFAULT_FORWARD(class_name) \
            template <typename T> \
            class class_name; \

#define __DEFAULT_FORWARD_EACH(...) \
            __DEFAULT_FORWARD(__VA_ARGS__)

#define __DEFAULT_FORWARD_VA(...) \
            __SELECT_FORWARD(__VA_ARGS__, __DEFAULT_FORWARD_6, __DEFAULT_FORWARD_5, \
                                __DEFAULT_FORWARD_4, __DEFAULT_FORWARD_3, \
                                    __DEFAULT_FORWARD_2, __DEFAULT_FORWARD_1) (__VA_ARGS__)