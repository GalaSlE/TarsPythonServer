'''Define some custom schema for the schema check''' 
import json


class DocumentSchemaError(Exception):
    def __init__(self, file_name, raw_bucket_path, json_obj, validate_result):
        super().__init__(file_name, raw_bucket_path, json_obj, validate_result)
        self.message = f"DocumentSchemaError: file <{file_name}> in bucket <{raw_bucket_path}> with content {json.dumps(json_obj, ensure_ascii=False)} fail in schema valid check. Reason is {validate_result}."