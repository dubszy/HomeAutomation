source ./utils/arduino/avrdude.sh

function upload() {
	printf "\n\nUploading to Arduino\n\n"
	if [[ ! -n "$1" ]]; then
		printf "[fatal] Serial port Arduino is connected to must be supplied\n"
		exit -1
	fi
	
	if [[ ! -n "$2" ]]; then
		printf "[fatal] Full path to HEX file outputted by Arduino Builder must be supplied\n"
		exit -2
	fi 
	call_avrdude -C"$avrdude_conf" -v -patmega328p -carduino -P$1 -b115200 -D -Uflash:w:$2:i
}