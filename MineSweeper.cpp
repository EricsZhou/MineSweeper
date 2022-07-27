// MineSweeper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <easyx.h>
/*
    如何写一个游戏？easyx图形库
    1，创建好对应的窗口initgraph(w,h,flag)
    2，根据界面分析，可以得出界面上的元素都是通过数组存储的（二维数组）
    2.1埋雷：随机产生雷,反到数组里面去（注意重复问题）
    2.2添加数字：让以雷为中心的九宫格的数字都加1操作（雷要除外）
    3,把数据和界面建立关联
    4,鼠标操作，打开格子，标记格子
    5,如果打开格子的时候，点到的是空白，一片一片的炸开
*/
#define Row 10  //分析我们的游戏可以得到我们的游戏界面是10行10列的图形框组成
#define Col 10
void OpenNull(int row, int col);
int map[Row][Col];
IMAGE img[12];  //定义图片数组，保存所有的图片

void show()
{
    for (int i = 0; i < Row; i++)
    {
        for (int k = 0; k < Col; k++)
        {
            printf("%2d ", map[i][k]);
        }
        printf("\n");
    }
}

void gameInit()
{
    //设置随机数种子
    srand((unsigned)time(NULL));
    //随机产生12个雷
    for (int i = 0; i < 23; )
    {
        int r = rand() % Row;   //[0-10]
        int c = rand() % Col;
        //确保一定埋雷成功不出现重复埋雷，雷少的事件发生
        if (map[r][c]==0)
        {
            map[r][c] = -1;
            i++;
        }
    }
    //处理雷周围的数据
    for (int i = 0; i < Row; i++)
    {
        for (int k = 0; k < Col; k++)
        {
            if (map[i][k] == -1)
            {
                //找到以雷为中心的九宫格
                for (int r = i-1; r < i+1; r++)
                {
                    for (int c = k - 1; c < k + 1; c++)
                    {
                        if ((r>=0 && r<Row && c>=0 && c<Col ) &&map[r][c] != -1)    //排除雷
                        {
                            map[r][c]++;
                        }
                    }
                }
            }
        }
    }
    //加载图片
    for (int i = 0; i < 12; i++)
    {
        char fileName[20] = "";
        sprintf_s(fileName, "./image/%d.png", i);
        loadimage(img+i, fileName, 40, 40);
        //putimage(40*i, 0, img);
    }   
    //数据加密（遮罩数据）
    for (int i = 0; i < Row; i++)
    {
        for (int k = 0; k < Col; k++)
        {
            map[i][k] += 20;
        }
    }
}

void gameDraw()
{
    for (int i = 0; i < Row; i++)   //i对应着y
    {
        for (int k = 0; k < Col; k++)   //k对应着x
        {
            int x = k * 40;
            int y = i * 40;
            if (map[i][k] >= 0 && map[i][k] <= 8)
            {
                putimage(x, y, img+ map[i][k]);
            }
           // putimage(x, y, img);
            else if (map[i][k]==-1)
            {
                putimage(x, y, img + 9);
                exit(0);
            }
            else if (map[i][k]>=19&&map[i][k]<=28)  //遮罩图
            {
                putimage(x, y, img + 10);
            }
            else if (map[i][k]>=39)
            {
                putimage(x, y, img + 11);
            }
        }
    }
}

void MouseEvent()
{
    //获取鼠标消息
    ExMessage msg = getmessage();
    //上面我们把下标转成了坐标，那么现在我们要反过来，把坐标转化成对应的数组下标
    int row = msg.y / 40;
    int col = msg.x / 40;
    //如果左键点击了格子，那么没有打开就打开
    if (msg.lbutton)
    {
        //判断格子有没有打开
        if (map[row][col] >= 19)
        {
            map[row][col] -= 20;
            OpenNull(row, col);
        }
    }
    //右击格子标记为雷，再让数据加20
    else if (msg.rbutton)
    {
        //已经被标记了
        if (map[row][col] >= 19 && map[row][col] <= 28)
        {
            map[row][col] += 20;
         }
    }
}

void OpenNull(int row,int col)
{
    //如果点击的是空白
    if (map[row][col] == 0)
    {
        for (int i = row -1; i <= row+1; i++)
        {
            for (int k = col-1; k <= col+1; k++)
            {
                if ((i >= 0 && i < Row && k >= 0 && k < Col) 
                    && map[i][k] > 19)
                {
                    map[i][k] -= 20;
                    OpenNull(i, k);
                }
            }
        }
    }
}

int main()
{
    //std::cout << "Hello World!\n";
    initgraph(Row*40, Col*40,EW_SHOWCONSOLE);
    gameInit();
    show();
    while (true)
    {
        gameDraw();
        MouseEvent();
    }
    //gameDraw();
    getchar();//防止闪退
    return 0;
}


