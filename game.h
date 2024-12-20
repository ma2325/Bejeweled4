#ifndef GAME_H
#define GAME_H

#include "globalvalue.h"
#include "stonelabel.h"
#include "gametimer.h"
#include "pause.h"
#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <iostream>
#include <QProgressDialog>
#include <QProgressBar>
// 使用前向声明 GameItems 类，而不是包含完整的头文件
class GameItems;

QT_BEGIN_NAMESPACE
namespace Ui {
class Game;
}
QT_END_NAMESPACE

class Game : public QWidget
{
    Q_OBJECT

public:
    ~Game();
    static Game* instance(QWidget* parent = nullptr);
    void init();
    void update();
    void handleStoneSwap(int row, int col, StoneLabel* curLabel);
    void clearStone(int row, int col);  // 清除指定位置的宝石
    StoneLabel* getStone(int row, int col);  // 获取指定位置的宝石
    static void delInstance(){
        gameInstance = nullptr;
    }
    bool isPaused = false;
    static int jewelNum;
    QWidget* parent;

    GameTimer *gameTimer;  // 计时器
   //void setSelectedPosition(int row, int col);

    int getScore();  //获取当前积分值

signals:
    void eliminateAgainSignal();
    void returnMainwindow();
    void initEndSignal();

private slots:
    void initEnd(){
        this->progressDialog->setValue(100);
        this->progressDialog->hide();
    }

    void onDropAnimationFinished() {
        animationsLeft--;

        if (animationsLeft == 0) {
            qDebug() << "OK";
            creatstones();
            this->initing = false;
        }
    }

    void onEliminateAgain() {
        if (checkFormatches()) {
            eliminateMatches();
        } else {
            int row1 = swapReturn[0], col1 = swapReturn[1], row2 = swapReturn[2], col2 = swapReturn[3];
            std::cout << "row1:" << row1 << ", row2:" << row2 << ", col1:" << col1 << ", col2:" << col2 << std::endl;
            std::swap(stones[row1][col1], stones[row2][col2]);
            stones[row1][col1]->setrow(row1);
            stones[row1][col1]->setcol(col1);
            stones[row2][col2]->setrow(row2);
            stones[row2][col2]->setcol(col2);
            emit initEndSignal();
        }
    }

    void on_pushButton_3_clicked();
    void resume();  // 处理继续游戏信号
    void on_returnFromPauseToMainMenu();  // 处理从暂停界面返回主菜单的信号
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void on_bombButton_clicked();
  //  void on_rainbowGemButton_clicked();
    //void on_freezeTimeButton_clicked();

private:
    explicit Game(QWidget *parent = nullptr);
    static Game* gameInstance;
    void mousePressEvent(QMouseEvent *event) override;
    bool checkFormatches();  // 判断哪些棋子将要被消去
    void eliminateMatches();
    void dropStones();  // 棋子下落，创建新子
    void dropLabel(StoneLabel* stoneLabel, int startX, int startY, int targetX, int targetY, int duration);  // 棋子下落动画
    void resetMatchedFlags();  // 重置所有棋子为不可消除
    void generateNewStone(int row, int col);  // 创建一个新子
    void creatstones();  // 创建所有需要的子
    void shuffleStones();  // 重排布
    GameItems *gameItems;  // 引入 GameItems 对象（已前向声明，不再需要包含头文件）

    QProgressBar *progressBar;  // 计时进度条
    void onTimeExpired();  // 倒计时结束时的处理
    void updateTimerDisplay();  // 更新界面上显示倒计时的QLabel的文本内容
    int animationsLeft;  // 重置动画计数器
    Pause *pause;  // 暂停界面指针
    void resetGameState();  // 用于重置游戏状态
    bool change = false;
    bool eliminateAgain = true;
    std::vector<int> swapReturn;
    QProgressDialog *progressDialog;
    bool initing;
    Ui::Game *ui;
    int score=0;  //记录游戏当前积分
    bool hasStartedScoring=false;  //标记是否可以开始计分，初始化为false，表示未开始计分
    int winScore;
    void triggerBomb(int row, int col);
    bool isBombMode = false;  // 标记是否处于炸弹模式


};

#endif // GAME_H
