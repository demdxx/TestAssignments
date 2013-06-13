# Use only JSON format of result

class FileController < ApplicationController

  ###
  # Пожалуй единственный метод выбивающийся из общей концепции
  # Get list of files from 'Rails.application.config.rest_file_workdir'
  # @return []
  ###
  def index
    render json: listOfFiles.to_json
  end

  ###
  # Opend file by name
  # @param :name
  # @return :id
  ###
  def open
    status = 200

    # Find file descriptor if it is open
    fdesc = FileDescriptor.find(:first, conditions: {filename: params[:fname]})

    if nil == fdesc
      # If file descriptor does not exists
      begin
        fdesc = FileDescriptor.new(filename: params[:fname])
        fdesc.save!
      rescue Exception => e
        # We have something error
        result = {status: 'error', message: e.message}
        status = 500
      else
        # New descriptor ID
        result = {id: fdesc.id}
      end
    else
      # Reture old descriptor id warn about it
      result = {id: fdesc.id, warning: 'Already open'}
    end

    render json: result.to_json, status: status
  end

  ###
  # Destroy file descriptor
  # @param :id
  ###
  def close
    FileDescriptor.delete(params[:id])
    render json: {status: 'ok'}.to_json
  end

  ###
  # Read file by file descriptor id
  # @param :id
  # @return :content
  ###
  def read
    status = 200

    # File path
    fpath = filepathById params[:id]

    if nil == fpath
      # File description does not exists
      result = {status: 'error', message: 'Bad request'}
      status = 400
    elsif File.exists? fpath
      result = {content: File.read(fpath)}
    else
      result = {content: ''}
    end
    render json: result.to_json, status: status
  end

  ###
  # Write data to file
  # @param :id
  # @param :content
  # @return :status
  ###
  def write
    # File path
    fpath = filepathById params[:id]

    if nil == fpath
      # File description does not exists
      result = {status: 'error', message: 'Bad request'}
      status = 400
    else
      begin
        File.open(fpath, "w") do |f|
          f.write(params[:content])
        end
        result = {status: 'ok', file:fpath}
        status = 200
      rescue Exception => e
        result = {status: 'error', message: e.message}
        status = 500
      end
    end

    render json: result.to_json, status: status
  end

  ###
  # Delete file by name
  # @param :fname
  ###
  def delete
    unless FileDescriptor.exists?(filename: params[:fname])
      fpath = filePath params[:fname]
      begin
        File.delete fpath
        result = {status: 'ok'}
        status = 200
      rescue Exception => e
        result = {status: 'error', message: e.message}
        status = 500
      end
    else
      result = {status: 'error', message: 'File is open'}
      status = 403 # Forbidden
    end
    render json: result.to_json, status: status
  end

private

  def listOfFiles
    Dir.entries(Rails.application.config.rest_file_workdir) - [".", ".."]
  end

  def filePath(fname)
    File.expand_path(Rails.application.config.rest_file_workdir+fname, __FILE__)
  end

  def filepathById(id)
    # Find file descriptor if it is open
    begin
      fdesc = FileDescriptor.find(id)
      fpath = filePath(fdesc.filename)
    rescue
      fpath = nil
    end
    fpath
  end
end