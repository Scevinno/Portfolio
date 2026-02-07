import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


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
    """Show portfolio of stocks"""

    portfolio = db.execute(
        "SELECT * FROM portfolio WHERE id = ?",
        session["user_id"]
    )

    cash = db.execute(
        "SELECT cash FROM users WHERE id = ?",
        session["user_id"]
    )

    total = cash[0]["cash"]
    for row in portfolio:
        total += row["invested_cash"]

    return render_template("index.html", portfolio=portfolio,cash=cash[0]["cash"],total=total)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    account = db.execute(
                "SELECT cash FROM users WHERE id = ?",
                session["user_id"]
                )

    if request.method == "POST":
        q = lookup(request.form.get("symbol").upper())

        if not request.form.get("symbol") or not request.form.get("quantity"):
            return apology("Must enter quote",403)

        if q == None:
            return apology("Invalid quote",403)

        if int(request.form.get("quantity")) <= 0:
            return apology("Please enter positive quantity",403)

        else:
            symb = request.form.get("symbol")
            quantity = request.form.get("quantity")
            purchase_price = float(quantity)*q['price']
            pps = float(purchase_price) / float(quantity)


            if purchase_price > account[0]["cash"]:
                return apology("Insufficient Funds",403)

            rows = db.execute(
                        "SELECT * FROM portfolio WHERE id = ? AND symbol = ?",
                        session["user_id"],
                        symb
                     )

            prev_i = rows[0]["invested_cash"] if rows else None
            prev_s = rows[0]["shares"] if rows else None

            if prev_i is None:
                # No row exists → INSERT
                db.execute(
                    "INSERT INTO portfolio (id, symbol, shares, price_per_share, invested_cash) VALUES (?, ?, ?, ?, ?)",
                    session["user_id"],
                    symb,
                    quantity,
                    pps,
                    purchase_price
                )
            else:
                # Row exists → UPDATE
                db.execute(
                    "UPDATE portfolio SET invested_cash = ?, price_per_share = ?, shares = ? WHERE id = ? AND symbol = ?",
                    prev_i + purchase_price,
                    (prev_i + purchase_price) / (prev_s + float(quantity)),
                    prev_s + float(quantity),
                    session["user_id"],
                    symb
                )

            db.execute(
                "UPDATE users SET cash = cash - ? WHERE id = ?",
                purchase_price,
                session["user_id"]
            )

            db.execute(
                "INSERT INTO history (id, symbol, shares, price_per_share, symbol_transaction, balance) VALUES (?, ?, ?, ?, ?, ?)",
                session["user_id"],
                symb,
                quantity,
                pps,
                purchase_price,
                account[0]["cash"]
            )

            return render_template("buy.html",name = q['name'],quantity = int(request.form.get("quantity")), price = usd(purchase_price))

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    hist = db.execute("SELECT * FROM history WHERE id = ?", session["user_id"])

    return render_template("history.html", hist=hist)

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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        q = lookup(request.form.get("quote").upper())

        if not request.form.get("quote"):
            return apology("Must enter quote",403)

        if q == None:
            return apology("Invalid quote",403)
        else:
            session["symbol"] = q['symbol']
            name = q['name']
            price = q['price']
            return render_template("quote.html",symbol=session["symbol"],name=name,price=price)


    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("confirm_password"):
            return apology("must provide password", 403)

         # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirm_password"):
            return apology("password must match", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username doesn't yet exist
        if len(rows) > 0:
            return apology("user already registered", 403)

        #Add user to the database table
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?,?)",
            request.form.get("username"),
            generate_password_hash(request.form.get("password"))
        );

        # Remember which user has logged in
        session["user_id"] = db.execute("SELECT id FROM users WHERE username = ?", request.form.get("username"))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    account = db.execute(
                "SELECT cash FROM users WHERE id = ?",
                session["user_id"]
                )

    if request.method == "POST":
        q = lookup(request.form.get("symbol").upper())

        if request.form.get("symbol") == "symbol" or not request.form.get("quantity"):
            return apology("Must enter quote",403)

        if int(request.form.get("quantity")) <= 0:
            return apology("Please enter positive quantity",403)

        else:
            symb = request.form.get("symbol")
            quantity = int(request.form.get("quantity"))
            purchase_price = float(quantity)*q['price']
            pps = float(purchase_price) / float(quantity)

            shares_available = db.execute(
                        "SELECT shares FROM portfolio WHERE id = ? AND symbol = ?",
                        session["user_id"],
                        symb
                     )

            if quantity > shares_available[0]["shares"]:
                return apology("Insufcicient shares",403)

            rows = db.execute(
                        "SELECT * FROM portfolio WHERE id = ? AND symbol = ?",
                        session["user_id"],
                        symb
                     )

            prev_i = rows[0]["invested_cash"] if rows else None
            prev_s = rows[0]["shares"] if rows else None

            if prev_s == quantity:
                # Sell All → DELETE
                db.execute(
                    "DELETE FROM portfolio WHERE id = ? AND symbol = ?",
                    session["user_id"],
                    symb
                )

            else:
                # Sell Some → UPDATE
                db.execute(
                    "UPDATE portfolio SET invested_cash = ?, price_per_share = ?, shares = ? WHERE id = ? AND symbol = ?",
                    prev_i - purchase_price,
                    (prev_i - purchase_price) / (prev_s - float(quantity)),
                    prev_s - float(quantity),
                    session["user_id"],
                    symb
                )

            db.execute(
                "UPDATE users SET cash = cash + ? WHERE id = ?",
                purchase_price,
                session["user_id"]
            )

            portfolio = db.execute(
                            "SELECT * FROM portfolio WHERE id = ?",
                            session["user_id"]
                        )

            db.execute(
                "INSERT INTO history (id, symbol, shares, price_per_share, symbol_transaction, balance) VALUES (?, ?, ?, ?, ?, ?)",
                session["user_id"],
                symb,
                -quantity,
                pps,
                -purchase_price,
                account[0]["cash"]
            )

            return render_template("sell.html",name = q['name'],quantity = int(request.form.get("quantity")), price = usd(purchase_price),portfolio=portfolio)

    else:
        portfolio = db.execute(
                "SELECT * FROM portfolio WHERE id = ?",
                session["user_id"]
            )

        return render_template("sell.html",portfolio=portfolio)
