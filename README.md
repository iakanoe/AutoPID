# AutoPID
Auto tuning PID controller in C

Basado en modelo ajeno, refactorizado y adaptado para el Club de Robótica

Esto es una red neuronal, un PID que se ajusta a si mismo. Actua teniendo en cuenta al controlador PID como una red neuronal de una capa, con tres inputs y un output. Cada epoch (iteracion) se hace backpropagation para poder ajustar los pesos (las constantes) del PID y así obtener el error más cercano a 0 posible. Una vez que el error está dentro del threshold, se deja de tunear.

En lo posible, este programa se debe instalar con un PID inicial que funcione, ya que caso contrario el robot se iría de la pista antes de poder aprender y auto-tunearse. Los pesos para el PID inicial se pueden modificar en las lineas 28 a 32 de este mismo archivo.
