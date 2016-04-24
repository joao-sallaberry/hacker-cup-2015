#include <iostream>
#include <vector>
#include <queue>

using namespace std;

using maze_t = vector<vector<char>>;
using square_t = pair<int, int>;

const char EMPTY = '.';
const char WALL = '#';
const char LEFT_TURRET = '<';
const char RIGHT_TURRET = '>';
const char UP_TURRET = '^';
const char DOWN_TURRET = 'v';

const char LASER = '*';

void print_maze(const maze_t& maze) {
    for (const auto& r : maze) {
	for (const auto& c : r) {
	    cout << c;
	}
	cout << endl;
    }
    cout << endl;
}

vector<maze_t> get_mazes(const maze_t& original_maze) {
    vector<maze_t> mazes(4);

    // copy original 4 times
    for (auto& m : mazes) {
	m = original_maze;
    }

    // set 4 states
    for (size_t n = 0; n < mazes.size(); n++) {
	maze_t& m = mazes[n];

	for (int i = 0; i < m.size(); i++) {
	    for (int j = 0; j < m[0].size(); j++) {
		// SHOOT LEFT
		if (m[i][j] == LEFT_TURRET) {
		    int k = j - 1;
		    while (k >= 0 && m[i][k] == EMPTY) {
			m[i][k] = LASER;
			k--;
		    }
		    if (n < mazes.size() - 1)
			mazes[n + 1][i][j] = UP_TURRET;
		}
		// SHOOT RIGHT
		if (m[i][j] == RIGHT_TURRET) {
		    int k = j + 1;
		    while (k < m[0].size() && m[i][k] == EMPTY) {
			m[i][k] = LASER;
			k++;
		    }
		    if (n < mazes.size() - 1)
			mazes[n + 1][i][j] = DOWN_TURRET;
		}
		// SHOOT UP
		if (m[i][j] == UP_TURRET) {
		    int k = i - 1;
		    while (k >= 0 && m[k][j] == EMPTY) {
			m[k][j] = LASER;
			k--;
		    }
		    if (n < mazes.size() - 1)
			mazes[n + 1][i][j] = RIGHT_TURRET;
		}
		// SHOOT DOWN
		if (m[i][j] == DOWN_TURRET) {
		    int k = i + 1;
		    while (k < m.size() && m[k][j] == EMPTY) {
			m[k][j] = LASER;
			k++;
		    }
		    if (n < mazes.size() - 1)
			mazes[n + 1][i][j] = LEFT_TURRET;
		}
	    }
	}
	
	print_maze(m);
    }

    return mazes;
}

void find_shortest_path(const maze_t& maze, const square_t& s, const square_t& g) {
    vector<maze_t> mazes = get_mazes(maze);
    //queue<square_t>
}

int main() {
    int T;
    cin >> T;

    while (T) {
	int M, N;
	cin >> M >> N;

	maze_t maze(M, vector<char>(N));
	square_t s, g;
	
	for (int i = 0; i < M; i++) {
	    for (int j = 0; j < N; j++) {
		char c;
		cin >> c;

		if (c == 'S') { s = {i, j}; c = EMPTY; }
		if (c == 'G') { g = {i, j}; c = EMPTY; }
	      		
		maze[i][j] = c;
	    }
	}

	find_shortest_path(maze, s, g);

	T--;
    }
}
