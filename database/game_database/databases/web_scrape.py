import requests, sys, os, boto3, time
from html.parser import HTMLParser


class GameInfoParser(HTMLParser):

    def __init__(self):
        super().__init__()
        
    def feed(self, data):
        self.release_date = "unknown"
        self.user_score = "unknown"
        self.num_user_ratings = "0"
        self.__state = 0
        super().feed(data)
        
    def handle_starttag(self, tag, attrs): 

        if self.__state == 0:
            # look through attributes and check for:
            # itemprop="datePublished", href=ending in "/user-reviews"
            for attr in attrs:
                if attr[0] == "itemprop" and attr[1] == "datePublished":
                    self.__state = 1
                elif attr[0] == "href" and attr[1].endswith("/user-reviews"):
                    self.__state = 4

        elif self.__state == 2:
            if tag == "div": self.__state = 3

            
    def handle_data(self, data):
        if self.__state == 0:
            if data.strip().lower() == "user score":
                self.__state = 2

        elif self.__state == 1:
            self.release_date = data.strip()
            self.__state = 0

        elif self.__state == 3:
            if self.user_score == "unknown":
                self.user_score = data.strip()
            self.__state = 0

        elif self.__state == 4:
            self.__state = 0

            # check for "123 ratings", record the "123"
            words = data.strip().split(' ')
            if len(words) == 2:
                if words[1].lower() == "ratings":
                    self.num_user_ratings = words[0]

        
class GameListParser(HTMLParser):

    def __init__(self):
        super().__init__()
        
    def feed(self, data):
        self.url = []
        self.title = []
        self.metascore = []
        self.__state = 0
        super().feed(data)
        
    def handle_starttag(self, tag, attrs): 

        if self.__state == 0:
            if tag == "div":
                # look through attributes and check for class="product_condensed"
                for attr in attrs:
                    if attr[0] == "class" and attr[1] == "product_condensed":
                        self.__state = 1

        elif self.__state == 1:
            if tag == "li":
                # look for "class" attribute that contains "game_product"
                for attr in attrs:
                    if attr[0] == "class" and "game_product" in attr[1]:
                        self.__state = 2

        elif self.__state == 2:
            if tag == "a": 
                for attr in attrs:
                    if attr[0] == "href":
                        self.url.append("http://www.metacritic.com" + attr[1].strip())
                        self.__state = 3

        elif self.__state == 4:
            if tag == "div":
              # look for "class" attribute that contains "metascore_w"
              for attr in attrs:
                    if attr[0] == "class" and "metascore_w" in attr[1]:
                        self.__state = 5
            
    def handle_data(self, data):
        if self.__state == 3:
            self.title.append(data.strip())
            self.__state = 4

        elif self.__state == 5:
            self.metascore.append(data.strip())
            self.__state = 1
            

class WebPages_List:

    def __init__(self):
        self.__web_pages_list = [
            ["strategy", "http://www.metacritic.com/browse/games/genre/date/strategy/pc?view=condensed&page=", 0, 34],
            ["action", "http://www.metacritic.com/browse/games/genre/date/action/pc?view=condensed&page=", 0, 48],
            ["adventure", "http://www.metacritic.com/browse/games/genre/date/adventure/pc?view=condensed&page=", 0, 23],
            ["first person shooters", "http://www.metacritic.com/browse/games/genre/date/first-person/pc?view=condensed&page=", 0, 22],
            ["flight", "http://www.metacritic.com/browse/games/genre/date/flight/pc?view=condensed&page=", 0, 2],
            ["platformer", "http://www.metacritic.com/browse/games/genre/date/platformer/pc?view=condensed&page=", 0, 4],
            ["puzzle", "http://www.metacritic.com/browse/games/genre/date/puzzle/pc?view=condensed&page=", 0, 1],
            ["racing", "http://www.metacritic.com/browse/games/genre/date/racing/pc?view=condensed&page=", 0, 7],
            ["real-time strategy", "http://www.metacritic.com/browse/games/genre/date/real-time/pc?view=condensed&page=", 0, 16],
            ["role-playing games", "http://www.metacritic.com/browse/games/genre/date/role-playing/pc?view=condensed&page=", 0, 20],
            ["simulation", "http://www.metacritic.com/browse/games/genre/date/simulation/pc?view=condensed&page=", 0, 12],
            ["sports", "http://www.metacritic.com/browse/games/genre/date/sports/pc?view=condensed&page=", 0, 7],
            ["third-person shooter", "http://www.metacritic.com/browse/games/genre/date/third-person/pc?view=condensed&page=", 0, 9],
            ["turn-based strategy", "http://www.metacritic.com/browse/games/genre/date/turn-based/pc?view=condensed&page=", 0, 9]
        ]

        self.__index_offset = [0]
        # __index_offset[i] = starting task number of __web_pages_list[i]
        # __index_offset[i] = __index_offset[i-1] + __web_pages_list[i-1][3] + 1
        last_index = len(self.__web_pages_list) - 1
        for i in range(1, last_index + 1):
            new_index = self.__index_offset[i-1] + self.__web_pages_list[i-1][3] + 1
            self.__index_offset.append(new_index)

        # total number of tasks there are
        self.__length = self.__index_offset[last_index] + self.__web_pages_list[last_index][3] + 1 

    def length(self):
        """Return the total number of web pages."""
        return self.__length

    def get_task(self, n):
        """Return the genre and address of the n-th web page."""
        # find the right genre for task number 'n'
        # n should be between self.__index_offset[genre_index] and
        # self.__index_offset[genre_index] + self.__web_pages_list[genre_index][3]
        genre_index = 0
        while self.__index_offset[genre_index] + self.__web_pages_list[genre_index][3] < n:
            genre_index += 1

        page_number = n - self.__index_offset[genre_index]

        return (self.__web_pages_list[genre_index][0], # genre
                self.__web_pages_list[genre_index][1] + str(page_number)) # web URL



def main(bucket_name, file_name, start_task_index, end_task_index):
    """Process the web pages, from index 'start_task_index' 
    to 'end_task_index'. The result will be uploaded to AWS S3 at
    'bucket_name' and 'file_name' as a tab delimited file."""

    # create local text file - output will be written here and 
    # later uploaded to S3
    f = open(file_name, mode = "w")

    # measure how long this process takes
    start_time = time.time()

    # parsers
    list_parser = GameListParser() # for web page containing list of games
    info_parser = GameInfoParser() # for web page containing info on one game

    # process web pages
    web_pages_list = WebPages_List()
    for i in range(start_task_index, end_task_index + 1):
        (genre, game_list_url) = web_pages_list.get_task(i)

        # status update
        print("Processing", genre, game_list_url)

        # extract list of games from game_list_url
        headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:57.0) Gecko/20100101 Firefox/57.0'}
        response = requests.get(game_list_url, headers=headers)
        list_parser.feed(response.text)

        # at this point, the list of games on this particular page are
        # in list_parser.url, list_parser.title, list_parser.metascore lists

        # the straight forward way to loop over j is:
        # for j in range(0, len(list_parser.url)):

        # However, the "requests.get(...)" call appears to fail
        # randomly, every few hundred web pages. So a retry mechanism
        # is necessary.

        num_pages = len(list_parser.url)
        num_retries = 0
        j = 0
        while j < num_pages:
            # extract information about the j-th game
            response = requests.get(list_parser.url[j], headers=headers)
            info_parser.feed(response.text)

            # check for possible errors
            error = False
            if info_parser.release_date == "unknown" or \
                info_parser.user_score == "unknown": 
                # the web site uses "tbd" for insufficient user_score, while 
                # my code currently defaults to "unknown"
                print("possible error @ web page #" + str(i), 
                      "game #" + str(j), genre, "game", list_parser.title[j], 
                      "due to release date or user_score being 'unknown'")
                error = True

            if error and (num_retries < 2):
                num_retries = num_retries + 1
                print("Retrying to process the web page.")

            else:
                # no error - write results to text file
                # format: genre, title, meta score, release_date, user_score, 
                # num_user_ratings
                f.write(genre + '\t' + list_parser.title[j] + '\t' 
                        + list_parser.url[j]  + '\t' + list_parser.metascore[j] 
                        + '\t' + info_parser.release_date
                        + '\t' + info_parser.user_score + '\t' 
                        + info_parser.num_user_ratings + '\n')
                j = j + 1
            
            time.sleep(2) # slow down to avoid trouble??
            
            
    f.close()

    # upload local text file
    s3 = boto3.client('s3')
    s3.upload_file(file_name, bucket_name, file_name)
    
    # clean up local text file
    os.remove(file_name)

    print("Task completed in", int(time.time() - start_time), 
          "seconds.")



if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("""Arguments required: 
            bucket_name - results will be uploaded to this AWS S3 bucket
            file_name - results will appear as this file in AWS S3
            start_task_index - the index of the first web page to process
            end_task_index - the index of the last web page to process""")
        print()
        print("Example: python web_scrape.py louis2018 data002_004.tsv 2 4")
        sys.exit()

    main(sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4]))






