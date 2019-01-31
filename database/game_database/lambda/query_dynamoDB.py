import boto3, json
from boto3.dynamodb.conditions import Key

###########################################################
# TODO - Database connection settings
dynamodb = boto3.resource('dynamodb', region_name='us-west-1')
###########################################################


table = dynamodb.Table('Games')

def lambda_handler(event, context):
        
    try:
        input_data = json.loads(event['body'])
    
        try:
            year = int(input_data['year'])
        except:
            raise Exception('The input "year" is not an integer.')

        genre_id = int(input_data['genre_id'])
        sort_by = input_data['sort_by']

        # for debug:
        # genre_id = 12; year = 2017; sort_by = "metascore"
        query_results = query_db(genre_id, year, sort_by)

        # the result contain "Decimal" values that need to 
        # be converted, as in: 'metascore': Decimal('88')
        # needs to become: 'metascore': 88

        convert_decimals(query_results)

        response_body = {
            "query_results": query_results
        }
               
    except Exception as ex:
        response_body = {
            "error": str(ex)
        }

    return {
            'statusCode': '200',
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Content-Type': 'application/json'
                },
            "isBase64Encoded": 'false',
            'body': json.dumps(response_body)
        }


def query_db(genre_id, year, sort_by):
    """Query 'table' for games for a particular genre (int), 
    year (int), and sort column (string)."""

    genre_year = year * 100 + genre_id

    if sort_by == "metascore":

        # query - sort by meta score
        response = table.query(
            IndexName = "sort_by_metascore",
            ProjectionExpression = "metascore, title, #url, release_date",
            ExpressionAttributeNames={ "#url": "url" },
            ScanIndexForward = False,
            KeyConditionExpression = Key('genre_year').eq(genre_year),
            Limit = 100
        )

    elif sort_by == "user_scorex100":

        # query - sort by user score
        response = table.query(
            IndexName = "sort_by_user_score",
            ProjectionExpression = "user_scorex100, title, #url, release_date",
            ExpressionAttributeNames={ "#url": "url" },
            ScanIndexForward = False,
            KeyConditionExpression = Key('genre_year').eq(genre_year) & Key('user_scorex100').gte(0),
            Limit = 100
        )

    elif sort_by == "num_user_ratings":

        # query - sort by number of user ratings
        response = table.query(
            IndexName = "sort_by_num_user_ratings",
            ProjectionExpression = "num_user_ratings, title, #url, release_date",
            ExpressionAttributeNames={ "#url": "url" },
            ScanIndexForward = False,
            KeyConditionExpression = Key('genre_year').eq(genre_year)  & Key('num_user_ratings').gt(0),
            Limit = 100
        )

    else:
        raise Exception("The 'sort_by' value of '" + sort_by + "' is not supported.")

    return response['Items']


def convert_decimals(results_list):
    """Convert decimal items that are present in the results_list.
    Example: 'metascore': Decimal('88') becomes 'metascore': 88
    """
    if len(results_list) == 0: return

    # detect what fields are present
    # metascore_present, user_scorex100_present, num_user_ratings_present
    if 'metascore' in results_list[0]: metascore_present = True
    else: metascore_present = False

    if 'user_scorex100' in results_list[0]: user_scorex100_present = True
    else: user_scorex100_present = False

    if 'num_user_ratings' in results_list[0]: num_user_ratings_present = True
    else: num_user_ratings_present = False

    for result in results_list:
        if metascore_present: result['metascore'] = int(result['metascore'])
        if user_scorex100_present: result['user_scorex100'] = int(result['user_scorex100'])
        if num_user_ratings_present: result['num_user_ratings'] = int(result['num_user_ratings'])




