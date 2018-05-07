
require_relative 'config.rb'

lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

Gem::Specification.new do |spec|
  spec.name          = 'ha_build'
  spec.version       = '0.0.1'
  spec.authors       = ['Martin Waltman']
  spec.email         = ['martin@mkwiseman.com']

  spec.summary       = 'Build and deploy service for home automation network'
  spec.homepage      = 'https://github.com/dubszy/HomeAutomation/build'

  # Prevent pushing this gem to RubyGems.org. To allow pushes either set the 'allowed_push_host'
  # to allow pushing to a single host or delete this section to allow pushing to any host.
  if spec.respond_to?(:metadata)
    spec.metadata['allowed_push_host'] = HA_BUILD_SERVER.to_s
  else
    raise 'RubyGems 2.0 or newer is required to protect against public gem pushes'
  end

  spec.files         = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']

  spec.add_development_dependency 'bundler', '~> 1.16'
  spec.add_development_dependency 'capistrano', '~> 3.10'
end
