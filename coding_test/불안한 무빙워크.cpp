#include <iostream>
#include <deque>

using namespace std;

int n, k;

deque<pair<int, bool>> movingwork_dq; // ������, ����� �ִ��� ������ ǥ��
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
		// ������ũ ��ĭ ȸ��
		pair<int, bool> value = movingwork_dq.back(); movingwork_dq.pop_back();
		movingwork_dq.push_front(value);
		// ������ũ�� ���� index ���� ó��
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

		// ���� ���� ������ũ�� �ö� ������� ȸ���ϴ� �������� ��ĭ �̵�
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

		// 1��ĭ�� ����� ���� �������� 0�� �ƴ϶�� ����� �Ѹ� �� �ø���.
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

