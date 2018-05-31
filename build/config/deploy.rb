require_relative '../config.rb'

require_relative '../../../Logger/src/ruby/logger'

# config valid for current version and patch releases of Capistrano
lock "~> 3.10.2"

set :repo_url, 'git@github.com:dubszy/HomeAutomation.git'
set :deploy_to, '/home/ha/deploy'

@log = CDAT::Logger.for_item('Deploy', :info)

def log
  @log
end

current_branch = `git rev-parse --abbrev-ref HEAD`.chomp

if "#{fetch(:stage)}" == 'prod'
  set :branch, 'master'
else
  set :branch, current_branch
end

# Check for existence of branch 'b'
def branch_exists(b)
  (!(`git branch -a | grep "remotes/integration/#{b}"`.to_s.empty?))
end

# Exit with an error message
def stop(message, *prefixes)
  log.error message, *prefixes
  exit
end

# Run a command locally and exit if the exit status is non-zero
def run_local(cmd)
  system cmd
  if $?.exitstatus != 0
    stop "#{cmd} failed; exit status: #{$?.exitstatus.to_s}"
  end
end

# Check that :branch exists
log.info "Checking that integration/#{fetch(:branch)} exists"
stop "integration/#{fetch(:branch)} doesn't exist" unless branch_exists(fetch(:branch))

# Check that the local branch matches the remote branch
log.info 'Checking that local branch matches remote'
local_matches_remote = `git diff integration/#{fetch(:branch)}`.to_s.empty?
#stop "Local branch differs from remote" unless local_matches_remote

# Check that the local working tree is clean
log.info 'Checking that the working tree is clean'
working_tree_clean = `git status --porcelain`.to_s.empty?
#stop 'Commit or stash your local changes' unless working_tree_clean

# Check for prerequisites on build server
# - cmake
# - git
namespace :deploy do

  desc 'Prerequisites'
  task :prerequisites do
    log.info 'Checking for cmake'
    which_cmake_output = ''
    on roles(:build) do
      which_cmake_output = capture :which, 'cmake', raise_on_non_zero_exit: false
    end
    stop 'cmake not found' if which_cmake_output.to_s.empty?
  end

  desc 'Copy HA Resources'
  task :copy_ha_resources do
    log.info 'Copying HA Resources'
    on roles(:hot_tub) do
      upload! '../res/etc/homeautomation/devices/appliances/hot-tub/default.properties',
              '/etc/homeautomation/devices/appliances/hot-tub/default.properties'
    end

    on roles(:ha_node) do
      upload! '../res/etc/homeautomation/ha_node/default.properties',
              '/etc/homeautomation/ha_node/default.properties'
    end
  end

  # Fetch latest or clone repos we depend on
  desc 'Fetch/Clone and Compile Dependent Repos'
  task :clone_or_fetch_deps do
    log.info 'Fetch/Clone and Compile: Logger, PropertiesManager, and drivers'
    on roles(:build) do
      within '/home/ha/deploy' do
        # Clone or fetch Logger
        if test('[ -d /home/ha/deploy/Logger ]')
          # Pull latest Logger if we have the repo already...
          within('Logger') { execute :git, 'pull' }
        else
          # ..or else clone the repo
          execute :git, 'clone', 'git@github.com:dubszy/Logger.git'
        end
        # Build Logger
        within ('Logger') do
          execute :cmake, '.' # Create or update cmake's autogen files
          execute :cmake, '--build', '.' # Build in-place
          execute :mv, 'liblogger.a', '/usr/local/lib/' # Install library
        end

        # Clone or fetch PropertiesManager
        if test('[ -d /home/ha/deploy/PropertiesManager ]')
          # Pull latest PropertiesManager if we have the repo already
          within('PropertiesManager') { execute :git, 'pull' }
        else
          # ... or else clone the repo
          execute :git, 'clone', 'git@github.com:dubszy/PropertiesManager.git'
        end
        # Build PropertiesManager
        within ('PropertiesManager') do
          execute :cmake, '.' # Create or update cmake's autogen files
          execute :cmake, '--build', '.' # Build in-place
          execute :mv, 'libPropertiesManager.a', '/usr/local/lib/' # Install library
        end

        # Clone or fetch drivers
        if test('[ -d /home/ha/deploy/drivers ]')
          # Pull latest drivers if we have the repo already
          within('drivers') { execute :git, 'pull' }
        else
          # ... or else clone the repo
          execute :git, 'clone', 'git@github.com:dubszy/drivers.git'
        end
        within ('drivers') do
          execute :cmake, '.' # Create or update cmake's autogen files
          execute :cmake, '--build', '.' # Build in-place
          execute :mv, '*.a', '/usr/local/lib/' # Install libraries
        end
      end
    end
  end

  desc 'Compile HomeAutomation'
  task :compile_home_automation do
    log.info 'Compiling Home Automation'
    on roles(:build) do
      within "#{release_path}" do
        execute :cmake, '.' # Create or update cmake's autogen files
        execute :cmake, '--build', '.' # Build in-place
        # Copy libraries
        execute :mv, 'hanoded', '/usr/local/bin'
        execute :mv, 'kitchend', '/usr/local/bin'
        execute :mv, 'livingroomd', '/usr/local/bin'
        execute :mv, 'bedroomd', '/usr/local/bin'
        execute :mv, 'HotTubServer', '/usr/local/bin/'
      end
    end
  end

  desc 'Copy Built Binaries'
  task :copy_built_binaries do
    log.info 'Copying built binaries'
    on roles(:ha_node) do
      download! "#{HA_BUILD_SERVER}:/usr/local/bin/ha_node", '/usr/local/bin/ha_node'
    end

    on roles(:living_room) do
      download! "#{HA_BUILD_SERVER}:/usr/local/bin/livingroomd", '/usr/local/bin/livingroomd'
    end

    on roles(:kitchen) do
      download! "#{HA_BUILD_SERVER}:/usr/local/bin/kitchend", '/usr/local/bin/kitchend'
    end

    on roles(:bedroom) do
      download! "#{HA_BUILD_SERVER}:/usr/local/bin/bedroomd", '/usr/local/bin/bedroomd'
    end

    on roles(:garage) do
      download! "#{HA_BUILD_SERVER}:/usr/local/bin/garaged", '/usr/local/bin/garaged'
    end

    on roles(:hot_tub) do
      download! "#{HA_BUILD_SERVER}:/usr/local/lib/HotTubServer", '/usr/local/lib/HotTubServer'
    end
  end

  desc 'Copy Web Server Assets'
  task :copy_web_server_assets do
    log.info 'Copying web server assets'
    on roles(:web) do
      upload! '/home/ha/web', 'src/web', recursive: true
    end
  end

  desc 'Restart Node'
  task :restart_node do
    log.info 'Restarting Node'
    on roles(:web) do
      within('/home/ha/web/server') do
        execute :node, 'server.js'
      end
    end
  end

  before 'deploy:symlink:shared', :prerequisites
  after :prerequisites, :copy_ha_resources
  after :prerequisites, :clone_or_fetch_deps
  after :clone_or_fetch_deps, :compile_home_automation
  after :compile_home_automation, :copy_built_binaries
  #after :copy_built_binaries, :copy_web_server_assets
  #after :copy_web_server_assets, :restart_node
end
