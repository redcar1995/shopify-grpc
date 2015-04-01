<?php
// DO NOT EDIT! Generated by Protobuf-PHP protoc plugin 1.0
// Source: messages.proto
//   Date: 2015-04-01 00:58:16

namespace grpc\testing {

  class PayloadType extends \DrSlump\Protobuf\Enum {
    const COMPRESSABLE = 0;
    const UNCOMPRESSABLE = 1;
    const RANDOM = 2;
  }
}
namespace grpc\testing {

  class Payload extends \DrSlump\Protobuf\Message {

    /**  @var int - \grpc\testing\PayloadType */
    public $type = \grpc\testing\PayloadType::COMPRESSABLE;
    
    /**  @var string */
    public $body = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.Payload');

      // OPTIONAL ENUM type = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "type";
      $f->type      = \DrSlump\Protobuf::TYPE_ENUM;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $f->reference = '\grpc\testing\PayloadType';
      $f->default   = \grpc\testing\PayloadType::COMPRESSABLE;
      $descriptor->addField($f);

      // OPTIONAL BYTES body = 2
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 2;
      $f->name      = "body";
      $f->type      = \DrSlump\Protobuf::TYPE_BYTES;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <type> has a value
     *
     * @return boolean
     */
    public function hasType(){
      return $this->_has(1);
    }
    
    /**
     * Clear <type> value
     *
     * @return \grpc\testing\Payload
     */
    public function clearType(){
      return $this->_clear(1);
    }
    
    /**
     * Get <type> value
     *
     * @return int - \grpc\testing\PayloadType
     */
    public function getType(){
      return $this->_get(1);
    }
    
    /**
     * Set <type> value
     *
     * @param int - \grpc\testing\PayloadType $value
     * @return \grpc\testing\Payload
     */
    public function setType( $value){
      return $this->_set(1, $value);
    }
    
    /**
     * Check if <body> has a value
     *
     * @return boolean
     */
    public function hasBody(){
      return $this->_has(2);
    }
    
    /**
     * Clear <body> value
     *
     * @return \grpc\testing\Payload
     */
    public function clearBody(){
      return $this->_clear(2);
    }
    
    /**
     * Get <body> value
     *
     * @return string
     */
    public function getBody(){
      return $this->_get(2);
    }
    
    /**
     * Set <body> value
     *
     * @param string $value
     * @return \grpc\testing\Payload
     */
    public function setBody( $value){
      return $this->_set(2, $value);
    }
  }
}

namespace grpc\testing {

  class SimpleRequest extends \DrSlump\Protobuf\Message {

    /**  @var int - \grpc\testing\PayloadType */
    public $response_type = \grpc\testing\PayloadType::COMPRESSABLE;
    
    /**  @var int */
    public $response_size = null;
    
    /**  @var \grpc\testing\Payload */
    public $payload = null;
    
    /**  @var boolean */
    public $fill_username = null;
    
    /**  @var boolean */
    public $fill_oauth_scope = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.SimpleRequest');

      // OPTIONAL ENUM response_type = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "response_type";
      $f->type      = \DrSlump\Protobuf::TYPE_ENUM;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $f->reference = '\grpc\testing\PayloadType';
      $f->default   = \grpc\testing\PayloadType::COMPRESSABLE;
      $descriptor->addField($f);

      // OPTIONAL INT32 response_size = 2
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 2;
      $f->name      = "response_size";
      $f->type      = \DrSlump\Protobuf::TYPE_INT32;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      // OPTIONAL MESSAGE payload = 3
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 3;
      $f->name      = "payload";
      $f->type      = \DrSlump\Protobuf::TYPE_MESSAGE;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $f->reference = '\grpc\testing\Payload';
      $descriptor->addField($f);

      // OPTIONAL BOOL fill_username = 4
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 4;
      $f->name      = "fill_username";
      $f->type      = \DrSlump\Protobuf::TYPE_BOOL;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      // OPTIONAL BOOL fill_oauth_scope = 5
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 5;
      $f->name      = "fill_oauth_scope";
      $f->type      = \DrSlump\Protobuf::TYPE_BOOL;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <response_type> has a value
     *
     * @return boolean
     */
    public function hasResponseType(){
      return $this->_has(1);
    }
    
    /**
     * Clear <response_type> value
     *
     * @return \grpc\testing\SimpleRequest
     */
    public function clearResponseType(){
      return $this->_clear(1);
    }
    
    /**
     * Get <response_type> value
     *
     * @return int - \grpc\testing\PayloadType
     */
    public function getResponseType(){
      return $this->_get(1);
    }
    
    /**
     * Set <response_type> value
     *
     * @param int - \grpc\testing\PayloadType $value
     * @return \grpc\testing\SimpleRequest
     */
    public function setResponseType( $value){
      return $this->_set(1, $value);
    }
    
    /**
     * Check if <response_size> has a value
     *
     * @return boolean
     */
    public function hasResponseSize(){
      return $this->_has(2);
    }
    
    /**
     * Clear <response_size> value
     *
     * @return \grpc\testing\SimpleRequest
     */
    public function clearResponseSize(){
      return $this->_clear(2);
    }
    
    /**
     * Get <response_size> value
     *
     * @return int
     */
    public function getResponseSize(){
      return $this->_get(2);
    }
    
    /**
     * Set <response_size> value
     *
     * @param int $value
     * @return \grpc\testing\SimpleRequest
     */
    public function setResponseSize( $value){
      return $this->_set(2, $value);
    }
    
    /**
     * Check if <payload> has a value
     *
     * @return boolean
     */
    public function hasPayload(){
      return $this->_has(3);
    }
    
    /**
     * Clear <payload> value
     *
     * @return \grpc\testing\SimpleRequest
     */
    public function clearPayload(){
      return $this->_clear(3);
    }
    
    /**
     * Get <payload> value
     *
     * @return \grpc\testing\Payload
     */
    public function getPayload(){
      return $this->_get(3);
    }
    
    /**
     * Set <payload> value
     *
     * @param \grpc\testing\Payload $value
     * @return \grpc\testing\SimpleRequest
     */
    public function setPayload(\grpc\testing\Payload $value){
      return $this->_set(3, $value);
    }
    
    /**
     * Check if <fill_username> has a value
     *
     * @return boolean
     */
    public function hasFillUsername(){
      return $this->_has(4);
    }
    
    /**
     * Clear <fill_username> value
     *
     * @return \grpc\testing\SimpleRequest
     */
    public function clearFillUsername(){
      return $this->_clear(4);
    }
    
    /**
     * Get <fill_username> value
     *
     * @return boolean
     */
    public function getFillUsername(){
      return $this->_get(4);
    }
    
    /**
     * Set <fill_username> value
     *
     * @param boolean $value
     * @return \grpc\testing\SimpleRequest
     */
    public function setFillUsername( $value){
      return $this->_set(4, $value);
    }
    
    /**
     * Check if <fill_oauth_scope> has a value
     *
     * @return boolean
     */
    public function hasFillOauthScope(){
      return $this->_has(5);
    }
    
    /**
     * Clear <fill_oauth_scope> value
     *
     * @return \grpc\testing\SimpleRequest
     */
    public function clearFillOauthScope(){
      return $this->_clear(5);
    }
    
    /**
     * Get <fill_oauth_scope> value
     *
     * @return boolean
     */
    public function getFillOauthScope(){
      return $this->_get(5);
    }
    
    /**
     * Set <fill_oauth_scope> value
     *
     * @param boolean $value
     * @return \grpc\testing\SimpleRequest
     */
    public function setFillOauthScope( $value){
      return $this->_set(5, $value);
    }
  }
}

namespace grpc\testing {

  class SimpleResponse extends \DrSlump\Protobuf\Message {

    /**  @var \grpc\testing\Payload */
    public $payload = null;
    
    /**  @var string */
    public $username = null;
    
    /**  @var string */
    public $oauth_scope = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.SimpleResponse');

      // OPTIONAL MESSAGE payload = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "payload";
      $f->type      = \DrSlump\Protobuf::TYPE_MESSAGE;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $f->reference = '\grpc\testing\Payload';
      $descriptor->addField($f);

      // OPTIONAL STRING username = 2
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 2;
      $f->name      = "username";
      $f->type      = \DrSlump\Protobuf::TYPE_STRING;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      // OPTIONAL STRING oauth_scope = 3
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 3;
      $f->name      = "oauth_scope";
      $f->type      = \DrSlump\Protobuf::TYPE_STRING;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <payload> has a value
     *
     * @return boolean
     */
    public function hasPayload(){
      return $this->_has(1);
    }
    
    /**
     * Clear <payload> value
     *
     * @return \grpc\testing\SimpleResponse
     */
    public function clearPayload(){
      return $this->_clear(1);
    }
    
    /**
     * Get <payload> value
     *
     * @return \grpc\testing\Payload
     */
    public function getPayload(){
      return $this->_get(1);
    }
    
    /**
     * Set <payload> value
     *
     * @param \grpc\testing\Payload $value
     * @return \grpc\testing\SimpleResponse
     */
    public function setPayload(\grpc\testing\Payload $value){
      return $this->_set(1, $value);
    }
    
    /**
     * Check if <username> has a value
     *
     * @return boolean
     */
    public function hasUsername(){
      return $this->_has(2);
    }
    
    /**
     * Clear <username> value
     *
     * @return \grpc\testing\SimpleResponse
     */
    public function clearUsername(){
      return $this->_clear(2);
    }
    
    /**
     * Get <username> value
     *
     * @return string
     */
    public function getUsername(){
      return $this->_get(2);
    }
    
    /**
     * Set <username> value
     *
     * @param string $value
     * @return \grpc\testing\SimpleResponse
     */
    public function setUsername( $value){
      return $this->_set(2, $value);
    }
    
    /**
     * Check if <oauth_scope> has a value
     *
     * @return boolean
     */
    public function hasOauthScope(){
      return $this->_has(3);
    }
    
    /**
     * Clear <oauth_scope> value
     *
     * @return \grpc\testing\SimpleResponse
     */
    public function clearOauthScope(){
      return $this->_clear(3);
    }
    
    /**
     * Get <oauth_scope> value
     *
     * @return string
     */
    public function getOauthScope(){
      return $this->_get(3);
    }
    
    /**
     * Set <oauth_scope> value
     *
     * @param string $value
     * @return \grpc\testing\SimpleResponse
     */
    public function setOauthScope( $value){
      return $this->_set(3, $value);
    }
  }
}

namespace grpc\testing {

  class StreamingInputCallRequest extends \DrSlump\Protobuf\Message {

    /**  @var \grpc\testing\Payload */
    public $payload = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.StreamingInputCallRequest');

      // OPTIONAL MESSAGE payload = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "payload";
      $f->type      = \DrSlump\Protobuf::TYPE_MESSAGE;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $f->reference = '\grpc\testing\Payload';
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <payload> has a value
     *
     * @return boolean
     */
    public function hasPayload(){
      return $this->_has(1);
    }
    
    /**
     * Clear <payload> value
     *
     * @return \grpc\testing\StreamingInputCallRequest
     */
    public function clearPayload(){
      return $this->_clear(1);
    }
    
    /**
     * Get <payload> value
     *
     * @return \grpc\testing\Payload
     */
    public function getPayload(){
      return $this->_get(1);
    }
    
    /**
     * Set <payload> value
     *
     * @param \grpc\testing\Payload $value
     * @return \grpc\testing\StreamingInputCallRequest
     */
    public function setPayload(\grpc\testing\Payload $value){
      return $this->_set(1, $value);
    }
  }
}

namespace grpc\testing {

  class StreamingInputCallResponse extends \DrSlump\Protobuf\Message {

    /**  @var int */
    public $aggregated_payload_size = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.StreamingInputCallResponse');

      // OPTIONAL INT32 aggregated_payload_size = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "aggregated_payload_size";
      $f->type      = \DrSlump\Protobuf::TYPE_INT32;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <aggregated_payload_size> has a value
     *
     * @return boolean
     */
    public function hasAggregatedPayloadSize(){
      return $this->_has(1);
    }
    
    /**
     * Clear <aggregated_payload_size> value
     *
     * @return \grpc\testing\StreamingInputCallResponse
     */
    public function clearAggregatedPayloadSize(){
      return $this->_clear(1);
    }
    
    /**
     * Get <aggregated_payload_size> value
     *
     * @return int
     */
    public function getAggregatedPayloadSize(){
      return $this->_get(1);
    }
    
    /**
     * Set <aggregated_payload_size> value
     *
     * @param int $value
     * @return \grpc\testing\StreamingInputCallResponse
     */
    public function setAggregatedPayloadSize( $value){
      return $this->_set(1, $value);
    }
  }
}

namespace grpc\testing {

  class ResponseParameters extends \DrSlump\Protobuf\Message {

    /**  @var int */
    public $size = null;
    
    /**  @var int */
    public $interval_us = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.ResponseParameters');

      // OPTIONAL INT32 size = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "size";
      $f->type      = \DrSlump\Protobuf::TYPE_INT32;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      // OPTIONAL INT32 interval_us = 2
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 2;
      $f->name      = "interval_us";
      $f->type      = \DrSlump\Protobuf::TYPE_INT32;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <size> has a value
     *
     * @return boolean
     */
    public function hasSize(){
      return $this->_has(1);
    }
    
    /**
     * Clear <size> value
     *
     * @return \grpc\testing\ResponseParameters
     */
    public function clearSize(){
      return $this->_clear(1);
    }
    
    /**
     * Get <size> value
     *
     * @return int
     */
    public function getSize(){
      return $this->_get(1);
    }
    
    /**
     * Set <size> value
     *
     * @param int $value
     * @return \grpc\testing\ResponseParameters
     */
    public function setSize( $value){
      return $this->_set(1, $value);
    }
    
    /**
     * Check if <interval_us> has a value
     *
     * @return boolean
     */
    public function hasIntervalUs(){
      return $this->_has(2);
    }
    
    /**
     * Clear <interval_us> value
     *
     * @return \grpc\testing\ResponseParameters
     */
    public function clearIntervalUs(){
      return $this->_clear(2);
    }
    
    /**
     * Get <interval_us> value
     *
     * @return int
     */
    public function getIntervalUs(){
      return $this->_get(2);
    }
    
    /**
     * Set <interval_us> value
     *
     * @param int $value
     * @return \grpc\testing\ResponseParameters
     */
    public function setIntervalUs( $value){
      return $this->_set(2, $value);
    }
  }
}

namespace grpc\testing {

  class StreamingOutputCallRequest extends \DrSlump\Protobuf\Message {

    /**  @var int - \grpc\testing\PayloadType */
    public $response_type = \grpc\testing\PayloadType::COMPRESSABLE;
    
    /**  @var \grpc\testing\ResponseParameters[]  */
    public $response_parameters = array();
    
    /**  @var \grpc\testing\Payload */
    public $payload = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.StreamingOutputCallRequest');

      // OPTIONAL ENUM response_type = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "response_type";
      $f->type      = \DrSlump\Protobuf::TYPE_ENUM;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $f->reference = '\grpc\testing\PayloadType';
      $f->default   = \grpc\testing\PayloadType::COMPRESSABLE;
      $descriptor->addField($f);

      // REPEATED MESSAGE response_parameters = 2
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 2;
      $f->name      = "response_parameters";
      $f->type      = \DrSlump\Protobuf::TYPE_MESSAGE;
      $f->rule      = \DrSlump\Protobuf::RULE_REPEATED;
      $f->reference = '\grpc\testing\ResponseParameters';
      $descriptor->addField($f);

      // OPTIONAL MESSAGE payload = 3
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 3;
      $f->name      = "payload";
      $f->type      = \DrSlump\Protobuf::TYPE_MESSAGE;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $f->reference = '\grpc\testing\Payload';
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <response_type> has a value
     *
     * @return boolean
     */
    public function hasResponseType(){
      return $this->_has(1);
    }
    
    /**
     * Clear <response_type> value
     *
     * @return \grpc\testing\StreamingOutputCallRequest
     */
    public function clearResponseType(){
      return $this->_clear(1);
    }
    
    /**
     * Get <response_type> value
     *
     * @return int - \grpc\testing\PayloadType
     */
    public function getResponseType(){
      return $this->_get(1);
    }
    
    /**
     * Set <response_type> value
     *
     * @param int - \grpc\testing\PayloadType $value
     * @return \grpc\testing\StreamingOutputCallRequest
     */
    public function setResponseType( $value){
      return $this->_set(1, $value);
    }
    
    /**
     * Check if <response_parameters> has a value
     *
     * @return boolean
     */
    public function hasResponseParameters(){
      return $this->_has(2);
    }
    
    /**
     * Clear <response_parameters> value
     *
     * @return \grpc\testing\StreamingOutputCallRequest
     */
    public function clearResponseParameters(){
      return $this->_clear(2);
    }
    
    /**
     * Get <response_parameters> value
     *
     * @param int $idx
     * @return \grpc\testing\ResponseParameters
     */
    public function getResponseParameters($idx = NULL){
      return $this->_get(2, $idx);
    }
    
    /**
     * Set <response_parameters> value
     *
     * @param \grpc\testing\ResponseParameters $value
     * @return \grpc\testing\StreamingOutputCallRequest
     */
    public function setResponseParameters(\grpc\testing\ResponseParameters $value, $idx = NULL){
      return $this->_set(2, $value, $idx);
    }
    
    /**
     * Get all elements of <response_parameters>
     *
     * @return \grpc\testing\ResponseParameters[]
     */
    public function getResponseParametersList(){
     return $this->_get(2);
    }
    
    /**
     * Add a new element to <response_parameters>
     *
     * @param \grpc\testing\ResponseParameters $value
     * @return \grpc\testing\StreamingOutputCallRequest
     */
    public function addResponseParameters(\grpc\testing\ResponseParameters $value){
     return $this->_add(2, $value);
    }
    
    /**
     * Check if <payload> has a value
     *
     * @return boolean
     */
    public function hasPayload(){
      return $this->_has(3);
    }
    
    /**
     * Clear <payload> value
     *
     * @return \grpc\testing\StreamingOutputCallRequest
     */
    public function clearPayload(){
      return $this->_clear(3);
    }
    
    /**
     * Get <payload> value
     *
     * @return \grpc\testing\Payload
     */
    public function getPayload(){
      return $this->_get(3);
    }
    
    /**
     * Set <payload> value
     *
     * @param \grpc\testing\Payload $value
     * @return \grpc\testing\StreamingOutputCallRequest
     */
    public function setPayload(\grpc\testing\Payload $value){
      return $this->_set(3, $value);
    }
  }
}

namespace grpc\testing {

  class StreamingOutputCallResponse extends \DrSlump\Protobuf\Message {

    /**  @var \grpc\testing\Payload */
    public $payload = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'grpc.testing.StreamingOutputCallResponse');

      // OPTIONAL MESSAGE payload = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "payload";
      $f->type      = \DrSlump\Protobuf::TYPE_MESSAGE;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $f->reference = '\grpc\testing\Payload';
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <payload> has a value
     *
     * @return boolean
     */
    public function hasPayload(){
      return $this->_has(1);
    }
    
    /**
     * Clear <payload> value
     *
     * @return \grpc\testing\StreamingOutputCallResponse
     */
    public function clearPayload(){
      return $this->_clear(1);
    }
    
    /**
     * Get <payload> value
     *
     * @return \grpc\testing\Payload
     */
    public function getPayload(){
      return $this->_get(1);
    }
    
    /**
     * Set <payload> value
     *
     * @param \grpc\testing\Payload $value
     * @return \grpc\testing\StreamingOutputCallResponse
     */
    public function setPayload(\grpc\testing\Payload $value){
      return $this->_set(1, $value);
    }
  }
}

