import QtQuick 2.0
import Testable 1.0
import Future 1.0

TestableCase {

    name: "CallbackTests";

    function test_onFinished() {
        var called = false;

        Future.onFinished(FileActor.read("a-file-not-existed"), function(value) {
            called = true;
        });
        wait(1000);
        compare(called, true);
    }

}
