

group_by_mean_schema = {
    "type": "object",
    "required": ["data", "group_by_column"],
    "properties": {
        "data": {
            "type": "object"
        },
        "group_by_column": {
            "type": "array"
        },
    }
}


schema_dict = {
    "example_group_by_mean": group_by_mean_schema
}


