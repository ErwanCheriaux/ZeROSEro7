# User interface with asm, regs and cmd windows
define split
  layout split
  layout asm
  layout regs
  focus cmd
end

define r
  mon halt
  load
  mon reset
end

define l
  layout n
end

target ext :2331
mon endian little
r
