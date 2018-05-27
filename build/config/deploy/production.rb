require_relative '../../config.rb'

server HA_BUILD_SERVER,       user: HA_USER, roles: %w{ha_node, build}
server HA_WEB_SERVER,         user: HA_USER, roles: %w{ha_node, web}
server HA_KITCHEN_SERVER,     user: HA_USER, roles: %w{ha_node, kitchen}
server HA_LIVING_ROOM_SERVER, user: HA_USER, roles: %w{ha_node, living_room}
server HA_BEDROOM_SERVER,     user: HA_USER, roles: %w{ha_node, bedroom}
server HA_GARAGE_SERVER,      user: HA_USER, roles: %w{ha_node, garage}
server HA_HOT_TUB_SERVER,     user: HA_USER, roles: %w{ha_node, hot_tub}
