#include <iostream>
#include <vector>
#include <random>

int main() {
    int n, m;
    std::cout << "Podaj liczbe wierszy (n): ";
    std::cin >> n;
    std::cout << "Podaj liczbe kolumn (m): ";
    std::cin >> m;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::discrete_distribution<> dist({0.7, 0.3});

    std::vector<std::vector<int>> macierz(n, std::vector<int>(m));


    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            macierz[i][j] = dist(gen); // 0 lub 1
        }
    }

    std::cout << "Wygenerowana macierz:\n";
    for (const auto& wiersz : macierz) {
        for (int val : wiersz) {
            std::cout << val;
        }
        std::cout << "\n";
    }

    return 0;
}