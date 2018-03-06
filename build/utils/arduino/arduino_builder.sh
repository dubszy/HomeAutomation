printf "[pushd] "
pushd ~
user_root=$(pwd)
printf "[popd ] "
popd

arduino_dir=""
arduino_user_libraries_dir=""

if [[ $system == 'windows' ]]; then
	arduino_dir="C:\\Program Files (x86)\\Arduino"
	arduino_user_libraries_dir="$user_root/Documents/Arduino/libraries"
fi

arduino_builder_bin="arduino-builder.exe"
arduino_builder_ver=""
arduino_hardware_dir="$arduino_dir/hardware"
arduino_tools_avr_dir="$arduino_hardware_dir/tools/avr"
arduino_tools_builder_dir="$arduino_dir/tools-builder"
arduino_libraries_dir="$arduino_dir/libraries"

prereq_passed=0

function prereq() {
	printf "[check] Checking for Arduino directory\n"
	if [[ ! -d $arduino_dir ]]; then
		printf "[fatal] Arduino directory does not exist: $arduino_dir\n"
		exit -1
	fi
	printf "[check] Checking for Arduino Builder binary\n"
	if [[ ! -f "$arduino_dir/$arduino_builder_bin" ]]; then
		printf "[fatal] Arduino Builder binary does not exist: $arduino_builder_bin\n"
		exit -2
	fi
	printf "[check] Checking for hardware directory\n"
	if [[ ! -d "$arduino_hardware_dir" ]]; then
		printf "[fatal] Hardware directory does not exist: $arduino_hardware_dir\n"
		exit -3
	fi
	printf "[check] Checking for tools AVR directory\n"
	if [[ ! -d "$arduino_tools_avr_dir" ]]; then
		printf "[fatal] Tools AVR directory does not exist: $arduino_tools_avr_dir\n"
		exit -4
	fi
	printf "[check] Checking for tools builder directory\n"
	if [[ ! -d "$arduino_tools_builder_dir" ]]; then
		printf "[fatal] Tools builder directory does not exist: $arduino_tools_builder_dir\n"
		exit -5
	fi
	printf "[check] Checking for built-in libraries directory\n"
	if [[ ! -d "$arduino_libraries_dir" ]]; then
		printf "[fatal] Built-in libraries directory does not exist: $arduino_libraries_dir\n"
		exit -6
	fi
	
	if [[ ! -d "$arduino_user_libraries_directory" ]]; then
		printf "[warn ] User libraries directory does not exist: $arduino_user_libraries_dir\n"
	fi
	
	prereq_passed=1
}

function call_builder() {
	printf "\n\nArduino Builder called\n\n"
	if [[ $prereq_passed == 0 ]]; then
		prereq
	fi
	
	if [[ $(pwd) != $arduino_dir ]]; then
		printf "[pushd] "
		pushd "$arduino_dir"
		pushed=1
	fi
	
	printf "\n\n[param] Arduino Builder Parameters\n\n"
	echo $@
	printf "\n[param] End Arduino Builder Parameters\n\n\n"
	printf "[exec ] Starting Arduino Builder\n\n"
	./$arduino_builder_bin "$@"
	
	if [[ $pushed ]]; then
		printf "\n[popd ] "
		popd
	fi
}

function get_builder_version() {
	arduino_builder_ver=$(call_builder --version | grep -o -e '[0-9]*[.][0-9]*[.][0-9]*')
}
