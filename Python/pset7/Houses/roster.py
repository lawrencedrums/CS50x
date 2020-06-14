# Started and submitted 14 June, 2020.
# Lawrence Wong. CS50x. Pset7 - Houses.

import csv
import sys
from cs50 import SQL


def main():
    if len(sys.argv) != 2:
        print("Usage: python roster.py house")
        sys.exit(1)
        
    db = SQL("sqlite:///students.db")
    queryResult = db.execute(f'SELECT first, middle, last, birth FROM students WHERE house = "{sys.argv[1].title()}" ORDER BY last, first')
    
    for line in queryResult:
        # Store every line of results in a list
        fullResult = list(line.values())
        # Don't print middle name if middle is empty
        if fullResult[1] == None:
            print(f"{fullResult[0]} {fullResult[2]}, born {fullResult[3]}")
        else:
            print(f"{fullResult[0]} {fullResult[1]} {fullResult[2]}, born {fullResult[3]}")

        
main()
