package.path = package.path .. ";;;/semi/?.lua"
local bencode = require 'bencode'

local mcc = require 'mcc'
local opcode_callbacks = mcc.opcode_callbacks

local sub = string.sub

local buffer=''
local function process_incomming_messages()
--[[
  local c = term.getchar(term.NOWAIT)
  if c~=-1 then  print ('X', c) if c<255 then print ('Y', string.char(c)) end end
  if c ~= -1 and c < 255 then 
    buffer=buffer..string.char(c)
    print ('Z', #buffer, buffer)
--]]
---[[
  local str = uart.read( 0, 512, uart.NO_TIMEOUT )
  if #str > 0 then
    buffer = buffer..str
--]] 
    --print ('Z', #buffer, buffer)

    local decoded, nextpos = bencode.decode(buffer,1)
    if decoded then
      buffer=sub(buffer, nextpos)
      if type(decoded)=='table' then
      
        --local out = { decoded, #buffer, math.floor(collectgarbage('count')*1024) }
        --print(bencode.encode(out))
        
        local tpid = decoded.tpid or decoded[1]
        local opcode = decoded.opcode or decoded[2]
        local data = decoded.data or decoded[3]
        
        --print ('A', tpid,opcode,data)
        
        if tpid and opcode and opcode_callbacks[tpid] and opcode_callbacks[tpid][opcode] then 
          opcode_callbacks[tpid][opcode](data)
          --mcc.send_message(tpid, opcode, data)
        end
      
      end
      
      --elseif decoded == nil and nextpos == 'invalid type' then
      --  buffer=sub(buffer, 2)
    elseif #buffer > 2048 then 
      buffer=sub(buffer, 2)
      --print ('???', buffer, tostring(decoded))
    end
  end
end

while true do
  process_incomming_messages()
  mcc.tick()
end
