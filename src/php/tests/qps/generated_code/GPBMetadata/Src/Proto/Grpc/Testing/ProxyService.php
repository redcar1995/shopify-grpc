<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/proxy-service.proto

namespace GPBMetadata\Src\Proto\Grpc\Testing;

class ProxyService
{
    public static $is_initialized = false;

    public static function initOnce() {
        $pool = \Google\Protobuf\Internal\DescriptorPool::getGeneratedPool();

        if (static::$is_initialized == true) {
          return;
        }
        \GPBMetadata\Src\Proto\Grpc\Testing\Control::initOnce();
        $pool->internalAddGeneratedFile(hex2bin(
            "0a97020a2a7372632f70726f746f2f677270632f74657374696e672f7072" .
            "6f78792d736572766963652e70726f746f120c677270632e74657374696e" .
            "671a247372632f70726f746f2f677270632f74657374696e672f636f6e74" .
            "726f6c2e70726f746f221c0a0950726f787953746174120f0a076c617465" .
            "6e6379180120012801328e010a1250726f7879436c69656e745365727669" .
            "6365123b0a09476574436f6e66696712122e677270632e74657374696e67" .
            "2e566f69641a1a2e677270632e74657374696e672e436c69656e74436f6e" .
            "666967123b0a0a5265706f727454696d6512172e677270632e7465737469" .
            "6e672e50726f7879537461741a122e677270632e74657374696e672e566f" .
            "69642801620670726f746f33"
        ));

        static::$is_initialized = true;
    }
}

