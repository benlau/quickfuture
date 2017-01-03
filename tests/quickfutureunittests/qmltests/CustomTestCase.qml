import QtQuick 2.0
import QtTest 1.1

TestCase {

    function waitUntil(callback) {
        while (!callback()) {
            wait(100);
        }
    }

    function tick() {
        var i = 3;
        while (i--) {
            wait(0);
        }
    }

}
