pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property string fontFamily: "Roboto Mono"
    readonly property QtObject fonts: QtObject {

        //line-height: 32px;
        readonly property font title_big: Qt.font({
                                                    "pixelSize": 32,
                                                    "weight": Font.ExtraBold,
                                                    "family": fontFamily
                                                 })
        //line-height: 24px;
        readonly property font title_accent: Qt.font({
                                                       "pixelSize": 24,
                                                       "weight": Font.ExtraBold,
                                                       "family": fontFamily
                                                    })

        readonly property font title: Qt.font({
                                                "pixelSize": 24,
                                                "weight": Font.Medium,
                                                "family": fontFamily
                                             })

        readonly property font subtitle_accent: Qt.font({
                                                          "pixelSize": 20,
                                                          "weight": Font.ExtraBold,
                                                          "family": fontFamily
                                                       })
        //line-height: 20px;
        readonly property font button_accent: Qt.font({
                                                        "pixelSize": 16,
                                                        "weight": Font.ExtraBold,
                                                        "family": fontFamily
                                                     })

        readonly property font button: Qt.font({
                                                 "pixelSize": 16,
                                                 "weight": Font.Medium,
                                                 "family": fontFamily
                                              })
        //line-height: 14px
        readonly property font button_middle: Qt.font({
                                                        "pixelSize": 14,
                                                        "weight": Font.ExtraBold,
                                                        "family": fontFamily
                                                     })
        //line-height: 12px
        readonly property font button_middle_subtext: Qt.font({
                                                                "pixelSize": 12,
                                                                "weight": Font.Medium,
                                                                "family": fontFamily
                                                             })
        //line-height: 16px
        readonly property font text_body_accent: Qt.font({
                                                           "pixelSize": 16,
                                                           "weight": Font.ExtraBold,
                                                           "family": fontFamily
                                                        })

        readonly property font text_body: Qt.font({
                                                    "pixelSize": 16,
                                                    "weight": Font.Medium,
                                                    "family": fontFamily
                                                 })

        readonly property font label_accent: Qt.font({
                                                       "pixelSize": 14,
                                                       "weight": Font.ExtraBold,
                                                       "family": fontFamily
                                                    })

        readonly property font label: Qt.font({
                                                "pixelSize": 14,
                                                "weight": Font.Medium,
                                                "family": fontFamily
                                             })

        readonly property font subtext_accent: Qt.font({
                                                         "pixelSize": 12,
                                                         "weight": Font.ExtraBold,
                                                         "family": fontFamily
                                                      })

        readonly property font subtext: Qt.font({
                                                  "pixelSize": 12,
                                                  "weight": Font.Medium,
                                                  "family": fontFamily
                                               })
    }

    readonly property QtObject units: QtObject {
        readonly property QtObject gaps: QtObject {
            readonly property int quarter: 4
            readonly property int half: 8
            readonly property int full: 16
            readonly property int topQuarter: quarter
            readonly property int topHalf: half
            readonly property int topFull: full
            readonly property int bottomQuarter: quarter
            readonly property int bottomHalf: half
            readonly property int bottomFull: full
            readonly property int leftQuarter: quarter
            readonly property int leftHalf: half
            readonly property int leftFull: full
            readonly property int rightQuarter: quarter
            readonly property int rightHalf: half
            readonly property int rightFull: full
        }

        property QtObject radius: QtObject {
            readonly property int quarter: 4
            readonly property int half: 8
            readonly property int full: 16
        }
    }
}
