# Started and submitted 7 June, 2020.
# Lawrence Wong. CS50x. Python - Mario(more)

def main():
    height = get_height()

    for row in range(height):
        spaces = int(height - row - 1)
        leftHash = 0
        rightHash = 0
        
        for leftSpace in range(spaces):
            print(" ", end="")

        while leftHash != (row + 1):
            print("#", end="")
            leftHash += 1
        
        print("  ", end="")

        while rightHash != (row + 1):
            print("#", end="")
            rightHash += 1

        print("")


def get_height():
    """Get height between 1-8"""
    while True:
        try:
            height = int(input("Height: "))
        except ValueError:
            continue
        if height < 1 or height > 8:
            continue
        else:
            return height


main()
