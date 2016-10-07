import QtQuick 2.0
import Testable 1.0
import Future 1.0

TestableCase {

    name: "CallbackTests";

    function test_onFinished() {
        var called = false;
        var result;

        Future.onFinished(FileActor.read("a-file-not-existed"), function(value) {
            called = true;
            result = value;
        });
        wait(1000);
        compare(called, true);
        compare(result, "");
    }

    function test_onFinished_void() {
        var called = false;
        var result;

        Future.onFinished(FileActor.dummy(), function(value) {
            called = true;
            result = value;
        });
        wait(1000);
        compare(called, true);
        compare(result, undefined);
    }


}
