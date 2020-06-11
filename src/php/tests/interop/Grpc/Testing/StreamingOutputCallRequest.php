<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/messages.proto

namespace Grpc\Testing;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * Server-streaming request.
 *
 * Generated from protobuf message <code>grpc.testing.StreamingOutputCallRequest</code>
 */
class StreamingOutputCallRequest extends \Google\Protobuf\Internal\Message
{
    /**
     * Desired payload type in the response from the server.
     * If response_type is RANDOM, the payload from each response in the stream
     * might be of different types. This is to simulate a mixed type of payload
     * stream.
     *
     * Generated from protobuf field <code>.grpc.testing.PayloadType response_type = 1;</code>
     */
    protected $response_type = 0;
    /**
     * Configuration for each expected response message.
     *
     * Generated from protobuf field <code>repeated .grpc.testing.ResponseParameters response_parameters = 2;</code>
     */
    private $response_parameters;
    /**
     * Optional input payload sent along with the request.
     *
     * Generated from protobuf field <code>.grpc.testing.Payload payload = 3;</code>
     */
    protected $payload = null;
    /**
     * Whether server should return a given status
     *
     * Generated from protobuf field <code>.grpc.testing.EchoStatus response_status = 7;</code>
     */
    protected $response_status = null;

    /**
     * Constructor.
     *
     * @param array $data {
     *     Optional. Data for populating the Message object.
     *
     *     @type int $response_type
     *           Desired payload type in the response from the server.
     *           If response_type is RANDOM, the payload from each response in the stream
     *           might be of different types. This is to simulate a mixed type of payload
     *           stream.
     *     @type \Grpc\Testing\ResponseParameters[]|\Google\Protobuf\Internal\RepeatedField $response_parameters
     *           Configuration for each expected response message.
     *     @type \Grpc\Testing\Payload $payload
     *           Optional input payload sent along with the request.
     *     @type \Grpc\Testing\EchoStatus $response_status
     *           Whether server should return a given status
     * }
     */
    public function __construct($data = NULL) {
        \GPBMetadata\Src\Proto\Grpc\Testing\Messages::initOnce();
        parent::__construct($data);
    }

    /**
     * Desired payload type in the response from the server.
     * If response_type is RANDOM, the payload from each response in the stream
     * might be of different types. This is to simulate a mixed type of payload
     * stream.
     *
     * Generated from protobuf field <code>.grpc.testing.PayloadType response_type = 1;</code>
     * @return int
     */
    public function getResponseType()
    {
        return $this->response_type;
    }

    /**
     * Desired payload type in the response from the server.
     * If response_type is RANDOM, the payload from each response in the stream
     * might be of different types. This is to simulate a mixed type of payload
     * stream.
     *
     * Generated from protobuf field <code>.grpc.testing.PayloadType response_type = 1;</code>
     * @param int $var
     * @return $this
     */
    public function setResponseType($var)
    {
        GPBUtil::checkEnum($var, \Grpc\Testing\PayloadType::class);
        $this->response_type = $var;

        return $this;
    }

    /**
     * Configuration for each expected response message.
     *
     * Generated from protobuf field <code>repeated .grpc.testing.ResponseParameters response_parameters = 2;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getResponseParameters()
    {
        return $this->response_parameters;
    }

    /**
     * Configuration for each expected response message.
     *
     * Generated from protobuf field <code>repeated .grpc.testing.ResponseParameters response_parameters = 2;</code>
     * @param \Grpc\Testing\ResponseParameters[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setResponseParameters($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::MESSAGE, \Grpc\Testing\ResponseParameters::class);
        $this->response_parameters = $arr;

        return $this;
    }

    /**
     * Optional input payload sent along with the request.
     *
     * Generated from protobuf field <code>.grpc.testing.Payload payload = 3;</code>
     * @return \Grpc\Testing\Payload
     */
    public function getPayload()
    {
        return $this->payload;
    }

    /**
     * Optional input payload sent along with the request.
     *
     * Generated from protobuf field <code>.grpc.testing.Payload payload = 3;</code>
     * @param \Grpc\Testing\Payload $var
     * @return $this
     */
    public function setPayload($var)
    {
        GPBUtil::checkMessage($var, \Grpc\Testing\Payload::class);
        $this->payload = $var;

        return $this;
    }

    /**
     * Whether server should return a given status
     *
     * Generated from protobuf field <code>.grpc.testing.EchoStatus response_status = 7;</code>
     * @return \Grpc\Testing\EchoStatus
     */
    public function getResponseStatus()
    {
        return $this->response_status;
    }

    /**
     * Whether server should return a given status
     *
     * Generated from protobuf field <code>.grpc.testing.EchoStatus response_status = 7;</code>
     * @param \Grpc\Testing\EchoStatus $var
     * @return $this
     */
    public function setResponseStatus($var)
    {
        GPBUtil::checkMessage($var, \Grpc\Testing\EchoStatus::class);
        $this->response_status = $var;

        return $this;
    }

}

