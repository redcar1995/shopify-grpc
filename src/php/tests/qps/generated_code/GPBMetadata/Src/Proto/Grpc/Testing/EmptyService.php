<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/empty_service.proto

namespace GPBMetadata\Src\Proto\Grpc\Testing;

class EmptyService
{
    public static $is_initialized = false;

    public static function initOnce() {
        $pool = \Google\Protobuf\Internal\DescriptorPool::getGeneratedPool();

        if (static::$is_initialized == true) {
          return;
        }
        $pool->internalAddGeneratedFile(hex2bin(
            "0a520a2a7372632f70726f746f2f677270632f74657374696e672f656d70" .
            "74795f736572766963652e70726f746f120c677270632e74657374696e67" .
            "320e0a0c456d70747953657276696365620670726f746f33"
        ), true);

        static::$is_initialized = true;
    }
}

