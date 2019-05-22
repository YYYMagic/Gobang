#include<iostream>
#include<vector>
#include<map>
#include <graphics.h>
#include <conio.h>
#include <time.h> 
using namespace std;

/*                 棋盘                     */
/*---x--->
|
y  board
|
v        */

/*              搜索思路                     */
//电脑先手时 玩家执白 电脑执黑 电脑希望(黑-白)MAX
//玩家先手时 玩家执黑 电脑执白 电脑希望(白-黑)MAX

/*             得分计算思路                  */
/*
  对于一个棋子 计算其周围几个方向的得分和作为该棋子得分
     col  slope = 1
   \  |  /
    \ | /
     \|/
  ----.---- row
     /|\
    / | \
   /  |  \
          slope = -1
  对于一个棋盘 计算所有棋子得分和作为棋盘得分
*/

//棋盘中间值
#define MID 7

//计算得分用 活五到死二的得分
#define live5 100000
#define live4 10000
#define live3 1000
#define live2 100
#define live1 10
#define die4 1000
#define die3 100
#define die2 10

//棋子颜色的宏定义
#define B 1
#define W 2

//鼠标位置的宏定义
#define inB 1
#define inW 2
#define in3 3
#define in4 4
#define in5 5
#define inBoard 0
#define inReload 1
#define inEnd 2

//后端
//打印棋盘数组
void print(int board[][15]) {
	for (int y = 0; y < 15; ++y) {
		for (int x = 0; x < 15; ++x) {
			cout << board[y][x];
		}
		cout << endl;
	}
	cout << "----------------" << endl;
}

//供score调用 输入7个连续位置的棋子序列 返回该序列的黑子得分 
double getTypeB(int* pieces) {
	//0.....6
	int count = 0;
	double score = 0;
	for (int i = 1; i <= 5; ++i) {
		if (pieces[i] == 1) {
			count = 0;
			int j = i;
			while (j <= 5) {
				if (pieces[j] == 1) {
					count++;
					j++;
				}
				else break;
			}
			//i：第一个黑 j：第一个非黑或6
			switch (count)
			{
			case 1:
				if (pieces[i - 1] != 2 && pieces[j] != 2) score += live1;
				break;
			case 2:
				if (pieces[i - 1] != 2 && pieces[j] != 2) score += live2;
				else score += die2;
				break;
			case 3:
				if (pieces[i - 1] != 2 && pieces[j] != 2) score += live3;
				else score += die3;
				break;
			case 4:
				if (pieces[i - 1] != 2 && pieces[j] != 2) score += live4;
				else score += die4;
				break;
			case 5:
				score += live5;
				break;
			default:
				break;
			}
			i = j - 1; //第二次i的位置从j开始
		}
	}
	return score;
}

//供score调用 输入7个连续位置的棋子序列 返回该序列的白子得分
double getTypeW(int* pieces) {
	//0.....6
	int count = 0;
	double score = 0;
	for (int i = 1; i <= 5; ++i) {
		if (pieces[i] == 2) {
			count = 0;
			int j = i;
			while (j <= 5) {
				if (pieces[j] == 2) {
					count++;
					j++;
				}
				else break;
			}
			//i：第一个白 j：第一个非白或6
			switch (count)
			{
			case 1:
				if (pieces[i - 1] != 1 && pieces[j] != 1) score += live1;
				break;
			case 2:
				if (pieces[i - 1] != 1 && pieces[j] != 1) score += live2;
				else score += die2;
				break;
			case 3:
				if (pieces[i - 1] != 1 && pieces[j] != 1) score += live3;
				else score += die3;
				break;
			case 4:
				if (pieces[i - 1] != 1 && pieces[j] != 1) score += live4;
				else score += die4;
				break;
			case 5:
				score += live5;
				break;
			default:
				break;
			}
			i = j - 1; //第二次i的位置从j开始
		}
	}
	return score;
}

//输入棋盘 返回黑子或白子得分 color指定黑子或白子(下同)
double score(int board[][15], int color) {
	double score = 0.0;
	int pieces[7]; //1-5处理
	int xx, yy;
	int F = 0;

	for (int len = 0; len <= 7; ++len) {
		int flag = 0;
		for (int y = MID - len; y <= MID + len; ++y) {
			for (int x = MID - len; x <= MID + len; ++x) {
				if (y != MID - len && y != MID + len) {
					if (x != MID - len && x != MID + len) continue;
				}
				if (board[y][x]) { //有子
					flag = 1;
					F = 1;
					//col
					for (int i = 5; i >= 0; --i) {
						for (int j = 0; j <= 6; ++j) {
							xx = x;
							yy = y - i + j;
							if (yy < 0 || yy > 14) pieces[j] = 2;
							else pieces[j] = board[yy][xx];
						}
						if (color == 1) score += getTypeB(pieces);
						else score += getTypeW(pieces);
					}
					//row
					for (int i = 5; i >= 0; --i) {
						for (int j = 0; j <= 6; ++j) {
							xx = x - i + j;
							yy = y;
							if (xx < 0 || xx > 14) pieces[j] = 2;
							else pieces[j] = board[yy][xx];
						}
						if (color == 1) score += getTypeB(pieces);
						else score += getTypeW(pieces);
					}
					//slope = -1
					for (int i = 5; i >= 0; --i) {
						for (int j = 0; j <= 6; ++j) {
							xx = x - i + j;
							yy = y - i + j;
							if (xx < 0 || xx > 14 || yy < 0 || yy > 14) pieces[j] = 2;
							else pieces[j] = board[yy][xx];
						}
						if (color == 1) score += getTypeB(pieces);
						else score += getTypeW(pieces);
					}
					//slope = 1
					for (int i = 5; i >= 0; --i) {
						for (int j = 0; j <= 6; ++j) {
							xx = x - i + j;
							yy = y + i - j;
							if (xx < 0 || xx > 14 || yy < 0 || yy > 14) pieces[j] = 2;
							else pieces[j] = board[yy][xx];
						}
						if (color == 1) score += getTypeB(pieces);
						else score += getTypeW(pieces);
					}
				}
			}
		}
		if (F == 1 && flag == 0) return score;
	}
	return score;
}

//供isWin调用 输入5个连续位置的棋子序列 返回是否形成5连
bool isWin_pieces(int* pieces, int color) {
	if (color == B) {
		for (int i = 0; i <= 4; ++i) {
			if (pieces[i] != B) return false;
		}
		return true;
	}
	else if (color == W) {
		for (int i = 0; i <= 4; ++i) {
			if (pieces[i] != W) return false;
		}
		return true;
	}
	return true;
}

//输入棋盘和最新落子位置 返回是否胜利
bool isWin(int board[][15], int x, int y, int color) {
	int pieces[5]; //1-5处理
	int xx = 0;
	int yy = 0;
	if (color == B) {
		//col
		for (int i = 4; i >= 0; --i) {
			for (int j = 0; j <= 4; ++j) {
				xx = x;
				yy = y - i + j;
				if (yy < 0 || yy > 14) pieces[j] = 2;
				else pieces[j] = board[yy][xx];
			}
			if (isWin_pieces(pieces, B) == true) return true;
		}
		//row
		for (int i = 4; i >= 0; --i) {
			for (int j = 0; j <= 4; ++j) {
				xx = x - i + j;
				yy = y;
				if (xx < 0 || xx > 14) pieces[j] = 2;
				else pieces[j] = board[yy][xx];
			}
			if (isWin_pieces(pieces, B) == true) return true;
		}
		//slope = -1
		for (int i = 4; i >= 0; --i) {
			for (int j = 0; j <= 4; ++j) {
				xx = x - i + j;
				yy = y - i + j;
				if (xx < 0 || xx > 14 || yy < 0 || yy > 14) pieces[j] = 2;
				else pieces[j] = board[yy][xx];
			}
			if (isWin_pieces(pieces, B) == true) return true;
		}
		//slope = 1
		for (int i = 4; i >= 0; --i) {
			for (int j = 0; j <= 4; ++j) {
				xx = x - i + j;
				yy = y + i - j;
				if (xx < 0 || xx > 14 || yy < 0 || yy > 14) pieces[j] = 2;
				else pieces[j] = board[yy][xx];
			}
			if (isWin_pieces(pieces, B) == true) return true;
		}
	}

	else if (color == W) {
		//col
		for (int i = 4; i >= 0; --i) {
			for (int j = 0; j <= 4; ++j) {
				xx = x;
				yy = y - i + j;
				if (yy < 0 || yy > 14) pieces[j] = 1;
				else pieces[j] = board[yy][xx];
			}
			if (isWin_pieces(pieces, W) == true) return true;
		}
		//row
		for (int i = 4; i >= 0; --i) {
			for (int j = 0; j <= 4; ++j) {
				xx = x - i + j;
				yy = y;
				if (xx < 0 || xx > 14) pieces[j] = 1;
				else pieces[j] = board[yy][xx];
			}
			if (isWin_pieces(pieces, W) == true) return true;
		}
		//slope = -1
		for (int i = 4; i >= 0; --i) {
			for (int j = 0; j <= 4; ++j) {
				xx = x - i + j;
				yy = y - i + j;
				if (xx < 0 || xx > 14 || yy < 0 || yy > 14) pieces[j] = 1;
				else pieces[j] = board[yy][xx];
			}
			if (isWin_pieces(pieces, W) == true) return true;
		}
		//slope = 1
		for (int i = 4; i >= 0; --i) {
			for (int j = 0; j <= 4; ++j) {
				xx = x - i + j;
				yy = y + i - j;
				if (xx < 0 || xx > 14 || yy < 0 || yy > 14) pieces[j] = 1;
				else pieces[j] = board[yy][xx];
			}
			if (isWin_pieces(pieces, W) == true) return true;
		}
	}
	return false;
}

//输入点p 返回是否出界
bool isOut(int* p) {
	if (p[0] < 0 || p[0] > 14 || p[1] < 0 || p[1] > 14) return true;
	else return false;
}

//供pre和pre2调用 输入棋盘和上次对手落子坐标(x,y) 若对手成四 输出可堵截点p1, p2
bool isFour(int board[][15], int x, int y, int color, int* p1, int*p2) {
	int pieces[5]; 
	int xx, yy;
	//col
	for (int i = 4; i >= 0; --i) {
		for (int j = 0; j <= 4; ++j) {
			xx = x;
			yy = y - i + j;
			if (yy < 0 || yy > 14) {
				if (color == B) pieces[j] = W;
				else if (color == W) pieces[j] = B;
			}
			else pieces[j] = board[yy][xx];
		}
		if (color == B) {
			if (pieces[0] == B && pieces[1] == B && pieces[2] == B && pieces[3] == B) {
				p1[0] = x;
				p1[1] = y - i - 1;
				p2[0] = x;
				p2[1] = y - i - 1 + 5;
				return true;
			}
			if (pieces[1] == B && pieces[2] == B && pieces[3] == B && pieces[4] == B) {
				p1[0] = x;
				p1[1] = y - i;
				p2[0] = x;
				p2[1] = y - i + 5;
				return true;
			}
		}
		else if (color == W) {
			if (pieces[0] == W && pieces[1] == W && pieces[2] == W && pieces[3] == W) {
				p1[0] = x;
				p1[1] = y - i - 1;
				p2[0] = x;
				p2[1] = y - i - 1 + 5;
				return true;
			}
			if (pieces[1] == W && pieces[2] == W && pieces[3] == W && pieces[4] == W) {
				p1[0] = x;
				p1[1] = y - i;
				p2[0] = x;
				p2[1] = y - i + 5;
				return true;
			}
		}
	}
	//row
	for (int i = 4; i >= 0; --i) {
		for (int j = 0; j <= 4; ++j) {
			xx = x - i + j;
			yy = y;
			if (xx < 0 || xx > 14) {
				if (color == B) pieces[j] = W;
				else if (color == W) pieces[j] = B;
			}
			else pieces[j] = board[yy][xx];
		}
		if (color == B) {
			if (pieces[0] == B && pieces[1] == B && pieces[2] == B && pieces[3] == B) {
				p1[0] = x - i - 1;
				p1[1] = y;
				p2[0] = x - i - 1 + 5;
				p2[1] = y;
				return true;
			}
			if (pieces[1] == B && pieces[2] == B && pieces[3] == B && pieces[4] == B) {
				p1[0] = x - i;
				p1[1] = y;
				p2[0] = x - i + 5;
				p2[1] = y;
				return true;
			}
		}
		else if (color == W) {
			if (pieces[0] == W && pieces[1] == W && pieces[2] == W && pieces[3] == W) {
				p1[0] = x - i - 1;
				p1[1] = y;
				p2[0] = x - i - 1 + 5;
				p2[1] = y;
				return true;
			}
			if (pieces[1] == W && pieces[2] == W && pieces[3] == W && pieces[4] == W) {
				p1[0] = x - i;
				p1[1] = y;
				p2[0] = x - i + 5;
				p2[1] = y;
				return true;
			}
		}
	}
	//slope = -1
	for (int i = 4; i >= 0; --i) {
		for (int j = 0; j <= 4; ++j) {
			xx = x - i + j;
			yy = y - i + j;
			if (xx < 0 || xx > 14 || yy < 0 || yy > 14) {
				if (color == B) pieces[j] = W;
				else if (color == W) pieces[j] = B;
			}
			else pieces[j] = board[yy][xx];
		}
		if (color == B) {
			if (pieces[0] == B && pieces[1] == B && pieces[2] == B && pieces[3] == B) {
				p1[0] = x - i - 1;
				p1[1] = y - i - 1;
				p2[0] = x - i - 1 + 5;
				p2[1] = y - i - 1 + 5;
				return true;
			}
			if (pieces[1] == B && pieces[2] == B && pieces[3] == B && pieces[4] == B) {
				p1[0] = x - i;
				p1[1] = y - i;
				p2[0] = x - i + 5;
				p2[1] = y - i + 5;
				return true;
			}
		}
		else if (color == W) {
			if (pieces[0] == W && pieces[1] == W && pieces[2] == W && pieces[3] == W) {
				p1[0] = x - i - 1;
				p1[1] = y - i - 1;
				p2[0] = x - i - 1 + 5;
				p2[1] = y - i - 1 + 5;
				return true;
			}
			if (pieces[1] == W && pieces[2] == W && pieces[3] == W && pieces[4] == W) {
				p1[0] = x - i;
				p1[1] = y - i;
				p2[0] = x - i + 5;
				p2[1] = y - i + 5;
				return true;
			}
		}
	}
	//slope = 1
	for (int i = 4; i >= 0; --i) {
		for (int j = 0; j <= 4; ++j) {
			xx = x - i + j;
			yy = y + i - j;
			if (xx < 0 || xx > 14 || yy < 0 || yy > 14) {
				if (color == B) pieces[j] = W;
				else if (color == W) pieces[j] = B;
			}
			else pieces[j] = board[yy][xx];
		}
		if (color == B) {
			if (pieces[0] == B && pieces[1] == B && pieces[2] == B && pieces[3] == B) {
				p1[0] = x - i - 1;
				p1[1] = y + i + 1;
				p2[0] = x - i - 1 + 5;
				p2[1] = y + i + 1 - 5;
				return true;
			}
			if (pieces[1] == B && pieces[2] == B && pieces[3] == B && pieces[4] == B) {
				p1[0] = x - i;
				p1[1] = y + i;
				p2[0] = x - i + 5;
				p2[1] = y + i - 5;
				return true;
			}
		}
		else if (color == W) {
			if (pieces[0] == W && pieces[1] == W && pieces[2] == W && pieces[3] == W) {
				p1[0] = x - i - 1;
				p1[1] = y + i + 1;
				p2[0] = x - i - 1 + 5;
				p2[1] = y + i + 1 - 5;
				return true;
			}
			if (pieces[1] == W && pieces[2] == W && pieces[3] == W && pieces[4] == W) {
				p1[0] = x - i;
				p1[1] = y + i;
				p2[0] = x - i + 5;
				p2[1] = y + i - 5;
				return true;
			}
		}
	}
	return false;
}

/**
* @brief       极大小树计算前的预处理 判断对手是否成四
* @param[in]   player           判断逻辑
*　　　  　    \ 1 电脑执黑，判断白子4连
*　　　  　    \ 2 电脑执白, 判断黑子4连
* @param[in]   board            棋盘
* @param[in]   x_last, y_last   对手(即玩家)上次落子位置
* @param[out]  max_place        电脑此次应该落子位置
*/
bool pre(int player, int board[][15], int x_last, int y_last, int * max_place) {
	int p1[2] = { 0 };
	int p2[2] = { 0 };
	if (player == 1) { //电脑执黑 检测白子的4连
		if (isFour(board, x_last, y_last, W, p1, p2)) {
			if (isOut(p1) && !isOut(p2) && board[p2[1]][p2[0]] == 0) {
				max_place[0] = p2[0];
				max_place[1] = p2[1];
				return true;
			}
			if (isOut(p2) && !isOut(p1) && board[p1[1]][p1[0]] == 0) {
				max_place[0] = p1[0];
				max_place[1] = p1[1];
				return true;
			}
			if (!isOut(p1) && board[p1[1]][p1[0]] == 0) {
				max_place[0] = p1[0];
				max_place[1] = p1[1];
				return true;
			}
			if (!isOut(p2) && board[p2[1]][p2[0]] == 0) {
				max_place[0] = p2[0];
				max_place[1] = p2[1];
				return true;
			}
		}
		return false;
	}


	if (player == 2) { //电脑执白 检测黑子的4连
		if (isFour(board, x_last, y_last, B, p1, p2)) {
			if (isOut(p1) && !isOut(p2) && board[p2[1]][p2[0]] == 0) {
				max_place[0] = p2[0];
				max_place[1] = p2[1];
				return true;
			}
			if (isOut(p2) && !isOut(p1) && board[p1[1]][p1[0]] == 0) {
				max_place[0] = p1[0];
				max_place[1] = p1[1];
				return true;
			}
			if (!isOut(p1) && board[p1[1]][p1[0]] == 0) {
				max_place[0] = p1[0];
				max_place[1] = p1[1];
				return true;
			}
			if (!isOut(p2) && board[p2[1]][p2[0]] == 0) {
				max_place[0] = p2[0];
				max_place[1] = p2[1];
				return true;
			}
		}
		return false;
	}
	return false;
}

/**
* @brief       极大小树计算前的预处理 判断自己是否成四
* @param[in]   player           判断逻辑
*　　　  　    \ 1 电脑执白，判断白子4连
*　　　  　    \ 2 电脑执黑, 判断黑子4连
* @param[in]   board            棋盘
* @param[in]   x_last, y_last   自己(即电脑)上次落子位置
* @param[out]  max_place        电脑此次应该落子位置
*/
bool pre2(int player, int board[][15], int x_last, int y_last, int * max_place) {
	int p1[2] = { 0 };
	int p2[2] = { 0 };
	if (player == 1) { //电脑执白 检测白子胜利
		if (isFour(board, x_last, y_last, W, p1, p2)) {
			if (isOut(p1) && !isOut(p2) && board[p2[1]][p2[0]] == 0) {
				max_place[0] = p2[0];
				max_place[1] = p2[1];
				return true;
			}
			if (isOut(p2) && !isOut(p1) && board[p1[1]][p1[0]] == 0) {
				max_place[0] = p1[0];
				max_place[1] = p1[1];
				return true;
			}
			if (!isOut(p1) && board[p1[1]][p1[0]] == 0) {
				max_place[0] = p1[0];
				max_place[1] = p1[1];
				return true;
			}
			if (!isOut(p2) && board[p2[1]][p2[0]] == 0) {
				max_place[0] = p2[0];
				max_place[1] = p2[1];
				return true;
			}
		}
		return false;
	}


	if (player == 2) { ////电脑执黑 检测黑子胜利
		if (isFour(board, x_last, y_last, B, p1, p2)) {
			if (isOut(p1) && !isOut(p2) && board[p2[1]][p2[0]] == 0) {
				max_place[0] = p2[0];
				max_place[1] = p2[1];
				return true;
			}
			if (isOut(p2) && !isOut(p1) && board[p1[1]][p1[0]] == 0) {
				max_place[0] = p1[0];
				max_place[1] = p1[1];
				return true;
			}
			if (!isOut(p1) && board[p1[1]][p1[0]] == 0) {
				max_place[0] = p1[0];
				max_place[1] = p1[1];
				return true;
			}
			if (!isOut(p2) && board[p2[1]][p2[0]] == 0) {
				max_place[0] = p2[0];
				max_place[1] = p2[1];
				return true;
			}
		}
		return false;
	}
	return false;
}

/**
* @brief       alpha-beta剪枝的极大级小搜索树
* @param[in]   player        固定填2 电脑执黑，电脑希望(黑-白)max
* @param[in]   board         棋盘
* @param[in]   alpha, beta   初始值分别为INT_MIN, INT_MAX
* @param[in]   step          当前思考深度 初始值为1
* @param[in]   steps         最大思考深度
* @param[out]  max_place     电脑此次应该落子位置
*/
double maxmin(int player, int board[][15], double alpha, double beta, int step, int max_step, int * max_place) {
	if (step > max_step) {
		return score(board, B) - score(board, W);
	}

	double best = 0.0;
	int F = 0;
	if (player == 1) { // MAX
		best = INT_MIN;
		for (int len = 0; len <= 7; ++len) { //棋盘遍历方式是由内圈向外圈遍历，直到某一圈找不到棋子
			int flag = 0;
			for (int y = MID - len; y <= MID + len; ++y) {
				for (int x = MID - len; x <= MID + len; ++x) {
					if (y != MID - len && y != MID + len) {
						if (x != MID - len && x != MID + len) continue;
					}
					if (board[y][x]) { //有子
						F = 1;
						flag = 1;
						for (int yy = y - 1; yy <= y + 1; ++yy) {
							for (int xx = x - 1; xx <= x + 1; ++xx) {
								if (xx == x && yy == y) continue; //当前点
								if (xx < 0 || xx > 14 || yy < 0 || yy > 14) continue; //出界
								if (board[yy][xx]) continue; //已有子
															 //if (isConsidered.find(yy * 15 + x) != isConsidered.end()) continue; //已考虑过
															 //generate a new board
								board[yy][xx] = 1;
								double cur = maxmin(2, board, alpha, beta, step + 1, max_step, max_place);
								board[yy][xx] = 0;
								if (cur > best) {
									best = cur;
									alpha = cur;
									max_place[0] = xx;
									max_place[1] = yy;
								}
								if (alpha > beta) return alpha;
							}
						}

					}
				}
			}
			if (F == 1 && flag == 0) {
				return best;
			}
		}
		return best;
	}

	else if (player == 2) { // MIN
		best = INT_MAX;
		for (int len = 0; len <= 7; ++len) {
			int flag = 0;
			for (int y = MID - len; y <= MID + len; ++y) {
				for (int x = MID - len; x <= MID + len; ++x) {
					if (y != MID - len && y != MID + len) {
						if (x != MID - len && x != MID + len) continue;
					}
					if (board[y][x]) {
						F = 1;
						flag = 1;
						for (int yy = y - 1; yy <= y + 1; ++yy) {
							for (int xx = x - 1; xx <= x + 1; ++xx) {
								if (xx == x && yy == y) continue; //当前点
								if (xx < 0 || xx > 14 || yy < 0 || yy > 14) continue; //出界
								if (board[yy][xx]) continue; //已有子
															 //if (isConsidered.find(yy * 15 + x) != isConsidered.end()) continue; //已考虑过
															 //generate a new board
								board[yy][xx] = 2;
								double cur = maxmin(1, board, alpha, beta, step + 1, max_step, max_place);
								board[yy][xx] = 0;
								if (cur < best) {
									best = cur;
									beta = cur;
								}
								if (alpha > beta) return beta;
							}
						}

					}
				}
			}
			if (F == 1 && flag == 0) {
				return best;
			}
		}
		return best;
	}

}

/**
* @brief       alpha-beta剪枝的极大级小搜索树
* @param[in]   player        固定填2 电脑执白，电脑希望(白-黑)max
* @param[in]   board         棋盘
* @param[in]   alpha, beta   初始值分别为INT_MIN, INT_MAX
* @param[in]   step          当前思考深度 初始值为1
* @param[in]   steps         最大思考深度
* @param[out]  max_place     电脑此次应该落子位置
*/
double maxmin2(int player, int board[][15], double alpha, double beta, int step, int max_step, int * max_place) {
	//电脑后手 执白
	if (step > max_step) {
		return score(board, W) - score(board, B);
	}

	double best = 0.0;
	int F = 0;
	if (player == 1) { // MIN
		best = INT_MAX;
		for (int len = 0; len <= 7; ++len) { ////棋盘遍历方式是由内圈向外圈遍历，直到某一圈找不到棋子
			int flag = 0;
			for (int y = MID - len; y <= MID + len; ++y) {
				for (int x = MID - len; x <= MID + len; ++x) {
					if (y != MID - len && y != MID + len) {
						if (x != MID - len && x != MID + len) continue;
					}
					if (board[y][x]) { //有子
						F = 1;
						flag = 1;
						for (int yy = y - 1; yy <= y + 1; ++yy) {
							for (int xx = x - 1; xx <= x + 1; ++xx) {
								if (xx == x && yy == y) continue; //当前点
								if (xx < 0 || xx > 14 || yy < 0 || yy > 14) continue; //出界
								if (board[yy][xx]) continue; //已有子
															 //if (isConsidered.find(yy * 15 + x) != isConsidered.end()) continue; //已考虑过
															 //generate a new board
								board[yy][xx] = 1;
								double cur = maxmin(2, board, alpha, beta, step + 1, max_step, max_place);
								board[yy][xx] = 0;
								if (cur < best) {
									best = cur;
									beta = cur;
								}
								if (alpha > beta) return beta;
							}
						}

					}
				}
			}
			if (F == 1 && flag == 0) {
				return best;
			}
		}
		return best;
	}

	else if (player == 2) { // MAX

		best = INT_MIN;
		for (int len = 0; len <= 7; ++len) {
			int flag = 0;
			for (int y = MID - len; y <= MID + len; ++y) {
				for (int x = MID - len; x <= MID + len; ++x) {
					if (y != MID - len && y != MID + len) {
						if (x != MID - len && x != MID + len) continue;
					}
					if (board[y][x]) {
						F = 1;
						flag = 1;
						for (int yy = y - 1; yy <= y + 1; ++yy) {
							for (int xx = x - 1; xx <= x + 1; ++xx) {
								if (xx == x && yy == y) continue; //当前点
								if (xx < 0 || xx > 14 || yy < 0 || yy > 14) continue; //出界
								if (board[yy][xx]) continue; //已有子
															 //if (isConsidered.find(yy * 15 + x) != isConsidered.end()) continue; //已考虑过
															 //generate a new board
								board[yy][xx] = 2;
								double cur = maxmin(1, board, alpha, beta, step + 1, max_step, max_place);
								board[yy][xx] = 0;
								if (cur > best) {
									best = cur;
									alpha = cur;
									max_place[0] = xx;
									max_place[1] = yy;
								}
								if (alpha > beta) return alpha;
							}
						}

					}
				}
			}
			if (F == 1 && flag == 0) {
				return best;
			}
		}
		return best;
	}

}

//前端
//欢迎页
void ini() {
	initgraph(600, 480);
	setbkcolor(YELLOW);
	cleardevice();
	RECT r = { 600 / 2 - 150, 480 / 2 - 100, 600 / 2 + 150, 480 / 2 - 100 + 40 };
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 30;
	settextstyle(&f);
	settextcolor(BLACK);
	drawtext("五子棋", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r0 = { 600 / 2 - 150, 455 - 230, 600 / 2 + 150, 471 - 230 };
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 20;
	settextstyle(&f);
	settextcolor(BLACK);
	drawtext("08116124 杨雨尧", &r0, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r1 = { 420, 455 - 100, 580, 471 - 100 };
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 20;
	settextstyle(&f);
	settextcolor(BLACK);
	drawtext("按任意键继续...", &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
//执方选择页
void chooseColor() {
	setbkcolor(YELLOW);
	cleardevice();
	RECT r = { 600 / 2 - 150, 480 / 2 - 100, 600 / 2 + 150, 480 / 2 - 100 + 40 };
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 30;
	settextstyle(&f);
	settextcolor(BLACK);
	drawtext("请选择执方", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setfillcolor(WHITE);
	solidrectangle(600 / 2 - 50, 480 / 2 - 40, 600 / 2 + 50, 480 / 2 - 40 + 40);
	RECT r1 = { 600 / 2 - 35, 480 / 2 - 40, 600 / 2 + 35, 480 / 2 - 40 + 40 };
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 25;
	settextstyle(&f);
	settextcolor(BLACK);
	setbkcolor(WHITE);
	drawtext(_T("先手(黑)"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setbkcolor(YELLOW);

	solidrectangle(600 / 2 - 50, 480 / 2 + 20, 600 / 2 + 50, 480 / 2 + 20 + 40);
	RECT r2 = { 600 / 2 - 35, 480 / 2 + 20, 600 / 2 + 35, 480 / 2 + 20 + 40 };
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 25;
	settextstyle(&f);
	settextcolor(BLACK);
	setbkcolor(WHITE);
	drawtext(_T("后手(白)"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setbkcolor(YELLOW);
}
//AI思考深度选择页
void chooseDeep() {
	setbkcolor(YELLOW);
	cleardevice();
	RECT r = { 600 / 2 - 150, 480 / 2 - 100, 600 / 2 + 150, 480 / 2 - 100 + 40 };
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 30;
	settextstyle(&f);
	settextcolor(BLACK);
	drawtext("AI思考深度", &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setfillcolor(WHITE);
	solidrectangle(600 / 2 - 30 - 30 - 60, 480 / 2 - 40, 600 / 2 - 30 - 30, 480 / 2 - 40 + 40);
	RECT r1 = { 600 / 2 - 30 - 30 - 60, 480 / 2 - 40, 600 / 2 - 30 - 30, 480 / 2 - 40 + 40 };
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 25;
	settextstyle(&f);
	settextcolor(BLACK);
	setbkcolor(WHITE);
	drawtext(_T("3"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setbkcolor(YELLOW);

	setfillcolor(WHITE);
	solidrectangle(600 / 2 - 30, 480 / 2 - 40, 600 / 2 + 30, 480 / 2 - 40 + 40);
	RECT r2 = { 600 / 2 - 30, 480 / 2 - 40, 600 / 2 + 30, 480 / 2 - 40 + 40 };
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 25;
	settextstyle(&f);
	settextcolor(BLACK);
	setbkcolor(WHITE);
	drawtext(_T("4"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setbkcolor(YELLOW);

	setfillcolor(WHITE);
	solidrectangle(600 / 2 + 30 + 30, 480 / 2 - 40, 600 / 2 + 30 + 30 + 60, 480 / 2 - 40 + 40);
	RECT r3 = { 600 / 2 + 30 + 30, 480 / 2 - 40, 600 / 2 + 30 + 30 + 60, 480 / 2 - 40 + 40 };
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfHeight = 25;
	settextstyle(&f);
	settextcolor(BLACK);
	setbkcolor(WHITE);
	drawtext(_T("5"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setbkcolor(YELLOW);


}
//棋盘主界面
void mainBoard() {
	IMAGE img1(452, 452);
	setbkcolor(WHITE);
	cleardevice();
	loadimage(&img1, _T("board3.png"), 452, 452, false);
	putimage(10, 10, &img1);

	//reload
	setlinecolor(RED);
	rectangle(500, 380, 570, 410);
	RECT r2 = { 500, 380, 570, 410 };
	drawtext(_T("重新开始"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//end
	setlinecolor(RED);
	rectangle(500, 420, 570, 450);
	RECT r3 = { 500, 420, 570, 450 };
	drawtext(_T("结束"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//yyy
	RECT r4 = { 420, 463, 570, 479 };
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 10;
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	drawtext(_T("08116124 杨雨尧"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}
//在执方选择页：输入鼠标坐标 返回鼠标所在区域
int where_chooseColor(int x, int y) {
	if (x >= 600 / 2 - 35 && x <= 600 / 2 + 35 && y >= 480 / 2 - 40 && y <= 480 / 2 - 40 + 40) return inB;
	else if (x >= 600 / 2 - 35 && x <= 600 / 2 + 35 && y >= 480 / 2 + 20 && y <= 480 / 2 + 20 + 40) return inW;
}
//在AI思考深度选择页：输入鼠标坐标 返回鼠标所在区域
int where_chooseDeep(int x, int y) {
	if (x >= 600 / 2 - 30 - 30 - 60 && x <= 600 / 2 - 30 - 30 && y >= 480 / 2 - 40 && y <= 480 / 2 - 40 + 40) return in3;
	else if (x >= 600 / 2 - 30 && x <= 600 / 2 + 30 && y >= 480 / 2 - 40 && y <= 480 / 2 - 40 + 40) return in4;
	else if (x >= 600 / 2 + 30 + 30 && x <= 600 / 2 + 30 + 30 + 60 && y >= 480 / 2 - 40 && y <= 480 / 2 - 40 + 40) return in5;
}
//在棋盘主界面：输入鼠标坐标 返回鼠标所在区域
int where_main(int x, int y) {
	if (x >= 10 && x < 10 + 452 && y >= 10 && y < 10 + 452) return inBoard;
	else if (x >= 500 && x <= 570 && y >= 380 && y <= 410) return inReload;
	else if (x >= 500 && x <= 570 && y >= 420 && y <= 450) return inEnd;
}
//鼠标坐标(x_in, y_in) 转化为 15*15棋盘上的索引 x_out = 0,1,2, ..., 14 y_out = 0,1,2, ..., 14
void getPoint(int x_in, int y_in, int &x_out, int &y_out) { // 0-14
	double lim = 0.33;
	double y_delta = (double)(y_in - 27) / 30.0;
	double x_delta = (double)(x_in - 27) / 30.0;
	y_out = round(y_delta);
	x_out = round(x_delta);
}
//15*15棋盘上的索引(x, y) 转化为 输出设备上的棋子位置坐标(x_out, y_out)
void getCor(int x, int y, int &x_out, int &y_out) {
	x_out = 27 + x * 30;
	y_out = 27 + y * 30;
}
//显示提示文字
void hint(std::string str) {
	RECT r1 = { 500, 300, 570, 330 };
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 10;
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	//setbkcolor(WHITE);
	drawtext(str.c_str(), &r1, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}
//清除提示文字
void clearHint() {
	setbkcolor(WHITE);
	clearrectangle(500, 300, 570, 330);
}

int main() {
	int	board[15][15] = { 0 }; 
	int max_place[2] = { 0 }; //电脑落子坐标
	//15*15棋盘上的索引
	int x = 0;
	int y = 0;
	//输出设备上的坐标
	int x_cor = 0;
	int y_cor = 0;
	int playerColor = 0; //玩家的执方
	int deep = 3; //AI思考深度
	int isReload = 0; //对"重新开始"的判断

	//欢迎页
	ini();
	MOUSEMSG m;
	while (true) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			break;
		}
	}

	while (true) {
		//执方选择
		chooseColor();
		while (true) {
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (where_chooseColor(m.x, m.y) == inB) {
					playerColor = B;
					break;
				}
				else if (where_chooseColor(m.x, m.y) == inW) {
					playerColor = W;
					break;
				}
			}
		}

		//AI思考深度选择
		chooseDeep();
		while (true) {
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (where_chooseDeep(m.x, m.y) == in3) {
					deep = 3;
					break;
				}
				else if (where_chooseDeep(m.x, m.y) == in4) {
					deep = 4;
					break;
				}
				else if (where_chooseDeep(m.x, m.y) == in5) {
					deep = 5;
					break;
				}
			}
		}

		//棋盘主界面
		mainBoard();
		if (playerColor == B) { //玩家先手
			while (true) {
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN) {
					if (where_main(m.x, m.y) == inBoard) {
						getPoint(m.x, m.y, x, y);
						getCor(x, y, x_cor, y_cor);
						setfillcolor(BLACK);
						solidcircle(x_cor, y_cor, 8);
						board[y][x] = 1;
						if (isWin(board, x, y, B)) {
							clearHint();
							hint("黑子胜！");
							break;
						}
						hint("思考中...");
						if (!pre2(1, board, max_place[0], max_place[1], max_place)) {
							if(!pre(2, board, x, y, max_place)) maxmin2(2, board, INT_MIN, INT_MAX, 1, deep, max_place);
						}
						getCor(max_place[0], max_place[1], x_cor, y_cor);
						setfillcolor(WHITE);
						solidcircle(x_cor, y_cor, 8);
						board[max_place[1]][max_place[0]] = 2;
						clearHint();
						if (isWin(board, max_place[0], max_place[1], W)) {
							clearHint();
							hint("白子胜！");
							break;
						}
						clearHint();
						hint("该您了");
					}
					else if (where_main(m.x, m.y) == inReload) {
						isReload = 1;
						break;
					}
					else if (where_main(m.x, m.y) == inEnd) {
						closegraph();
						return 0;
					}

				}
			}
		}
		else if (playerColor == W) { //玩家后手
			//通过随机数产生先手落子地点
			srand((unsigned int)time(NULL));
			max_place[0] = 7 + (rand() % 3 - 1);
			max_place[1] = 7 + (rand() % 3 - 1);
			getCor(max_place[0], max_place[1], x_cor, y_cor);
			board[max_place[1]][max_place[0]] = 1;
			setfillcolor(BLACK);
			solidcircle(x_cor, y_cor, 8);
			while (true) {
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN) {
					if (where_main(m.x, m.y) == inBoard) {
						getPoint(m.x, m.y, x, y);
						getCor(x, y, x_cor, y_cor);
						setfillcolor(WHITE);
						solidcircle(x_cor, y_cor, 8);
						board[y][x] = 2;
						if (isWin(board, x, y, W)) {
							clearHint();
							hint("白子胜！");
							break;
						}
						hint("思考中...");
						if (!pre2(2, board, max_place[0], max_place[1], max_place)) {
							if (!pre(1, board, x, y, max_place)) {
								maxmin(1, board, INT_MIN, INT_MAX, 1, deep, max_place);
							}
						}
						getCor(max_place[0], max_place[1], x_cor, y_cor);
						setfillcolor(BLACK);
						solidcircle(x_cor, y_cor, 8);
						board[max_place[1]][max_place[0]] = 1;
						if (isWin(board, max_place[0], max_place[1], B)) {
							clearHint();
							hint("黑子胜！");
							break;
						}
						clearHint();
						hint("该您了");
					}
					else if (where_main(m.x, m.y) == inReload) {
						isReload = 1;
						break;
					}
					else if (where_main(m.x, m.y) == inEnd) {
						closegraph();
						return 0;
					}
				}
			}
		}

		if (!isReload) { //某一方获胜了
			while (true) {
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN) {
					if (where_main(m.x, m.y) == inReload) {
						isReload = 1;
						break;
					}
					else if (where_main(m.x, m.y) == inEnd) {
						closegraph();
						return 0;
					}
				}
			}
		}

		if (isReload) { //检测到玩家点击"重新开始"
			for (int i = 0; i < 15; ++i) {
				for (int j = 0; j < 15; ++j) {
					board[i][j] = 0;
				}
			}
			max_place[0] = 0;
			max_place[1] = 0;
			x = 0;
			y = 0;
			playerColor = 0;
			deep = 3;
			x_cor = 0;
			y_cor = 0;
			isReload = 0;
		}

	}


	getchar();
	return 0;




}
