#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
const int MAX_ASCII_VALUE = 127;
using namespace std;
struct node {
	char data;
	node* up;
	node* down;
	node* left;
	node* right;
};
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
class TwoDDLLADT {
private:
	node* first;
	node* cursor;
	int x, y;
public:
	node* createnode(char c) {
		node* ptr = new node;
		ptr->data = c;
		ptr->down = NULL;
		ptr->up = NULL;
		ptr->left = NULL;
		ptr->right = NULL;
		return ptr;
	}
	TwoDDLLADT() {
		first = NULL;
		cursor = first;
		x = 0, y = 0;
	}
	~TwoDDLLADT() {
		node* ptr = first;
		while (ptr != NULL) {
			node* next_row = ptr->down;
			node* curr_row = ptr;
			while (curr_row != NULL) {
				node* next_col = curr_row->right;
				delete curr_row;
				curr_row = next_col;
			}
			ptr = next_row;
		}
	}
	void Insert(char c) {
		if (cursor == NULL) {
			first = createnode(c);
			cursor = first;
		}
		else if (cursor->right == NULL) {
			if (c == 10) {
				node* nextLine = createnode(c);
				cursor->left->right = NULL;
				cursor->right = nextLine;
				nextLine->left = cursor;
				cursor = nextLine;
				x = 0;
				y++;
				gotoxy(x, y);
			}
			else {
				node* ptr = createnode(c);
				cursor->right = ptr;
				ptr->left = cursor;
				if (cursor->up != NULL) {
					node* upNextRight = cursor->up->right;
					ptr->up = cursor->up->right;
					upNextRight->down = ptr;
				}
				if (cursor->down != NULL) {
					node* downNextRight = cursor->down->right;
					ptr->down = cursor->down->right;
					downNextRight->up = ptr;
				}
				cursor = ptr;
				x++;
				gotoxy(x, y);
			}
		}
		else if (cursor->left == NULL) {
			node* ptr = createnode(c);
			first->left = ptr;
			ptr->right = first;
			first = ptr;
			cursor = ptr;
			x = 0;
			gotoxy(x, y);
		}
		else {
			node* ptr = createnode(c);
			cursor->left->right = ptr;
			ptr->left = cursor->left;
			if (cursor->up != NULL) {
				node* upNextRight = cursor->up->right;
				ptr->up = cursor->up->right;
				upNextRight->down = ptr;
			}
			if (cursor->down != NULL) {
				node* downNextRight = cursor->down->right;
				ptr->down = cursor->down->right;
				downNextRight->up = ptr;
			}
			ptr->right = cursor;
			cursor->left = ptr;
			cursor = ptr;
			x++;
			gotoxy(x, y);
		}
	}
	void Delete() {
		if (cursor == NULL) {
			return;
		}
		node* temp = cursor;
		cursor->left->right = cursor->right;
		if (cursor->right != NULL) {
			cursor->right->left = cursor->left;
			cursor = cursor->right;
		}
		else {
			cursor = cursor->left;
		}
		delete temp;
		gotoxy(x, y);
	}
	void Up() {
		if (cursor->up != NULL) {
			cursor = cursor->up;
			if (cursor->right == NULL) {
				x = 0;
			}
			else {
				x++;
			}
			y--;
			gotoxy(x, y);
		}
	}

	void Down() {
		if (cursor->down != NULL) {
			cursor = cursor->down;
			if (cursor->right == NULL) {
				x = 0;
			}
			else {
				x++;
			}
			y++;
			gotoxy(x, y);
		}
	}
	void Left() {
		if (cursor->left != nullptr) {
			cursor = cursor->left;
			x--;
			gotoxy(x, y);
		}
	}
	void Right() {
		if (cursor->right != nullptr) {
			cursor = cursor->right;
		}
		x++;
		gotoxy(x, y);
	}
	void Load() {
		ifstream infile("data.txt");
		if (!infile) {
			cout << "Error: Unable to open file 'data.txt'" << endl;
			return;
		}

		// Clear existing content
		while (first != NULL) {
			node* temp = first;
			first = first->down;
			delete temp;
		}
		cursor = NULL;

		char c;
		node* prev = NULL;
		node* first_elem = NULL;

		while (infile.get(c)) {
			if (c == '\n') {
				prev = NULL;
				if (first_elem != NULL) {
					first_elem = first_elem->down;
				}
			}
			else if (c < MAX_ASCII_VALUE && c >= 0) {  // Check if character is valid
				node* temp = createnode(c);
				temp->up = prev;
				if (prev == NULL) {
					first_elem = temp;
					prev = temp;
				}
				else {
					prev->right = temp;
					temp->up = prev;
					prev = temp;
				}
				if (first == NULL) {
					first = temp;
					cursor = temp;
				}
			}
		}
		infile.close();
	}

	void save() {
		fstream file("data.txt", ios::out);  // Use ios::out instead of ios::app
		if (!file) {
			cout << "Error: Unable to open file 'data.txt'" << endl;
			return;
		}

		node* curr_row = first;
		while (curr_row != NULL) {
			node* curr_col = curr_row;
			while (curr_col != NULL) {
				if (curr_col->data < MAX_ASCII_VALUE && curr_col->data >= 0) {
					file << curr_col->data;
				}
				curr_col = curr_col->right;
			}
			curr_row = curr_row->down;
		}
		file.close();
	}
	void Print() {
		node* current_row = first;
		while (current_row != nullptr) {
			node* current_node = current_row;
			while (current_node != nullptr) {
				cout << current_node->data;
				current_node = current_node->right;
			}
			cout << endl;
			current_row = current_row->down;
		}
		gotoxy(x, y);
	}
};
int main() {
	TwoDDLLADT notepad;
	char c;
	while (1) {
		c = _getch();
		if (c == -32) {
			c = _getch();
			if (c == 'H')
				notepad.Up();
			else if (c == 'K')
				notepad.Left();

			else if (c == 'M')
				notepad.Right();

			else if (c == 'P')
				notepad.Down();

		}
		else if (c == 8 || c == 127)
			notepad.Delete();

		else if (c == 'S')
			notepad.save();

		else if (c == 'L')
			notepad.Load();

		else if (c == 'Q')
			break;

		else if (c < MAX_ASCII_VALUE && c >= 0)   // Check if character is valid
			notepad.Insert(c);

		system("cls");
		notepad.Print();
	}
	
	return 0;
}
