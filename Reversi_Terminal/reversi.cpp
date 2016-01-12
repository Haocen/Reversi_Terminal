#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <memory>

using namespace std;

class Move
{
private:
	char side = NULL;
	int x = 0;
	int y = 0;

public:
	Move(char setside = NULL, int setx = 0, int sety = 0)
	{
		side = setside;
		x = setx;
		y = sety;
	}

	Move(const Move& other)
	{
		side = other.side;
		x = other.x;
		y = other.y;
	}

	Move(Move && other)
	{
		swap(side, other.side);
		swap(x, other.x);
		swap(y, other.y);
	}

	~Move()
	{
		//Nothing to do
	}

	Move& operator=(const Move & other)
	{
		side = other.side;
		x = other.x;
		y = other.y;
		return *this;
	}

	Move& operator=(Move && other)
	{
		side = other.side;
		x = other.x;
		y = other.y;
		return *this;
	}

	bool operator==(const Move & other)
	{
		if (side == other.side&&x == other.x&&y == other.y)
			return true;
		else
			return false;
	}

	bool operator!=(const Move & other)
	{
		return !this->operator==(other);
	}

	char getSide()
	{
		return side;
	}

	int getX()
	{
		return x;
	}

	int getY()
	{
		return y;
	}
};

/*ostream & operator<<(ostream & stream, Move move)
{
	stream << "Chess=" << move.getSide() << " X=" << move.getX() << " Y=" << move.getY();
	return stream;
}*/

class ChessBoard
{
private:
	vector< vector<char> > chessboard;

	Move searchL(Move & obj)
	{
		//Search all the same on the left, return last same chess on the left.
		shared_ptr<Move> left_corner;
		for (int i = obj.getX(); i >= 1; i--)
		{
			if (chessboard[i - 1][obj.getY() - 1] == obj.getSide())
				left_corner = make_shared<Move>(obj.getSide(), i, obj.getY());
			else
				break;
		}
		return *left_corner;
	}

	Move searchNL(Move & obj)
	{
		//Search all the opponent on the left, return first same chess on the left.
		if (obj.getX() < 3)
			return obj;
		else if (chessboard[obj.getX() - 2][obj.getY() - 1] == ' ')
			return obj;
		else
		{
			Move left_opp = searchL(Move(chessboard[obj.getX() - 2][obj.getY() - 1], obj.getX() - 1, obj.getY()));
			if (left_opp.getX() > 1 && chessboard[left_opp.getX() - 2][left_opp.getY() - 1] == obj.getSide())
			{
				return Move(obj.getSide(), left_opp.getX() - 1, left_opp.getY());
			}
			else
				return obj;
		}
	}

	Move searchR(Move & obj)
	{
		//Search all the same on the right, return last same chess on the right.
		shared_ptr<Move> right_corner;
		for (int i = obj.getX(); i <= chessboard.size(); i++)
		{
			if (chessboard[i - 1][obj.getY() - 1] == obj.getSide())
				right_corner = make_shared<Move>(obj.getSide(), i, obj.getY());
			else
				break;
		}
		return *right_corner;
	}

	Move searchNR(Move & obj)
	{
		//Search all the opponent on the right, return first same chess on the right.
		if (obj.getX() > chessboard.size() - 2)
			return obj;
		else if (chessboard[obj.getX()][obj.getY() - 1] == ' ')
			return obj;
		else
		{
			Move right_opp = searchR(Move(chessboard[obj.getX()][obj.getY() - 1], obj.getX() + 1, obj.getY()));
			if (right_opp.getX() < chessboard.size() && chessboard[right_opp.getX()][right_opp.getY() - 1] == obj.getSide())
			{
				return Move(obj.getSide(), right_opp.getX() + 1, right_opp.getY());
			}
			else
				return obj;
		}
	}

	Move searchT(Move & obj)
	{
		//Search all the same on the top, return last same chess on the top.
		shared_ptr<Move> top_corner;
		for (int j = obj.getY(); j >= 1; j--)
		{
			if (chessboard[obj.getX() - 1][j - 1] == obj.getSide())
				top_corner = make_shared<Move>(obj.getSide(), obj.getX(), j);
			else
				break;
		}
		return *top_corner;
	}

	Move searchNT(Move & obj)
	{
		//Search all the opponent on the top, return first same chess on the top.
		if (obj.getY() < 3)
			return obj;
		else if (chessboard[obj.getX() - 1][obj.getY() - 2] == ' ')
			return obj;
		else
		{
			Move top_opp = searchT(Move(chessboard[obj.getX() - 1][obj.getY() - 2], obj.getX(), obj.getY() - 1));
			if (top_opp.getY() > 1 && chessboard[top_opp.getX() - 1][top_opp.getY() - 2] == obj.getSide())
			{
				return Move(obj.getSide(), top_opp.getX(), top_opp.getY() - 1);
			}
			else
				return obj;
		}
	}

	Move searchB(Move & obj)
	{
		//Search all the same on the bottom, return last same chess on the bottom.
		shared_ptr<Move> bottom_corner;
		for (int j = obj.getY(); j <= chessboard[0].size(); j++)
		{
			if (chessboard[obj.getX() - 1][j - 1] == obj.getSide())
				bottom_corner = make_shared<Move>(obj.getSide(), obj.getX(), j);
			else
				break;
		}
		return *bottom_corner;
	}

	Move searchNB(Move & obj)
	{
		//Search all the opponent on the bottom, return first same chess on the bottom.
		if (obj.getY() > chessboard[0].size() - 2)
			return obj;
		else if (chessboard[obj.getX() - 1][obj.getY()] == ' ')
			return obj;
		else
		{
			Move bottom_opp = searchB(Move(chessboard[obj.getX() - 1][obj.getY()], obj.getX(), obj.getY() + 1));
			if (bottom_opp.getY() < chessboard[0].size() && chessboard[bottom_opp.getX() - 1][bottom_opp.getY()] == obj.getSide())
			{
				return Move(obj.getSide(), bottom_opp.getX(), bottom_opp.getY() + 1);
			}
			else
				return obj;
		}
	}

	Move searchNTL(Move & obj)
	{
		//Search all the opponent on the topleft, return first same chess on the topleft.
		shared_ptr<Move> topleft_opp = nullptr;
		int limit = 0;
		if ((obj.getX() - 1) <= (obj.getY() - 1))
			limit = obj.getX() - 1;
		else
			limit = obj.getY() - 1;
		for (int n = 1; n <= limit; n++)
		{
			if (chessboard[obj.getX() - 1 - n][obj.getY() - 1 - n] != obj.getSide() && chessboard[obj.getX() - 1 - n][obj.getY() - 1 - n] != ' ')
			{
				topleft_opp = make_shared<Move>(chessboard[obj.getX() - 1 - n][obj.getY() - 1 - n], obj.getX() - n, obj.getY() - n);
			}
			else
				break;
		}
		if (topleft_opp != nullptr && topleft_opp->getX() > 1 && topleft_opp->getY() > 1 && chessboard[topleft_opp->getX() - 2][topleft_opp->getY() - 2] == obj.getSide())
		{
			return Move(obj.getSide(), topleft_opp->getX() - 1, topleft_opp->getY() - 1);
		}
		else
			return obj;
	}

	Move searchNTR(Move & obj)
	{
		//Search all the opponent on the topright, return first same chess on the topright.
		shared_ptr<Move> topright_opp = nullptr;
		int limit = 0;
		if ((chessboard.size() - obj.getX()) <= (obj.getY() - 1))
			limit = chessboard.size() - obj.getX();
		else
			limit = obj.getY() - 1;
		for (int n = 1; n <= limit; n++)
		{
			if (chessboard[obj.getX() - 1 + n][obj.getY() - 1 - n] != obj.getSide() && chessboard[obj.getX() - 1 + n][obj.getY() - 1 - n] != ' ')
			{
				topright_opp = make_shared<Move>(chessboard[obj.getX() - 1 + n][obj.getY() - 1 - n], obj.getX() + n, obj.getY() - n);
			}
			else
				break;
		}
		if (topright_opp != nullptr && topright_opp->getX() < chessboard.size() && topright_opp->getY() > 1 && chessboard[topright_opp->getX()][topright_opp->getY() - 2] == obj.getSide())
		{
			return Move(obj.getSide(), topright_opp->getX() + 1, topright_opp->getY() - 1);
		}
		else
			return obj;
	}

	Move searchNBL(Move & obj)
	{
		//Search all the opponent on the bottomleft, return first same chess on the bottomleft.
		shared_ptr<Move> bottomleft_opp = nullptr;
		int limit = 0;
		if ((obj.getX() - 1) <= (chessboard[0].size() - obj.getY()))
			limit = obj.getX() - 1;
		else
			limit = chessboard[0].size() - obj.getY();
		for (int n = 1; n <= limit; n++)
		{
			if (chessboard[obj.getX() - 1 - n][obj.getY() - 1 + n] != obj.getSide() && chessboard[obj.getX() - 1 - n][obj.getY() - 1 + n] != ' ')
			{
				bottomleft_opp = make_shared<Move>(chessboard[obj.getX() - 1 - n][obj.getY() - 1 + n], obj.getX() - n, obj.getY() + n);
			}
			else
				break;
		}
		if (bottomleft_opp != nullptr && bottomleft_opp->getX() > 1 && bottomleft_opp->getY() < chessboard[0].size() && chessboard[bottomleft_opp->getX() - 2][bottomleft_opp->getY()] == obj.getSide())
		{
			return Move(obj.getSide(), bottomleft_opp->getX() - 1, bottomleft_opp->getY() + 1);
		}
		else
			return obj;
	}

	Move searchNBR(Move & obj)
	{
		//Search all the opponent on the bottomright, return first same chess on the bottomright.
		shared_ptr<Move> bottomright_opp = nullptr;
		int limit = 0;
		if ((chessboard.size() - obj.getX()) <= (chessboard[0].size() - obj.getY()))
			limit = chessboard.size() - obj.getX();
		else
			limit = chessboard[0].size() - obj.getY();
		for (int n = 1; n <= limit; n++)
		{
			if (chessboard[obj.getX() - 1 + n][obj.getY() - 1 + n] != obj.getSide() && chessboard[obj.getX() - 1 + n][obj.getY() - 1 + n] != ' ')
			{
				bottomright_opp = make_shared<Move>(chessboard[obj.getX() - 1 + n][obj.getY() - 1 + n], obj.getX() + n, obj.getY() + n);
			}
			else
				break;
		}
		if (bottomright_opp!=nullptr && bottomright_opp->getX() < chessboard.size() && bottomright_opp->getY() < chessboard[0].size() && chessboard[bottomright_opp->getX()][bottomright_opp->getY()] == obj.getSide())
		{
			return Move(obj.getSide(), bottomright_opp->getX() + 1, bottomright_opp->getY() + 1);
		}
		else
			return obj;
	}

public:
	ChessBoard(int w = 8, int l = 8)
	{
		if (w < 3 || w>999)
		{
			cout << "Invalid width, width will be set to 8 ." << endl;
			w = 8;
		}
		if (l < 3 || l>999)
		{
			cout << "Invalid length, length will be set to 8 ." << endl;
			l = 8;
		}
		for (int i = 0; i < w; i++)
		{
			chessboard.push_back(vector<char>());
			for (int j = 0; j < l; j++)
			{
				chessboard[i].push_back(' ');
			}
		}
		chessboard[floor(w / 2) - 1][floor(l / 2) - 1] = 'O';
		chessboard[floor(w / 2)][floor(l / 2)] = 'O';
		chessboard[floor(w / 2)][floor(l / 2) - 1] = 'X';
		chessboard[floor(w / 2) - 1][floor(l / 2)] = 'X';
	}

	~ChessBoard()
	{
		//Nothing to do
	}

	ChessBoard(const ChessBoard & obj)
	{
		this->chessboard = obj.chessboard;
	}

	ChessBoard(ChessBoard && obj)
	{
		swap(chessboard, obj.chessboard);
	}

	bool playerMove(Move move)
	{
		char side = move.getSide();
		int x = move.getX();
		int y = move.getY();
		if (x<1 || x>chessboard.size())
		{
			cout << "Invalid move. The width of the chessboard is " << chessboard.size() << " ." << endl;
			return false;
		}
		if (y<1 || y>chessboard[0].size())
		{
			cout << "Invalid move. The length of the chessboard is " << chessboard[0].size() << " ." << endl;
			return false;
		}
		if (chessboard[x - 1][y - 1] == ' ' && possibleMove(move))
		{
			chessboard[x - 1][y - 1] = side;
			flipChess(move);
			return true;
		}
		else
		{
			cout << "Cannot make this move." << endl;
			return false;
		}
	}

	bool possibleMove(Move move)
	{
		//check chess on left
		if (move != searchNL(move))
		{
			return true;
		}
		//check chess on right
		if (move != searchNR(move))
		{
			return true;
		}
		//check chess on above
		if (move != searchNT(move))
		{
			return true;
		}
		//check chess on below
		if (move != searchNB(move))
		{
			return true;
		}
		//check chess on top left
		if (move != searchNTL(move))
		{
			return true;
		}
		//check chess on top right
		if (move != searchNTR(move))
		{
			return true;
		}
		//check chess on bottom left
		if (move != searchNBL(move))
		{
			return true;
		}
		//check chess on bottom right
		if (move != searchNBR(move))
		{
			return true;
		}
		return false;
	}

	vector<Move> getAllPossibleMove(char chess)
	{
		vector<Move> moves;
		for (int i = 1; i <= chessboard.size(); ++i)
		{
			for (int j = 1; j <= chessboard[0].size(); ++j)
			{
				if (chessboard[i - 1][j - 1] == ' ')
				{
					if (possibleMove(Move(chess, i, j)))
					{
						moves.push_back(Move(chess, i, j));
					}
				}
			}
		}
		{
			for (auto it = moves.begin(); it != moves.end(); ++it)
			{
				cout << it->getSide() << ' ' << it->getX() << ' ' << it->getY() << endl;
			}
		}
		return moves;
	}

	void printChessboard()
	{
		cout << endl << endl;
		{
			cout << setw(3) << setiosflags(ios::right) << " ";
			for (int i = 1; i <= chessboard.size(); i++)
			{
				cout << setw(3) << setiosflags(ios::right) << i << setw(3) << setiosflags(ios::right) << " ";
			}
			cout << endl;
		}
		for (int j = 1; j <= chessboard[0].size(); j++)
		{
			cout << setw(3) << setiosflags(ios::right) << j << " ";
			for (int i = 1; i <= chessboard.size(); i++)
			{
				cout << setw(2) << setiosflags(ios::right) << chessboard[i - 1][j - 1] << "  ";
				if (i < chessboard.size())
					cout << "| ";
			}
			cout << setw(3) << setiosflags(ios::right) << j << endl;
			if (j < chessboard[0].size())
			{
				cout << setw(3) << setiosflags(ios::right) << " ";
				for (int k = 1; k <= chessboard.size(); k++)
				{
					cout << setw(5) << setiosflags(ios::right) << "-----";
					if (k < chessboard.size())
						cout << "+";
				}
				cout << endl;
			}
		}
		{
			cout << setw(3) << setiosflags(ios::right) << " ";
			for (int i = 1; i <= chessboard.size(); i++)
			{
				cout << setw(3) << setiosflags(ios::left) << i << setw(3) << setiosflags(ios::right) << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	bool flipChess(Move move)
	{
		if (possibleMove(move))
		{
			//flip chess on left
			char side = move.getSide();
			int x = move.getX();
			int y = move.getY();
			Move left_most = searchNL(move);
			for (int i = x - 1; i > left_most.getX(); --i)
			{
				chessboard[i - 1][y - 1] = side;
			}
			//flip chess on right
			Move right_most = searchNR(move);
			for (int i = x + 1; i < right_most.getX(); ++i)
			{
				chessboard[i - 1][y - 1] = side;
			}
			//flip chess on above
			Move top_most = searchNT(move);
			for (int j = y - 1; j > top_most.getY(); --j)
			{
				chessboard[x - 1][j - 1] = side;
			}
			//flip chess on below
			Move below_most = searchNB(move);
			for (int j = y + 1; j < below_most.getY(); ++j)
			{
				chessboard[x - 1][j - 1] = side;
			}
			//flip chess on top left
			Move topleft_most = searchNTL(move);
			for (int k = 1; k < (x - topleft_most.getX()) && k < (y - topleft_most.getY()); ++k)
			{
				chessboard[x - 1 - k][y - 1 - k] = side;
			}
			//flip chess on top right
			Move topright_most = searchNTR(move);
			for (int k = 1; k < (topright_most.getX() - x) && k < (y - topright_most.getY()); ++k)
			{
				chessboard[x - 1 + k][y - 1 - k] = side;
			}
			//flip chess on bottom left
			Move bottomleft_most = searchNBL(move);
			for (int k = 1; k < (x - bottomleft_most.getX()) && k < (bottomleft_most.getY() - y); ++k)
			{
				chessboard[x - 1 - k][y - 1 + k] = side;
			}
			//flip chess on bottom right
			Move bottomright_most = searchNBR(move);
			for (int k = 1; k < (bottomright_most.getX() - x) && k < (bottomright_most.getY() - y); ++k)
			{
				chessboard[x - 1 + k][y - 1 + k] = side;
			}
			return true;
		}
		return false;
	}
	
	vector<Move> getAllPossibleFlip(Move move)
	{
		vector<Move> moves;
		if (possibleMove(move))
		{
			//Count chess on left
			char side = move.getSide();
			int x = move.getX();
			int y = move.getY();
			Move left_most = searchNL(move);
			for (int i = x - 1; i > left_most.getX(); --i)
			{
				moves.push_back(Move(side, i, y));
			}
			//Count chess on right
			Move right_most = searchNR(move);
			for (int i = x + 1; i < right_most.getX(); ++i)
			{
				moves.push_back(Move(side, i, y));
			}
			//Count chess on above
			Move top_most = searchNT(move);
			for (int j = y - 1; j > top_most.getY(); --j)
			{
				moves.push_back(Move(side, x, j));
			}
			//Count chess on below
			Move below_most = searchNB(move);
			for (int j = y + 1; j < below_most.getY(); ++j)
			{
				moves.push_back(Move(side, x, j));
			}
			//Count chess on top left
			Move topleft_most = searchNTL(move);
			for (int k = 1; k < (x - topleft_most.getX()) && k < (y - topleft_most.getY()); ++k)
			{
				moves.push_back(Move(side, x - k, y - k));
			}
			//Count chess on top right
			Move topright_most = searchNTR(move);
			for (int k = 1; k < (topright_most.getX() - x) && k < (y - topright_most.getY()); ++k)
			{
				moves.push_back(Move(side, x + k, y - k));
			}
			//Count chess on bottom left
			Move bottomleft_most = searchNBL(move);
			for (int k = 1; k < (x - bottomleft_most.getX()) && k < (bottomleft_most.getY() - y); ++k)
			{
				moves.push_back(Move(side, x - k, y + k));
			}
			//flip chess on bottom right
			Move bottomright_most = searchNBR(move);
			for (int k = 1; k < (bottomright_most.getX() - x) && k < (bottomright_most.getY() - y); ++k)
			{
				moves.push_back(Move(side, x + k, y + k));
			}
		}
		return moves;
	}

	const Move minimalNextOppMoveSuggestor(const char side)
	{
		//Must ensure AI can make a move before you call this function.
		vector<Move> moves = getAllPossibleMove(side);
		if (moves.size() == 0)
			throw exception("No possible moves.");
		Move suggest_move;
		shared_ptr<ChessBoard> nextChessboard;
		size_t max_opp_move_num = 64;
		for (auto it = moves.cbegin(); it != moves.cend(); ++it)
		{
			nextChessboard = make_shared<ChessBoard>(*this);
			nextChessboard->playerMove(*it);
			vector<Move> next_opp_moves = nextChessboard->getAllPossibleMove(getOppChess(side));
			if (max_opp_move_num >= next_opp_moves.size())
			{
				max_opp_move_num = next_opp_moves.size();
				suggest_move = *it;
			}
		}
		return suggest_move;
	}

	const Move minimalNextOppFlipSuggestor(const char side)
	{
		//Must ensure AI can make a move before you call this function.
		vector<Move> moves = getAllPossibleMove(side);
		if (moves.size() == 0)
			throw exception("No possible moves.");
		Move suggest_move;
		shared_ptr<ChessBoard> nextChessboard;
		size_t max_opp_flip_num = 64;
		size_t next_max_possible_opp_flip_num = 0;
		for (auto it = moves.cbegin(); it != moves.cend(); ++it)
		{
			nextChessboard = make_shared<ChessBoard>(*this);
			nextChessboard->playerMove(*it);
			vector<Move> next_opp_moves = nextChessboard->getAllPossibleMove(getOppChess(side));
			next_max_possible_opp_flip_num = 0;
			for (auto iter = next_opp_moves.cbegin(); iter != next_opp_moves.cend(); ++iter)
			{
				vector<Move> next_opp_flips = nextChessboard->getAllPossibleFlip(*iter);
				if (next_max_possible_opp_flip_num <= next_opp_flips.size())
				{
					next_max_possible_opp_flip_num = next_opp_flips.size();
				}
			}
			if (max_opp_flip_num >= next_max_possible_opp_flip_num)
			{
				max_opp_flip_num = next_max_possible_opp_flip_num;
				suggest_move = *it;
			}
		}
		return suggest_move;
	}

	const char getOppChess(char side)
	{
		if (side == 'X')
			return 'O';
		else if (side == 'O')
			return 'X';
		else
			throw exception("No such chess.");
	}

	bool checkTie()
	{
		//Can still tie if no possible move for either players.
		for (int i = 1; i <= chessboard.size(); i++)
		{
			for (int j = 1; j <= chessboard[0].size(); j++)
			{
				if (chessboard[i - 1][j - 1] == ' ')
				{
					return false;
				}
			}
		}
		cout << "Chessboard full. Game over." << endl;
		return true;
	}

	bool checkSameColor()
	{
		//Can still tie if no possible move for either players.
		char color = NULL;
		for (int i = 1; i <= chessboard.size(); i++)
		{
			for (int j = 1; j <= chessboard[0].size(); j++)
			{
				if (chessboard[i - 1][j - 1] != ' ')
				{
					if (color == NULL)
					{
						color = chessboard[i - 1][j - 1];
					}
					else if (color != chessboard[i - 1][j - 1])
					{
						return false;
					}
				}
			}
		}
		cout << "Opponent had been cleared. Game over." << endl;
		return true;
	}
};

int main()
{
	int x = 0;
	int y = 0;
	int width = 8;
	int length = 8;
	int stepcount = 0;
	char chess = 'X';
	shared_ptr<ChessBoard> chessboard;
	cout << "Welcome to Reversi!" << endl;
	cout << "Please enter \'Y\' to play with AI or anything else to continue: ";
	char choice = NULL;
	bool play_ai = false;
	choice = cin.get();
	if (choice == 'Y' || choice == 'y')
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		play_ai = true;
		cout << "You're now playing with AI." << endl;
	}
	chessboard = make_shared<ChessBoard>(8, 8);
	chessboard->printChessboard();
	while (!chessboard->checkTie() && !chessboard->checkSameColor())
	{
		++stepcount;
		vector<Move> possible_moves_X = chessboard->getAllPossibleMove('X');
		vector<Move> possible_moves_O = chessboard->getAllPossibleMove('O');
		if (possible_moves_O.size() == 0 && possible_moves_X.size() == 0)
		{
			cout << "No possible move for either player." << endl;
			break;
		}
		if (fmod(stepcount, 2) == 1)
		{
			chess = 'X';
			cout << "Current Player is X." << endl;
			if (possible_moves_X.size() == 0)
			{
				cout << "There is no possible move for X. Skipping player X." << endl;
				continue;
			}
		}
		else
		{
			chess = 'O';
			cout << "Current Player is O." << endl;
			if (possible_moves_O.size() == 0)
			{
				cout << "There is no possible move for O. Skipping player O." << endl;
				continue;
			}
			if (play_ai)
			{
				//chessboard->playerMove(chessboard->minimalNextOppMoveSuggestor('O'));
				chessboard->playerMove(chessboard->minimalNextOppFlipSuggestor('O'));
				chessboard->printChessboard();
				continue;
			}
		}
		do
		{
			cout << "Please make your move." << endl;
			cout << "Which column: ";
			while (!(cin >> x))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please input number only: ";
			};
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Which row: ";
			while (!(cin >> y))
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid input. Please input number only: ";
			}
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} while (!chessboard->playerMove(Move(chess, x, y)));
		chessboard->printChessboard();
	}
	chessboard->printChessboard();
	cout << "Thanks for playing." << endl;
	system("pause");
	return 0;
}