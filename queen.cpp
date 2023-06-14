#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

int n = 8;          
int p = 500;        
int maxGenerations = 0;

typedef std::vector<int> Chromosome;
typedef std::vector<Chromosome> Generation;

Generation currentGeneration;   
Generation newGeneration;       

Generation randomGeneration(int numberOfRows, int numberOfQueens) {
    Generation generationList;
    for (int i = 0; i < numberOfRows; ++i) {
        Chromosome gene;
        for (int j = 0; j < numberOfQueens; ++j) {
            gene.push_back((rand() % n) + 1);
        }
        gene.push_back(0);
        generationList.push_back(gene);
    }
    return generationList;
}

Generation fitness(Generation& populationList) {
    for (int i = 0; i < populationList.size(); ++i) {
        int conflict = 0;
        for (int j = 0; j < n; ++j) {
            for (int l = j + 1; l < n; ++l) {
                if (populationList[i][j] == populationList[i][l]) {
                    conflict++;
                }
                if (abs(j - l) == abs(populationList[i][j] - populationList[i][l])) {
                    conflict++;
                }
            }
        }
        populationList[i][populationList[i].size() - 1] = conflict;
    }

    for (int i = 0; i < populationList.size(); ++i) {
        int min = i;
        for (int j = i; j < populationList.size(); ++j) {
            if (populationList[j][n] < populationList[min][n]) {
                min = j;
            }
        }
        Chromosome temp = populationList[i];
        populationList[i] = populationList[min];
        populationList[min] = temp;
    }
    return populationList;
}

Generation crossover(Generation& generationList) {
    Generation newGenerationList;
    for (int i = 0; i < generationList.size(); i += 2) {
        Chromosome newKid1;
        Chromosome newKid2;
        for (int z = 0; z < n; ++z) {
            if (z < n / 2) {
                newKid1.push_back(generationList[i][z]);
                newKid2.push_back(generationList[i + 1][z]);
            } else {
                newKid1.push_back(generationList[i + 1][z]);
                newKid2.push_back(generationList[i][z]);
            }
        }
        newKid1.push_back(0);
        newKid2.push_back(0);
        newGenerationList.push_back(newKid1);
        newGenerationList.push_back(newKid2);
    }
    return newGenerationList;
}

bool findChrome(int val, std::vector<int> muted_list) {
    for(size_t i = 0; i < muted_list.size(); i++){
        if(muted_list[i] == val){
            return true;
        }
    }
    return false;
}

Generation mutation(Generation& generationList) {
    std::vector<int> mutedList;
    int i = 0;
    
    while (i < generationList.size() / 2) {
        int newRand = rand() % (generationList.size() / 2) + generationList.size() / 2;
        if (std::find(mutedList.begin(), mutedList.end(), newRand) == mutedList.end()) {
            mutedList.push_back(newRand);
            int randomIndex = rand() % n;
            generationList[newRand][randomIndex] = (rand() % (n - 1)) + 1;
            i++;
            //std::cout << i << std::endl;
        }
    }
    return generationList;
}

void showTable(Chromosome res) {
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            std::cout << ((res[j]-1) == k ? "Q " : "- ");
        }
        std::cout << std::endl;
    }
}

int main() {
    

    std::cout << "Qual o tamanho do tabuleiro desejado? (minimo 4)" << std::endl;
    std::cin >> n;

    if(n < 4) {
        std::cout << "Valor informado fora do escopo!" << std::endl;
        system("pause");
        return 1;
    }

    std::cout << "Qual a populacao maxima desejada? (default 500)" << std::endl;
    std::cin >> p;

    if(p <= 0) {
        p = 500;
    }

    std::cout << "Qual o maximo de geracoes desejada? (0 para sem limite)" << std::endl;
    std::cin >> maxGenerations;

    if(maxGenerations <= 0) {
        maxGenerations = 0;
    }

    srand(time(0));  // Initialize random seed

    currentGeneration = randomGeneration(p, n);
    currentGeneration = fitness(currentGeneration);

    int epoch = 1;
    bool max_declared = true;
    while (true && max_declared) {
        std::cout << "-------------------------------------------------------\n";
        std::cout << "Epoch " << epoch << "\n";

        currentGeneration.resize(p / 2);
        newGeneration = crossover(currentGeneration);
        newGeneration = mutation(newGeneration);
        currentGeneration = newGeneration;
        currentGeneration = fitness(currentGeneration);
        

        if (currentGeneration[0][n] == 0) {

            std::cout << "Solution Found: ";
            for (int i = 0; i < n; ++i) {
                std::cout << currentGeneration[0][i] << " ";
            }
            std::cout << "\n";
            showTable(currentGeneration[0]);
            break;
        } else {
            std::cout << "Best Solution: ";
            for (int i = 0; i < n; ++i) {
                std::cout << currentGeneration[0][i] << " ";
            }
            std::cout << "\n";
        }
        epoch++;
        if(maxGenerations && epoch > maxGenerations) {
            max_declared = false;
        }
    }

    system("pause");
    return 0;
}
