import pickle
import numpy as np
import pandas as pd
from sklearn.metrics.pairwise import cosine_similarity
from app.utils.preprocess import clean_text

# Load main disease dataset
df_main = pd.read_csv("data/updated_disease_with_description.csv")
df_main = df_main.rename(columns={'D_Name': 'main_disease'})
df_main['cleaned_text'] = df_main['Description'].apply(clean_text)

# Load main disease vectorizer & vectors
with open("models/vectorizers/vectorizer_main.pkl", "rb") as f:
    vectorizer_main = pickle.load(f)
with open("models/main_disease_vectors.pkl", "rb") as f:
    main_disease_vectors = pickle.load(f)

def predict_disease(user_input):
    """Predict main and sub-disease based on user symptoms."""
    cleaned_input = clean_text(user_input)

    # Compute cosine similarity for main disease
    input_vector = vectorizer_main.transform([cleaned_input])
    similarity_scores = cosine_similarity(input_vector, main_disease_vectors)
    main_disease_index = np.argmax(similarity_scores)
    main_disease = df_main.iloc[main_disease_index]['main_disease']

    # Load corresponding sub-disease model and vectorizer
    with open(f"models/vectorizers/{main_disease}_vectorizer.pkl", "rb") as f:
        vectorizer_sub = pickle.load(f)
    with open(f"models/subdisease_models/{main_disease}_model.pkl", "rb") as f:
        model = pickle.load(f)

    # Predict sub-disease
    input_vector_sub = vectorizer_sub.transform([cleaned_input])
    sub_disease = model.predict(input_vector_sub)[0]

    return {"main_disease": main_disease, "sub_disease": sub_disease}
