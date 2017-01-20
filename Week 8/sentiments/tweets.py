#!/usr/bin/env python3
import os
import sys
import nltk

from helpers import get_user_timeline
from analyzer import Analyzer
from termcolor import colored

def main():
    
    # ensure proper usage
    if len(sys.argv) != 2:
        sys.exit("Usage: ./tweet @screen-name")
    
    # Get 50 tweets and store list in variable 'tweet'    
    tweet = get_user_timeline(sys.argv[1], 50)
    
    # Check if tweet retrieval failed. If so, exit with appropriate message
    if tweet == None:
        sys.exit("Tweet return failed")
        
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    
    tokenizer = nltk.tokenize.TweetTokenizer()
    analyzer = Analyzer(positives, negatives)
    
    # Loop through tweet list
    for i in range(len(tweet)):
        # Create token list
        tokens = tokenizer.tokenize(tweet[i])
        sum = 0
        # Loop through token list and get sum of tweet using scores of individual tokens (words)
        for j in range(len(tokens)):
            score = analyzer.analyze(tokens[j])
            if score > 0:
                sum += 1
            if score < 0:
                sum -= 1
        # Print tweet in appropriate color
        if sum > 0:
            print(colored("{: d} {}".format(sum, tweet[i]), "green"))
        elif sum < 0:
            print(colored("{} {}".format(sum, tweet[i]), "red"))
        else:
            print(colored("{: d} {}".format(sum, tweet[i]), "yellow"))
            
    
if __name__ == "__main__":
    main()
    
    
        
    
