<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/messages.proto

namespace Grpc\Testing;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * <pre>
 * TODO(dgq): Go back to using well-known types once
 * https://github.com/grpc/grpc/issues/6980 has been fixed.
 * import "google/protobuf/wrappers.proto";
 * </pre>
 *
 * Protobuf type <code>grpc.testing.BoolValue</code>
 */
class BoolValue extends \Google\Protobuf\Internal\Message
{
    /**
     * <pre>
     * The bool value.
     * </pre>
     *
     * <code>bool value = 1;</code>
     */
    private $value = false;

    public function __construct() {
        \GPBMetadata\Src\Proto\Grpc\Testing\Messages::initOnce();
        parent::__construct();
    }

    /**
     * <pre>
     * The bool value.
     * </pre>
     *
     * <code>bool value = 1;</code>
     */
    public function getValue()
    {
        return $this->value;
    }

    /**
     * <pre>
     * The bool value.
     * </pre>
     *
     * <code>bool value = 1;</code>
     */
    public function setValue($var)
    {
        GPBUtil::checkBool($var);
        $this->value = $var;
    }

}

