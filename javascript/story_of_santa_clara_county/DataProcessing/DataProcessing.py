import datetime


def extract_stock_data(file_name):
    """ Extract stock data for a single company.

    The data should be in the file_name given. First column is the 
    "Date" and seventh column is the "adj Close". Extract data from
    "adj Close". 
    """

    f = open(file_name)
    data = []

    for line in f:
        columns = line.split()
        if len(columns) == 7:
            """
            Need to check the "Date" column here.

            Most of the "Date" is from early in the month, but 
            Yahoo stock will insert data at the end of every web page
            that is from the end of the month. That is why the "Date" 
            column must be checked.
            """
            date = None
            try:
                # column[0] can be in "1-Mar-16"
                date = datetime.datetime.strptime(columns[0], "%d-%b-%y")

            except: 
                try: 
                    # alternative format: "12/1/1969"
                    date = datetime.datetime.strptime(columns[0], "%m/%d/%Y")
                except:
                    # date will be "None"
                    # print an error message
                    print("Failed to process date: " + columns[0])
                    
            if date != None and date.day < 15:
                # extract data from the 7th column
                number = columns[6].replace(',', '')
                data.append(float(number))

    f.close()

    # write "data" to disk
    f = open(file_name + ".js", mode="w")

    f.write("var data = [")
    
    # output data in chronological order, so write the lats data first
    for i in range(len(data) - 1, -1, -1):
        f.write(str(data[i]))
        if i != 0:
            f.write(", ")

        if (i % 10 == 0) and (i != 0) and (i != (len(data)-1)):
            f.write("\n  ")

    f.write("];")

    f.close()



    
    



def main():
    # extract stock data
    extract_stock_data("./stock_data/aapl.tsv")
    extract_stock_data("./stock_data/csco.tsv")
    extract_stock_data("./stock_data/goog.tsv")
    extract_stock_data("./stock_data/hpq.tsv")
    extract_stock_data("./stock_data/intc.tsv")
    extract_stock_data("./stock_data/sp500.tsv")



if __name__ == "__main__": 
    main()
