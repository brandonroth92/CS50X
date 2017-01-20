from flask import Flask, redirect, render_template, request, url_for

import os
import sys
import nltk
import helpers
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets and check for error
    tweets = helpers.get_user_timeline(screen_name, 100)
    if tweets == None:
        return redirect(url_for("index"))

     # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    
    tokenizer = nltk.tokenize.TweetTokenizer()
    analyzer = Analyzer(positives, negatives)
    positive = 0
    negative = 0
    neutral = 0
    
    # Loop through tweets list and inidividual tokens. Add each tweet to corresponding variable (positive, negative, neutral)
    for i in range(len(tweets)):
        tokens = tokenizer.tokenize(tweets[i])
        sum = analyzer.analyze(tokens)
        if sum > 0:
            positive += 1
        elif sum < 0:
            negative += 1
        else:
            neutral += 1

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
