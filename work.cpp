#include <stdio.h>
#include <string>
#include <queue>
#include <stack>
#include <math.h>
#include <cstring>
#include <time.h>
#include <malloc.h>

using namespace std;

//�������ö�ٱ���
enum Direction {
	up = 0,
	down = 1,
	right = 2,
	left = 3
};

//n��������m������
int n, m;

//��¼��ѷ������һ����ָ��
struct Step* Best_step = NULL;

//��¼�Ѿ���õ���߷���
int highest_score;

//���ڼ�¼�Ѿ����ֹ���״̬����Ϲ�ϣ��ʹ��
struct Store {
	int state;
	Store* next;
	Step* step;
};

//���ڴ洢��ϣ������ݿ�
Store* store[1000];

//���ڼ�¼ÿһ�����߷���ǰһ����λ��
struct Step {
	//����
	int steps;
	//�߸ò��ƶ�������
	int numberofstep;
	//�ò���ɺ������״̬
	char* state;
	//0��������
	char dx;
	//0��������
	char dy;
	//ָ����һ����ָ��
	struct Step* stepbefore;
	//�ò��ķ���
	int score;
	//�ò�������ֵ
	double value;
	
};

//���طº���
struct cmp {
	bool operator()(Step* a, Step* b) {
		return a->value > b->value;
	}
};

//�洢·����Ϣ�Ķ���
queue<Step*> Mystep;

//�洢��̽��δ�̶���Ϣ�Ķ���
priority_queue<Step*,vector<Step*>,cmp > Open;

//��ӡ����������������յ���ѽ��
void print(Step* Beststep)
{
	stack<Step> MyStep;

	while (Beststep->steps != 0)
	{
		MyStep.push(*Beststep);
		Beststep = Beststep->stepbefore;
	}
	while (!MyStep.empty())
	{
		fprintf(stdout,"%d\n", MyStep.top().numberofstep);
		MyStep.pop();
	}

	fprintf(stdout,"Over\n");
}

//��������
double G(char* array)
{
	double value = 0;
	for (int i = 0;i < m * n;i++)
	{
		if (array[i])
		{
			value += sqrt(pow(abs((array[i] - 1) / m - i / m),2)) + sqrt(pow(abs((array[i] - 1) % m - i % m),2));
		}
	}
	return 5.1*value;
}

//�����жϵ�ǰ״̬��ʵʱ����
int judgescore(char* array)
{
	int score = 0;
	for (int i = 0;i < m * n - 1;i++)
		if (array[i] == i + 1)
			score += 100;
	return score;
}

//�ж�һ�������Ƿ����ͨ��
bool Can_Move(Direction dir, int x, int y)
{
	switch (dir)
	{
	case up:
	{
		if (x > 0)
			return true;
		else
			return false;
		break;
	}
	case down:
	{
		if (x < n - 1)
			return true;
		else
			return false;
		break;
	}
	case right:
	{
		if (y < m - 1)
			return true;
		else
			return false;
		break;
	}
	case left:
	{
		if (y > 0)
			return true;
		else
			return false;
		break;
	}
	default:
		return false;
	}
}

//����ÿ��״̬��hash����
unsigned int Hash(char* array)
{
	int hash, i;
	for (hash = m * n, i = 0; i < m * n; ++i)
		hash = (hash << 4) ^ (hash >> 28) ^ array[i];
	return hash;
}


//��Ѱ��״̬�Ƿ���ֹ�������A*��
bool search(unsigned int hashnumber,bool add)
{
	int pos = hashnumber % 1000;
	Store** ptr = &(store[pos]);
	while (*ptr != NULL)
	{
		//��������״̬�з��ָ�����Ѿ����ڹ�������false
		if ((*ptr)->state == hashnumber)
		{
			/*if ((*ptr)->step->value > step->value)
			{
				Open.push(step);
			}*/
			return false;
		}

		ptr = &((*ptr)->next);
	}
	//��δ���ָ�������ڣ��򽫸������������ݿ��У���������ֵ
	if (add)
	{
		*ptr = new Store;
		(*ptr)->next = NULL;
		(*ptr)->state = hashnumber;
	}
	return true;
}


//�ƶ��������ô��ƶ�ָ�����ƶ��ո񣬼��ԡ�0�����в��������ɹ��ƶ�ʱ����true�����򷵻�false������A*�㷨��
bool move(char state[], Direction dir, Step* step)
{
	int dx = step->dx;
	int dy = step->dy;
	char temp;

	if (Can_Move(dir, dx, dy))
	{
		switch (dir)
		{
		case up:
		{
			if (state[(dx - 1) * m + dy] == (dx - 1) * m + dy + 1)
			{
				step->score -= 101;


			}
			else if (state[(dx - 1) * m + dy] == dx * m + dy + 1)
			{
				step->score += 99;

			}
			else
			{
				step->score -= 1;
				
			}

			state[dx * m + dy] = state[(dx - 1) * m + dy];
			temp = state[dx * m + dy];
			dx -= 1;
			state[dx * m + dy] = 0;
			step->dx = dx;
			if (search(Hash(state),false))
			{
				step->steps++;
				memcpy(step->state, state, m * n * sizeof(char));
				step->numberofstep = temp;
				step->value = G(state) + step->steps;
				return true;
			}
			return false;
		}
		case down:
		{
			if (state[(dx + 1) * m + dy] == (dx + 1) * m + dy + 1)
			{
				step->score -= 101;

			}
			else if (state[(dx + 1) * m + dy] == dx * m + dy + 1)
			{
				step->score += 99;

			}
			else
			{
				step->score -= 1;

			}
			state[dx * m + dy] = state[(dx + 1) * m + dy];
			temp = state[dx * m + dy];
			dx += 1;
			state[dx * m + dy] = 0;
			step->dx = dx;
			if (search(Hash(state), false))
			{
				step->steps++;
				memcpy(step->state, state, m * n * sizeof(char));
				step->numberofstep = temp;
				step->value = G(state) + step->steps;
				return true;
			}
			else
				return false;
		}

		case left:
		{
			if (state[dx * m + dy - 1] == dx * m + dy)
			{
				step->score -= 101;

			}
			else if (state[dx * m + dy - 1] == dx * m + dy + 1)
			{
				step->score += 99;

			}
			else
			{
				step->score -= 1;

			}
			state[dx * m + dy] = state[dx * m + dy - 1];
			temp = state[dx * m + dy];
			dy -= 1;
			step->dy = dy;
			state[dx * m + dy] = 0;
			if (search(Hash(state), false))
			{
				step->steps++;
				memcpy(step->state, state, m * n * sizeof(char));
				step->numberofstep = temp;
				step->value = G(state) + step->steps;
				return true;
			}
			else
			{
				return false;
			}
		}
		case right:
		{
			if (state[dx * m + dy + 1] == dx * m + dy + 2)
			{
				step->score -= 101;

			}
			else if (state[dx * m + dy + 1] == dx * m + dy + 1)
			{
				step->score += 99;

			}
			else
			{
				step->score -= 1;

			}
			state[dx * m + dy] = state[dx * m + dy + 1];
			temp = state[dx * m + dy];
			dy += 1;
			state[dx * m + dy] = 0;
			step->dy = dy;

			if (search(Hash(state), false))
			{
				step->steps++;
				memcpy(step->state, state, m * n * sizeof(char));
				step->numberofstep = temp;
				step->value = G(state) + step->steps;
				return true;
			}
			else
				return false;
		}
		default:
			throw("Overflow");
			return false;
		}
	}
	else
		return false;

}

//AStarѰ·�㷨
void Findway()
{
	clock_t start = clock();

	while (!Open.empty())
	{
		Step* temp = Open.top();
		search(Hash(temp->state), true);
		Open.pop();
		Step* MyStep[4];
		char* state = (char*)malloc(m * n * sizeof(char));
		for (int i = 0;i < 4;i++)
		{			
			MyStep[i] = new Step;
			memcpy(state, temp->state, m * n * sizeof(char));
			MyStep[i]->steps = temp->steps;
			MyStep[i]->dx = temp->dx;
			MyStep[i]->dy = temp->dy;
			MyStep[i]->stepbefore = temp;
			MyStep[i]->state = new char[m * n];
			MyStep[i]->score = temp->score;
			if (clock() - start >= 1000)
			{
					return;
			}
			if (move(state, (Direction)i, MyStep[i]))
			{
				if (clock() - start >= 1000)
				{
					return;
				}


				int temp_num = MyStep[i]->score;
				
				if (G(MyStep[i]->state) == 0)
				{
				
					Best_step = MyStep[i];
					return;
				}
				if (temp_num > highest_score)
				{
					highest_score = temp_num;
					Best_step = MyStep[i];
				}
				if (MyStep[i]->steps > (m * n - 1) * 100)
				{
					return;
				}
				Open.push(MyStep[i]);
			}

			else
			{
				delete(MyStep[i]);
			}

		}
		free(state);
	}
}


int main()
{
	//��������������
	scanf("%d %d", &n, &m);

	char* array = new char[m * n];
	Step* MyStep = new Step;
	for (int i = 0, temp;i < m * n;i++)
	{
		scanf("%d", &temp);
		if (!temp)
		{
			MyStep->dx = i / m;
			MyStep->dy = i % m;
		}
		array[i] = temp;
	}

	//����߷ֳ�ʼ��Ϊ��ǰ״̬����
	highest_score = judgescore(array);

	MyStep->stepbefore = NULL;
	MyStep->steps = 0;
	MyStep->state = new char[m * n];
	MyStep->score = highest_score;
	MyStep->value = G(array);

	Best_step = MyStep;
	memcpy(MyStep->state, array, m * n * sizeof(char));


		search(Hash(array), false);

		Open.push(MyStep);

		for (int i = 0;i < 1000;i++)
		{
			store[i] = NULL;
		}

		Findway();

	print(Best_step);

	return 0;
}