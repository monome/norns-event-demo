local mod = require 'core/mods'

-- grab this mods name (as installed)
local this_name = mod.this_name

mod.hook.register("system_post_startup", "event_demo", function()
  -- extend the compile module path and load event_demo module globally
  package.cpath = package.cpath .. ";" .. paths.code .. this_name .. "/lib/?.so"
  event_demo = require 'event_demo'
end)
