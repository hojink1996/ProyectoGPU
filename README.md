# Proyecto Final: Computación en GPU

#### Integrantes: Jou-Hui Ho, Hojin Kang

## Estructura del Proyecto

### TestProyecto

Contiene los UnitTest desarrollados para el proyecto. Utiliza la 
librería `CPPUnitTestFramework` que viene incluida con Visual Studio Code.
Los test se estructuran de la siguiente manera:

- `TEST_CLASS`: Todos los test que se encuentran bajo un mismo `TEST_CLASS`
corresponden a tests para una misma clase de C++.
- `TEST_METHOD`: Cada `TEST_METHOD` contiene los test de una funcionalidad
en particular de una clase de C++.

### ProyectoGPU

Contiene la implementación de las distintas Clases implementadas en el proyecto. 
Una vista de alto nivel de la mayoría de las Clases implementadas se puede ver
en el siguiente diagrama.

![Diagrama_Implementacion](uml_implementacion.png)

Los rectángulos rojos corresponden a partes del código en el que se pudo 
realizar alguna paralelización para volver el código más rápido. A continuación 
se presenta una breve descripción de las Clases implementadas y sus métodos más 
importantes.

- `Suit` y `Value`: Estas no son Clases. Corresponden a `enum class` que se 
  utilizan para representar la pinta y valor de las cartas en el juego de 
Texas Hold'em. 
- `Card`: Esta no es una Clase. Es un `typedef` para un `std::pair` que contiene 
contiene un `Suit` y un `Value`. Representa una de las 52 cartas posibles en el juego 
de Texas Hold'em. Hay funciones que utilizan `Card` en el `namespace` `CardEvaluation`,
que se utilizan para comparar `Card`.
- `Deck`: Es una Clase que representa un mazo de cartas en el juego de Texas Hold'em. 
Siempre tiene 52 cartas que representan las 52 cartas en una baraja inglesa. Contiene el 
método `reset()` para reiniciar el mazo después de una partida (devuelve todas las cartas 
al mazo y las baraja), y el método `drawCard()` para sacar la siguiente carta del mazo.
- `Agent`: Es una Clase Abstracta que representa un agente que toma una decisión en 
una partida de Texas Hold'em. Sumado a la Clase Abstracta, se incluyen `RandomAgent` 
(toma decisiones aleatorias), `LinearAgent` (utiliza un vector linear para tomar la decisión) 
e `InputAgent` (una persona puede usar la línea de comando para poder tomar las decisiones). 
Los `Agent` tienen un método `makeDecision()` que toma la decisión dada un estado del juego.
- `Player`: Es una Clase que representa un jugador en una partida de Texas Hold'em. 
El `Player` tiene un `Agent` que toma las decisiones por el jugador. Además el jugador 
tiene un `Hand` que corresponden a las dos cartas en su mano en Texas Hold'em. 
El `Player` tiene métodos `addCardToHand()` para agregar una carta a su mano, 
`resetHand()` para borrar las cartas de su mano cuando termina una ronda, 
`getHand()` para ver las cartas de su mano, `decide()` para tomar una decisión 
a partir del agente que tiene, entre otros.
- `TexasHoldem`: Es una Clase que contiene la lógica del juego de TexasHold'em. 
La clase contiene punteros a los `Player` que están jugadno el juego, una 
referencia al `Deck` que se está utilizando, lleva la cuenta del dinero apostado y 
el estado del juego, entre  otros. La Clase tiene métodos `addPlayer()`  
para agregar un jugador a la partida, `playRound()` para jugar una ronda, 
`playMultipleRounds()` para jugar múltiples rondas, además de métodos privados 
como `determineWinner()` que permite determinar el ganador de una ronda y 
`evaluateHand()` que permite evaluar el valor de una mano.
- `Individual`: Representa un individuo del Algoritmo Genético. Tiene un puntero 
a un jugador que está jugando la partida de Texas Hold'em. Lleva cuenta del puntaje 
que tiene el individuo en el juego y actualizaciones para el Algoritmo. Tiene métodos 
como `crossOver()` que realiza crossover con otro individuo, `mutateStrategyElementByIndexVector()` 
con lo que hace mutación, `beginPlaying()` y `endPlaying()` que le permite 
empezar y terminar de jugar una partida respectivamente.
- `GeneticAlgorithm`: Contiene la lógica para el Algoritmo Genético. Contiene 
un vector de individuos que corresponde a los individuos de la generación actual. 
Contiene métodos como `trainOneEpoch()` para entrenar una época del Algoritmo Genético, 
`getCurrentIndividuals()` para traer a los individuos de la generación actual, 
y `evaluate()` , `crossover()`, `mutate()`, `selectBest()` para las operaciones 
del Algoritmo Genético.

Para entrenar el Algoritmo Genético basta con correr el archivo `main.cpp`, 
lo cual entrena el Algoritmo Genético durante 100 generaciones, y guarda los 
individuos finales (además de cada 10 generaciones).