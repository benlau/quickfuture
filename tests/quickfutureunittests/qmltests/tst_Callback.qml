import QtQuick 2.0
import QtTest 1.1
import Testable 1.0
import QuickFuture 1.0
import FutureTests 1.0

CustomTestCase {

    name: "CallbackTests";

    function test_onFinished() {
        var called = false;
        var result;
        var future = Actor.read("a-file-not-existed");

        Future.onFinished(future, function(value) {
            called = true;
            result = value;
        });
        compare(called, false);
        wait(1000);
        compare(called, true);
        compare(result, "");
    }

    function test_onFinished_void() {
        var called = false;
        var result;

        var future = Actor.dummy();

        compare(Future.isFinished(future), false);
        compare(Future.isRunning(future), true);

        Future.onFinished(future, function(value) {
            called = true;
            result = value;
        });
        wait(1000);
        compare(called, true);
        compare(result, undefined);
        compare(Future.isRunning(future), false);
    }

    function test_alreadyFinished() {
        var called = false;

        var future = Actor.alreadyFinished();
        compare(Future.isFinished(future), true);
        Future.onFinished(future, function(value) {
            called = true;
        });
        compare(called, false);
        wait(10);
        compare(called, true);
    }

    function test_onCanceled() {
        var future = Actor.canceled();
        var called = false;

        compare(Future.isFinished(future), true);
        compare(Future.isRunning(future), false);
        compare(Future.isCanceled(future), true);

        Future.onCanceled(future, function() {
            called = true;
        });

        waitUntil(function() {
            return called;
        }, 1000);

        compare(called, true);
    }

    function test_onCanceledInt() {
        var future = Actor.canceledInt();
        var called = false;

        compare(Future.isFinished(future), true);
        compare(Future.isRunning(future), false);
        compare(Future.isCanceled(future), true);

        Future.onCanceled(future, function() {
            called = true;
        });

        waitUntil(function() {
            return called;
        }, 1000);

        compare(called, true);
    }

    function test_result() {
        var future, result;
        future = Actor.delayReturnBool(true);
        waitUntil(function() {
            return Future.isFinished(future);
        }, 1000);

        compare(Future.result(future), true);

        future = Actor.delayReturnQSize(Qt.size(1, 2));

        waitUntil(function() {
            return Future.isFinished(future);
        }, 1000);

        result = Future.result(future);
        compare(result.width, 1);
        compare(result.height, 2);

        future = Actor.delayReturnReply();

        waitUntil(function() {
            return Future.isFinished(future);
        }, 1000);

        result = Future.result(future);
        compare(result.code, -1);
        compare(result.message, "reply");
    }

    function test_results() {
        var future, result;
        var count = 50;
        future = Actor.delayMapped(3);

        waitUntil(function() {
            return Future.isFinished(future);
        }, 7000);

        var results = Future.results(future);

        compare(results, [0,1,4]);
    }

    function test_progressValueChanged() {
        var future, result;
        var count = 50;
        future = Actor.delayMapped(count);
        compare(Future.progressValue(future), 0);
        compare(Future.progressMinimum(future), 0);
        compare(Future.progressMaximum(future), count);
        var list = [];

        Future.onProgressValueChanged(future, function(value) {
            list.push(value);
        });

        waitUntil(function() {
            return Future.isFinished(future);
        }, 7000);

        compare(Future.isFinished(future), true);

        compare(Future.progressValue(future), count);
        compare(Future.progressMinimum(future), 0);
        compare(Future.progressMaximum(future), count);
        compare(list.length > 3, true);
    }


}
