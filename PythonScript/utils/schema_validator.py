import json
import jsonschema
from jsonschema import validate


def validate_json(json_string):
    try:
        param_dict = json.loads(json_string)
    except ValueError as err:
        return False
    return param_dict


def validate_schema(param_dict, param_schema):
    try:
        validate(instance=param_dict, schema=param_schema)
    except jsonschema.exceptions.ValidationError as err:
        return err.message
    return True


if __name__ == '__main__':
    validate_json('{"name": "jane doe", "rollnumber": 25, "marks": 72}', 'test')
