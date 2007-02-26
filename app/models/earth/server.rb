require 'socket'

module Earth

  class Server < ActiveRecord::Base
    has_many :directories, :dependent => :delete_cascade, :order => :lft
  
    cattr_accessor :config
    cattr_accessor :heartbeat_grace_period
    self.config = YAML.load(::File.open(::File.dirname(__FILE__) + "/../../../config/earth-webapp.yml"))
    self.heartbeat_grace_period = eval(self.config["heartbeat_grace_period"])

    def Server.this_server
      Server.find_or_create_by_name(ENV["EARTH_HOSTNAME"] || this_hostname)
    end
    
    def Server.this_hostname
      Socket.gethostbyname(Socket.gethostname)[0]
    end
    
    # Equivalent to [size, recursive_file_count] (but faster)
    def size_and_count
      size_sum = 0
      count_sum = 0
      Earth::Directory.roots_for_server(self).each do |d|
        size, blocks, count = d.size_blocks_and_count
        size_sum += size
        count_sum += count
      end
      [size_sum, count_sum]
    end
    
    def size
      Earth::Directory.roots_for_server(self).map{|d| d.size}.sum      
    end
    
    def recursive_file_count
      Earth::Directory.roots_for_server(self).map{|d| d.recursive_file_count}.sum
    end
    
    def has_files?
      recursive_file_count > 0
    end
    
    def heartbeat
      self.heartbeat_time = Time.now
      save!
    end
    
    def daemon_alive?
      if heartbeat_time.nil? or daemon_version.nil?
        false
      else
        (heartbeat_time + heartbeat_interval + Earth::Server.heartbeat_grace_period) >= Time::now
      end
    end

    def cache_complete?
      roots = Earth::Directory.roots_for_server(self) 
      (not roots.empty?) and roots.all? { |d| d.cache_complete? and not d.children.empty? }
    end
  end
end
