/*
 * MQKPIteratedGreedy.cpp
 *
 * Fichero que define las funciones de la clase MQKPIteratedGreedy. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPIteratedGreedy.h"
#include "MQKPObjectAssignmentOperation.h"
#include "MQKPSolution.h"
#include <iostream>

using namespace std;

void MQKPIteratedGreedy::chooseOperation(
	MQKPObjectAssignmentOperation &operation)
{

	int bestObj = 0;
	int bestKnapsack = 0;
	double bestDensity = 0;
	double bestDeltaFitness = 0;
	bool initialisedBestDensity = false;
	unsigned numObjs = _instance->getNumObjs();
	unsigned numKnapsacks = _instance->getNumKnapsacks();

	/**
	 * TODO	DONE
	 * Recorrer todos los objetos que no están en ninguna mochila
	 *   Recorrer todas las mochilas
	 *     Calcular el delta del fitness de asignar dicho objeto a dicha mochila y la densidad
	 *     Almacenar dicha asignación como la mejor en caso de que sea la de mayor densidad
	 */

	for (unsigned i = 0; i < numObjs; i++)
	{

		int indexObj = i;

		if (_sol->whereIsObject(indexObj) == 0)
		{ //TODO comprobar que no está en ninguna mochila

			for (unsigned j = 1; j <= numKnapsacks; j++)
			{ //TODO para todas las mochilas disponibles (saltarse la 0)

				//TODO Calcular delta fitness, densidad como deltaFitness dividido por el peso, y actualizar la mejor opción
				int indexKnapsack = j;

				double deltaFitness = MQKPEvaluator::computeDeltaFitness(*this->_instance, *this->_sol, indexObj, indexKnapsack);
				double density = deltaFitness / this->_instance->getWeight(indexObj);

				if (density > bestDensity || initialisedBestDensity == false)
				{
					initialisedBestDensity = true;
					bestDensity = density;
					bestObj = indexObj;
					bestKnapsack = indexKnapsack;
					bestDeltaFitness = deltaFitness;
				}
			}
		}
	}

	operation.setValues(bestObj, bestKnapsack, bestDeltaFitness);
}

void MQKPIteratedGreedy::rebuild()
{

	/** Seleccionar la primera operación */
	MQKPObjectAssignmentOperation operation;
	chooseOperation(operation);

	/** TODO	DONE
	 * Mientras la operación tenga un incremento de fitness positivo, operation.getDeltaFitness(),
	 *  1. aplicar la operación en _sol
	 *  2. Almacenar el fitness de la solución en _result (para las gráficas)
	 *  3. seleccionar una nueva operación
	 */
	while (operation.getDeltaFitness() > 0)
	{
		operation.apply(*this->_sol);
		this->_results.push_back(this->_sol->getFitness());
		chooseOperation(operation);
	}
}

void MQKPIteratedGreedy::destroy()
{

	/**
	 * TODO	DONE
	 * Recorrer los objetos y sacarlos de su mochila con probabilidad _alpha
	 */

	unsigned numObjs = _instance->getNumObjs();

	for (unsigned i = 0; i < numObjs; i++)
	{

		double randSample = ((double)(rand())) / RAND_MAX;

		if (randSample >= this->_alpha)
		{
			_sol->putObjectIn(i, 0);
		}
	}

	double fitness = MQKPEvaluator::computeFitness(*_instance, *_sol);
	_sol->setFitness(fitness);
	_results.push_back(_sol->getFitness());
}

void MQKPIteratedGreedy::initialise(double alpha, MQKPInstance &instance)
{
	_sol = new MQKPSolution(instance);
	_bestSolution = new MQKPSolution(instance);
	_bestSolution->copy(*_sol);
	_instance = &instance;
	_alpha = alpha;
}

void MQKPIteratedGreedy::run(MQKPStopCondition &stopCondition)
{

	if (_sol == NULL)
	{
		cerr << "IG was not initialised" << endl;
		exit(-1);
	}

	/** Crear la primera solución */
	rebuild();

	if (MQKPEvaluator::compare(_sol->getFitness(), _bestSolution->getFitness()) > 0)
		_bestSolution->copy(*_sol);

	/**
	 * TODO		DONE
	 * Mientras no se alcance la condición de parada
	 *  1. Destruir parcialmente la solución
	 *  2. Reconstruir la solución
	 *  3. Almacenar el nuevo fitness en _results para las gráficas
	 *  4. Actualizar la mejor solución y volver a ella si la nueva es peor
	 */

	while (stopCondition.reached() == false)
	{
		destroy();
		rebuild();
		this->_results.push_back(_sol->getFitness());

		if (MQKPEvaluator::compare(_sol->getFitness(), _bestSolution->getFitness()) > 0)
		{
			this->_bestSolution->copy(*this->_sol);
			this->_bestSolution->setFitness(this->_sol->getFitness());
		}
		else
		{
			this->_sol->copy(*this->_bestSolution);
		}

		stopCondition.notifyIteration();
	}
}
