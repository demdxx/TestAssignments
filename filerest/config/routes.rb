Filerest::Application.routes.draw do
  scope "/api/v1" do
    scope "/file" do
      get    "/"          => "file#index"
      get    ":fname"     => "file#open",   :constraints => {:fname => /\w+/}     # Return ID
      put    ":id/close"  => "file#close",  :constraints => {:id => /\d+/}
      get    ":id/read"   => "file#read",   :constraints => {:id => /\d+/}
      put    ":id"        => "file#write",  :constraints => {:id => /\d+/}
      delete ":fname"     => "file#delete", :constraints => {:fname => /\w+/}
    end
  end
end
