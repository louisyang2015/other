import boto3, datetime, os

###############################################
# TODO - Database connection settings
dynamodb = boto3.resource('dynamodb', region_name='us-west-1')
###############################################

table = dynamodb.Table('Games')


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
# file_names = [r'.\data\data000.tsv']

# process one file at a time
with table.batch_writer() as batch:
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
                release_date = fields[4]

                # genre_year is a hash = (release year) * 100 + genre_id
                release_datetime = datetime.datetime.strptime(fields[4], "%b %d, %Y")
                genre_year = release_datetime.year * 100 + genre_id

                # user_score - DynamoDB does not support floating point,
                # so store it as an x100 integer. (3.3 is stored as 330)
                try: 
                    user_score = int(float(fields[5]) * 100)
                except: user_score = -1

                num_user_ratings = int(fields[6])

                # for debug:
                # print(genre_year, title, url, metascore, release_date, user_score, num_user_ratings)

                # insert into "Games" table
                batch.put_item(Item={
                    'genre_year': genre_year,
                    'title': title,
                    'metascore': metascore,
                    'user_scorex100': user_score,
                    'num_user_ratings': num_user_ratings,
                    'url': url,
                    'release_date': release_date
                    })
            










