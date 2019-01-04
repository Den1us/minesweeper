#pragma once 
class MineField
{
private:
	class Tile//���� ������� ����
	{
	private:
		enum State{Hidden,Flagged,Revealed};//��������� ���������
		bool bomb;//������� ����� � ��������
		int curState;//������� ���������
	public:
		Tile();
		void SetBomb(bool has);
		void SetCurState(int State);
		int GetCurState();
		bool HasBomb();
	};
	struct Player//��������� ������ �� ����
	{
		int posX;
		int posY;
	};
	Player player;//�����
	int width;//������ ����
	int height;//������
	int bombNumber;//���-�� ���� �� ����
	int curProgress;//������� �������� �������� ����
	Tile **field;//������� width*height
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