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
    static double learningRate = learnRate;

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
            // integraci�n de todos los errores.
            currentError = sqrt(cumulativeError / epochLength);

            // Evaluar si el error del epoch est� dentro de un threshold.
            // Cuando el error es menor al threshold es porque est� lo
            // suficientemente acercado a 0 como para seguir tuneando.
            // El threshold debe ser bajo pero no tanto, ya que si es
            // muy bajo podr�a nunca llegarse a lograr un PID "�ptimo".
            needsTraining = currentError > errorThreshold;

            // NEEDS TESTING
            // Ajusta el learning rate para que decaiga en base al error,
            // herramienta clásica de optimización de redes neuronale;
            // ya que al acercarse al mínimo error posible se necesita
            // ser linealmente más preciso.
            learningRate *= fabs(currentError) / 100;

            // Si es necesario despu�s de hacer el c�lculo,
            // se hace el entrenamiento (backpropagation).
            if(needsTraining){

                // Calcular los errores.
                double deltaError = previousError - currentError;
                previousError = currentError;

                // Tunear las constantes.
                _Kp -= _Kp * Perror * deltaError * learningRate;
                _Ki -= _Ki * Ierror_abs * deltaError * learningRate;
                _Kd -= _Kd * Derror * deltaError * learningRate;
            }

            // Resetear los contadores de errores, para el pr�ximo epoch.
            Ierror_abs = 0;
            cumulativeError = 0;
        }
    }

    // Devolver el valor de steer, para poder aplicarlo
    // a los motores o servos, dependiendo del uso.
    return steer;
}
