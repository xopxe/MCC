local bencode = require  'bencode'
local encode = bencode.encode

local uwrite = uart.write


-- SENDING
----------------------------------------------------
local function send_message (spid, opcode, data)
  local s, err = encode({spid, opcode, data})
  uwrite (0, s)
end
----------------------------------------------------


-- POLLING
----------------------------------------------------
local poll_callbacks = {}
local MAX_POLL_CALLBACKS = 1024

local function register_poll_callback(cb) 
	if #poll_callbacks >= MAX_POLL_CALLBACKS then return -1 end
	poll_callbacks[#poll_callbacks+1] = cb
	return #poll_callbacks
end
local function unregister_poll_callback(n)
    if n<#poll_callbacks then 
      poll_callbacks[n] = poll_callbacks[#poll_callbacks]
    end
    poll_callbacks[#poll_callbacks]=nil
end
local function call_poll_callbacks(n)
  for i = 1, #poll_callbacks do
    poll_callbacks[i]()
  end
end
----------------------------------------------------

-- OPCODES
----------------------------------------------------
local opcode_callbacks = { 
  [0] = { -- admin
    [0] = function() end,
    [1] = function(data)
      send_message(0, 1, data)
    end,
  }
}

local function register_opcode_callbacks(cbs) 
	opcode_callbacks[#opcode_callbacks+1] = cbs
	return #opcode_callbacks  --pid
end
local function unregister_opcode_callbacks(pid)
    opcode_callbacks[pid]=false
end
----------------------------------------------------



return {
  -- polling
  tick = call_poll_callbacks,
  register_poll_callback = register_poll_callback,
  unregister_poll_callback = unregister_poll_callback,
  
  -- opcode
  opcode_callbacks = opcode_callbacks, 
  register_opcode_callbacks = register_opcode_callbacks,
  unregister_opcode_callbacks = unregister_opcode_callbacks,
  
  -- sending
  send_message = send_message,
}
