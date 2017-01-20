import nltk

# Implements sentiment analysis
class Analyzer():

    def __init__(self, positives, negatives):
        
        # Load positive words file and store individual words in set
        p = open(positives, "r")
        self.pSet = set()
        
        for line in p:
            if not line.startswith(";"):
                if line.strip():
                    line = line.strip('\n')
                    self.pSet.add(line)
        
        # Load negative words file and store individual words in set
        n = open(negatives, "r")
        self.nSet = set()
        
        for line in n:
            if not line.startswith(";"):
                if line.strip():
                    line = line.strip('\n')
                    self.nSet.add(line)
            
        
    def analyze(self, text):
        
        if type(text) != list:
            text = [text]
        
        # Loop through word or tweet list
        for i in range(len(text)):
            sum = 0
            # Loop through token list and get sum of tweet using scores of individual tokens (words)
            for j in range(len(text)):
                if text[j].lower() in self.pSet:
                    sum += 1
                if text[j].lower() in self.nSet:
                    sum -= 1
            return sum

        
        