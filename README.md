# Entrega3IAA
## Autor
Martin Crisostomo 201673609-3
## Ejecución

- Estando en la carpeta realizar el comando "make" en la consola.
- Se ejecuta con el comando ./GGen "nombre instancia" ChanceMutacion Poblacion ChanceCruce Elitismo Tiempo[Segundos] Penalizacion Modo Semilla

Donde: 

- ChanceMutacion: Probabilidad de que mute [0.0,1.0]
- Poblacion: Poblacion en el algoritmo genetico [0,inf]
- ChanceCruce: Probabilidad de que ocurra cruce [0.0,1.0]
- Elitismo: Cantidad de genes a ser elites [0,inf]
- Tiempo: Cantidad de tiempo total de ejecucion [0,inf]
- Penalizacion: La penalizacion que se le otorgara por cada violacion de restriccion realizada [0,inf]
- Modo: Como se realiza la mutacion [0,1,2(proximamente)]
- Semilla: La semilla para los randoms.


Esto generara un archivo txt con el nombre "Resultados_Nombre_Instancia_semilla_modo.txt" con los resultados obtenidos.
