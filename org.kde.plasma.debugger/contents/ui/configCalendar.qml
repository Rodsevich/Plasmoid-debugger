import QtQuick 2.0
import QtQuick.Controls 1.0 as QtControls
import QtQuick.Layouts 1.0 as QtLayouts
import org.kde.plasma.calendar 2.0 as PlasmaCalendar

Item {
    id: calendarPage
    width: childrenRect.width
    height: childrenRect.height

    signal configurationChanged

    property alias cfg_showWeekNumbers: showWeekNumbers.checked

    function saveConfig()
    {
        plasmoid.configuration.enabledCalendarPlugins = PlasmaCalendar.EventPluginsManager.enabledPlugins;
    }

    QtLayouts.ColumnLayout {
        QtControls.CheckBox {
            id: showWeekNumbers
            text: i18n("Show week numbers in Calendar")
        }

        QtControls.GroupBox {
            QtLayouts.Layout.fillWidth: true
            title: i18n("Available Calendar Plugins")
            flat: true

            Repeater {
                id: calendarPluginsRepeater
                model: PlasmaCalendar.EventPluginsManager.model
                delegate: QtLayouts.RowLayout {
                    QtControls.CheckBox {
                        text: model.display
                        checked: model.checked
                        onClicked: {
                            //needed for model's setData to be called
                            model.checked = checked;
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        PlasmaCalendar.EventPluginsManager.populateEnabledPluginsList(plasmoid.configuration.enabledCalendarPlugins);
    }
}
