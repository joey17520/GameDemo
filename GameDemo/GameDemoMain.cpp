#include <easyx.h>
#include <deque>

#define WIDTH 640
#define HEIGHT 480

#define SIZE 20		// �߳�Ϊ20��������

#define DELAY 100

// ���ƶ�����
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// ����
struct Snake
{
	int x;
	int y;
	Direction dir;	// �ƶ�����
};

// ʳ��
struct Food
{
	int x;
	int y;
};

// ��������
void drawSnake(std::deque<Snake>& snake)
{
	// ��ɫ��ʾ�����������ɫΪ��ɫ��
	setfillcolor(GREEN);
	// �����������
	for (const auto& body : snake)
	{
		// ����һ�����ο��ĸ������ֱ������ϽǺ����½ǵĺ�������
		// ��20 * 20�ľ��ο��ʾ����
		fillrectangle(body.x * SIZE, body.y * SIZE, (body.x + 1) * SIZE, (body.y + 1) * SIZE);
	}
	// �������ƶ����ٶ�
	Sleep(DELAY);
}

// �ж���Ϸ�Ƿ����
void isOver(std::deque<Snake>& snake)
{
	// �����ײ
	// ��������ʹ�õ���������������ͷ����һ��Ԫ�ؿ�ʼ����
	for (auto iter = snake.begin() + 1; iter != snake.end(); ++iter)
	{
		if ((snake.front().x == iter->x && snake.front().y == iter->y)		// ��ͷײ�����Լ�����Ϸ����
			|| (snake.front().x < 0) || (snake.front().x >= WIDTH / SIZE)	// Խ�磬��Ϸ����
			|| (snake.front().y < 0) || (snake.front().y >= WIDTH / SIZE))	// Խ�磬��Ϸ����
		{
			// ���õ�ǰ������ɫΪ��ɫ
			settextcolor(RED);
			// ���õ�ǰ������ʽ��ָ���߶�Ϊ100�����Ϊ0��ʾ����Ӧ
			settextstyle(100, 0, _T("Consolas"));
			TCHAR endStr[16];
			_stprintf_s(endStr, _T("GAME OVER!!"));
			// ��ָ��λ������ַ���
			outtextxy(WIDTH / SIZE, HEIGHT / SIZE, endStr);
			// ͣ������
			Sleep(3000);

			// �رմ���
			closegraph();
			exit(0);
		}
	}
}

// ���ƶ���grow��ʾ�Ƿ�Ե���ʳ��ò���������������ͨ�ƶ��ͳԵ�ʳ��
void moveSnake(std::deque<Snake>& snake, bool grow = false)
{
	// ��ȡ����ͷ
	Snake newHead = snake.front();

	// �ж����ƶ�����
	switch (newHead.dir)
	{
	case UP:
		newHead.y--;
		break;
	case DOWN:
		newHead.y++;
		break;
	case LEFT:
		newHead.x--;
		break;
	case RIGHT:
		newHead.x++;
		break;
	}

	// ����λ����Ϊ�µ���ͷ
	snake.push_front(newHead);
	// ÿ�ƶ�һ�ζ�Ҫ�ж���Ϸ�Ƿ������
	isOver(snake);
	// grow=true��ʾ�Ե���ʳ��		grow=false��ʾ����ͨ�ƶ�
	if (!grow)
	{
		// û�Ե�ʳ��
		// ���������һ��Ԫ�صľ��ο��������Ϊ�Ѳ��������һ������
		clearrectangle(snake.back().x * SIZE, snake.back().y * SIZE, (snake.back().x + 1) * SIZE, (snake.back().y + 1) * SIZE);
		// ��˫�˶������һ��Ԫ��ȥ��
		snake.pop_back();
	}

	// ��������
	drawSnake(snake);
}

// �������ƶ�����
void changeDirection(std::deque<Snake>& snake)
{
	ExMessage msg = { 0 };
	// ���������Ϣ
	peekmessage(&msg, EX_KEY);

	// ������ĳ����
	if (msg.message == WM_KEYDOWN)
	{
		// �ж�����İ���
		switch (msg.vkcode)
		{
		case VK_UP:
			// �����Է������ƶ�
			if (snake.front().dir != DOWN)
				snake.front().dir = UP;
			break;
		case VK_DOWN:
			// �����Է������ƶ�
			if (snake.front().dir != UP)
				snake.front().dir = DOWN;
			break;
		case VK_LEFT:
			// �����Է������ƶ�
			if (snake.front().dir != RIGHT)
				snake.front().dir = LEFT;
			break;
		case VK_RIGHT:
			// �����Է������ƶ�
			if (snake.front().dir != LEFT)
				snake.front().dir = RIGHT;
			break;
		}
	}
}

// ����ʳ�����������������
void generateFood(Food& food, std::deque<Snake>& snake)
{
	// һֱ���ɣ�ֱ�����ɵ�ʳ�ﲻ��������Ϊֹ
	while (true)
	{
		// �������
		// ���������ڿ�����һ�����飬ÿ������Ԫ��ռ�Ŀռ��СΪ20*20�����ص�
		food.x = rand() % (WIDTH / SIZE);
		food.y = rand() % (HEIGHT / SIZE);

		// ��һ�������洢���ɵ�λ���Ƿ�������λ�غ�
		bool overlap = false;
		// ����������Χforѭ��
		for (const auto& body : snake)
		{
			// ʳ��λ���������غ�
			if (food.x == body.x && food.y == body.y)
			{
				// �Ѿ��������غ��ˣ������ٱ���������forѭ������whileѭ������������
				overlap = true;
				break;
			}
		}

		// ������غϣ���ʳ�����ɳɹ������Խ���whileѭ����
		if (!overlap)
		{
			break;
		}
	}
}

// ����Ƿ���ʳ���λ���ص�
bool checkCollision(std::deque<Snake>& snake, Food& food)
{
	// ��ͷ��ʳ���غ�
	if (snake.front().x == food.x && snake.front().y == food.y)
	{
		return true;
	}
	// ����Ƿ��������λ���ص������Բ��ã���Ϊ��ͷû������λ�ã������Ȼ���ᾭ����
	for (auto iter = snake.begin() + 1; iter != snake.end(); ++iter)
	{
		if (iter->x == food.x && iter->y == food.y)
		{
			return true;
		}
	}
	return false;
}

int main()
{
	initgraph(WIDTH, HEIGHT);		//���ƴ��ڣ����������ֱ�Ϊ���ڵĿ�Ⱥ͸߶�

	// ����һ����
	std::deque<Snake> snake;
	// ��ʼ����
	// ��˫�˶���ͷ������һ��Ԫ����Ϊ��ͷ
	snake.push_front({ WIDTH / SIZE / 2, HEIGHT / SIZE / 2, RIGHT });

	// ����ʳ��
	Food food;
	generateFood(food, snake);
	// ����ʳ���ɫ��ʾʳ��
	setfillcolor(RED);
	fillrectangle(food.x * SIZE, food.y * SIZE, (food.x + 1) * SIZE, (food.y + 1) * SIZE);

	drawSnake(snake);

	// ������ʼ��Ϊ0
	int score = 0;
	while (true)
	{
		// ���ƶ�
		moveSnake(snake);
		// �������ƶ�����
		changeDirection(snake);

		// �Ƿ�Ե�ʳ��
		if (checkCollision(snake, food))
		{
			score++;
			generateFood(food, snake);

			moveSnake(snake, true);
		}
		// ����ʳ��
		setfillcolor(RED);
		fillrectangle(food.x * SIZE, food.y * SIZE, (food.x + 1) * SIZE, (food.y + 1) * SIZE);

		// ���õ�ǰ������ɫΪ��ɫ
		settextcolor(YELLOW);
		// ���õ�ǰ������ʽ��ָ���߶�Ϊ20�����0��ʾ����Ӧ��_T֧��Unicode���룬consolas��ʾ����
		settextstyle(20, 0, _T("Consolas"));
		TCHAR strScore[16];
		_stprintf_s(strScore, _T("Score: %d"), score);
		// ��ָ��λ������ַ���
		outtextxy(10, 10, strScore);
	}

	system("pause");
	closegraph();
	return 0;
}