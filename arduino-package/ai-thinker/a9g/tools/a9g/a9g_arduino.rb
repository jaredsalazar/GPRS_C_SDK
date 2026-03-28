require "base.rb"

puts "A9G Arduino upload helper loaded."

lod = ENV["A9G_ARDUINO_LOD"].to_s.dup
flash = ENV["A9G_ARDUINO_FLASHPROG"].to_s.dup
port = ENV["A9G_ARDUINO_COMPORT"].to_s.dup
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

def run_fastpf_upload(flash, lod)
  begin
    fastpfGo(flash.dup, lod.dup)
    puts "A9G Arduino fastpf upload finished."
  rescue Exception => e
    puts "A9G Arduino auto-upload failed: #{e.class}: #{e.message}"
    puts "Use Flash -> Launch fastpf with current LOD file and Flash Programmer."
  end
end

if autorun
  # CoolWatcher already opens the connection from the profile during startup.
  # Reopening it here can trigger extra prompts and race the upload.
  Thread.new do
    sleep 0.5
    run_fastpf_upload(flash, lod)
  end
  puts "A9G Arduino auto-upload armed."
else
  puts "A9G Arduino helper armed. Use Flash -> Launch fastpf with current LOD file and Flash Programmer."
end
