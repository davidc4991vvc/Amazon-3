#include <iostream>

using namespace std;

class FBTNode{
private:
     int bookId;
     int fbtValue;

public:
	FBTNode *next;

	FBTNode(){
		bookId = 0;
		fbtValue = 0;
		next = NULL;
	}

	FBTNode(int book, int value){
		bookId = book;
		fbtValue = value;
		next = NULL;
	}

	void incValue(){
		fbtValue++;
	}

	void setBookId(int input){
		bookId = input;
	}

	int getBookId(){
		return bookId;
	}

	int getValue(){
		return fbtValue;
	}

};

class BookNode{

protected:
	char title[1024];
	int id;
	float price;
	FBTNode *fbt;

public:
	BookNode *next;

	BookNode(){
		id = 0;
		price = 0;
		fbt = NULL;
	}

	BookNode(char c[1024], int i, float f){
		strcpy_s(title,c);
		id = i;
		price = f;
		fbt = NULL;
	}

	virtual void display(){
		cout << "The title of the book is: " << title << endl;
		cout << "The ID of the book is: " << id << endl;
		cout << "The price of the book is: " << price << "\n" << endl;
	}
	
	void setTitle(char *c){
		strcpy_s(title,c);
	}

	void setId(int i){
		id = i;
	}

	void setPrice(float f){
		price = f;
	}

	void setFbt(FBTNode *p){
		if(fbt != NULL){
				FBTNode *head,*tail;
				head = fbt;
				tail = fbt->next;
				if(p->getValue() > head->getValue()){
					p->next = head;
					fbt = p;
					return;
				}
				while(tail != NULL){
					if(p->getValue() > tail->getValue()){
						p->next = tail;
						head->next = p;
						return;
					}
					head = head->next;
					tail = tail->next;
				}
				head->next = p;
		}

		else{
			p->next = NULL;
			fbt = p;
		}
	}

	FBTNode* getFbt(){
		return fbt;
	}

	int getId(){
		return id;
	}

	char *getTitle(){
		return title;
	}

	float getPrice(){
		return price;
	}

	void quit(FBTNode *node){
		FBTNode *p,*k;

		if(node != NULL){
			p = node;
			k = node->next;

			while(p != NULL){
				delete p;
				p = k;

				if(k != NULL)
					k = k->next;
			}
		}
	}

	~BookNode(){
		quit(fbt);
	}

};BookNode *Catalog=NULL,*Cart=NULL;

class FictionNode : public BookNode{
private:
	char author[1024];

public:
	FictionNode(char title[1024], int id, float price, char c[1024]):BookNode(title, id, price){

		strcpy_s(author,c);
	}

	void display(){
		cout << "The title of the fiction book is: " << title << endl;
		cout << "The ID of the fiction book is: " << id << endl;
		cout << "The price of the fiction book is: " << price << endl;
		cout << "The author of the fiction book is: " << author << "\n" << endl;
	}
};

class MagnazineNode : public BookNode{
private:
	int issue;

public:
	MagnazineNode(char title[1024], int id, float price, int i):BookNode(title, id, price){
		issue = i;
	}

	void display(){
		cout << "The title of the magnazine is: " << title << endl;
		cout << "The ID of the magnazine is: " << id << endl;
		cout << "The price of the magnazine is: " << price << endl;
		cout << "The issue of the magnazine is: " << issue << "\n" << endl;
	}
};

class TextbookNode : public BookNode{
private:
	char author[1024];
	int edition;
	char isbn[1024];

public:
	TextbookNode(char title[1024], int id, float price, char tauthor[1024], int i, char tisbn[1024]):BookNode(title, id, price){
		strcpy_s(author,tauthor);
		edition = i;
		strcpy_s(isbn,tisbn);
	}

	void display(){
		cout << "The title of the textbook is: " << title << endl;
		cout << "The ID of the textbook is: " << id << endl;
		cout << "The price of the textbook is: " << price << endl;
		cout << "The author of the textbook is: " << author << endl;
		cout << "The edition of the textbook is: " << edition << endl;
		cout << "The ISBN of the textbook is: " << isbn << "\n" << endl;
	}
};

// forward declaration //

void menu();
void loadCatalog();
void branching(char option);
void displayCatalog();
void displayCart();
void checkOut();
void addToCart();
void loadFbt();
void promptFbt(FBTNode* List);
void updateFbt();
void quit(BookNode *option);
BookNode *searchId(int i);
BookNode *searchTitle(char *c);
const float afterTax = 1.09;

int main(){
		
	char ch; 

	cout << "\n\nWelcome to CSE240: Bookstore\n";
	loadCatalog();
	loadFbt();

	do {
			 menu();
		 
		 ch = tolower(getchar()); // read a char, convert to lower case
		 cin.ignore();

		 branching(ch);
	} while (ch != 'q');
	
	return 0;

}

void menu()
{
	cout << "\nMenu Options\n";
	cout << "------------------------------------------------------\n";
	cout << "a: Add a book to cart\n";
	cout << "c: Check out\n";
	cout << "m: Display Cart\n";
	cout << "d: Display Catalog\n"; 
	cout << "q: Quit\n";
	cout << "\n\nPlease enter a choice (a,c,m,d or q) ---> "; 
}

void branching(char option)
{
	switch(option)
	{
		case 'a':
			addToCart();
		break;

		case 'c':
			checkOut();
		break;
			
		case 'm':
			displayCart();
		break;

		case 'd':
			displayCatalog();
		break;

		case 'q':
			quit(Catalog);
			if(Cart != NULL){
				quit(Cart);
			}
			Catalog = NULL;
			Cart = NULL;
			exit(0);
		break;

		default:
			cout << "\nError: Invalid Input.  Please try again..."; 
		break;
	}
}

	void loadCatalog()
	{
		FILE *fileName;
		char title[1024];
		int id,type;
		float price;

		fileName = fopen("iBookCatalog.txt","r");

		if(fileName != NULL){
			while(fread(&type, sizeof(int), 1, fileName) == 1){
				fread(title,1024,1,fileName);
				fread(&id, sizeof(int), 1, fileName);
				fread(&price, sizeof(float), 1, fileName);

				BookNode *node;
				switch(type){
					case 0:
						char fictionAuthor[1024];
						fread(fictionAuthor, 1024, 1, fileName);
						node = new FictionNode(title, id, price, fictionAuthor);
						node->next = Catalog;
						Catalog = node;
						break;

					case 1:
						int issue;
						fread(&issue, sizeof(int), 1, fileName);
						node = new MagnazineNode(title, id, price, issue);
						node->next = Catalog;
						Catalog = node;
						break;
				
					case 2:
						char textbookAuthor[1024];
						int edition;
						char isbn[1024];
						fread(textbookAuthor, 1024, 1, fileName);
						fread(&edition, sizeof(int), 1, fileName);
						fread(isbn, 1024, 1, fileName);
						node = new TextbookNode(title, id, price, textbookAuthor,edition,isbn);
						node->next = Catalog;
						Catalog = node;
						break;

					case 3:
						node = new BookNode();
						node->setTitle(title);
						node->setId(id);
						node->setPrice(price);
						node->next = Catalog;
						Catalog = node;
						break;
				}
			}
		}
		fclose(fileName);
	}

	void displayCatalog(){
		BookNode *node;
		node = Catalog;
		while(node != NULL){
			node->display();
			node = node->next;
		}
	}

	void displayCart(){
		BookNode *node;
		node = Cart;
		while(node != NULL){
			node->display();
			node = node->next;
		}
	}

	void checkOut(){
		cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
		cout.precision(2);
		BookNode *node;
		node = Cart;
		char input;
		int count = 1;
		cout << "So far you have the following items in your cart:\n" << endl;
		while(node != NULL){
			cout << count << ") " << node->getTitle() << " X 1" << endl;
			node = node->next;
			count++;
		}
		cout << "\nAre you sure you have all that you need?\nPlease enter \"y\" or \"n\"" << endl;
		cin >> input;
		cin.ignore();

		if(input == 'y'){ 
			FBTNode *fbt;
			BookNode *head,*phead;
			bool flag = false;
			int id;
			head = Cart;
			phead = Cart;

			node = Cart;
			float total=0;
			while(node != NULL){
				total += node->getPrice();
				node = node->next;
			}
			total = total * afterTax;
			cout << "Your grand total after taxes is : $" << total << endl;

			if(Cart->next != NULL && Cart != NULL){
				while(head != NULL) {
					phead = Cart;
					while(phead != NULL){
						fbt = searchId(head->getId())->getFbt();
						if(phead != head){
							id = phead->getId();
							flag = false;
							while(fbt != NULL){
								if(id == fbt->getBookId()){
									fbt->incValue();
									flag = true;
								}
								fbt = fbt->next;
							}
							if(flag == false){
							FBTNode *p = new FBTNode(id,1);
							searchId(head->getId())->setFbt(p);
							}
						}
						phead = phead->next;
						
					}
					head = head->next;				
				}

				updateFbt();
			}

			quit(Catalog);

			if(Cart != NULL){
				quit(Cart);
			}
			
			Catalog = NULL;
			Cart = NULL;
			exit(0);
		}
	}

	void addToCart(){
		BookNode *node;
		node = NULL;
		char c;
		cout << "Please choose \"i\" if you are going to enter by ID and \"t\" by title" << endl;
		c = tolower(getchar());
		cin.ignore();

		BookNode *p;

		switch(c){
		case 'i':
			int input;
			cout << "Please enter the ID of the book you wish to add to your cart" << endl;
			cin >> input;
			cin.ignore();			
			
			if(input <= 17 && input >= 0){

				node = searchId(input);
	
				p = new BookNode(node->getTitle(),node->getId(),node->getPrice());
				p->next = Cart;
				Cart = p;

				promptFbt(node->getFbt());
				break;
			}

			else{
					cout << "Invalid input" << endl;
					break;
			}

			case 't':
				char title[1024];
				cout << "Please enter the title of the book you wish to add to your cart" << endl;
				cin.getline(title,1024,'\n');
				node = searchTitle(title);
					if(node != NULL){
						p = new BookNode(node->getTitle(),node->getId(),node->getPrice());
						p->next = Cart;
						Cart = p;

						promptFbt(node->getFbt());
						break;
					}

				else{
					cout << "Invalid input" << endl;
					break;
				}

			}
		}
		
	void loadFbt(){
		FILE *fileName;
		int parent, book, value;
		BookNode *node;

		fileName = fopen("FBT.txt","r");

		if(fileName != NULL){
			FBTNode *fbtNode;
			while(fread(&parent, sizeof(int), 1, fileName) == 1){
				fread(&book, sizeof(int), 1, fileName);
				fread(&value, sizeof(int), 1, fileName);
				
				node = searchId(parent);

				fbtNode = new FBTNode(book,value);

				node->setFbt(fbtNode);

			}
		}
		fclose(fileName);
	}

	BookNode *searchId(int i){
			BookNode *node;
			node = Catalog;
			while(node != NULL){
				if(node->getId() == i){
					return node;
				}
				node = node->next;
			}
			return NULL;
	}

	BookNode *searchTitle(char *c){
		BookNode *node;
		node = Catalog;
		while(node != NULL){
				if(stricmp(node->getTitle(),c) == 0){
					return node;
				}
				node = node-> next;
			}
		return NULL;
	}

	void promptFbt(FBTNode* List){

		int temp;
		BookNode *node;

		cout << "You have successfully added \"" << Cart->getTitle() << "\" to your cart\n" << endl;

		temp = Cart->getId();
		node = searchId(temp);

		if (node->getFbt() != NULL)
		{
			FBTNode *fbt;
			BookNode *book;
			
			fbt = node->getFbt();		
			int i = 1, id;
			char input;

			cout << "Please consider the following books most frequently bought with \"" << Cart->getTitle() << "\":" << endl;

				while(fbt != NULL && i<4){
					id = fbt->getBookId();
					book = searchId(id);

					cout << i << ") " << book->getTitle() << "\t\tBook ID: " << book->getId() << endl;

					fbt= fbt->next;
					i++;
				}
			cout << "\nWould you like to buy any of the books listed above?\t(y/n)" << endl;
			input = tolower(getchar());
			cin.ignore();
			if(input == 'y')
				addToCart();
			}

			else{
			cout << "Sorry, no suggestions available..." << endl;
		}
	}

	void updateFbt(){
		FILE *fileName;
		BookNode *book;
		FBTNode *fbt;
		book = Catalog;
		fileName = fopen("FBT.txt", "w");

			if(fileName != NULL){
				int temp;
				while(book != NULL){
					if(book->getFbt() != NULL){
						fbt = book->getFbt();
						while(fbt != NULL){
							temp = book->getId();
							fwrite(&temp, sizeof(int), 1, fileName);
							temp = fbt->getBookId();
							fwrite(&temp, sizeof(int), 1, fileName);
							temp = fbt->getValue();
							fwrite(&temp, sizeof(int), 1, fileName);
							fbt = fbt->next;
						}
					}
					book = book->next;
				}
			}
		fclose(fileName);
		}

	void quit(BookNode *option){
		BookNode *p,*k;
		p = option;
		k = option->next;

		while(p != NULL){
			delete p;
			p = k;

			if(k != NULL)
				k = k->next;
		}
	}
