import os, re, time

# export API_KEY=pk_0119cf9acb354ba4be02c102066c1bfe

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get username and id from finance table
    rows = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])

    # Get past transaction details from transaction table
    stocks = db.execute("SELECT * FROM transactions WHERE id = :user_id GROUP BY symbol HAVING max(tx_id)", user_id=session["user_id"])

    grandTotal = rows[0]["cash"]

    # lookup() the current price of each stocks the user owns, add to transaction details, update grand total
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock.update( {'price': round(quote['price'], 2)} )
        grandTotal += (stock['price'] * stock['balance'])

    return render_template("index.html", username=rows[0]["username"].title(),
                            cash=round(rows[0]["cash"], 2), stocks=stocks, grandTotal=round(grandTotal, 2))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Showing info of stock the user is about to buy"""
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not lookup(request.form.get("symbol")):
            return apology("please provide a valid stock symbol", 403)

        # Ensure buy amount is not 0
        elif not request.form.get("shares"):
            return apology("please enter how much stock you'd like to buy", 403)

        # Ensure buy amount is larger than 0
        elif int(request.form.get("shares")) < 1:
            return apology("please buy at least 1 stock", 403)

        else:
            # Get stock price and cash amount needed to purchase
            stockQuote = lookup(request.form.get("symbol"))
            buyAmount = stockQuote['price'] * int(request.form.get("shares"))
            userInfo = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session['user_id'])

            # Ensure user has enough cash
            if userInfo[0]['cash'] < buyAmount:
                return apology("not enough cash", 403)

            else:
                # Get transaction timestamp
                txTime = time.strftime('%Y-%m-%d %H:%M:%S')

                # Get the adjusted amount of stock after purchasing
                newTx = db.execute("SELECT balance, tx_id FROM transactions WHERE id = :user_id AND symbol = :symbol GROUP BY symbol HAVING max(tx_id)",
                                    user_id=session['user_id'], symbol=stockQuote['symbol'])

                if not newTx:
                    newTx = [{}]
                    newTx[0]["balance"] = int(request.form.get("shares"))
                    newTx[0]["tx_id"] = 1

                else:
                    newTx[0]["balance"] += int(request.form.get("shares"))
                    newTx[0]["tx_id"] += 1

                # Insert details of new transaction into table
                db.execute("INSERT INTO transactions (id, symbol, action, shares, balance, price, tx_id, date) VALUES (:user_id, :symbol, 'Buy', :shares, :balance, :price, :tx_id, :date)",
                            user_id=session['user_id'], symbol=stockQuote['symbol'],
                            shares=request.form.get("shares"), balance=newTx[0]['balance'],
                            price=stockQuote['price'], tx_id=newTx[0]['tx_id'], date=txTime)

                # Update new cash amount
                cashLeft = userInfo[0]['cash'] - (stockQuote['price'] * int(request.form.get("shares")))
                db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=cashLeft, user_id=session['user_id'])

                return redirect("/")

    else:
        return render_template("buy.html", stockQuote=0)



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    histories = db.execute("SELECT * FROM transactions WHERE id = :user_id ORDER BY date DESC" , user_id=session['user_id'])
    return render_template("history.html", histories=histories)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Apologise if input is not valid
        if not lookup(request.form.get("stock")):
            return apology("please quote a real stock", 403)

        # Return lookup() values
        else:
            stockQuote = lookup(request.form.get("stock"))
            return render_template("quote.html", stockQuote=stockQuote)

    else:
        return render_template("quote.html", stockQuote=False)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password doesn't contain any special characters
        elif contain_symbols(request.form.get("password")) == True:
            return apology('password must only contain letters or numbers', 403)

        # Confirm password
        elif request.form.get("password") != request.form.get("confirm_password"):
            return apology('please confirm your password', 403)

        # Check duplicate username
        username = request.form.get("username")
        checkDup = db.execute("SELECT * FROM users WHERE username = :username", username=username)

        if len(checkDup) != 1:
            # Insert username and hashed password into database
            hashedPW = generate_password_hash(request.form.get("password"))
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)",
                        username=username, password=hashedPW)

        else:
            return apology("the username is taken", 403)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        # Ensure stock symbol was submitted
        if not lookup(request.form.get("symbol")):
            return apology("please provide a valid stock symbol", 403)

        # Ensure buy amount is not 0
        elif not request.form.get("shares"):
            return apology("please enter how much stock you'd like to sell", 403)

        # Ensure buy amount is larger than 0
        elif int(request.form.get("shares")) < 1:
            return apology("please sell at least 1 stock", 403)

        selling = db.execute("SELECT * FROM transactions WHERE symbol = :symbol AND id = :user_id",
                              symbol=request.form.get("symbol"), user_id=session['user_id'])

        if not selling:
            return apology("you can't sell a stock you don't own", 403)

        elif selling[0]['balance'] < int(request.form.get("shares")):
            return apology("you can't sell more than you own", 403)

        else:
            # Get transaction timestamp
            txTime = time.strftime('%Y-%m-%d %H:%M:%S')

            # Get the price of stock the user's selling
            stockQuote = lookup(request.form.get("symbol"))
            sellTotal = stockQuote['price'] * int(request.form.get("shares"))

            # Get the user's current cash count
            currentCash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session['user_id'])
            newTotal = sellTotal + currentCash[0]['cash']

            # Update the cash values of user after selling
            db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=newTotal, user_id=session['user_id'])

            # Get balance after selling and new transaction ID
            newTx = db.execute("SELECT balance, tx_id FROM transactions WHERE id = :user_id AND symbol = :symbol GROUP BY symbol HAVING max(tx_id)",
                                    user_id=session['user_id'], symbol=stockQuote['symbol'])

            newTx[0]['balance'] -= int(request.form.get("shares"))
            newTx[0]['tx_id'] += 1

            db.execute("INSERT INTO transactions (id, symbol, action, shares, balance, price, tx_id, date) VALUES (:user_id, :symbol, 'Sell', :shares, :balance, :price, :tx_id, :date)",
                            user_id=session['user_id'], symbol=stockQuote['symbol'],
                            shares=request.form.get("shares"), balance=newTx[0]['balance'],
                            price=stockQuote['price'], tx_id=newTx[0]['tx_id'], date=txTime)

            return redirect("/")

    else:
        return render_template("sell.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

def contain_symbols(test):
    """Check if password contains any special characters"""
    char_check = re.compile('[@_!#$%^&*()<>?/\|}{~:]')

    if(char_check.search(test) == None):
        return False
    else:
        return True

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
