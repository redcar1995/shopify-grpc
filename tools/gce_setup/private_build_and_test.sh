#!/bin/bash

# This script has to be run from the same directory as grpc_docker.sh and after grpc_docker.sh is sourced
#
# Sample Usage:
# ===============================
# ./private_build_and_test.sh [language] [environment: interop|cloud] [test case]
#                              [git base directory] [server name in interop environment] 
# sh private_build_and_test.sh java interop large_unary /usr/local/google/home/donnadionne/grpc-git grpc-docker-server1
# sh private_build_and_test.sh java cloud large_unary /usr/local/google/home/donnadionne/grpc-git
# =============================== 

# Arguments
LANGUAGE=$1
ENV=$2
TEST=$3
GIT=$4
PROJECT=${5:-"stoked-keyword-656"}
ZONE=${6:-"asia-east1-a"}
CLIENT=${7:-"grpc-docker-testclients1"}
SERVER=${8:-"grpc-docker-server"}

current_time=$(date "+%Y-%m-%d-%H-%M-%S")
result_file_name=private_result.$current_time.txt

sudo docker run --name="private_images" -v $GIT:/var/local/git-clone grpc/$LANGUAGE /var/local/git-clone/grpc/tools/dockerfile/grpc_$LANGUAGE/build.sh

sudo docker commit -m "private image" -a $USER private_images grpc/private_images

sudo docker tag -f grpc/private_images 0.0.0.0:5000/grpc/private_images

sudo docker push 0.0.0.0:5000/grpc/private_images

sudo docker rmi -f grpc/private_images

sudo docker rm private_images

gcloud compute --project $PROJECT ssh --zone $ZONE $CLIENT --command "sudo docker pull 0.0.0.0:5000/grpc/private_images"

gcloud compute --project $PROJECT ssh --zone $ZONE $CLIENT --command "sudo docker tag 0.0.0.0:5000/grpc/private_images grpc/$LANGUAGE"

source grpc_docker.sh

if [ $ENV == 'interop' ]
then
  grpc_interop_test $TEST $CLIENT $LANGUAGE $SERVER cxx
  grpc_interop_test $TEST $CLIENT $LANGUAGE $SERVER java
  grpc_interop_test $TEST $CLIENT $LANGUAGE $SERVER go
  grpc_interop_test $TEST $CLIENT $LANGUAGE $SERVER ruby
  grpc_interop_test $TEST $CLIENT $LANGUAGE $SERVER node
  grpc_interop_test $TEST $CLIENT $LANGUAGE $SERVER python
else
  if [ $ENV == 'cloud' ]
  then
    grpc_cloud_prod_test $TEST $CLIENT $LANGUAGE > /tmp/$result_file_name 2>&1
    gsutil cp /tmp/$result_file_name gs://$PROJECT-output/private_result/$result_file_name
  else
    grpc_cloud_prod_auth_test $TEST $CLIENT $LANGUAGE
  fi
fi

