#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "instance.hpp"

using namespace std;

Instance::Instance(char const *fileName) {
    string placeholder; // use this var to ignore values
    ifstream fileIn;

    fileIn.open(fileName);

    if (!fileIn.is_open() ) {
        throw std::exception();
        // TODO : better exception
    }

    fileIn >> nbJob;
    fileIn >> nbMac;
    resizeMatrixMemory(nbJob, nbMac);

    for (int j=0; j < nbJob; ++j) {
        for (int m=0; m < nbMac; ++m) {
            fileIn >> placeholder; // The number of each machine, not important !
            fileIn >> processingTimesMatrix[j][m]; // Process Time
        }
    }

    fileIn >> placeholder; // Skip the "Reldue" line and ignore it

    for (int j=0; j < nbJob; ++j) {
        fileIn >> placeholder; // -1
        fileIn >> dueDates[j];
        fileIn >> placeholder; // -1
        fileIn >> priority[j];
    }

    fileIn.close();
}


Instance::~Instance() {}

int Instance::getNbJob() const{
    return nbJob;
}

int Instance::getNbMac() const{
    return nbMac;
}

int Instance::getWeight(int job) const{
    return priority[job];
}

int Instance::getProcessingTime(int job, int machine) const{
    return processingTimesMatrix[job][machine];
}

int Instance::getTotalProcessingTime(int job) const{
    int total = 0;

    for (int i=0; i < getNbMac(); ++i){
        total = total + getProcessingTime(job, i);
    }

    return total;
}

float Instance::weightedProcessingTime(int job) const{
    return (float) getTotalProcessingTime(job) / (float) getWeight(job);
}

void Instance::resizeMatrixMemory(int nbJ, int nbM) {
    processingTimesMatrix.resize(nbJ);          // 1st dimension

    for (int cpt=0; cpt < nbJ; ++cpt) {
        processingTimesMatrix[cpt].resize(nbM); // 2nd dimension
    }

    dueDates.resize(nbJ);
    priority.resize(nbJ);
}

/* Compute the weighted tardiness of a given solution */
int Instance::score(const Solution &sol) const{
    auto completionTimes = std::vector<int>(sol.size());

    int lastJobEnd = 0;
    for (size_t job_indice=0; job_indice < sol.size(); ++job_indice) {
        int job_id = sol[job_indice];
        lastJobEnd = lastJobEnd + getProcessingTime(job_id, 0);
        completionTimes[job_indice] = lastJobEnd;
    }

    for (int machine_id=1; machine_id < getNbMac(); ++machine_id) {
        for (size_t job_indice=0; job_indice < sol.size(); ++job_indice) {
            int job_id = sol[job_indice];
            completionTimes[job_indice] = std::max(
                completionTimes[job_indice],
                (job_indice == 0) ? 0 : completionTimes[job_indice - 1]
            ) + getProcessingTime(job_id, machine_id);
        }
    }

    int score = 0;

    for(size_t job_indice=0; job_indice < sol.size(); ++job_indice) {
        int job_id = sol[job_indice];
        score = score + (completionTimes[job_indice] * priority[job_id]);
    }

    return score;
}
