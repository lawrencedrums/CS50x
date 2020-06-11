# Started and submitted 7 June, 2020.
# Lawrence Wong. CS50x. Python - Credit

def main():
    number = int(input("Number: "))
    checkNumber = number
    luhnSum = 0
    luhnProduct = 0

    if number < 10000000000000000 and number >= 1000000000000:
        while number > 0:
            product = ((number // 10) % 10) * 2
            if product > 9:
                luhnProduct += int((product % 10) + (product // 10))
            else:
                luhnProduct += product

            luhnSum += number % 10

            number = number // 100

    else:
        print("1 INVALID\n")
        return

    luhnValid = (luhnProduct + luhnSum) % 10

    if luhnValid == 0:
        if (checkNumber // 1000000000000 == 4) or (checkNumber // 1000000000000000 == 4):
            print("VISA\n")
        elif (checkNumber // 10000000000000 == 34) or (checkNumber // 10000000000000 == 37):
            print("AMEX\n")
        elif (checkNumber // 100000000000000 > 50) and (checkNumber // 100000000000000 < 56):
            print("MASTERCARD\n")
        else:
            print("2 INVALID\n")
            
    else:
        print("3 INVALID\n")


main()
