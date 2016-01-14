#!/bin/bash
#Interesante de ver: http://stackoverflow.com/questions/307503/whats-the-best-way-to-check-that-environment-variables-are-set-in-unix-shellscr
#  : ${STATE?"Need to set STATE"}
#  : ${DEST:?"Need to set DEST non-empty"}
#

#kdialog --passivepopup "tenia escruito... $(cat ~/ver_plasmoide)" 2
#if [ ! -e "~/ver_plasmoide" ]
#then
#	kdialog --passivepopup "tenia eso... $(cat ~/ver_plasmoide)" 2
#	echo "org.kde." > ~/ver_plasmoide
#fi

#set -xe
arch_cache=~/.cache/ver_plasmoide

OPTIND=1

while getopts "f:ir" opt; do
	case "$opt" in i)
		export QML_IMPORT_TRACE=1
		;;
	esac
	case "$opt" in f)
		echo "$(pwd)/$OPTARG" > $arch_cache
		;;
	esac
	case "$opt" in r)
		`kdialog --getopenfilename "$(cat $arch_cache || echo "~/src/")" 'application/x-desktop' > $arch_cache`
		;;
	esac
done

export QT_LOGGING_DEBUG=org.kde.plasma.debug=true
#export QML2_IMPORT_PATH=$HOME/custom_libs
#export QML2_IMPORT_PATH=/usr/lib/qt/qml/:/opt/kde/install/lib64/qml:$HOME/custom_libs
export QML2_IMPORT_PATH=/opt/kde/install/lib64/qml:$HOME/custom_libs

if [ ! -e $arch_cache ] || [ $(( `date +%s` - `stat -c %X $arch_cache` )) -gt 3600 ] #Si se accedio por ultima vez hace mas de una hora o no existe $arch_cache
then
	`kdialog --getopenfilename "$(cat $arch_cache || echo "~/src/")" 'application/x-desktop' > $arch_cache`
fi

dir=$(cat $arch_cache)
arch_desktop=`echo $dir | rev | cut -d / -f 1 | rev`
dir_desktop=`echo $dir | grep -o ^.*/`
nombre_carpeta_instalacion=`echo $dir_desktop | sed "s%.*/\(.*\)/$%\1%"`
nombre_real_instalacion=`cat $dir | grep "X-KDE-PluginInfo-Name" | sed "s/^.*=\(.*\)$/\1/"`
echo "Entrando a $dir_desktop$arch_desktop (=== $dir) Instalando desde '$nombre_carpeta_instalacion' con '$nombre_real_instalacion' de nombre real"
#if [ $nombre_real_instalacion != $nombre_carpeta_instalacion ]
#then
#	kdialog --error "it's a good practise to name the folder of plasmoid, the one containing $arch_desktop, same as the field X-KDE-PluginInfo-Name contained in there.\nAborting..."
#	exit 1
#fi

touch $arch_cache

if [[ -d "$HOME/.local/share/plasma/plasmoids/$nombre_real_instalacion" ]]; then
        plasmapkg2 -u $dir_desktop
else
        plasmapkg2 -i $dir_desktop
fi
#kdialog --passivepopup "largando plasmoidviewer '~/.local/share/plasma/plasmoid/$nombre_real_instalacion'..." 2
#plasmoidviewer "~/.local/share/plasma/plasmoid/$nombre_real_instalacion"
kdialog --passivepopup "largando plasmawindowed '$nombre_real_instalacion'..." 2
kbuildsycoca5
killall plasmawindowed
plasmawindowed "$nombre_real_instalacion" 2>&1 | tee ~/.cache/ver_plasmoide.out
