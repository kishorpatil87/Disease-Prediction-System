from fastapi import FastAPI
from app.routes.predict import router as predict_router

app = FastAPI(title="Disease Prediction API")

# Include prediction routes
app.include_router(predict_router)

@app.get("/")
def home():
    return {"message": "Welcome to the Disease Prediction API!"}
