# В дальгейшем открытые фалы можно привязать к пользователям

class FileDescriptor < ActiveRecord::Base
  attr_accessible :filename
end
