import boto3, datetime, os

###############################################
# TODO - Database connection settings
dynamodb = boto3.resource('dynamodb', region_name='us-west-1')
###############################################

def create_attribute_definition(column_name, data_type):
    """Helper for dynamodb.create_table(...)
    Example:
    create_attribute_definition('year', 'N') returns {
        'AttributeName': 'year',
        'AttributeType': 'N'
    }"""
    return {
        'AttributeName': column_name,
        'AttributeType': data_type
        }

def create_key_schema(partition_key, sort_key):
    """Helper for dynamodb.create_table(...)
    Example:
    create_key_schema('year', 'title') returns [
        {
            'AttributeName': 'year',
            'KeyType': 'HASH'  #Partition key
        },
        {
            'AttributeName': 'title',
            'KeyType': 'RANGE'  #Sort key
        }
    ]
    """
    return [
        {
            'AttributeName': partition_key,
            'KeyType': 'HASH'
        },
        {
            'AttributeName': sort_key,
            'KeyType': 'RANGE'
        }
    ]


table = dynamodb.create_table(
    TableName='Games',
    AttributeDefinitions = [
        create_attribute_definition('genre_year', 'N'),
        create_attribute_definition('title', 'S'),
        create_attribute_definition('metascore', 'N'),
        create_attribute_definition('user_scorex100', 'N'),
        create_attribute_definition('num_user_ratings', 'N')
    ],
    KeySchema = create_key_schema('genre_year', 'title'),
    LocalSecondaryIndexes=[
        {
            'IndexName': 'sort_by_metascore',
            'KeySchema': create_key_schema('genre_year', 'metascore'),
            'Projection': { 'ProjectionType': 'KEYS_ONLY' }
        },
        {
            'IndexName': 'sort_by_user_score',
            'KeySchema': create_key_schema('genre_year', 'user_scorex100'),
            'Projection': { 'ProjectionType': 'KEYS_ONLY' }
        },
        {
            'IndexName': 'sort_by_num_user_ratings',
            'KeySchema': create_key_schema('genre_year', 'num_user_ratings'),
            'Projection': { 'ProjectionType': 'KEYS_ONLY' }
        }
    ],
    ProvisionedThroughput={
        'ReadCapacityUnits': 10,
        'WriteCapacityUnits': 10
    }
)
