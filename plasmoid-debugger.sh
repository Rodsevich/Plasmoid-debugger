#!/bin/bash

desktop_path="$(dirname $0)/org.kde.plasma.debugger/metadata.desktop"
dir_desktop=`dirname $desktop_path`
nombre_real_instalacion=`cat $desktop_path | grep "X-KDE-PluginInfo-Name" | sed "s/^.*=\(.*\)$/\1/"`

if [[ -d "$HOME/.local/share/plasma/plasmoids/$nombre_real_instalacion" ]]; then
        plasmapkg2 -u $dir_desktop
else
        plasmapkg2 -i $dir_desktop
fi
#kdialog --passivepopup "largando plasmoidviewer '~/.local/share/plasma/plasmoid/$nombre_real_instalacion'..." 2
#plasmoidviewer "~/.local/share/plasma/plasmoid/$nombre_real_instalacion"
kdialog --passivepopup "Running plasmawindowed '$nombre_real_instalacion'..." 2
kbuildsycoca5
killall plasmawindowed
plasmawindowed "$nombre_real_instalacion" 2>&1 | tee ~/.cache/plasmoid_debugger.out
