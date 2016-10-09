Quick Future
============

Quick Future is a QML wrapper of QFuture.

This project is still under development. 

**Example**

```
import Future 1.0

...

var future = FileActor.read(“tmp.txt”);
// FileActor is a C++ class registered as context property
// QFuture<QString> FileActor::read(const QString&file);
// It is not a part of the library

Future.onFinished(future, function(value) {
  // do something when it is finished.
});

Future.promise(future).then(function(value) {
  // Future.promise creates a promise object.
});

...

```

Custom Type Registration
========================

By default, QFuture<T> is not a standard type recognized by QVariant.
Therefore, it can not be used in QML environment.
You have to register QFuture<T> as a QMetaType per template type in order to get rid the error message.

The same rule applies in Quick Future too.
Common types like bool, int, qreal, QString, QByteArray, QVariantMap are pre-registered already.
For your own custom type, you can register it by:

```
#include <QFFuture>
Q_DECLARE_METATYPE(QFuture<CustomType>)

...

int main(int argc, char *argv[])
{

...
   QFFuture::registerType<CustomType>();
...

}


```


API
===

List of implemented API

Future.isFinished(future);

Future.onFinished(future, callback);
