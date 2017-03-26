#include "genetic.hpp"

void copyByOrder(const Solution& parent, Solution& sibling) {
    for (size_t i=0; i < parent.size(); ++i){
        auto job = parent[i];
        if(std::find(sibling.begin(), sibling.end(), job) == sibling.end()) {
            // Job not yet in the sibling
            // Find the earliest possible place
            auto pos = std::find(sibling.begin(), sibling.end(), -1) - sibling.begin();
            sibling[pos] = job;
        }
    }
}

std::vector<Solution> crossover(std::vector<Solution> population) {
    static std::mt19937 gen(42);

    auto next_population = std::vector<Solution>();

    while(population.size() != 0) {
        std::uniform_int_distribution<> dis_f(0, population.size() - 1);
        std::uniform_int_distribution<> dis_m(0, population.size() - 2);

        auto father_i = dis_f(gen);
        auto mother_i = dis_m(gen);

        const Solution father = population[father_i];
        population.erase(population.begin() + father_i);

        const Solution mother = population[mother_i];
        population.erase(population.begin() + mother_i);

        std::uniform_int_distribution<> dis_c(0, father.size() - 1);
        auto crossover_point = dis_c(gen);

        auto common = Solution(father.size());
        std::fill(common.begin(), common.end(), -1);

        // Fill the identical jobs
        for (size_t i=0; i < father.size(); ++i) {
            if(mother[i] == father[i]) {
                common[i] = mother[i];
            }
        }

        auto son = common;
        auto daughter = common;

        for (int i=0; i < crossover_point; ++i){
            son[i] = father[i];
            daughter[i] = mother[i];
        }

        copyByOrder(father, daughter);
        copyByOrder(mother, son);

        next_population.push_back(son);
        next_population.push_back(daughter);

    }

    return next_population;
}

Solution makeArtificial(std::vector<Solution> elites, const Instance& instance) {
    auto dominance = std::vector<std::vector<int>>();
    // Fill with 0
    for (int i=0; i < instance.getNbJob(); ++i){
        dominance.push_back(std::vector<int>(instance.getNbJob()));
    }

    for(size_t elite_pos=0; elite_pos < elites.size(); ++elite_pos) {
        auto sol = elites[elite_pos];
        for (int i=0; i < instance.getNbJob(); ++i) {
            auto j = sol[i];
            dominance[i][j] = dominance[i][j] + (N_ELITES - elite_pos);
        }
    }

    auto artificial = Solution(instance.getNbJob());
    std::fill(artificial.begin(), artificial.end(), -1);


    for (int i=0; i < instance.getNbJob(); ++i){
        size_t max_job = 0;
        size_t max_seq = 0;
        int max_val = 0;

        // Find the higher cell
        for (int job=0; job < instance.getNbJob(); ++job) {
            for (int seq=0; seq < instance.getNbJob(); ++seq) {
                if(dominance[job][seq] > max_val) {
                    max_val = dominance[job][seq];
                    max_job = job;
                    max_seq = seq;
                }
            }
        }

        artificial[max_seq] = max_job;

        // Fill line an col with -1
        std::fill(dominance[max_job].begin(), dominance[max_job].end(), -1);
        for (int j=0; j < instance.getNbJob(); ++j) {
            dominance[i][max_seq] = -1;
        }

    }

    return artificial;

}

std::vector<Solution> crossoverElite(std::vector<Solution> population, const Instance& instance) {
    std::stable_sort(
        population.begin(),
        population.end(),
        [&instance](const Solution &a, const Solution &b) -> bool{
            return instance.score(a) < instance.score(b);
        }
    );

    auto elites = std::vector<Solution>(population.begin(), population.begin() + N_ELITES);

    Solution artificial = makeArtificial(elites, instance);

    static std::mt19937 gen(42);

    auto next_population = std::vector<Solution>();

    while(population.size() != 0) {
        std::uniform_int_distribution<> dis_f(0, population.size() - 1);
        std::uniform_int_distribution<> dis_m(0, population.size() - 2);

        auto father_i = dis_f(gen);
        auto mother_i = dis_m(gen);

        const Solution father = population[father_i];
        population.erase(population.begin() + father_i);

        const Solution mother = population[mother_i];
        population.erase(population.begin() + mother_i);

        std::uniform_int_distribution<> dis_c(0, father.size() - 1);
        auto crossover_point = dis_c(gen);

        auto common = Solution(father.size());
        std::fill(common.begin(), common.end(), -1);

        // Fill the identical jobs
        for (size_t i=0; i < father.size(); ++i) {
            if(mother[i] == father[i]) {
                common[i] = mother[i];
            }
        }

        auto son = common;
        auto daughter = common;

        for (size_t i=0; i < father.size(); ++i) {
            if(artificial[i] == father[i]) {
                son[i] = artificial[i];
            }
            if(artificial[i] == mother[i]) {
                daughter[i] = artificial[i];
            }
        }

        for (int i=0; i < crossover_point; ++i){
            son[i] = father[i];
            daughter[i] = mother[i];
        }

        copyByOrder(father, daughter);
        copyByOrder(mother, son);

        next_population.push_back(son);
        next_population.push_back(daughter);

    }

    return next_population;
}



std::vector<Solution> selectBestPopulation(std::vector<Solution> population, const Instance& instance) {
    std::stable_sort(
        population.begin(),
        population.end(),
        [&instance](const Solution &a, const Solution &b) -> bool{
            return instance.score(a) < instance.score(b);
        }
    );
    return std::vector<Solution>(population.begin(), population.begin() + S);
}


std::vector<Solution> selectUniformPopulation(std::vector<Solution> population, const Instance& instance) {
    auto _ = instance; // ignore the instance param

    std::random_shuffle(population.begin(), population.end());
    return std::vector<Solution>(population.begin(), population.begin() + S);
}


std::vector<Solution> selectRoulettePopulation(std::vector<Solution> population, const Instance& instance) {
    static std::mt19937 gen(42);
    std::vector<int> weights;

    std::transform(
        population.begin(), population.end(), std::back_inserter(weights),
        [&instance](Solution x) {
            return instance.score(x);
        }
    );

    std::vector<Solution> selected;

    for (int i=0; i < S; ++i){
        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        auto indice = dist(gen);

        selected.push_back(population[indice]);
        population.erase(population.begin() + indice);
        weights.erase(weights.begin() + indice);

    }
    return selected;
}
