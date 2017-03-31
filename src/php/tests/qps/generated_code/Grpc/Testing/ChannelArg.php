<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/control.proto

namespace Grpc\Testing;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * Protobuf type <code>grpc.testing.ChannelArg</code>
 */
class ChannelArg extends \Google\Protobuf\Internal\Message
{
    /**
     * <code>string name = 1;</code>
     */
    private $name = '';
    protected $value;

    public function __construct() {
        \GPBMetadata\Src\Proto\Grpc\Testing\Control::initOnce();
        parent::__construct();
    }

    /**
     * <code>string name = 1;</code>
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * <code>string name = 1;</code>
     */
    public function setName($var)
    {
        GPBUtil::checkString($var, True);
        $this->name = $var;
    }

    /**
     * <code>string str_value = 2;</code>
     */
    public function getStrValue()
    {
        return $this->readOneof(2);
    }

    /**
     * <code>string str_value = 2;</code>
     */
    public function setStrValue($var)
    {
        GPBUtil::checkString($var, True);
        $this->writeOneof(2, $var);
    }

    /**
     * <code>int32 int_value = 3;</code>
     */
    public function getIntValue()
    {
        return $this->readOneof(3);
    }

    /**
     * <code>int32 int_value = 3;</code>
     */
    public function setIntValue($var)
    {
        GPBUtil::checkInt32($var);
        $this->writeOneof(3, $var);
    }

    public function getValue()
    {
        return $this->whichOneof("value");
    }

}

