import re
import pickle

# Load stop words
with open("data/stop_words.ob", "rb") as fp:
    domain_stop_word = pickle.load(fp)

def clean_text(text):
    """Clean and preprocess the input text."""
    text = str(text).lower()
    text = re.sub(r"[^a-z ]", " ", text)
    text = " ".join([word for word in text.split() if word not in domain_stop_word])
    return text
