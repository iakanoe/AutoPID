typedef enum {false, true} bool;
#include <math.h> //#define abs(x) ((x) < 0 ? -(x) : (x))

double Kp = 1;
double Ki = 1;
double Kd = 1;

double Perror = 0;
double Ierror = 0;
double Derror = 0;
double Ierror_abs = 0;

int counter = 0;
bool needsTraining = true;
double currentError = 0;
double previousError = 0;
double cumulativeError = 0;
const int epochLength = 200;

const double errorThreshold = 0.005;
const double learnRate = 0.01;

void updateEpochError(double error){
    Ierror_abs = abs(error);
    cumulativeError += (error * error);
}

void resetError(){
    Ierror_abs = 0;
    cumulativeError = 0;
}

void evaluatePID(){
    if(!needsTraining) return;
    currentError = sqrt(cumulativeError / epochLength);
    needsTraining = (currentError > errorThreshold);
}

void adjust(double * Kx, double dx, double dE){
    double partialDKx = *(Kx) * dx * dE * learnRate;
    *(Kx) -= partialDKx;
}

void backpropagation(){
    double deltaError = previousError - currentError;
    previousError = currentError;
    
    adjust(&Kp, Perror, deltaError);
    adjust(&Ki, Ierror_abs, deltaError);
    adjust(&Kd, Derror, deltaError);
}

void updateError(double err){
    Derror = err - Perror;
    Perror = err;
    Ierror += err;
    
    updateEpochError(err);
}

double calcPID(){
    return (Kp * Perror + Kd * Derror + Ki * Ierror);
}