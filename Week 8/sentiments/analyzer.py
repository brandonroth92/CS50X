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
        
        # Analyze text (word) for sentiment, returning its corresponding value
        if text.lower() in self.pSet:
            return 1
        elif text.lower() in self.nSet:
            return -1
        return 0
        
        