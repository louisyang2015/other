import datetime, os, pymysql


###############################################
# TODO - Database connection settings
connection = pymysql.connect(host='localhost',
                             user='root',
                             password='123456',
                             db='mydb')
###############################################


def get_game_id(title, release_date):
    """Search the Games table and returns "id" if game found.
    If not found, returns None."""
    # for debug:
    # title = "Tiny Metal"
    # release_date = "2017-12-21"

    with  connection.cursor() as cursor:
        sql = """select id from Games where
        title = %s and release_date = %s"""
        cursor.execute(sql, (title, release_date))

    result = cursor.fetchone()
    if result != None: return result[0]
    
    return None



# Genre ID Encoding
genre_str_to_genre_id = {
    "action": 1, "adventure": 2, "first person shooters": 3,
    "flight": 4, "platformer": 5, "puzzle": 6, "racing": 7,
    "real-time strategy": 8, "role-playing games": 9,
    "simulation": 10, "sports": 11, "strategy": 12,
    "third-person shooter": 13, "turn-based strategy": 14
    }


# generate list of data files by searching ".\\data" for ".tsv" files
file_names = []
for file in os.listdir('.\\data'):
    if file.endswith(".tsv"):
        file_names.append('.\\data\\' + file)

# for debug:
# file_names = ['.\\data\\data000.tsv']

# process one file at a time
for file_name in file_names:
    with open(file_name, 'r') as f:
        for line in f:
            fields = line.strip().split('\t')

            # for debug:
            # print(fields)
            
            # at this point, fields look like
            # fields = ['strategy', 'Tiny Metal', 'http://...', '71', 'Dec 21, 2017', 'tbd', '11']

            # extract the data from "fields"
            genre_id = genre_str_to_genre_id[fields[0]]
            title = fields[1]
            url = fields[2]
            metascore = int(fields[3])
            
            # the release_date has to be in SQL's "yyyy-mm-dd" format
            # datetime.datetime.strptime('Dec 21, 2017', "%b %d, %Y").strftime("%Y-%m-%d")
            # produces "2017-12-21"
            release_date = datetime.datetime.strptime(fields[4], "%b %d, %Y").strftime("%Y-%m-%d")

            try: user_score = float(fields[5])
            except: user_score = -1

            num_user_ratings = int(fields[6])

            # for debug:
            # print(genre_id, title, url, metascore, release_date, user_score, num_user_ratings)

            # insert into "Games" table if not present
            game_id = get_game_id(title, release_date)
            
            if game_id == None:
                with  connection.cursor() as cursor:
                    sql = """insert into Games (title, url, release_date, metascore, user_score, num_user_ratings) 
                    values (%s, %s, %s, %s, %s, %s)"""
                    cursor.execute(sql, (title, url, release_date, str(metascore), str(user_score), str(num_user_ratings)))

                connection.commit() 

                game_id = get_game_id(title, release_date)

            # insert into "GameGenre" table
            with  connection.cursor() as cursor:
                sql = """insert into GameGenre (game_id, genre_id) 
                values (%s, %s)"""
                cursor.execute(sql, (str(game_id), str(genre_id)))
            connection.commit() 










