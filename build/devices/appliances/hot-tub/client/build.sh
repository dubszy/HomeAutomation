source ./utils/arduino/arduino_builder.sh

mkdir -p ../out/devices/appliances/hot-tub/client
mkdir -p ../out/devices/appliances/hot-tub/client/cache

call_builder -verbose -compile -logger=machine -hardware "$arduino_hardware_dir" -tools "$arduino_tools_builder_dir" -tools "$arduino_tools_avr_dir" -built-in-libraries "$arduino_libraries_dir" -libraries "$arduino_user_libraries_dir" -fqbn=arduino:avr:uno -build-path "$(pwd)/../out/devices/appliances/hot-tub/client" -warnings=all -build-cache "$(pwd)/../out/devices/appliances/hot-tub/client/cache" -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.arduinoOTA.path="$arduino_tools_avr_dir" -prefs=runtime.tools.avr-gcc.path="$arduino_tools_avr_dir" -prefs=runtime.tools.avrdude.path="$arduino_tools_avr_dir" "$(pwd)/../src/devices/appliances/hot-tub/client/client.cpp"