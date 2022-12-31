import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Constants for inputs
MONTHS = ["January","February","March","April","May","June","July",
            "August","September","October","November","December"]

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        day = int(request.form.get("day"))
        month = request.form.get("month")
        month = MONTHS.index(month)

        if day < 1 or day > 31:
            print("Day format is invalid")
        elif month == -1:
            print("Month format is invalid")
        else:
            db.execute("INSERT INTO birthdays (name, day, month) VALUES(?, ?, ?)", name, day, month + 1)

        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")
        print(birthdays)
        return render_template("index.html", birthdays=birthdays, months=MONTHS)


