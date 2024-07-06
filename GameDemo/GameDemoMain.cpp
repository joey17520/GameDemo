#include <easyx.h>
#include <deque>

#define WIDTH 640
#define HEIGHT 480

#define SIZE 20		// 边长为20的正方形

#define DELAY 100

// 蛇移动方向
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// 蛇身
struct Snake
{
	int x;
	int y;
	Direction dir;	// 移动方向
};

// 食物
struct Food
{
	int x;
	int y;
};

// 绘制蛇身
void drawSnake(std::deque<Snake>& snake)
{
	// 绿色表示（设置填充颜色为绿色）
	setfillcolor(GREEN);
	// 遍历蛇身绘制
	for (const auto& body : snake)
	{
		// 绘制一个矩形框，四个参数分别是左上角和右下角的横纵坐标
		// 用20 * 20的矩形框表示蛇身
		fillrectangle(body.x * SIZE, body.y * SIZE, (body.x + 1) * SIZE, (body.y + 1) * SIZE);
	}
	// 控制蛇移动的速度
	Sleep(DELAY);
}

// 判断游戏是否结束
void isOver(std::deque<Snake>& snake)
{
	// 检测碰撞
	// 遍历蛇身，使用迭代器遍历，从蛇头后面一个元素开始遍历
	for (auto iter = snake.begin() + 1; iter != snake.end(); ++iter)
	{
		if ((snake.front().x == iter->x && snake.front().y == iter->y)		// 蛇头撞到了自己，游戏结束
			|| (snake.front().x < 0) || (snake.front().x >= WIDTH / SIZE)	// 越界，游戏结束
			|| (snake.front().y < 0) || (snake.front().y >= WIDTH / SIZE))	// 越界，游戏结束
		{
			// 设置当前文字颜色为红色
			settextcolor(RED);
			// 设置当前文字样式。指定高度为100，宽度为0表示自适应
			settextstyle(100, 0, _T("Consolas"));
			TCHAR endStr[16];
			_stprintf_s(endStr, _T("GAME OVER!!"));
			// 在指定位置输出字符串
			outtextxy(WIDTH / SIZE, HEIGHT / SIZE, endStr);
			// 停留三秒
			Sleep(3000);

			// 关闭窗口
			closegraph();
			exit(0);
		}
	}
}

// 蛇移动，grow表示是否吃到了食物，该参数用来区分蛇普通移动和吃到食物
void moveSnake(std::deque<Snake>& snake, bool grow = false)
{
	// 获取新蛇头
	Snake newHead = snake.front();

	// 判读蛇移动方向
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

	// 将新位置作为新的蛇头
	snake.push_front(newHead);
	// 每移动一次都要判断游戏是否结束了
	isOver(snake);
	// grow=true表示吃到了食物		grow=false表示蛇普通移动
	if (!grow)
	{
		// 没吃到食物
		// 将蛇身最后一个元素的矩形框清楚，因为已不是蛇身的一部分了
		clearrectangle(snake.back().x * SIZE, snake.back().y * SIZE, (snake.back().x + 1) * SIZE, (snake.back().y + 1) * SIZE);
		// 将双端队列最后一个元素去除
		snake.pop_back();
	}

	// 绘制蛇身
	drawSnake(snake);
}

// 更改蛇移动方向
void changeDirection(std::deque<Snake>& snake)
{
	ExMessage msg = { 0 };
	// 捕获键盘消息
	peekmessage(&msg, EX_KEY);

	// 当按下某个键
	if (msg.message == WM_KEYDOWN)
	{
		// 判断输入的按键
		switch (msg.vkcode)
		{
		case VK_UP:
			// 不可以反方向移动
			if (snake.front().dir != DOWN)
				snake.front().dir = UP;
			break;
		case VK_DOWN:
			// 不可以反方向移动
			if (snake.front().dir != UP)
				snake.front().dir = DOWN;
			break;
		case VK_LEFT:
			// 不可以反方向移动
			if (snake.front().dir != RIGHT)
				snake.front().dir = LEFT;
			break;
		case VK_RIGHT:
			// 不可以反方向移动
			if (snake.front().dir != LEFT)
				snake.front().dir = RIGHT;
			break;
		}
	}
}

// 生成食物，不能生成在蛇身上
void generateFood(Food& food, std::deque<Snake>& snake)
{
	// 一直生成，直到生成的食物不在蛇身上为止
	while (true)
	{
		// 随机生成
		// 将整个窗口看做是一个数组，每个数组元素占的空间大小为20*20个像素点
		food.x = rand() % (WIDTH / SIZE);
		food.y = rand() % (HEIGHT / SIZE);

		// 用一个变量存储生成的位置是否与蛇身位重合
		bool overlap = false;
		// 遍历蛇身，范围for循环
		for (const auto& body : snake)
		{
			// 食物位置与蛇身重合
			if (food.x == body.x && food.y == body.y)
			{
				// 已经与蛇身重合了，不必再遍历，跳出for循环，在while循环中重新生成
				overlap = true;
				break;
			}
		}

		// 如果不重合，则食物生成成功，可以结束while循环了
		if (!overlap)
		{
			break;
		}
	}
}

// 检测是否与食物的位置重叠
bool checkCollision(std::deque<Snake>& snake, Food& food)
{
	// 蛇头与食物重合
	if (snake.front().x == food.x && snake.front().y == food.y)
	{
		return true;
	}
	// 检测是否与蛇身的位置重叠（可以不用，因为蛇头没经过的位置，蛇身必然不会经过）
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
	initgraph(WIDTH, HEIGHT);		//绘制窗口，两个参数分别为窗口的宽度和高度

	// 定义一条蛇
	std::deque<Snake> snake;
	// 初始化蛇
	// 在双端队列头部插入一个元素作为蛇头
	snake.push_front({ WIDTH / SIZE / 2, HEIGHT / SIZE / 2, RIGHT });

	// 生成食物
	Food food;
	generateFood(food, snake);
	// 绘制食物，红色表示食物
	setfillcolor(RED);
	fillrectangle(food.x * SIZE, food.y * SIZE, (food.x + 1) * SIZE, (food.y + 1) * SIZE);

	drawSnake(snake);

	// 分数初始化为0
	int score = 0;
	while (true)
	{
		// 蛇移动
		moveSnake(snake);
		// 更改蛇移动方向
		changeDirection(snake);

		// 是否吃到食物
		if (checkCollision(snake, food))
		{
			score++;
			generateFood(food, snake);

			moveSnake(snake, true);
		}
		// 绘制食物
		setfillcolor(RED);
		fillrectangle(food.x * SIZE, food.y * SIZE, (food.x + 1) * SIZE, (food.y + 1) * SIZE);

		// 设置当前文字颜色为黄色
		settextcolor(YELLOW);
		// 设置当前文字样式，指定高度为20，宽度0表示自适应，_T支持Unicode编码，consolas表示字体
		settextstyle(20, 0, _T("Consolas"));
		TCHAR strScore[16];
		_stprintf_s(strScore, _T("Score: %d"), score);
		// 在指定位置输出字符串
		outtextxy(10, 10, strScore);
	}

	system("pause");
	closegraph();
	return 0;
}