// https://www.facebook.com/hackercup/problem/1523599254559737/
#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;

const bool DEBUG = false;

const char EMPTY = '.';
const char WALL = '#';
const char LEFT_TURRET = '<';
const char RIGHT_TURRET = '>';
const char UP_TURRET = '^';
const char DOWN_TURRET = 'v';
const char START = 'S';
const char GOAL = 'G';

const char LASER = '*';

enum moves_t {MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};
using maze_t = vector<vector<char>>;
using square_t = pair<int, int>;
using path_t = pair<square_t, vector<moves_t>>;


inline bool is_free(char c) {
    return c == EMPTY || c == LASER;
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
		    while (k >= 0 && is_free(m[i][k])) {
			m[i][k] = LASER;
			k--;
		    }
		    if (n < mazes.size() - 1)
			mazes[n + 1][i][j] = UP_TURRET;
		}
		// SHOOT RIGHT
		if (m[i][j] == RIGHT_TURRET) {
		    int k = j + 1;
		    while (k < m[0].size() && is_free(m[i][k])) {
			m[i][k] = LASER;
			k++;
		    }
		    if (n < mazes.size() - 1)
			mazes[n + 1][i][j] = DOWN_TURRET;
		}
		// SHOOT UP
		if (m[i][j] == UP_TURRET) {
		    int k = i - 1;
		    while (k >= 0 && is_free(m[k][j])) {
			m[k][j] = LASER;
			k--;
		    }
		    if (n < mazes.size() - 1)
			mazes[n + 1][i][j] = RIGHT_TURRET;
		}
		// SHOOT DOWN
		if (m[i][j] == DOWN_TURRET) {
		    int k = i + 1;
		    while (k < m.size() && is_free(m[k][j])) {
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

void print_maze(const maze_t& maze, const square_t& me, const square_t& goal) {
    for (size_t i = 0; i < maze.size(); i++) {
	for (size_t j = 0; j < maze[0].size(); j++) {
	    if (me.first == i && me.second == j)
		cout << 'X';
	    else if (goal.first == i && goal.second == j)
		cout << 'G';
	    else
		cout << maze[i][j];
	}
	cout << endl;
    }
    cout << endl;
}

void print_path(const maze_t& original_maze, const vector<maze_t>& mazes, const square_t& s, const square_t& g, const vector<moves_t>& moves) {
    print_maze(original_maze, s, g);
		
    square_t sq = s;
    for (int i = 0; i < moves.size(); i++) {
	cout << "MOVE " << moves[i] << endl;
		    
	switch (moves[i]) {
	case MOVE_UP:
	    sq.first--;
	    break;
	case MOVE_DOWN:
	    sq.first++;
	    break;
	case MOVE_LEFT:
	    sq.second--;
	    break;
	case MOVE_RIGHT:
	    sq.second++;
	    break;
	}

	print_maze(get_curr_maze(mazes, i + 1), sq, g);
    }
}

int find_shortest_path(const maze_t& original_maze, const square_t& s, const square_t& g) {
    vector<maze_t> mazes = get_mazes(original_maze);
    queue<path_t> to_visit;
    set<pair<square_t, int>> visited;

    to_visit.push({s, vector<moves_t>()});

    while (!to_visit.empty()) {
	path_t curr_path = to_visit.front();
	to_visit.pop();

	maze_t maze = get_curr_maze(mazes, curr_path.second.size());

	int curr_x = curr_path.first.first;
	int curr_y = curr_path.first.second;
	
	// ignore move if out of maze
	if (curr_x < 0 || curr_x >= maze.size() ||
	    curr_y < 0 || curr_y >= maze[0].size())
	    continue;

	// ignore move if blocked or dead
	if (maze[curr_x][curr_y] != EMPTY &&
	    curr_path.second.size()) // skips check on starting point
	    continue;
	
	// ignore move if state is already visited
	if (visited.count({curr_path.first, curr_path.second.size()%mazes.size()}))
	    continue;

	// set visited state
	visited.insert({curr_path.first, curr_path.second.size()%mazes.size()});

	// found goal
	if (curr_path.first == g) {
	    if (DEBUG) {
		print_path(original_maze, mazes, s, g, curr_path.second);
	    }

	    return curr_path.second.size();
	}

	// visit neighbours
	path_t move_up = {{curr_path.first.first - 1, curr_path.first.second}, curr_path.second};
	move_up.second.push_back(MOVE_UP);
	to_visit.push(move_up);
	
	path_t move_down = {{curr_path.first.first + 1, curr_path.first.second}, curr_path.second};
	move_down.second.push_back(MOVE_DOWN);
	to_visit.push(move_down);

	path_t move_left = {{curr_path.first.first, curr_path.first.second - 1}, curr_path.second};
	move_left.second.push_back(MOVE_LEFT);
	to_visit.push(move_left);

	path_t move_right = {{curr_path.first.first, curr_path.first.second + 1}, curr_path.second};
	move_right.second.push_back(MOVE_RIGHT);
	to_visit.push(move_right);
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

		if (c == START) { s = {i, j}; c = EMPTY; }
		if (c == GOAL) { g = {i, j}; c = EMPTY; }
	      		
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
