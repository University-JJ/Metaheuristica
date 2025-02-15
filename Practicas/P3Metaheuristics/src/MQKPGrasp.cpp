/*
 * MQKPGrasp.cpp
 *
 * Fichero que define las funciones de la clase MQKPGrasp. Forma parte del código esqueleto para el problema de las múltiples mochilas cuadráticas, ofrecido para las prácticas de la asignatura Metaheurísticas del Grado de Ingeniería Informática de la Universidad de Córdoba
 *
 * @author Carlos García cgarcia@uco.es
 */

#include "MQKPGrasp.h"
#include "MQKPSimpleFirstImprovementNO.h"
#include "MQKPLocalSearch.h"
#include <iostream>
#include <vector>

using namespace std;

void MQKPGrasp::chooseOperation(MQKPObjectAssignmentOperation &operation)
{

	int bestObj = 0;
	int bestKnapsack = 0;
	double bestDensity = 0;
	double bestDeltaFitness = 0;
	bool initialisedBestDensity = false;
	unsigned numObjs = _instance->getNumObjs();
	unsigned numKnapsacks = _instance->getNumKnapsacks();

	/**
	 * Calcular el número de intentos como el porcentaje _alpha por el número de posibilidades, que es el número de objetos por el número de mochilas.
	 *
	 * En este paso no se considerará que ya haya objetos asignados a alguna mochila, la mochila 0 que representa objetos sin asignar a ninguna mochila, ni que haya mochilas ya completamente llenas
	 */
	unsigned numTries = ((unsigned)(numObjs * numKnapsacks * _alpha));

	/**
	 * TODO	DONE
	 * Generar alternativas de objeto y mochila aleatorias (no se considera la mochila 0) y quedarse con la alterantiva de mejor densidad
	 */
	for (unsigned i = 0; i < numTries; i++)
	{
		int indexObj = rand() % numObjs;
		int indexKnapsack = (rand() % numKnapsacks) + 1;

		double deltaFitness = MQKPEvaluator::computeDeltaFitness(*this->_instance, *this->_sol, indexObj, indexKnapsack); //TODO obtener la mejora en fitness de dicha operación 	DONE
		double density = deltaFitness / this->_instance->getWeight(indexObj);											  //TODO calcular la densidad de dicha operación como la diferencia en fitness dividido por el peso del objeto	DONE

		//TODO actualizar si resulta ser la mejor
		if (density > bestDensity || initialisedBestDensity == false)
		{
			initialisedBestDensity = true;
			bestDensity = density;
			bestObj = indexObj;
			bestKnapsack = indexKnapsack;
			bestDeltaFitness = deltaFitness;
		}
	}

	operation.setValues(bestObj, bestKnapsack,
						bestDeltaFitness);
}

void MQKPGrasp::buildInitialSolution()
{

	/**
	 * TODO	DONE
	 * Vaciar la solución _sol asignándole un fitness de 0 y poniendo todos los objetos en la mochila 0
	 */
	unsigned numObjs = _instance->getNumObjs();

	_sol->setFitness(0);
	for (unsigned i = 0; i < numObjs; i++)
	{
		this->_sol->putObjectIn(i, 0); // Set all sacks to 0
	}

	/** Seleccionar la primera operación */
	MQKPObjectAssignmentOperation operation;
	chooseOperation(operation);

	/**
	 * TODO	DONE
	 * Mientras la operación tenga un incremento de fitness positivo, operation.getDeltaFitness(),
	 *  1. aplicar la operación en _sol
	 *  2. Almacenar el fitness de la solución en _result (para las gráficas)
	 *  3. seleccionar una nueva operación
	 */
	while (operation.getDeltaFitness() > 0)
	{
		operation.apply(*this->_sol);						// Apply the operation into the private variable _sol
		this->_results.push_back(this->_sol->getFitness()); // Keep track of the solutions fitness
		chooseOperation(operation);
	}
}

void MQKPGrasp::initialise(double alpha, MQKPInstance &instance)
{

	_sol = new MQKPSolution(instance);
	_bestSolution = new MQKPSolution(instance);
	_bestSolution->copy(*_sol);
	_instance = &instance;
	_alpha = alpha;
}

void MQKPGrasp::run(MQKPStopCondition &stopCondition)
{

	if (_sol == NULL)
	{
		cerr << "GRASP was not initialised" << endl;
		exit(-1);
	}

	/**
	 * TODO	DONE
	 * Mientras no se alcance el criterio de parada
	 *   1. Generar una solución inicial invocando al método correspondiente
	 *   2. Almacenar el fitness de la solución en _results
	 *   3. Optimizar _sol con la búsqueda local y el operador de vecindario de la metaheurística
	 *   4. Actualizar la mejor solución
	 */
	while (stopCondition.reached() == false)
	{
		buildInitialSolution();
		this->_results.push_back(_sol->getFitness());
		this->_ls.optimise(*this->_instance, this->_no, *this->_sol);

		vector<double> &auxResults = _ls.getResults();

		for (auto result : auxResults)
		{
			_results.push_back(result);
		}

		if (MQKPEvaluator::compare(_sol->getFitness(), _bestSolution->getFitness()) > 0)
			_bestSolution->copy(*_sol);

		stopCondition.notifyIteration();
	}
}
