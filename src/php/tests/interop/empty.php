<?php
// DO NOT EDIT! Generated by Protobuf-PHP protoc plugin 1.0
// Source: empty.proto
//   Date: 2015-04-01 00:58:16

namespace grpc\testing {

  class EmptyMessage extends \DrSlump\Protobuf\Message {


    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.EmptyMessage');

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }
  }
}

