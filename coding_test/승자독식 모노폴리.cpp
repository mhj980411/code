/*
* 22:32
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define X first
#define Y second

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1 };

int n, m, k;

int board[20][20];
pair<int, int> contract_board[20][20];
vector<vector<vector<int>>> dir_priority;

struct player {
	int x;
	int y;
	int num;
	int dir;
};

struct cmp {
	bool operator()(player left, player right) {
		if (left.num < right.num)
			return false;
		if (left.num > right.num)
			return true;
		return false;
	}
};

bool v_cmp(player left, player right) {
	if (left.num < right.num)
		return true;
	if (left.num > right.num)
		return false;
	return false;
}

priority_queue<player,vector<player>,cmp> player_board[20][20];
queue<player> player_info_q;
vector<player> input_player_v;

void Input() {
	cin >> n >> m >> k;
	
	int num;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> num;
			if (num > 0) {
				player p = { i,j,num };
				input_player_v.push_back(p);
			}
		}
	}
	sort(input_player_v.begin(), input_player_v.end(), v_cmp);
	
	for (int i = 0; i < m; i++) {
		player p = input_player_v[i];
		cin >> p.dir;
		player_info_q.push(p);
		player_board[p.x][p.y].push(p);
		contract_board[p.x][p.y] = { p.num,k};
	}

	for (int i = 0; i < m; i++) {
		vector<vector<int>> dir_v_v;
		for (int j = 0; j < 4; j++) {
			vector<int> dir_v;
			int dir;
			for (int k = 0; k < 4; k++) {
				cin >> dir;
				dir_v.push_back(dir);
			}
			dir_v_v.push_back(dir_v);
		}
		dir_priority.push_back(dir_v_v);
	}
}

void reduce_time() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (contract_board[i][j].second > 0 && player_board[i][j].size() == 0) {
				contract_board[i][j].second--;
				if (contract_board[i][j].second == 0)
					contract_board[i][j].first = 0;
			}
		}
	}
}

void player_move() {
	int size = player_info_q.size();
	for (int i = 0; i < size; i++) {
		player p = player_info_q.front();	player_info_q.pop();
		player_board[p.x][p.y].pop();

		bool find_contract_board = false;

		for (int j = 0; j < 4; j++) {
			int dir = dir_priority[p.num - 1][p.dir - 1][j] - 1;
			int nx = p.x + dx[dir];
			int ny = p.y + dy[dir];
			if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
			if (contract_board[nx][ny].first == 0) {
				find_contract_board = true;
				p.x = nx;	p.y = ny;
				p.dir = dir + 1;
				break;
			}
		}

		if (find_contract_board == false) {
			for (int j = 0; j < 4; j++) {
				int dir = dir_priority[p.num - 1][p.dir - 1][j] - 1;
				int nx = p.x + dx[dir];
				int ny = p.y + dy[dir];
				if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
				if (contract_board[nx][ny].first == p.num) {
					p.x = nx;	p.y = ny;
					p.dir = dir + 1;
					break;
				}
			}
		}

		player_info_q.push(p);
		player_board[p.x][p.y].push(p);
	}
}

void contract() {
	int size = player_info_q.size();

	for (int i = 0; i < size; i++) {
		player p = player_info_q.front();	player_info_q.pop();

		if (player_board[p.x][p.y].size() >= 1 && p.num == player_board[p.x][p.y].top().num) {
			contract_board[p.x][p.y] = { p.num,k};
			player_info_q.push(p);
			int remain_size = player_board[p.x][p.y].size();
			for (int j = 0; j < remain_size; j++) {
				player_board[p.x][p.y].pop();
			}
			player_board[p.x][p.y].push(p);
		}
		
		
	}
}

void print_situation() {
	cout << "player_loc" << '\n';
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (player_board[i][j].size() == 0) {
				cout << 0 << " ";
			}
			else if (player_board[i][j].size() >= 1) {
				cout << player_board[i][j].top().num << " ";
			}
		}
		cout << '\n';
	}
	cout << '\n';
}

void simulation() {
	int turn = 1;
	while (true) {
		player_move();
		/*print_situation();
		int de = 1;*/
		contract();
		reduce_time();
		if (player_info_q.size() == 1) break;
		turn++;
		if (turn >= 1000) {
			turn = -1;
			break;
		}
	}
	cout << turn;
}

int main(void) {
	Input();
	simulation();
	return 0;
}