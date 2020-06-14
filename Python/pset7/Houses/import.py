# Started and submitted 14 June, 2020.
# Lawrence Wong. CS50x. Pset7 - Houses.

import csv
import sys
from cs50 import SQL


def main():
    if len(sys.argv) != 2:
        print("Usage: python import.py characters.csv")
        sys.exit(1)
        
    db = SQL("sqlite:///students.db")

    with open(sys.argv[1], "r") as f:
        csv_file = csv.reader(f)
        # Skips the header line
        next(csv_file)
    
        for line in csv_file:
            # Split the full names by spaces
            fullName = line[0].split(" ")
            firstName = fullName[0]

            # Leave middle name as NULL and record last name if full name has only 2 values
            if len(fullName) == 2:
                lastName = fullName[1]
                db.execute("INSERT INTO students (first, last, house, birth) VALUES(?, ?, ?, ?)", 
                           firstName, lastName, line[1], line[2])
            
            # Record middle and last name when full name has 3 values
            else:
                midName = fullName[1]
                lastName = fullName[2]
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", 
                           firstName, midName, lastName, line[1], line[2])
                
        sys.exit(0)


main()
