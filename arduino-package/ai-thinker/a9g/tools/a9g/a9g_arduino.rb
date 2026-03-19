require "base.rb"

puts "A9G Arduino upload helper loaded."

lod = ENV["A9G_ARDUINO_LOD"].to_s
flash = ENV["A9G_ARDUINO_FLASHPROG"].to_s
port = ENV["A9G_ARDUINO_COMPORT"].to_s
autorun = ENV["A9G_ARDUINO_AUTORUN"].to_s == "1"

unless defined?(fastpfGo)
  begin
    require File.join($SOFT_WORKDIR, "toolpool/plugins/fastpf/fastpfplugin")
  rescue LoadError
  end
end

begin
  cwSetProfileEntry("lastLodForDownload", lod) unless lod.empty?
  cwSetProfileEntry("lastFlashProgForFastpf", flash) unless flash.empty?
  cwSetProfileEntry("lastcomport", port) unless port.empty?
rescue Exception => e
  puts "A9G Arduino profile setup warning: #{e.class}: #{e.message}"
end

if !port.empty?
  begin
    copen(port.to_i)
  rescue Exception => e
    puts "A9G Arduino could not open COM#{port}: #{e.class}: #{e.message}"
  end
end

if autorun
  begin
    fastpfGo(flash, lod)
    puts "A9G Arduino fastpf upload finished."
  rescue Exception => e
    puts "A9G Arduino auto-upload failed: #{e.class}: #{e.message}"
    puts "Use Flash -> Launch fastpf with current LOD file and Flash Programmer."
  end
else
  puts "A9G Arduino helper armed. Use Flash -> Launch fastpf with current LOD file and Flash Programmer."
end
