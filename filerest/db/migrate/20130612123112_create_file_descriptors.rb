class CreateFileDescriptors < ActiveRecord::Migration
  def change
    create_table :file_descriptors do |t|
      t.string :filename

      t.timestamps
    end
    add_index :file_descriptors, :filename, :unique => true
  end
end
