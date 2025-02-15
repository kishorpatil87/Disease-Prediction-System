from fastapi import APIRouter, Form
from app.models.disease_model import predict_disease

router = APIRouter()

@router.post("/predict")
def get_prediction(user_input: str = Form(...)):
    """API endpoint to predict disease from user symptoms."""
    prediction = predict_disease(user_input)
    return prediction
