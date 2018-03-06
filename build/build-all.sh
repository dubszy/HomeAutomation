source ./utils/bash/windowsify.sh
source ./utils/bash/detect_system.sh

### Hot Tub Arduino ##

source ./devices/appliances/hot-tub/client/build.sh
source ./devices/appliances/hot-tub/client/upload.sh

if [[ $system == 'windows' ]]; then
	# avrdude can't find directories formatted like Git Bash likes to format them on Windows
	windowsify_directory "$(pwd)/../out/devices/appliances/hot-tub/client/client.cpp.hex"
fi

# FIXME: This is a temporary solution, COM3 will not always be where the Arduino is connected
upload COM3 $windowsified_dir

### End Hot Tub Arduino ##