#ifndef QUICKFUTURE_H
#define QUICKFUTURE_H

#include "qffuture.h"

namespace QuickFuture {


    template <typename T>
    static void registerType() {
        QFFuture::registerType<T>();
    }

}

#endif // QUICKFUTURE_H
