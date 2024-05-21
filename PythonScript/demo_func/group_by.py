import pandas as pd


def example_group_by_mean(data, group_by_column):
    df = pd.DataFrame(data)
    result =  df.groupby(group_by_column).mean()
    return result.to_json()