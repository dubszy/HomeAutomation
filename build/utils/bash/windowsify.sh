windowsified_dir=""

function windowsify_directory() {
	dir=$1
	dir=${dir/'/'/}
	dir=${dir/'/'/':\\'}
	dir=${dir//'/'/'\\'}
	windowsified_dir=$dir
}
