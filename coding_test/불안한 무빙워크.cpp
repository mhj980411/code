#include <iostream>
#include <deque>

using namespace std;

int n, k;

deque<pair<int, bool>> movingwork_dq; // 안정성, 사람이 있는지 없는지 표현
deque<int> person_index_dq;
int answer;

void Input() {
	cin >> n >> k;

	int stable;
	for (int i = 0; i < 2 * n; i++) {
		cin >> stable;
		movingwork_dq.push_back({ stable,false });
	}
}

void simulation() {
	int time = 1;
	while (true) {
		// 무빙워크 한칸 회전
		pair<int, bool> value = movingwork_dq.back(); movingwork_dq.pop_back();
		movingwork_dq.push_front(value);
		// 무빙워크로 인한 index 변경 처리
		int size = person_index_dq.size();
		for (int i = 0; i < size; i++) {
			int person_index = person_index_dq.front();	person_index_dq.pop_front();
			person_index++;
			movingwork_dq[person_index].second = false;
			if (person_index != n - 1) {
				person_index_dq.push_back(person_index);
				movingwork_dq[person_index].second = true;
			}
		}

		// 가장 먼저 무빙워크에 올라간 사람부터 회전하는 방향으로 한칸 이동
		size = person_index_dq.size();
		for (int i = 0; i < size; i++) {
			int person_index = person_index_dq.front();	person_index_dq.pop_front();
			movingwork_dq[person_index].second = false;

			if (!(movingwork_dq[person_index + 1].second == true || movingwork_dq[person_index + 1].first == 0)) {
				person_index++;
				movingwork_dq[person_index].first--;
				if (movingwork_dq[person_index].first == 0) {
					answer++;
				}
			}

			if (person_index != n - 1) {
				person_index_dq.push_back(person_index);
				movingwork_dq[person_index].second = true;
			}

		}

		// 1번칸에 사람이 없고 안정성이 0이 아니라면 사람을 한명 더 올린다.
		if (movingwork_dq[0].first != 0 && movingwork_dq[0].second == false) {
			movingwork_dq[0].first--;
			if (movingwork_dq[0].first == 0)
				answer++;
			movingwork_dq[0].second = true;
			person_index_dq.push_back(0);
		}

		if (answer >= k)
			break;
		time++;
	}
	cout << time;
}

int main(void) {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	Input();
	simulation();
	return 0;
}

