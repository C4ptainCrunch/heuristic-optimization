#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <string>
#include <vector>

typedef std::vector<int> Solution;

class Instance {
  private:
    int nbJob;
    int nbMac;
    std::vector< long int > dueDates;
    std::vector< long int > priority;

    std::vector< std::vector <long int> > processingTimesMatrix;

    void resizeMatrixMemory(int nbJ, int nbM);

  public:
    Instance(char const *fileName);
    ~Instance();

    /* Read write privates attributs : */
    int getNbJob() const;
    int getNbMac() const;

    int getWeight(int job) const;
    int getProcessingTime(int job, int machine) const;
    int getTotalProcessingTime(int job) const;
    float weightedProcessingTime(int job) const;


    /* Read\Write values in the matrix : */
    long int getTime(int job, int machine) const;
    void setTime(int job, int machine, long int processTime);

    long int getDueDate(int job) const;
    void setDueDate(int job, int value);

    long int getPriority(int job) const;
    void setPriority(int job, int value);

    int score(const Solution &sol) const;
};

#endif
