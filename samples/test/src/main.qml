import QtQuick 2.2
import QtQuick.Window 2.1
import QtMultimedia 5.0
import CVCamera 1.0
import SVO 1.0

Window {
    visible: true
    width: camera.size.width
    height: camera.size.height
    maximumWidth: camera.size.width
    maximumHeight: camera.size.height

    CVCamera{
        id: camera
        device: 0
        size: "640x480"
    }

    SVO{
        id: svo
        sourceImage: camera.cvImage
    }

    VideoOutput{
        anchors.fill: parent
        source: camera
    }
}

