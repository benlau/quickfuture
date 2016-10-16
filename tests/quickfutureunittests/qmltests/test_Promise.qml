import QtQuick 2.0
import Testable 1.0
import Future 1.0

TestableCase {

    name: "PromiseTests";

    function test_Promise() {
        console.log("test_Promise");

        var called = false;
        var result;

        var promise = Future.promise(Actor.read("a-file-not-existed"));

        promise.then(function(value) {
            called = true;
            result = value
        });

        wait(1000);
        compare(called, true);
        compare(result, "");
    }

}
