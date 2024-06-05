#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <algorithm> // For std::find, std::count

using namespace std;

// Hash function for pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

// Function to transfer similar numbers
void transferSimilarNumbers(vector<int>& from, vector<int>& to) {
    // Identify common elements
    unordered_map<int, int> fromCount, toCount;
    for (int num : from) {
        if (num != 0) fromCount[num]++;
    }
    for (int num : to) {
        if (num != 0) toCount[num]++;
    }

    // Transfer numbers based on their frequency in the vectors
    for (const auto& pair : fromCount) {
        int num = pair.first;
        int countFrom = pair.second;
        int countTo = toCount[num];

        if (countFrom > 0 && countTo > 0) {
            if (countTo > countFrom) {
                for (int& slot : from) {
                    if (slot == num) {
                        auto it_to = find(to.begin(), to.end(), 0); // Find an empty slot in "to"
                        if (it_to != to.end()) {
                            *it_to = num;
                            slot = 0;
                        }
                    }
                }
            } else if (countFrom > countTo) {
                for (int& slot : to) {
                    if (slot == num) {
                        auto it_from = find(from.begin(), from.end(), 0); // Find an empty slot in "from"
                        if (it_from != from.end()) {
                            *it_from = num;
                            slot = 0;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    // Initialize an unordered_map to store the vectors with coordinates as keys
    unordered_map<pair<int, int>, vector<int>, pair_hash> matrix;
    int x, y;
    srand(time(0));
    bool a = false;

    while (!a) { 
        // Accessing and printing elements
        cout << "Current matrix state:" << endl;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 4; ++j) {
                auto key = make_pair(i, j);
                cout << "[";
                if (matrix.find(key) != matrix.end()) {
                    for (int k = 0; k < 6; ++k) {
                        cout << matrix[key][k] << " ";
                    }
                } else {
                    for (int k = 0; k < 6; ++k) {
                        cout << "0 ";
                    }
                }
                cout << "] ";
            }
            cout << endl;
        }
        
        // Generate plate content
        vector<int> numbers(6);
        cout << "isi piring:" << endl;
        for(int i = 0; i < 6; ++i) {
            numbers[i] = rand() % 7;
            cout << numbers[i] << " ";
        }
        cout << "\nMasukkan ke piring yang kamu pilih (format: x y)" << endl;
        cin >> x >> y;

        // Check if the coordinates are valid
        if(x < 0 || x >= 5 || y < 0 || y >= 4) {
            cout << "Koordinat tidak valid." << endl;
            continue; // Skip the rest of the loop and start the next iteration
        }
        matrix[make_pair(x, y)] = numbers;

        // Check neighbors (top, bottom, left, right)
        cout << "Neighbors of (" << x << ", " << y << "):" << endl;
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (nx >= 0 && nx < 5 && ny >= 0 && ny < 4) {
                auto neighbor_key = make_pair(nx, ny);
                cout << "Neighbor (" << nx << ", " << ny << "): ";
                if (matrix.find(neighbor_key) != matrix.end()) {
                    for (int k = 0; k < 6; ++k) {
                        cout << matrix[neighbor_key][k] << " ";
                    }
                    cout << endl;

                    // Perform swapping if current plate has empty slots
                    if (find(numbers.begin(), numbers.end(), 0) != numbers.end()) {
                        transferSimilarNumbers(matrix[neighbor_key], numbers);
                    }

                    // Transfer similar numbers if common elements exist
                    transferSimilarNumbers(matrix[neighbor_key], numbers);
                } else {
                    cout << "Empty";
                }
                cout << endl;
            }
        }
        matrix[make_pair(x, y)] = numbers; // Update the current plate with swapped values
    }
    return 0;
}