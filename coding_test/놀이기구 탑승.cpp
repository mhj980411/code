#include <iostream>
#include <unordered_map>
#include <cmath>

using namespace std;

#define X first
#define Y second

int dx[] = { 1,0,-1,0 };
int dy[] = { 0,1,0,-1 };

int n;

int board[20][20];
int student_info[401][401];
unordered_map<int,int> student_num_index_um;

void Input() {
	cin >> n;

	int num;
	for (int i = 0; i < n * n; i++) {
		cin >> num;
		student_info[i][0] = num;
		student_num_index_um.insert({ num,i });
		
		for (int j = 0; j < 4; j++) {
			cin >> num;
			student_info[i][num]++;
		}
	}
}

void find_square(int index, int student_num) {
	int max_friend_cnt = -1;
	int max_empty_square_cnt = -1;
	pair<int, int> select_loc;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] > 0) continue;
			int friend_cnt = 0;
			int empty_square_cnt = 0;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];

				if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
				if (board[nx][ny] != 0 && student_info[index][board[nx][ny]] == 1) friend_cnt++;
				if (board[nx][ny] == 0) empty_square_cnt++;
			}

			if (max_friend_cnt == -1) {
				max_friend_cnt = friend_cnt;
				max_empty_square_cnt = empty_square_cnt;
				select_loc = { i,j };
			}

			if (friend_cnt > max_friend_cnt) {
				max_friend_cnt = friend_cnt;
				max_empty_square_cnt = empty_square_cnt;
				select_loc = { i,j };
			}
			else if (friend_cnt == max_friend_cnt) {
				if (empty_square_cnt > max_empty_square_cnt) {
					max_empty_square_cnt = empty_square_cnt;
					select_loc = { i,j };
				}
			}
		}
	}
	board[select_loc.X][select_loc.Y] = student_num;
}

int calc_score() {
	int ret = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int student_num = board[i][j];
			int index = -1;
			if (student_num_index_um.find(student_num) != student_num_index_um.end()) {
				index = student_num_index_um[student_num];
			}
			int friend_cnt = 0;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];
				if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
				if (board[nx][ny] != 0 && student_info[index][board[nx][ny]] == 1) friend_cnt++;
			}
			//cout << "index : " << index << " ";
			if (friend_cnt > 0) {
				friend_cnt--;
				//cout << pow(10, friend_cnt) << '\n';
				ret += pow(10, friend_cnt);
			}
			/*else
				cout << 0 << '\n';*/
		}
	}
	return ret;
}

void simulation() {
	for (int i = 0; i < n * n; i++) {
		int num = student_info[i][0];
		find_square(i, num);
		int de = 1;
	}
	cout << calc_score();
	int de = 2;
}

int main(void) {
	Input();
	simulation();

	return 0;
}