#include <math.h>
#include "AutoPID.h"

double PID(double err){
    static double _Kp = Kp;
    static double _Ki = Ki;
    static double _Kd = Kd;
    static double Perror = 0;
    static double Ierror = 0;
    static double Derror = 0;
    static double Ierror_abs = 0;
    static char needsTraining = 1;
    static double currentError = 0;
    static double previousError = 0;
    static double cumulativeError = 0;
    static int count = 0;
    
    // Calcular el PID y obtener el valor para doblar (steer)
    Derror = err - Perror;
    Perror = err;
    Ierror += err;
    Ierror_abs += fabs(err);
    cumulativeError += (err * err);
    double steer = _Kp * Perror + _Kd * Derror + _Ki * Ierror;
    
    // Contador de epoch
    count++;
    
    // Cada epoch:
    if(count == epochLength){
        count = 0;
        
        // Si hace falta seguir tuneando:
        if(needsTraining){
            
            // Calcular el error del epoch, siendo el promedio de la
            // integración de todos los errores.
            currentError = sqrt(cumulativeError / epochLength);

            // Evaluar si el error del epoch está dentro de un threshold.
            // Cuando el error es menor al threshold es porque está lo
            // suficientemente acercado a 0 como para seguir tuneando.
            // El threshold debe ser bajo pero no tanto, ya que si es
            // muy bajo podría nunca llegarse a lograr un PID "óptimo".
            needsTraining = currentError > errorThreshold;

            // Si es necesario después de hacer el cálculo,
            // se hace el entrenamiento (backpropagation).
            if(needsTraining){
                
                // Calcular los errores.
                double deltaError = previousError - currentError;
                previousError = currentError;

                // Tunear las constantes.
                _Kp -= _Kp * Perror * deltaError * learnRate;
                _Ki -= _Ki * Ierror_abs * deltaError * learnRate;
                _Kd -= _Kd * Derror * deltaError * learnRate;
            }

            // Resetear los contadores de errores, para el próximo epoch.
            Ierror_abs = 0;
            cumulativeError = 0;
        }
    }
    
    // Devolver el valor de steer, para poder aplicarlo
    // a los motores o servos, dependiendo del uso.
    return steer;
}
