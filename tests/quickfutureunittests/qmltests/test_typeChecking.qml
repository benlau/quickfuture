import QtQuick 2.0

Item {

    Component.onCompleted: {
        var future = FileActor.read("a-file-not-existed");
    }

}
