from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from time import gmtime, strftime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    grandTotal = 0
    
    # select stocks and info with at least 1 share owned
    stocks = db.execute("SELECT symbol, name, sum(shares) FROM portfolio WHERE user_id = :id GROUP BY name HAVING sum(shares) > 0 ORDER BY symbol asc", id=session["user_id"])
    
    # loop through list, looking up current price, and compute total owned value of each stock and grand total
    for i in range(len(stocks)):
        quote = lookup(stocks[i]["symbol"])
        stocks[i]["price"] = quote["price"]
        stocks[i]["total"] = stocks[i]["sum(shares)"] * stocks[i]["price"]
        grandTotal += stocks[i]["total"]
        stocks[i]["price"] = usd(quote["price"])
        stocks[i]["total"] = usd(stocks[i]["total"])
    
    # get current cash value of user from users table
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]
    
    # add cash to running grand total of stock values
    grandTotal += cash
    
    return render_template("index.html", stocks=stocks, cash=usd(cash), grandTotal=usd(grandTotal))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    if request.method == "POST":
        
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must submit stock symbol")
            
        # ensure shares is an integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("invalid shares")
        
        # ensure shares is a positive integer
        if not shares > 0:
            return apology("invalid shares")
            
        quote = lookup(request.form.get("symbol"))
        
        # ensure stock symbol was valid
        if not quote:
            return apology("invalid stock symbol")
        
        # get cash value of user
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        
        # initialize variables for sql statement
        cost = quote["price"] * shares
        symbol = request.form.get("symbol")
        name = quote["name"]
        saleType="buy"
        
        # ensure user has enough cash for purchase
        if not cash[0]["cash"] >= cost:
            return apology("insufficient funds")
        
        # execute sale and insert into portfolio
        db.execute("INSERT INTO portfolio ('user_id', 'symbol', 'name', 'transaction', 'price', 'shares', 'date') VALUES (:user_id, :symbol, :name, :transaction, :price, :shares, :date)", user_id=session["user_id"], symbol=symbol, name=name, transaction=saleType, price=usd(quote["price"]), shares=shares, date=strftime("%Y-%m-%d %H:%M:%S", gmtime()))
        
        # update users cash value
        db.execute("UPDATE users SET cash = cash - :cost WHERE id = :id", cost=cost, id=session["user_id"])
        
        # redirect user to home page
        flash("Bought!")
        return redirect(url_for("index"))
    
    # if get request, render buy shares form
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    # get all transactions for logged-in user
    transactions = db.execute("SELECT * FROM portfolio WHERE user_id = :id ORDER BY date asc", id=session["user_id"])
 
    # render history template
    return render_template("history.html", transactions=transactions)
    

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")
            
        quote = lookup(request.form.get("symbol"))
        
        # ensure stock symbol was valid
        if not quote:
            return apology("must provide valid stock symbol")
        
        # get price of stock
        quote["price"] = usd(quote["price"])
        
        # display quote via quoted.html
        return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=quote["price"])
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # forget any user_id
    session.clear()
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")
            
        # ensure both password fields were submitted
        elif not request.form.get("password") or not request.form.get("password-confirmation"):
            return apology("must provide password")
        
        # ensure password and password-confirmation fields match
        elif not request.form.get("password") == request.form.get("password-confirmation"):
            return apology("passwords do not match")
        
        # create hash from password    
        hash = pwd_context.encrypt(request.form.get("password"))
        
        # insert username and hash into users database
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=hash)
        
        # ensure username does not already exist
        if not result:
            return apology("username already exists")
            
        # remember which user has registered
        session["user_id"] = result
        
        # redirect user to home page
        return redirect(url_for("index"))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    if request.method == "POST":
        
        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must submit stock symbol")
            
        # ensure shares amount was submitted and is an integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("invalid shares")
        
        # ensure shares is a positive integer
        if not shares > 0:
            return apology("invalid shares")
            
        symbol = request.form.get("symbol")
            
        # get dict containing stock info    
        quote = lookup(symbol)
        
        # ensure stock symbol was valid
        if not quote:
            return apology("invalid stock symbol")
            
        # check if user owns enough shares of stock for sale
        currentShares = db.execute("SELECT sum(shares) FROM portfolio WHERE user_id = :id AND symbol = :symbol", id=session["user_id"], symbol=symbol)
        if currentShares[0]["sum(shares)"] == None:
            return apology("you do not own this stock")
        
        if not currentShares[0]["sum(shares)"] >= shares:
            return apology("you do not own enough shares")
        
        # set variables for sql statement    
        earning = quote["price"] * shares
        name = quote["name"]
        saleType="sell"
        shares = -shares
        
        # execute sale and insert into portfolio
        db.execute("INSERT INTO portfolio ('user_id', 'symbol', 'name', 'transaction', 'price', 'shares', 'date') VALUES (:user_id, :symbol, :name, :transaction, :price, :shares, :date)", user_id=session["user_id"], symbol=symbol, name=name, transaction=saleType, price=usd(quote["price"]), shares=shares, date=strftime("%Y-%m-%d %H:%M:%S", gmtime()))
        
        # add value of sale to users cash amount
        db.execute("UPDATE users SET cash = cash + :earning WHERE id = :id", earning=earning, id=session["user_id"])
        
        # redirect user to home page
        flash("Sold!")
        return redirect(url_for("index"))
            
        return render_template("sell.html", currentShares=currentShares)
        
    # if get request, render sell form template    
    else:
        return render_template("sell.html")
    
    
@app.route("/changepass", methods=["GET", "POST"])
def changepass():
    
    if request.method == "POST":
        
        # ensure all fields are completed
        if not request.form.get("oldpass") or not request.form.get("newpass") or not request.form.get("newpass-confirmation"):
            return apology("form incomplete")
        
        # get current hash value of user 
        hash = db.execute("SELECT hash FROM users WHERE id = :id", id=session["user_id"])[0]["hash"]
        
        # check old password against hash
        if not pwd_context.verify(request.form.get("oldpass"), hash):
            return apology("old password is incorrect")
        
        # ensure new passwords match    
        if not request.form.get("newpass") == request.form.get("newpass-confirmation"):
            return apology("new password does not match")
        
        # create new password hash
        newHash = pwd_context.encrypt(request.form.get("newpass"))
        
        # update users database with new hash 
        db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=newHash, id=session["user_id"])
        
        # redirect user to home page
        flash("Password changed!")
        return redirect(url_for("index"))
    
    # if get reqeust, render changepass form template        
    else:
        return render_template("changepass.html")
