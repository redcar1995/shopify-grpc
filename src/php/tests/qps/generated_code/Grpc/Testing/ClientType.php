<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/control.proto

namespace Grpc\Testing;

use UnexpectedValueException;

/**
 * Protobuf type <code>grpc.testing.ClientType</code>
 */
class ClientType
{
    /**
     * Many languages support a basic distinction between using
     * sync or async client, and this allows the specification
     *
     * Generated from protobuf enum <code>SYNC_CLIENT = 0;</code>
     */
    const SYNC_CLIENT = 0;
    /**
     * Generated from protobuf enum <code>ASYNC_CLIENT = 1;</code>
     */
    const ASYNC_CLIENT = 1;
    /**
     * used for some language-specific variants
     *
     * Generated from protobuf enum <code>OTHER_CLIENT = 2;</code>
     */
    const OTHER_CLIENT = 2;
    /**
     * Generated from protobuf enum <code>CALLBACK_CLIENT = 3;</code>
     */
    const CALLBACK_CLIENT = 3;

    private static $valueToName = [
        self::SYNC_CLIENT => 'SYNC_CLIENT',
        self::ASYNC_CLIENT => 'ASYNC_CLIENT',
        self::OTHER_CLIENT => 'OTHER_CLIENT',
        self::CALLBACK_CLIENT => 'CALLBACK_CLIENT',
    ];

    public static function name($value)
    {
        if (!isset(self::$valueToName[$value])) {
            throw new UnexpectedValueException(sprintf(
                    'Enum %s has no name defined for value %s', __CLASS__, $value));
        }
        return self::$valueToName[$value];
    }


    public static function value($name)
    {
        $const = __CLASS__ . '::' . strtoupper($name);
        if (!defined($const)) {
            throw new UnexpectedValueException(sprintf(
                    'Enum %s has no value defined for name %s', __CLASS__, $name));
        }
        return constant($const);
    }
}

