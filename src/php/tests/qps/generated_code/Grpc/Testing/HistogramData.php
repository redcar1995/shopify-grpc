<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/stats.proto

namespace Grpc\Testing;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * Histogram data based on grpc/support/histogram.c
 *
 * Generated from protobuf message <code>grpc.testing.HistogramData</code>
 */
class HistogramData extends \Google\Protobuf\Internal\Message
{
    /**
     * Generated from protobuf field <code>repeated uint32 bucket = 1;</code>
     */
    private $bucket;
    /**
     * Generated from protobuf field <code>double min_seen = 2;</code>
     */
    private $min_seen = 0.0;
    /**
     * Generated from protobuf field <code>double max_seen = 3;</code>
     */
    private $max_seen = 0.0;
    /**
     * Generated from protobuf field <code>double sum = 4;</code>
     */
    private $sum = 0.0;
    /**
     * Generated from protobuf field <code>double sum_of_squares = 5;</code>
     */
    private $sum_of_squares = 0.0;
    /**
     * Generated from protobuf field <code>double count = 6;</code>
     */
    private $count = 0.0;

    public function __construct() {
        \GPBMetadata\Src\Proto\Grpc\Testing\Stats::initOnce();
        parent::__construct();
    }

    /**
     * Generated from protobuf field <code>repeated uint32 bucket = 1;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getBucket()
    {
        return $this->bucket;
    }

    /**
     * Generated from protobuf field <code>repeated uint32 bucket = 1;</code>
     * @param int[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setBucket($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::UINT32);
        $this->bucket = $arr;

        return $this;
    }

    /**
     * Generated from protobuf field <code>double min_seen = 2;</code>
     * @return float
     */
    public function getMinSeen()
    {
        return $this->min_seen;
    }

    /**
     * Generated from protobuf field <code>double min_seen = 2;</code>
     * @param float $var
     * @return $this
     */
    public function setMinSeen($var)
    {
        GPBUtil::checkDouble($var);
        $this->min_seen = $var;

        return $this;
    }

    /**
     * Generated from protobuf field <code>double max_seen = 3;</code>
     * @return float
     */
    public function getMaxSeen()
    {
        return $this->max_seen;
    }

    /**
     * Generated from protobuf field <code>double max_seen = 3;</code>
     * @param float $var
     * @return $this
     */
    public function setMaxSeen($var)
    {
        GPBUtil::checkDouble($var);
        $this->max_seen = $var;

        return $this;
    }

    /**
     * Generated from protobuf field <code>double sum = 4;</code>
     * @return float
     */
    public function getSum()
    {
        return $this->sum;
    }

    /**
     * Generated from protobuf field <code>double sum = 4;</code>
     * @param float $var
     * @return $this
     */
    public function setSum($var)
    {
        GPBUtil::checkDouble($var);
        $this->sum = $var;

        return $this;
    }

    /**
     * Generated from protobuf field <code>double sum_of_squares = 5;</code>
     * @return float
     */
    public function getSumOfSquares()
    {
        return $this->sum_of_squares;
    }

    /**
     * Generated from protobuf field <code>double sum_of_squares = 5;</code>
     * @param float $var
     * @return $this
     */
    public function setSumOfSquares($var)
    {
        GPBUtil::checkDouble($var);
        $this->sum_of_squares = $var;

        return $this;
    }

    /**
     * Generated from protobuf field <code>double count = 6;</code>
     * @return float
     */
    public function getCount()
    {
        return $this->count;
    }

    /**
     * Generated from protobuf field <code>double count = 6;</code>
     * @param float $var
     * @return $this
     */
    public function setCount($var)
    {
        GPBUtil::checkDouble($var);
        $this->count = $var;

        return $this;
    }

}

