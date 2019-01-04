#pragma once 
class MineField
{
private:
	class Tile//один квадрат поля
	{
	private:
		enum State{Hidden,Flagged,Revealed};//возможные состояния
		bool bomb;//наличие бомбы в квадрате
		int curState;//текущее состояние
	public:
		Tile();
		void SetBomb(bool has);
		void SetCurState(int State);
		int GetCurState();
		bool HasBomb();
	};
	struct Player//положение игрока на поле
	{
		int posX;
		int posY;
	};
	Player player;//игрок
	int width;//ширина поля
	int height;//высота
	int bombNumber;//кол-во бомб на поле
	int curProgress;//текущий прогресс открытия поля
	Tile **field;//матрица width*height
	void DownRight(int&playerPos,int edge);
	void UpLeft(int&playerPos,int edge);
	void ChangePlayerPos();
	void ChangePlayerPos(short x,short y);
	int CheckNeighboringMines(int x,int y);
	void RevealRegion(int curx,int cury);
	bool OnField(int x,int y);
	void EndGameFailure();
	void EndGameSuccess();
	void SetColor(int text,int background);
public:
	MineField(int width,int height,int bombNumber);
	void StartGame();
	void GenerateBombs();
	void DisplayField();
	void SetPlayer();
	~MineField();
};