#ifndef QUICKFUTURE_H
#define QUICKFUTURE_H

#include <functional>
#include "qffuture.h"

namespace QuickFuture {

    template <typename T>
    static void registerType() {
        Future::registerType<T>();
    }

    template <typename T>
    static void registerType(std::function<QVariant(T)> converter) {
        Future::registerType<T>(converter);
    }

}

#endif // QUICKFUTURE_H
