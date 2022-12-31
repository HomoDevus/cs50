import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, get_current_date

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    def addPrice(share):
        lookedUp = lookup(share["symbol"])
        share["price"] = usd(lookedUp["price"])
        share["name"] = lookedUp["name"]
        share["sum"] = usd(share["amount"] * lookedUp["price"])
        return share

    """Show portfolio of stocks"""
    userId = session.get("user_id")
    sharesOnAccount = db.execute("SELECT * FROM accounts WHERE user_id = ?", userId)
    sharesOnAccount = list(map(addPrice, sharesOnAccount))
    userCash = db.execute("SELECT cash FROM users WHERE id = ?", userId)[0]["cash"]
    return render_template("portfolio.html", shares=sharesOnAccount, total=usd(userCash))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Shares amount of wrong type")

        if shares < 1:
            return apology("Number of shares you want to buy below one")

        if type(shares) != int:
            return apology("Shares amount of wrong type")

        stock = lookup(symbol)

        if not stock:
            return apology(f"Symbol {symbol} dosen't exist")

        userId = session.get("user_id")
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", userId)[0]["cash"]
        cashAfterTransaction = userCash - (stock["price"] * shares)

        if cashAfterTransaction < 0:
            return apology("Not enogh money on your account")

        currentDate = get_current_date()
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cashAfterTransaction, userId)
        db.execute("INSERT INTO transactions (user_id, symbol, price, date, amount, operation) VALUES (?, ?, ?, ?, ?, ?)", \
             userId, symbol, stock["price"], currentDate, shares, "buy")
        sharesOnAccount = db.execute("SELECT amount FROM accounts WHERE user_id = ? AND symbol = ?", \
             userId, symbol)

        # Update accounts table taht stores current amount of shares
        if len(sharesOnAccount) == 0:
            db.execute("INSERT INTO accounts (user_id, symbol, amount) VALUES(?, ?, ?)", \
                 userId, symbol, shares)
        else:
            db.execute("UPDATE accounts SET amount = ? WHERE user_id = ? AND symbol = ?", \
             sharesOnAccount[0]["amount"] + shares, userId, symbol)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    def parseDollars(transaction):
        transaction["price"] = usd(transaction["price"])
        return transaction

    userId = session.get("user_id")
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", userId)
    transactions = list(map(parseDollars, transactions))

    return render_template("history.html", transactions=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
        symbol = request.form.get("symbol")
        stocks = lookup(symbol)

        if not stocks:
            return apology("There is no stocks with this symbol")

        stocks["price"] = usd(stocks["price"])
        return render_template("quoted.html", stocks=stocks)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username").strip()
        password = request.form.get("password").strip()
        confirmation = request.form.get("confirmation").strip()

        if password != confirmation:
            return apology("Password and confirmation do not much")

        if len(username) < 1 or len(password) < 1:
            return apology("Username field is empty")

        isUsernameExist = db.execute(
            "SELECT COUNT(*) FROM users WHERE username = ?", username)[0]["COUNT(*)"]

        if isUsernameExist != 0:
            return apology("This uername already exist")

        password_hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, password_hash)
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    userId = session.get("user_id")
    userShares = list(map(lambda share: share["symbol"], db.execute(
        "SELECT symbol FROM accounts WHERE user_id = ?", userId)))

    if request.method == "POST":
        symbolToSell = request.form.get("symbol")
        amountToSell = int(request.form.get("shares"))

        if amountToSell < 1:
            return apology("It isn't possible to sell negative or zero number of shares")

        if symbolToSell not in userShares:
            return apology(f"User doesn't have {symbolToSell} on his accound")

        amountOnAccount = db.execute("SELECT amount FROM accounts WHERE user_id = ? AND symbol = ?",
         userId, symbolToSell)[0]["amount"]

        if amountOnAccount < amountToSell:
            return apology("User dosen't have this amount of shares to sell")

        stock = lookup(symbolToSell)

        currentDate = get_current_date()
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", userId)[0]["cash"]
        cashAfterTransaction = userCash + (amountToSell * stock["price"])
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   cashAfterTransaction, userId)
        db.execute("INSERT INTO transactions (user_id, symbol, price, date, amount, operation) VALUES (?, ?, ?, ?, ?, ?)", \
         userId, symbolToSell, stock["price"], currentDate, amountToSell, "sell")

        if amountOnAccount == amountToSell:
            db.execute("DELETE FROM accounts WHERE user_id = ? AND symbol = ?", userId, symbolToSell)
        else:
            db.execute("UPDATE accounts SET amount = ? WHERE user_id = ? AND symbol = ?", \
             amountOnAccount - amountToSell, userId, symbolToSell)

        return redirect('/')
    else:
        return render_template("sell.html", symbols=userShares)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    if request.method == "POST":
        depositSum = int(request.form.get("sum"))
        userId = session.get("user_id")

        if depositSum < 1:
            return apology("Sum of deposit must be possible")

        userCash = db.execute("SELECT cash FROM users WHERE id = ?", userId)[0]["cash"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?", userCash + depositSum, userId)
        return redirect("/")
    else:
        return render_template("deposit.html")
