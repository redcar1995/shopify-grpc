<?php
// DO NOT EDIT! Generated by Protobuf-PHP protoc plugin 1.0
// Source: math.proto
//   Date: 2014-11-14 00:00:41

namespace math {

  class DivArgs extends \DrSlump\Protobuf\Message {

    /**  @var int */
    public $dividend = null;
    
    /**  @var int */
    public $divisor = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'math.DivArgs');

      // REQUIRED INT64 dividend = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "dividend";
      $f->type      = \DrSlump\Protobuf::TYPE_INT64;
      $f->rule      = \DrSlump\Protobuf::RULE_REQUIRED;
      $descriptor->addField($f);

      // REQUIRED INT64 divisor = 2
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 2;
      $f->name      = "divisor";
      $f->type      = \DrSlump\Protobuf::TYPE_INT64;
      $f->rule      = \DrSlump\Protobuf::RULE_REQUIRED;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <dividend> has a value
     *
     * @return boolean
     */
    public function hasDividend(){
      return $this->_has(1);
    }
    
    /**
     * Clear <dividend> value
     *
     * @return \math\DivArgs
     */
    public function clearDividend(){
      return $this->_clear(1);
    }
    
    /**
     * Get <dividend> value
     *
     * @return int
     */
    public function getDividend(){
      return $this->_get(1);
    }
    
    /**
     * Set <dividend> value
     *
     * @param int $value
     * @return \math\DivArgs
     */
    public function setDividend( $value){
      return $this->_set(1, $value);
    }
    
    /**
     * Check if <divisor> has a value
     *
     * @return boolean
     */
    public function hasDivisor(){
      return $this->_has(2);
    }
    
    /**
     * Clear <divisor> value
     *
     * @return \math\DivArgs
     */
    public function clearDivisor(){
      return $this->_clear(2);
    }
    
    /**
     * Get <divisor> value
     *
     * @return int
     */
    public function getDivisor(){
      return $this->_get(2);
    }
    
    /**
     * Set <divisor> value
     *
     * @param int $value
     * @return \math\DivArgs
     */
    public function setDivisor( $value){
      return $this->_set(2, $value);
    }
  }
}

namespace math {

  class DivReply extends \DrSlump\Protobuf\Message {

    /**  @var int */
    public $quotient = null;
    
    /**  @var int */
    public $remainder = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'math.DivReply');

      // REQUIRED INT64 quotient = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "quotient";
      $f->type      = \DrSlump\Protobuf::TYPE_INT64;
      $f->rule      = \DrSlump\Protobuf::RULE_REQUIRED;
      $descriptor->addField($f);

      // REQUIRED INT64 remainder = 2
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 2;
      $f->name      = "remainder";
      $f->type      = \DrSlump\Protobuf::TYPE_INT64;
      $f->rule      = \DrSlump\Protobuf::RULE_REQUIRED;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <quotient> has a value
     *
     * @return boolean
     */
    public function hasQuotient(){
      return $this->_has(1);
    }
    
    /**
     * Clear <quotient> value
     *
     * @return \math\DivReply
     */
    public function clearQuotient(){
      return $this->_clear(1);
    }
    
    /**
     * Get <quotient> value
     *
     * @return int
     */
    public function getQuotient(){
      return $this->_get(1);
    }
    
    /**
     * Set <quotient> value
     *
     * @param int $value
     * @return \math\DivReply
     */
    public function setQuotient( $value){
      return $this->_set(1, $value);
    }
    
    /**
     * Check if <remainder> has a value
     *
     * @return boolean
     */
    public function hasRemainder(){
      return $this->_has(2);
    }
    
    /**
     * Clear <remainder> value
     *
     * @return \math\DivReply
     */
    public function clearRemainder(){
      return $this->_clear(2);
    }
    
    /**
     * Get <remainder> value
     *
     * @return int
     */
    public function getRemainder(){
      return $this->_get(2);
    }
    
    /**
     * Set <remainder> value
     *
     * @param int $value
     * @return \math\DivReply
     */
    public function setRemainder( $value){
      return $this->_set(2, $value);
    }
  }
}

namespace math {

  class FibArgs extends \DrSlump\Protobuf\Message {

    /**  @var int */
    public $limit = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'math.FibArgs');

      // OPTIONAL INT64 limit = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "limit";
      $f->type      = \DrSlump\Protobuf::TYPE_INT64;
      $f->rule      = \DrSlump\Protobuf::RULE_OPTIONAL;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <limit> has a value
     *
     * @return boolean
     */
    public function hasLimit(){
      return $this->_has(1);
    }
    
    /**
     * Clear <limit> value
     *
     * @return \math\FibArgs
     */
    public function clearLimit(){
      return $this->_clear(1);
    }
    
    /**
     * Get <limit> value
     *
     * @return int
     */
    public function getLimit(){
      return $this->_get(1);
    }
    
    /**
     * Set <limit> value
     *
     * @param int $value
     * @return \math\FibArgs
     */
    public function setLimit( $value){
      return $this->_set(1, $value);
    }
  }
}

namespace math {

  class Num extends \DrSlump\Protobuf\Message {

    /**  @var int */
    public $num = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'math.Num');

      // REQUIRED INT64 num = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "num";
      $f->type      = \DrSlump\Protobuf::TYPE_INT64;
      $f->rule      = \DrSlump\Protobuf::RULE_REQUIRED;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <num> has a value
     *
     * @return boolean
     */
    public function hasNum(){
      return $this->_has(1);
    }
    
    /**
     * Clear <num> value
     *
     * @return \math\Num
     */
    public function clearNum(){
      return $this->_clear(1);
    }
    
    /**
     * Get <num> value
     *
     * @return int
     */
    public function getNum(){
      return $this->_get(1);
    }
    
    /**
     * Set <num> value
     *
     * @param int $value
     * @return \math\Num
     */
    public function setNum( $value){
      return $this->_set(1, $value);
    }
  }
}

namespace math {

  class FibReply extends \DrSlump\Protobuf\Message {

    /**  @var int */
    public $count = null;
    

    /** @var \Closure[] */
    protected static $__extensions = array();

    public static function descriptor()
    {
      $descriptor = new \DrSlump\Protobuf\Descriptor(__CLASS__, 'math.FibReply');

      // REQUIRED INT64 count = 1
      $f = new \DrSlump\Protobuf\Field();
      $f->number    = 1;
      $f->name      = "count";
      $f->type      = \DrSlump\Protobuf::TYPE_INT64;
      $f->rule      = \DrSlump\Protobuf::RULE_REQUIRED;
      $descriptor->addField($f);

      foreach (self::$__extensions as $cb) {
        $descriptor->addField($cb(), true);
      }

      return $descriptor;
    }

    /**
     * Check if <count> has a value
     *
     * @return boolean
     */
    public function hasCount(){
      return $this->_has(1);
    }
    
    /**
     * Clear <count> value
     *
     * @return \math\FibReply
     */
    public function clearCount(){
      return $this->_clear(1);
    }
    
    /**
     * Get <count> value
     *
     * @return int
     */
    public function getCount(){
      return $this->_get(1);
    }
    
    /**
     * Set <count> value
     *
     * @param int $value
     * @return \math\FibReply
     */
    public function setCount( $value){
      return $this->_set(1, $value);
    }
  }
}

namespace math {

  class MathClient extends \Grpc\BaseStub {
    /**
     * @param math\DivArgs $input
     * @return math\DivReply
     */
    public function Div(\math\DivArgs $argument, $metadata = array()) {
      return $this->_simpleRequest('/Math/Div', $argument, '\math\DivReply::deserialize', $metadata);
    }
    /**
     * @param math\DivArgs $input
     * @return math\DivReply
     */
    public function DivMany($metadata = array()) {
      return $this->_bidiRequest('/Math/DivMany', '\math\DivReply::deserialize', $metadata);
    }
    /**
     * @param math\FibArgs $input
     * @return math\Num
     */
    public function Fib($argument, $metadata = array()) {
      return $this->_serverStreamRequest('/Math/Fib', $argument, '\math\Num::deserialize', $metadata);
    }
    /**
     * @param math\Num $input
     * @return math\Num
     */
    public function Sum($arguments, $metadata = array()) {
      return $this->_clientStreamRequest('/Math/Sum', $arguments, '\math\Num::deserialize', $metadata);
    }
  }
}
