require_relative '../config.rb'

require_relative '../../../Logger/src/ruby/logger'

# config valid for current version and patch releases of Capistrano
lock "~> 3.10.2"

set :repo_url, 'git@github.com:dubszy/home-automation.git'
set :deploy_to, "/home/#{HA_USER}/deploy"

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
stop "integration/#{fetch(:branch)} doesn't exist" unless branch_exists(fetch(:branch))

# Check that the local branch matches the remote branch
local_matches_remote = `git diff integration/#{fetch(:branch)}`.to_s.empty?
stop "Local branch differs from remote" unless local_matches_remote

# Check that the local working tree is clean
working_tree_clean = `git status --porcelain`.to_s.empty?
stop 'Commit or stash your local changes' unless working_tree_clean

# Check for prerequisites on build server
# - cmake
# - git
namespace :deploy do
  desc 'Prerequisites'
  task :prerequisites do
    on roles(:build) do
      as HA_USER do
        puts 'Checking for cmake'
        execute 'which cmake'
        execute 'which git'
      end
    end
  end

  before 'deploy:symlink:shared', :prerequisites
end

# Fetch latest or clone repos we depend on
desc 'Checkout Dependent Repos'
task :checkout_logger do
  on roles(:build) do
    within "/home/#{HA_USER}/code" do
      as HA_USER do
        # FIXME: there is a bit of a cyclic issue here:
        # Logger depends on PropertiesManager and vice-versa, so if both don't exist, we're in trouble.
        # Logger and PropertiesManager should be built somewhere else, then we should fetch the compiled
        # binaries for them instead.
        #
        # TODO: drivers should be built elsewhere, and then we should fetch the binaries instead

        # Clone or fetch Logger
        if test('[ -d Logger ]')
          # Pull latest Logger if we have the repo already...
          within("/home/#{HA_USER}/code/Logger") { execute :git, 'pull', 'integration/master' }
        else
          # ..or else clone the repo
          execute :git, 'clone', 'git@github.com:/dubszy/Logger.git'
        end
        # Build Logger
        execute :cmake, '--build', "cmake-build-#{stage}"

        # Clone or fetch PropertiesManager
        if test('[ -d PropertiesManager ]')
          # Pull latest PropertiesManager if we have the repo already
          within("/home/#{HA_USER}/code/PropertiesManager") { execute :git, 'pull', 'integration/master' }
        else
          # ... or else clone the repo
          execute :git, 'clone', 'git@github.com:/dubszy/PropertiesManager.git'
        end
        # Build PropertiesManager

        # Clone or fetch drivers
        if test('[ -d drivers ]')
          # Pull latest drivers if we have the repo already
          within("/home/#{HA_USER}/code/drivers") { execute :git, 'pull', 'integration/master' }
        else
          # ... or else clone the repo
          execute :git, 'clone', 'git@github.com:dubszy/drivers.git'
        end
        execute :cmake, '--build', "cmake-build-#{stage}"
      end
    end
  end
end

desc 'Compile HANode'
task :compile_ha_node do
  on roles(:build) do
    puts 'Compiling HANode'
    execute 'cd /home/ha/code'
    execute "cmake --build cmake-build-#{stage} --target hanoded"
  end
end

desc 'Compile Kitchen Server'
task :compile_kitchen_server do
  on roles(:build) do
    puts 'Compiling Kitchen Server'
    execute 'cd /home/ha/code'
    execute "cmake --build cmake-build-#{stage} --target kitchend"
  end
end

desc 'Compile Living Room Server'
task :compile_living_room_server do
  on roles(:build) do
    puts 'Compiling Living Room Server'
    execute 'cd /home/ha/code'
    execute "cmake --build cmake-build-#{stage} --target livingroomd"
  end
end

desc 'Compile Bedroom Server'
task :compile_bedroom_server do
  on roles(:build) do
    puts 'Compiling Bedroom Server'
    execute 'cd /home/ha/code'
    execute "cmake --build cmake-build-#{stage} --target bedroomd"
  end
end

desc 'Compile Hot Tub Client'
task :compile_hot_tub_client do
  on roles(:build) do
    puts 'Compiling Hot Tub Client'
    execute 'cd /home/ha/code/build/devices/appliances/hot-tub/client'
    execute './build.sh'
  end
end

desc 'Upload Hot Tub Client'
task :upload_hot_tub_client do
  on roles(:hot_tub) do
    puts 'Uploading Hot Tub Client'
    execute 'cd /home/ha/code/build/devices/appliances/hot-tub/client'
    execute './upload.sh'
  end
end

desc 'Compile Hot Tub Server'
task :compile_hot_tub_server do
  on roles(:build) do
    puts 'Compiling Hot Tub Server'
    execute 'cd /home/ha/code'
    execute "cmake --build cmake-build-#{environment} --target HotTubServer"
  end
end

# after :compile_ha_node, :compile_kitchen_server
# after :compile_kitchen_server, :compile_living_room_server
# after :compile_living_room_server, :compile_bedroom_server
# after :compile_bedroom_server, :compile_garage_server
# after :compile_garage_server, :compile_hot_tub_client
# after :compile_hot_tub_client, :compile_hot_tub_server
# after :compile_hot_tub_server, :upload_hot_tub_client

namespace :deploy_web do
  desc 'Check Prerequisites'
  task :check_prerequisites do
    puts 'Checking Prerequisites'
    execute `which node`
  end

  desc 'Copy Web Server Assets'
  task :copy_web_server_assets do
    on roles(:web) do
      upload! '/home/ha/web', 'src/web', recursive: true
    end
  end

  desc 'Restart Node'
  task :restart_node do
    on roles(:web) do
      execute "cd /home/ha/web/server"
      execute "node server.js"
    end
  end
end

# after :check_prerequisites, :copy_web_server_assets
# after :copy_web_server_assets, :restart_node