require_relative '../../config.rb'

server HA_BUILD_SERVER,       roles: %w{ha_node, build},        ssh_options: { forward_agent: true }
server HA_WEB_SERVER,         roles: %w{ha_node, web},          ssh_options: { forward_agent: true }
server HA_LIVING_ROOM_SERVER, roles: %w{ha_node, living_room},  ssh_options: { forward_agent: true }
server HA_KITCHEN_SERVER,     roles: %w{ha_node, kitchen},      ssh_options: { forward_agent: true }
server HA_BEDROOM_SERVER,     roles: %w{ha_node, bedroom},      ssh_options: { forward_agent: true }
server HA_GARAGE_SERVER,      roles: %w{ha_node, garage},       ssh_options: { forward_agent: true }
server HA_HOT_TUB_SERVER,     roles: %w{ha_node, hot_tub},      ssh_options: { forward_agent: true }
