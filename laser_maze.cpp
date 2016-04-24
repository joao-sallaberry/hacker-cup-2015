#include <iostream>
#include <vector>
#include <queue>
#include <set>

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

	for (size_t i = 0; i < m.size(); i++) {
	    for (size_t j = 0; j < m[0].size(); j++) {
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

	//print_maze(m);
    }

    return mazes;
}

maze_t get_curr_maze(const vector<maze_t>& mazes, int round) {
    return mazes[round%mazes.size()];
}

void print_me(const maze_t& maze, const square_t& me) {
    for (size_t i = 0; i < maze.size(); i++) {
	for (size_t j = 0; j < maze[0].size(); j++) {
	    if (me.first == i && me.second == j)
		cout << 'X';
	    else
		cout << maze[i][j];
	}
	cout << endl;
    }
    cout << endl;
}

int find_shortest_path(const maze_t& original_maze, const square_t& s, const square_t& g) {
    vector<maze_t> mazes = get_mazes(original_maze);
    queue<pair<square_t, int>> to_visit;
    set<pair<square_t, int>> visited;

    to_visit.push({s, 0});

    while (!to_visit.empty()) {
	pair<square_t, int> curr_sq = to_visit.front();
	to_visit.pop();

	maze_t maze = get_curr_maze(mazes, curr_sq.second);

	// is move possible?
	int curr_x = curr_sq.first.first;
	int curr_y = curr_sq.first.second;
	if (curr_x < 0 || curr_x >= maze.size() ||
	    curr_y < 0 || curr_y >= maze[0].size() ||
	    visited.count({curr_sq.first, curr_sq.second%mazes.size()}) ||
	    (maze[curr_x][curr_y] != EMPTY &&
	     curr_sq.first != s)) // skips check on starting point
	    continue;

	// set visited
	visited.insert({curr_sq.first, curr_sq.second%mazes.size()});
	
	//print_me(maze, curr_sq.first);

	// found goal
	if (curr_sq.first == g)
	    return curr_sq.second;

	// visit neighbours
	to_visit.push({{curr_sq.first.first + 1, curr_sq.first.second}, curr_sq.second + 1});
	to_visit.push({{curr_sq.first.first, curr_sq.first.second + 1}, curr_sq.second + 1});
	to_visit.push({{curr_sq.first.first - 1, curr_sq.first.second}, curr_sq.second + 1});
	to_visit.push({{curr_sq.first.first, curr_sq.first.second - 1}, curr_sq.second + 1});
    }

    return 0;
}

int main() {
    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
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

	int shortest = find_shortest_path(maze, s, g);
	cout << "Case #" << t << ": ";
	if (shortest)
	    cout << shortest;
	else
	    cout << "impossible";
	cout << endl;
    }
}
