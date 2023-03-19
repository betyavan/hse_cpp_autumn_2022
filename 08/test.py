#! /usr/bin/env python3

import json
import cjson
import ujson


def test_defsult():
    json_str = '{"hello": 10, "world": "value"}'
    json_doc = json.loads(json_str)
    ujson_doc = ujson.loads(json_str)
    cjson_doc = cjson.loads(json_str)
    assert json_doc ==  cjson_doc == ujson_doc == {'aaa': 1}
    assert json_str == cjson.dumps(cjson.loads(json_str))

def test_empty():
    json_str = '{}'
    json_doc = json.loads(json_str)
    ujson_doc = ujson.loads(json_str)
    cjson_doc = cjson.loads(json_str)
    assert json_doc ==  cjson_doc == ujson_doc
    assert json_str == cjson.dumps(cjson.loads(json_str))

def test_only_num():
    json_str = '{"1":1,"2":2}'
    json_doc = json.loads(json_str)
    ujson_doc = ujson.loads(json_str)
    cjson_doc = cjson.loads(json_str)
    assert json_doc ==  cjson_doc == ujson_doc
    assert json_str == cjson.dumps(cjson.loads(json_str))

def test_only_str():
    json_str = '{"1":  "one", "2": "two", "3": "three"}'
    json_doc = json.loads(json_str)
    ujson_doc = ujson.loads(json_str)
    cjson_doc = cjson.loads(json_str)
    assert json_doc ==  cjson_doc == ujson_doc
    assert json_str == cjson.dumps(cjson.loads(json_str))

if __name__ == "__main__":
    test_defsult()
    test_empty()
    test_only_num()
    test_only_str()