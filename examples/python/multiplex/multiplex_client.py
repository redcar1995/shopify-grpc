# Copyright 2016 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""A client that makes both Greeter and RouteGuide RPCs."""

from __future__ import print_function

import random
import time
import logging

import grpc

hw_protos, hw_services = grpc.protos_and_services("protos/helloworld.proto",
                                                  include_paths=["../.."])
rg_protos, rg_services = grpc.protos_and_services("protos/route_guide.proto",
                                                  include_paths=["../.."])
import route_guide_resources


def make_route_note(message, latitude, longitude):
    return rg_protos.RouteNote(message=message,
                               location=rg_protos.Point(latitude=latitude,
                                                        longitude=longitude))


def guide_get_one_feature(route_guide_stub, point):
    feature = route_guide_stub.GetFeature(point)
    if not feature.location:
        print("Server returned incomplete feature")
        return

    if feature.name:
        print("Feature called %s at %s" % (feature.name, feature.location))
    else:
        print("Found no feature at %s" % feature.location)


def guide_get_feature(route_guide_stub):
    guide_get_one_feature(
        route_guide_stub,
        rg_protos.Point(latitude=409146138, longitude=-746188906))
    guide_get_one_feature(route_guide_stub,
                          rg_protos.Point(latitude=0, longitude=0))


def guide_list_features(route_guide_stub):
    rectangle = rg_protos.Rectangle(lo=rg_protos.Point(latitude=400000000,
                                                       longitude=-750000000),
                                    hi=rg_protos.Point(latitude=420000000,
                                                       longitude=-730000000))
    print("Looking for features between 40, -75 and 42, -73")

    features = route_guide_stub.ListFeatures(rectangle)

    for feature in features:
        print("Feature called %s at %s" % (feature.name, feature.location))


def generate_route(feature_list):
    for _ in range(0, 10):
        random_feature = feature_list[random.randint(0, len(feature_list) - 1)]
        print("Visiting point %s" % random_feature.location)
        yield random_feature.location
        time.sleep(random.uniform(0.5, 1.5))


def guide_record_route(route_guide_stub):
    feature_list = route_guide_resources.read_route_guide_database()

    route_iterator = generate_route(feature_list)
    route_summary = route_guide_stub.RecordRoute(route_iterator)
    print("Finished trip with %s points " % route_summary.point_count)
    print("Passed %s features " % route_summary.feature_count)
    print("Travelled %s meters " % route_summary.distance)
    print("It took %s seconds " % route_summary.elapsed_time)


def generate_messages():
    messages = [
        make_route_note("First message", 0, 0),
        make_route_note("Second message", 0, 1),
        make_route_note("Third message", 1, 0),
        make_route_note("Fourth message", 0, 0),
        make_route_note("Fifth message", 1, 0),
    ]
    for msg in messages:
        print("Sending %s at %s" % (msg.message, msg.location))
        yield msg
        time.sleep(random.uniform(0.5, 1.0))


def guide_route_chat(route_guide_stub):
    responses = route_guide_stub.RouteChat(generate_messages())
    for response in responses:
        print("Received message %s at %s" %
              (response.message, response.location))


def run():
    # NOTE(gRPC Python Team): .close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with grpc.insecure_channel('localhost:50051') as channel:
        greeter_stub = hw_services.GreeterStub(channel)
        route_guide_stub = rg_services.RouteGuideStub(channel)
        greeter_response = greeter_stub.SayHello(
            hw_protos.HelloRequest(name='you'))
        print("Greeter client received: " + greeter_response.message)
        print("-------------- GetFeature --------------")
        guide_get_feature(route_guide_stub)
        print("-------------- ListFeatures --------------")
        guide_list_features(route_guide_stub)
        print("-------------- RecordRoute --------------")
        guide_record_route(route_guide_stub)
        print("-------------- RouteChat --------------")
        guide_route_chat(route_guide_stub)


if __name__ == '__main__':
    logging.basicConfig()
    run()
