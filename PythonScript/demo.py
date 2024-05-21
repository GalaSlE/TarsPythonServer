'''gateway for the enb'''


import json
import traceback
from utils.schema_validator import validate_schema, validate_json
from demo_func.group_by import example_group_by_mean
from schemas.example_schema import schema_dict


def function_gateway(request_json, func_name):
    """
    The gateway for the scripts.

    :param request_json: Json string  contains all parameters that the c++ passed in.
    :param func_name: The key name of the target method in func_name_dict.
    :return: tuple of return response and error message
    """
    response = {
        "code": 1,
        "msg": "unknown error"
    }
    error_message = None
    params_dict = validate_json(request_json)

    if not params_dict:
        response["msg"] = "JsonError: request_json is not valid json string."
        error_message = f"JsonError: {request_json} is not valid json string."
        return json.dumps(response, ensure_ascii=False), error_message
    param_schema = schema_dict[func_name]
    validate_result = validate_schema(params_dict, param_schema)
    if validate_result is True:
        try:
            response["result"] = func_name_dict[func_name](**params_dict)
            response["code"] = 0
            response["msg"] = "success"
        except KeyError:
            response["msg"] = "KeyError: groupby key does not exists."
            error_message = f"KeyError: {request_json} groupby key does not exists."
    else:
        response["msg"] = f"SchemaError: request_json fail in schema valid check. Reason is {validate_result}."
        error_message = f"SchemaError: {request_json} fail in schema valid check. Reason is {validate_result}."
    return json.dumps(response, ensure_ascii=False), error_message


func_name_dict = {
    "example_group_by_mean": example_group_by_mean,
}