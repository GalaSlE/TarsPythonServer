import json
import unittest
from example import function_gateway


class exampleTest(unittest.TestCase):

    def test_group_by_mean(self):
        self.maxDiff = None
        expected_result = {'code': 0, 'msg': 'success', 'result': '{"Max Speed":{"Falcon":375.0,"Parrot":25.0}}'}
        input_data = {
            "data":{
                "Animal": ["Falcon", "Falcon","Parrot", "Parrot"],
                "Max Speed": [380., 370., 24., 26.]},
            "group_by_column": ["Animal"]
            }
        print(json.dumps(input_data, ensure_ascii=False))
        test_result, test_error_message = function_gateway(json.dumps(input_data, ensure_ascii=False), "example_group_by_mean")
        self.assertDictEqual(expected_result, json.loads(test_result))


    def test_group_by_mean_schema_error(self):
        self.maxDiff = None
        expected_result = {
            "code": 1,
            "msg": "SchemaError: request_json fail in schema valid check. Reason is 'Animal' is not of type 'array'."
        }
        input_data = {
            "data":{
                "Animal": ["Falcon", "Falcon","Parrot", "Parrot"],
                "Max Speed": [380., 370., 24., 26.]},
            "group_by_column": "Animal"
            }
        test_result, test_error_message = function_gateway(json.dumps(input_data, ensure_ascii=False), "example_group_by_mean")
        self.assertDictEqual(expected_result, json.loads(test_result))
 
    def test_group_by_mean_some_error(self):
        self.maxDiff = None
        expected_result = {
            "code": 1,
            "msg": "KeyError: groupby key does not exists."
        }
        input_data = {
            "data":{
                "Animal": ["Falcon", "Falcon","Parrot", "Parrot"],
                "Max Speed": [380., 370., 24., 26.]},
            "group_by_column": ["Animals"]
            }
        test_result, test_error_message = function_gateway(json.dumps(input_data, ensure_ascii=False), "example_group_by_mean")
        print(test_error_message, json.loads(test_result))
        self.assertDictEqual(expected_result, json.loads(test_result))

if __name__ == '__main__':
    unittest.main()