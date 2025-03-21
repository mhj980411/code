#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 0,-1,-1,-1,0,1,1,1 };
int dy[] = { 1,1,0,-1,-1,-1,0,1 };

int n, m;
vector<pair<int, int>> move_rule_v;
vector<pair<int, int>> special_nutrient_loc_v;
int board[15][15];
bool visited[15][15];

void Input() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}
	pair<int, int> move_rule;
	for (int i = 0; i < m; i++) {
		cin >> move_rule.first >> move_rule.second;
		move_rule.first--;
		move_rule_v.push_back(move_rule);
	}

	for (int i = n - 2; i < n; i++) {
		for (int j = 0; j < 2; j++) {
			special_nutrient_loc_v.push_back({ i,j });
		}
	}
}

void move_nutrient(pair<int, int> move_rule) {
	for (int i = 0; i < special_nutrient_loc_v.size(); i++) {
		pair<int, int> special_nutrient_loc = special_nutrient_loc_v[i];
		int nx = (special_nutrient_loc.X + 
			(dx[move_rule.first] * move_rule.second) + n * move_rule.second) % n;
		int ny = (special_nutrient_loc.Y +
			(dy[move_rule.first] * move_rule.second) + n * move_rule.second) % n;

		special_nutrient_loc_v[i] = { nx,ny };
	}
}

void grow_nutrient_loc() {
	memset(visited, 0, sizeof(visited));
	for (int i = 0; i < special_nutrient_loc_v.size(); i++) {
		pair<int, int> special_nutrient_loc = special_nutrient_loc_v[i];
		board[special_nutrient_loc.X][special_nutrient_loc.Y]++;
		visited[special_nutrient_loc.X][special_nutrient_loc.Y] = 1;
	}
}

void grow_diagonol() {
	for (int i = 0; i < special_nutrient_loc_v.size(); i++) {
		pair<int, int> special_nutrient_loc = special_nutrient_loc_v[i];
		
		for (int dir = 1; dir < 8; dir += 2) {
			int nx = special_nutrient_loc.X + dx[dir];
			int ny = special_nutrient_loc.Y + dy[dir];
			if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
			if (board[nx][ny] >= 1) {
				board[special_nutrient_loc.X][special_nutrient_loc.Y]++;
			}
		}
	}
	special_nutrient_loc_v.clear();
}

void buy_nutrient() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (visited[i][j] == 0 && board[i][j] >= 2) {
				board[i][j] -= 2;
				special_nutrient_loc_v.push_back({ i,j });
			}
		}
	}
}

void simulation() {
	for (int i = 0; i < m; i++) {
		move_nutrient(move_rule_v[i]);
		grow_nutrient_loc();
		grow_diagonol();
		buy_nutrient();
	}
	int answer = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			answer += board[i][j];
		}
	}
	cout << answer;
}

int main(void) {
	Input();
	simulation();
	return 0;
}