# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: math.proto

require 'google/protobuf'

Google::Protobuf::DescriptorPool.generated_pool.build do
  add_file("math.proto", :syntax => :proto3) do
    add_message "math.DivArgs" do
      optional :dividend, :int64, 1
      optional :divisor, :int64, 2
    end
    add_message "math.DivReply" do
      optional :quotient, :int64, 1
      optional :remainder, :int64, 2
    end
    add_message "math.FibArgs" do
      optional :limit, :int64, 1
    end
    add_message "math.Num" do
      optional :num, :int64, 1
    end
    add_message "math.FibReply" do
      optional :count, :int64, 1
    end
  end
end

module Math
  DivArgs = Google::Protobuf::DescriptorPool.generated_pool.lookup("math.DivArgs").msgclass
  DivReply = Google::Protobuf::DescriptorPool.generated_pool.lookup("math.DivReply").msgclass
  FibArgs = Google::Protobuf::DescriptorPool.generated_pool.lookup("math.FibArgs").msgclass
  Num = Google::Protobuf::DescriptorPool.generated_pool.lookup("math.Num").msgclass
  FibReply = Google::Protobuf::DescriptorPool.generated_pool.lookup("math.FibReply").msgclass
end
