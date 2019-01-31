import json, pymysql

#####################################################
# TODO - Database connection settings
#connection = pymysql.connect(host='localhost',
#                             user='root',
#                             password='123456',
#                             db='mydb')

connection = pymysql.connect(host='louis2018.c2idvzt6cutj.us-west-1.rds.amazonaws.com',
                             user='louisy',
                             password='12345678',
                             db='mydb')

#####################################################


def lambda_handler(event, context):
        
    try:
        input_data = json.loads(event['body'])
    
        try: start_year = int(input_data['start_year'])
        except: raise Exception('The input "start_year" is not an integer.')

        try: end_year = int(input_data['end_year'])
        except: raise Exception('The input "end_year" is not an integer.')

        genre_id = int(input_data['genre_id'])
        sort_by = input_data['sort_by']

        # for testing:
        # genre_id = 12; start_year = 2016; end_year = 2017; sort_by = "metascore"
        if genre_id == 0:
            query_results = query_db_all_genre(start_year, end_year, sort_by)
        else:
            query_results = query_db(genre_id, start_year, end_year, sort_by)

        # the 4th column of the "query_results" is release_date of 
        # type "datetime.date" that need to be converted to string
        # so json.dumps(...) can work on it.
        # For example, datetime.date(2017, 4, 27) needs to become: "2017-04-27"
        converted_results = []

        for result in query_results:
            current_item = list(result)
            current_item[3] = current_item[3].strftime("%Y-%m-%d")
            converted_results.append(current_item)
                    

        response_body = {
            "query_results": converted_results
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




def query_db(genre_id, start_year, end_year, sort_by):
    """Query 'connection' for games of a particular genre (int), 
    being released between start_year (int) and end_year (int),
    and sort the results according to 'sort_by' column (string)."""

    sql_template = """select distinct {0}, Games.title, Games.url, Games.release_date 
        from Games inner join GameGenre on Games.id = GameGenre.game_id
        where Games.release_date >= %s
        and Games.release_date < %s
        and GameGenre.genre_id = %s
        order by {1} desc
        limit 100;
        """

    with  connection.cursor() as cursor:

        if sort_by == "metascore":

            sql = sql_template.format("Games.metascore", "Games.metascore")
            cursor.execute(sql, (str(start_year) + "-01-01",
                                 str(end_year + 1) + "-01-01", 
                                 str(genre_id)))

        elif sort_by == "num_user_ratings":
            sql = sql_template.format("Games.num_user_ratings", "Games.num_user_ratings")
            cursor.execute(sql, (str(start_year) + "-01-01",
                                 str(end_year + 1) + "-01-01", 
                                 str(genre_id)))

        elif sort_by == "user_score":
            sql = """select distinct Games.user_score, Games.title, Games.url, Games.release_date 
                from Games inner join GameGenre on Games.id = GameGenre.game_id
                where Games.release_date >= %s
                and Games.release_date < %s
                and GameGenre.genre_id = %s
                and Games.user_score >= 0
                order by Games.user_score desc
                limit 100;
                """
            cursor.execute(sql, (str(start_year) + "-01-01",
                                 str(end_year + 1) + "-01-01", 
                                 str(genre_id)))

        else:
            raise Exception("The 'sort_by' value of '" + sort_by + "' is not supported.")

        results = cursor.fetchall()
        
    return results



def query_db_all_genre(start_year, end_year, sort_by):
    """ Similar to "query_db(...)" but applies the search to all genre.
    Query 'connection' for games being released between start_year (int) 
    and end_year (int), and sort the results according to 'sort_by' 
    column (string)."""

    sql_template = """select distinct {0}, Games.title, Games.url, Games.release_date 
        from Games inner join GameGenre on Games.id = GameGenre.game_id
        where Games.release_date >= %s
        and Games.release_date < %s
        order by {1} desc
        limit 100;
        """

    with  connection.cursor() as cursor:

        if sort_by == "metascore":

            sql = sql_template.format("Games.metascore", "Games.metascore")
            cursor.execute(sql, (str(start_year) + "-01-01",
                                 str(end_year + 1) + "-01-01"))

        elif sort_by == "num_user_ratings":
            sql = sql_template.format("Games.num_user_ratings", "Games.num_user_ratings")
            cursor.execute(sql, (str(start_year) + "-01-01",
                                 str(end_year + 1) + "-01-01"))

        elif sort_by == "user_score":
            sql = """select distinct Games.user_score, Games.title, Games.url, Games.release_date 
                from Games inner join GameGenre on Games.id = GameGenre.game_id
                where Games.release_date >= %s
                and Games.release_date < %s
                and Games.user_score >= 0
                order by Games.user_score desc
                limit 100;
                """
            cursor.execute(sql, (str(start_year) + "-01-01",
                                 str(end_year + 1) + "-01-01"))

        else:
            raise Exception("The 'sort_by' value of '" + sort_by + "' is not supported.")

        results = cursor.fetchall()
        
    return results