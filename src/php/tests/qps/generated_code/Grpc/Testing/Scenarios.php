<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/control.proto

namespace Grpc\Testing;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * <pre>
 * A set of scenarios to be run with qps_json_driver
 * </pre>
 *
 * Protobuf type <code>grpc.testing.Scenarios</code>
 */
class Scenarios extends \Google\Protobuf\Internal\Message
{
    /**
     * <code>repeated .grpc.testing.Scenario scenarios = 1;</code>
     */
    private $scenarios;

    public function __construct() {
        \GPBMetadata\Src\Proto\Grpc\Testing\Control::initOnce();
        parent::__construct();
    }

    /**
     * <code>repeated .grpc.testing.Scenario scenarios = 1;</code>
     */
    public function getScenarios()
    {
        return $this->scenarios;
    }

    /**
     * <code>repeated .grpc.testing.Scenario scenarios = 1;</code>
     */
    public function setScenarios(&$var)
    {
        GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::MESSAGE, \Grpc\Testing\Scenario::class);
        $this->scenarios = $var;
    }

}

