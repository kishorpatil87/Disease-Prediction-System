#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

const vector<vector<int>> GOAL_STATE = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

struct Node {
    vector<vector<int>> state;
    string stateStr;
    int g, h, f;
    Node* parent;

    Node(vector<vector<int>> s, Node* p, int cost) : state(s), parent(p), g(cost) {
        h = calculateHeuristic();
        f = g + h;
        stateStr = convertStateToString();
    }

    int calculateHeuristic() {
        int distance = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int value = state[i][j];
                if (value != 0) {
                    int targetRow = (value - 1) / 3;
                    int targetCol = (value - 1) % 3;
                    distance += abs(i - targetRow) + abs(j - targetCol);
                }
            }
        }
        return distance;
    }

    string convertStateToString() {
        string s;
        for (const auto& row : state) {
            for (int num : row) {
                s += to_string(num);
            }
        }
        return s;
    }
};

struct CompareNodes {
    bool operator()(Node* a, Node* b) {
        return a->f > b->f;
    }
};

vector<Node*> getChildren(Node* node) {
    vector<Node*> children;
    vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int row, col;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (node->state[i][j] == 0) {
                row = i;
                col = j;
            }
        }
    }

    for (auto dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {
            vector<vector<int>> newState = node->state;
            swap(newState[row][col], newState[newRow][newCol]);
            children.push_back(new Node(newState, node, node->g + 1));
        }
    }

    return children;
}

vector<Node*> aStar(Node* initialNode) {
    priority_queue<Node*, vector<Node*>, CompareNodes> openList;
    unordered_set<string> closedList;
    
    openList.push(initialNode);
    string goalStateStr = Node(GOAL_STATE, nullptr, 0).stateStr;
    
    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();
        
        if (current->stateStr == goalStateStr) {
            vector<Node*> path;
            while (current) {
                path.insert(path.begin(), current);
                current = current->parent;
            }
            return path;
        }

        if (closedList.count(current->stateStr)) continue;
        closedList.insert(current->stateStr);
        
        vector<Node*> children = getChildren(current);
        for (Node* child : children) {
            if (!closedList.count(child->stateStr)) {
                openList.push(child);
            }
        }
    }
    return {};
}

void printState(const vector<vector<int>>& state) {
    for (const auto& row : state) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> initialState = {{2, 8, 3}, {1, 6, 4}, {7, 0, 5}};
    Node* initialNode = new Node(initialState, nullptr, 0);
    vector<Node*> path = aStar(initialNode);
    
    if (!path.empty()) {
        cout << "Solution found in " << path.size() - 1 << " moves:" << endl;
        for (Node* node : path) {
            printState(node->state);
            cout << endl;
        }
    } else {
        cout << "No solution found." << endl;
    }
    return 0;
}
