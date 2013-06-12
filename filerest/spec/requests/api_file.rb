require 'spec_helper'

describe "api v1 file" do
  it "Get list of files" do
    get "/api/v1/file/"
    expect(response).to be_success
    expect(response.status).to eq(200)
  end

  fileID = nil
  testFile = 'test_file1'
  it "Open/Read/Write/Close/Delete file" do
    get "/api/v1/file/%s" % testFile
    expect(response).to be_success
    expect(response.status).to eq(200)
    result = JSON.parse(response.body)
    expect(result.has_key?("id")).to eq(true)
    fileID = result['id']

    if nil != fileID
      get "/api/v1/file/%s/read" % fileID
      expect(response).to be_success
      expect(response.status).to eq(200)
      result = JSON.parse(response.body)
      expect(result.has_key?("content")).to eq(true)

      put "/api/v1/file/%s" % fileID, :content => 'Text for testing'
      expect(response).to be_success
      expect(response.status).to eq(200)
      result = JSON.parse(response.body)
      expect(result.has_key?("status")).to eq(true)
      expect(result["status"]).to eq('ok')

      put "/api/v1/file/%s/close" % fileID
      expect(response).to be_success
      expect(response.status).to eq(200)
      result = JSON.parse(response.body)
      expect(result.has_key?("status")).to eq(true)
      expect(result["status"]).to eq('ok')

      delete "/api/v1/file/%s" % testFile
      expect(response).to be_success
      expect(response.status).to eq(200)
      result = JSON.parse(response.body)
      expect(result.has_key?("status")).to eq(true)
      expect(result["status"]).to eq('ok')
    end
  end
end