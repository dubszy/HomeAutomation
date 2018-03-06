tools_avr_dir=""

if [[ $system == 'windows' ]]; then
	tools_avr_dir="C:\\Program Files (x86)\\Arduino\\hardware\\tools\\avr"
fi

avrdude_bin="bin/avrdude"
avrdude_conf="$tools_avr_dir/etc/avrdude.conf"

prereq_passed=0

function prereq() {
	printf "[check] Checking for tools AVR directory\n"
	if [[ ! -d "$tools_avr_dir" ]]; then
		printf "[fatal] Tools AVR directory does not exist: $tools_avr_dir\n"
		exit -1
	fi
	printf "[check] Checking for avrdude binary\n"
	if [[ ! -f "$tools_avr_dir/$avrdude_bin" ]]; then
		printf "[fatal] avrdude binary does not exist: $avrdude_bin\n"
		exit -2
	fi
	printf "[check] Checking for avrdude conf\n"
	if [[ ! -f "$avrdude_conf" ]]; then
		printf "[fatal] avrdude conf does not exist: $avrdude_conf\n"
	fi
	prereq_passed=1
}

function call_avrdude() {
	printf "\n\navrdude called\n\n"
	if [[ $prereq_passed == 0 ]]; then
		prereq
	fi
	
	if [[ $(pwd) != "$tools_avr_dir" ]]; then
		printf "\n[pushd] "
		pushd "$tools_avr_dir"
		pushed=1
	fi
	
	printf "\n\n[param] avrdude parameters\n\n"
	echo $@
	printf "\n[param] end avrdude parameters\n\n\n"
	printf "[exec ] Starting avrdude\n\n"
	printf "\033[38;5;208m"
	./$avrdude_bin "$@"
	printf "\033[0m"
	
	if [[ $pushed ]]; then
		printf "\n[popd ] "
		popd
	fi
}