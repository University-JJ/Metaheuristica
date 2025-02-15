/*
 * MQKPSolGenerator.cpp
 *
 * Fichero que define los métodos de la clase MQKPInstance. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPSolGenerator.h"
#include "MQKPInstance.h"
#include "MQKPSolution.h"

void MQKPSolGenerator::genRandomSol(MQKPInstance &instance, MQKPSolution &solution)
{
	int numObjs = instance.getNumObjs();
	int numKnapsacks = instance.getNumSacks();

	for (int i = 0; i < numObjs; i++)
	{
		int randomKnapsack = rand() % (1 + numKnapsacks); 
		solution.putObjectIn(i, randomKnapsack);
	}
}


//TODO Cambiar para seleccionar una de las mochilas disponibles