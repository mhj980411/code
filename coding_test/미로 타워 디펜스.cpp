/*
18:02
*/

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define X first
#define Y second

int dx[] = { 0,1,0,-1 };
int dy[] = { 1,0,-1,0 };

int n, m;
int board[25][25];
vector<pair<int, int>> attack_dir_dist_v;
int answer;

void Input() {
	cin >> n >> m;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> board[i][j];
		}
	}

	pair<int, int> attack_dir_dist;
	for (int i = 0; i < m; i++) {
		cin >> attack_dir_dist.first >> attack_dir_dist.second;
		attack_dir_dist_v.push_back(attack_dir_dist);
	}
}

void player_attack(pair<int, int> attack_dir_dist) {
	
	int nx = n / 2, ny = n / 2;
	for (int i = 0; i < attack_dir_dist.second; i++) {
		nx += dx[attack_dir_dist.first];
		ny += dy[attack_dir_dist.first];
		if (nx < 0 || ny < 0 || nx >= n || ny >= n) break;
		if (board[nx][ny] == 0) break;
		answer += board[nx][ny];
		board[nx][ny] = 0;
	}
}

vector<int> refill_process() {
	vector<int> monster_v;

	int nx = n / 2, ny = n / 2;
	int dir = 2;
	int dist = 1;
	int cur_dist = 0;
	int ch_dir_cnt = 0;

	while (true) {
		nx += dx[dir];
		ny += dy[dir];
		cur_dist++;

		if (board[nx][ny] > 0) {
			monster_v.push_back(board[nx][ny]);
		}

		if (cur_dist == dist) {
			dir = ((dir - 1) + 4) % 4;
			cur_dist = 0;
			ch_dir_cnt++;
		}

		if (ch_dir_cnt == 2) {
			dist++;
			ch_dir_cnt = 0;
		}

		if (nx == 0 && ny == 0) break;
	}

	while (true) {
		vector<int> remove_result_monster_v;
		bool remove_flag = false;
		int cur_monster_num = -1;
		vector<int> cur_monster_v;
		for (int i = 0; i < monster_v.size(); i++) {
			if (monster_v[i] != cur_monster_num) {

				if (cur_monster_v.size() >= 4) {
					answer += (cur_monster_num * cur_monster_v.size());
					remove_flag = true;
				}
				else {
					for (int k = 0; k < cur_monster_v.size(); k++) {
						remove_result_monster_v.push_back(cur_monster_v[k]);
					}
				}

				cur_monster_v.clear();

				cur_monster_num = monster_v[i];
				cur_monster_v.push_back(monster_v[i]);
			}
			else {
				cur_monster_v.push_back(monster_v[i]);
			}
		}

		if (cur_monster_v.size() >= 4) {
			answer += (cur_monster_num * cur_monster_v.size());
			remove_flag = true;
		}
		else {
			for (int k = 0; k < cur_monster_v.size(); k++) {
				remove_result_monster_v.push_back(cur_monster_v[k]);
			}
		}

		monster_v = remove_result_monster_v;

		if (remove_flag == false) break;
	}
	return monster_v;
}

void refill_board(vector<int>& monster_v) {
	memset(board, 0, sizeof(board));
	vector<int> refill_v;

	int cur_monster_num = -1;
	int cur_monster_cnt = 0;
	for (int i = 0; i < monster_v.size(); i++) {
		if (monster_v[i] != cur_monster_num) {
			if (cur_monster_cnt > 0) {
				refill_v.push_back(cur_monster_cnt);
				refill_v.push_back(cur_monster_num);
			}
			cur_monster_num = monster_v[i];
			cur_monster_cnt = 1;
		}
		else {
			cur_monster_cnt++;
		}
	}
	refill_v.push_back(cur_monster_cnt);
	refill_v.push_back(cur_monster_num);

	int nx = n / 2, ny = n / 2;
	int dir = 2;
	int dist = 1;
	int cur_dist = 0;
	int ch_dir_cnt = 0;
	int iter = 0;

	while (true) {
		nx += dx[dir];
		ny += dy[dir];
		cur_dist++;

		if (iter >= refill_v.size()) break;
		board[nx][ny] = refill_v[iter];
		iter++;

		if (cur_dist == dist) {
			dir = ((dir - 1) + 4) % 4;
			cur_dist = 0;
			ch_dir_cnt++;
		}

		if (ch_dir_cnt == 2) {
			dist++;
			ch_dir_cnt = 0;
		}

		if (nx == 0 && ny == 0) break;
	}
}

void simulation() {
	for (int i = 0; i < m; i++) {
		player_attack(attack_dir_dist_v[i]);
		int de = 1;
		vector<int> monster_v = refill_process();
		de = 2;
		refill_board(monster_v);
		de = 3;
	}
	cout << answer;
}

int main(void) {
	Input();
	simulation();

	return 0;
}