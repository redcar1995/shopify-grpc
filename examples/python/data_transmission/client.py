import time
import grpc

import demo_pb2_grpc
import demo_pb2

SERVER_ADDRESS = "localhost:23333"
CLIENT_ID = 1


# 一元模式(在一次调用中, 客户端只能向服务器传输一次请求数据, 服务器也只能返回一次响应)
# unary-unary(In a single call, the client can only send request once, and the server can
# only respond once.)
def simple_method(stub):
    print("--------------Call SimpleMethod Begin--------------")
    request = demo_pb2.Request(client_id=CLIENT_ID, request_data="called by Python client")
    response = stub.SimpleMethod(request)
    print("resp from server(%d), the message=%s" % (response.server_id, response.response_data))
    print("--------------Call SimpleMethod Over---------------")


# 客户端流模式（在一次调用中, 客户端可以多次向服务器传输数据, 但是服务器只能返回一次响应）
# stream-unary (In a single call, the client can transfer data to the server several times,
# but the server can only return a response once.)
def client_streaming_method(stub):
    print("--------------Call ClientStreamingMethod Begin--------------")

    # 创建一个生成器
    # create a generator
    def request_messages():
        for i in range(5):
            request = demo_pb2.Request(client_id=CLIENT_ID, request_data=("called by Python client, message:%d" % i))
            yield request

    response = stub.ClientStreamingMethod(request_messages())
    print("resp from server(%d), the message=%s" % (response.server_id, response.response_data))
    print("--------------Call ClientStreamingMethod Over---------------")


# 服务端流模式（在一次调用中, 客户端只能一次向服务器传输数据, 但是服务器可以多次返回响应）
# unary-stream (In a single call, the client can only transmit data to the server at one time,
# but the server can return the response many times.)
def server_streaming_method(stub):
    print("--------------Call ServerStreamingMethod Begin--------------")
    request = demo_pb2.Request(client_id=CLIENT_ID, request_data="called by Python client")
    response_iterator = stub.ServerStreamingMethod(request)
    for response in response_iterator:
        print("recv from server(%d), message=%s" % (response.server_id, response.response_data))

    print("--------------Call ServerStreamingMethod Over---------------")


# 双向流模式 (在一次调用中, 客户端和服务器都可以向对方多次收发数据)
# stream-stream (In a single call, both client and server can send and receive data
# to each other multiple times.)
def bidirectional_streaming_method(stub):
    print("--------------Call BidirectionalStreamingMethod Begin---------------")

    # 创建一个生成器
    # create a generator
    def request_messages():
        for i in range(5):
            request = demo_pb2.Request(client_id=CLIENT_ID, request_data=("called by Python client, message: %d" % i))
            yield request
            time.sleep(1)

    response_iterator = stub.BidirectionalStreamingMethod(request_messages())
    for response in response_iterator:
        print("recv from server(%d), message=%s" % (response.server_id, response.response_data))

    print("--------------Call BidirectionalStreamingMethod Over---------------")


def main():
    with grpc.insecure_channel(SERVER_ADDRESS) as channel:
        stub = demo_pb2_grpc.GRPCDemoStub(channel)

        simple_method(stub)

        client_streaming_method(stub)

        server_streaming_method(stub)

        bidirectional_streaming_method(stub)


if __name__ == '__main__':
    main()
