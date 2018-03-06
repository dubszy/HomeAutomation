uname_str=$(uname)
system=''

if [[ $uname_str == "MINGW"* ]]; then
	system='windows'
fi
