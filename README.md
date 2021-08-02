# Proyecto Final: Computaci�n en GPU

#### Integrantes: Jou-Hui Ho, Hojin Kang

## Estructura del Proyecto

### TestProyecto

Contiene los UnitTest desarrollados para el proyecto. Utiliza la 
librer�a `CPPUnitTestFramework` que viene incluida con Visual Studio Code.
Los test se estructuran de la siguiente manera:

- `TEST_CLASS`: Todos los test que se encuentran bajo un mismo `TEST_CLASS`
corresponden a tests para una misma clase de C++.
- `TEST_METHOD`: Cada `TEST_METHOD` contiene los test de una funcionalidad
en particular de una clase de C++.

### ProyectoGPU

Contiene la implementaci�n de las distintas Clases implementadas en el proyecto. 
Una vista de alto nivel de la mayor�a de las Clases implementadas se puede ver
en el siguiente diagrama.

![Diagrama_Implementacion](uml_implementacion.png)

Los rect�ngulos rojos corresponden a partes del c�digo en el que se pudo 
realizar alguna paralelizaci�n para volver el c�digo m�s r�pido. A continuaci�n 
se presenta una breve descripci�n de las Clases implementadas y sus m�todos m�s 
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
m�todo `reset()` para reiniciar el mazo despu�s de una partida (devuelve todas las cartas 
al mazo y las baraja), y el m�todo `drawCard()` para sacar la siguiente carta del mazo.
- `Agent`: Es una Clase Abstracta que representa un agente que toma una decisi�n en 
una partida de Texas Hold'em. Sumado a la Clase Abstracta, se incluyen `RandomAgent` 
(toma decisiones aleatorias), `LinearAgent` (utiliza un vector linear para tomar la decisi�n) 
e `InputAgent` (una persona puede usar la l�nea de comando para poder tomar las decisiones). 
Los `Agent` tienen un m�todo `makeDecision()` que toma la decisi�n dada un estado del juego.
- `Player`: Es una Clase que representa un jugador en una partida de Texas Hold'em. 
El `Player` tiene un `Agent` que toma las decisiones por el jugador. Adem�s el jugador 
tiene un `Hand` que corresponden a las dos cartas en su mano en Texas Hold'em. 
El `Player` tiene m�todos `addCardToHand()` para agregar una carta a su mano, 
`resetHand()` para borrar las cartas de su mano cuando termina una ronda, 
`getHand()` para ver las cartas de su mano, `decide()` para tomar una decisi�n 
a partir del agente que tiene, entre otros.
- `TexasHoldem`: Es una Clase que contiene la l�gica del juego de TexasHold'em. 
La clase contiene punteros a los `Player` que est�n jugadno el juego, una 
referencia al `Deck` que se est� utilizando, lleva la cuenta del dinero apostado y 
el estado del juego, entre  otros. La Clase tiene m�todos `addPlayer()`  
para agregar un jugador a la partida, `playRound()` para jugar una ronda, 
`playMultipleRounds()` para jugar m�ltiples rondas, adem�s de m�todos privados 
como `determineWinner()` que permite determinar el ganador de una ronda y 
`evaluateHand()` que permite evaluar el valor de una mano.
- `Individual`: Representa un individuo del Algoritmo Gen�tico. Tiene un puntero 
a un jugador que est� jugando la partida de Texas Hold'em. Lleva cuenta del puntaje 
que tiene el individuo en el juego y actualizaciones para el Algoritmo. Tiene m�todos 
como `crossOver()` que realiza crossover con otro individuo, `mutateStrategyElementByIndexVector()` 
con lo que hace mutaci�n, `beginPlaying()` y `endPlaying()` que le permite 
empezar y terminar de jugar una partida respectivamente.
- `GeneticAlgorithm`: Contiene la l�gica para el Algoritmo Gen�tico. Contiene 
un vector de individuos que corresponde a los individuos de la generaci�n actual. 
Contiene m�todos como `trainOneEpoch()` para entrenar una �poca del Algoritmo Gen�tico, 
`getCurrentIndividuals()` para traer a los individuos de la generaci�n actual, 
y `evaluate()` , `crossover()`, `mutate()`, `selectBest()` para las operaciones 
del Algoritmo Gen�tico.

Para entrenar el Algoritmo Gen�tico basta con correr el archivo `main.cpp`, 
lo cual entrena el Algoritmo Gen�tico durante 100 generaciones, y guarda los 
individuos finales (adem�s de cada 10 generaciones).