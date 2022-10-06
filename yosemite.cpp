#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoxy(short x, short y)	 	          //definition of gotoxy function//                                               
{
 COORD pos ={x,y};
 SetConsoleCursorPosition(hConsole, pos);
}

void highlight(char a, int p)						// To emphasise text by adding colour
{
	SetConsoleTextAttribute(hConsole,p);
	cout<<a;
	SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

string str_in;
bool done=0;
int h=3,l=3; //h up to down		l is left to right
int mine_num;
int flag_num;
int spacer=1;


bool flag[100][100]={0};
bool mines[100][100]={0};
bool uncovered[100][100]={0};

void counter(void);
void game(void);
void setup(void);
void draw(void);
void uncoverer(int,int);
int neighbour(int,int);
void check(void);

int main()
{
	game();
	
	gotoxy(25,25);
	return 0;
}



void game()
{
	cout<<"\n This is a pre-release "<<(char)225<<"-version of MINESWEEPER \n";
	SetConsoleTextAttribute(hConsole,0x0);
	cout<<"By 0x412\n";
	SetConsoleTextAttribute(hConsole,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout<<" Please give feed back for improvements\n"
		<<" DON'T FORGET TO KEEP loser.wav & wnner.wav IN THE SAME FOLDER AS THE GAME FILES!!!\n\n";
	cout<<   "Enter no. of rows(enter 9 if not sure)\t\t";cin>>h;
	cout<<"\nEnter no. of columns(enter 9 if not sure)\t";cin>>l;
	mine_num=0.00023*(h*l*h*l)+0.09375*(l*h)+1.89366;
	cout<<" Creating New Game.\n ";
	setup();
	cout<<"\t100%"
		<<"\n Enter anything to start timer\n\t";
	cin>>str_in;
	auto t_start = chrono::high_resolution_clock::now(),t_end=chrono::high_resolution_clock::now();
	system("cls");
	int num=l*h;		//it will first allow spacer then it will work in the loop
	while(num>0){num/=10; spacer+=1;}
	draw();
	cin>>str_in;
	
	while(str_in!="Q" && str_in!="q" && !done){
		while(str_in.front()>'9' || str_in.front()<'0'){
			cout<<" INVALID INPUT\t TRY AGAIN\t";
			cin>>str_in;		
			}
		num=stoi(str_in)-1;
		if(uncovered[num%l][num/l]==0 && (str_in.back()=='F' || str_in.back()=='f')){
			flag[num%l][num/l]=!flag[num%l][num/l];
		}else if(mines[num%l][num/l]==1){
			done=1;
			t_end=chrono::high_resolution_clock::now();
			cout<<"BOOM!!!";
			cin>>str_in;
			break;
		}else if(uncovered[num%l][num/l]==0 && flag[num%l][num/l]==0){
			uncoverer(num%l,num/l);
			t_end = std::chrono::high_resolution_clock::now();
			check();
			if(done){
			cout<<"\n\n\tMISSION COMPLETE\n\n";
			cin>>str_in;
			break;}
		}
		
		system("cls");
		draw();
		cin>>str_in;
	}
	system("cls");
	draw();
	cout<< "Time taken "<< chrono::duration<double>(t_end-t_start).count()<< " s\n";
	cin>>str_in;
	
	return;
}

void setup()
{
	srand(time(NULL));
	int num=0,i,j;
	while(num<mine_num){
		Sleep(15);
		i=rand()%l;
		Sleep(15);
		j=rand()%h;
		if(!mines[i][j]){
			mines[i][j]=1;
			num++;
			cout<<'=';
			Sleep(15);
			cout<<'=';
		}
	}
	return;
}

void draw()
{	flag_num=0;
	for(int i=0; i<h;i++){for(int j=0;j<l;j++){if(flag[j][i]){flag_num++;}}}
	cout<<" Minesweeper->spots:"<<l*h<<" mines:"<<mine_num<<"\n"
		<<" Enter the spot you want to check\n"
		<<" Type 'f' after the spot to (un)flag it like '36f' \n"
		<<" The timer will stop once all the spots are found.\n"
		<<" The numbers denote the number of adjacent spots having a mine.\n ";
	highlight(159,0xD);	cout<<"->FLAGGED\t";
	highlight('C',0x2);highlight('O',0x3);highlight('L',0x4);highlight('O',0x5);
	highlight('U',0x6);highlight('R',0x7);highlight('S',0x8);
	cout<<"->#of neighbours having mines\t#of Flags left->"<<mine_num-flag_num<<'\n';
	for(int i=0; i<h;i++){for(int j=0;j<l;j++){
		//due to L rule of matrix we have to do this this way; ""\(`+_+`)/""
		// column is j and goes first i is row and goes second
		gotoxy(spacer*j+8,i+8);
		if(flag[j][i]){
			highlight(159,0xD);
		}else if(!uncovered[j][i] && !mines[j][i]){
			cout<<i*l+j+1;
			//cout<<neighbour(i,j);
		}else if(uncovered[j][i] && !mines[j][i]){
			//if(!neighbour(i,j)){
				highlight(neighbour(i,j)+48,neighbour(i,j));
			//}
		}else if(mines[j][i] && done){
			highlight('M',0xC);
		}else if(mines[j][i] && !done){
			cout<<i*l+j+1;
			//cout<<'#';
		}
		
	}}
	cout<<"\n\n\t";
}

void uncoverer(int a,int b)
{
	/*uncovered[a][b]=1;
	if(a!=0 && flag[a-1][b]==0 && uncovered[a-1][b]==0 &&
	mines[a-1][b]==0 && mines[a][b+1]==0 && mines[a+1][b]==0 && mines[a][b-1]==0 ){
		uncoverer(a-1,b);
	}
	if(b!=8 && flag[a][b+1]==0 && uncovered[a][b+1]==0 &&
	mines[a-1][b]==0 && mines[a][b+1]==0 && mines[a+1][b]==0 && mines[a][b-1]==0 ){
		uncoverer(a,b+1);
	}
	if(a!=8 && flag[a+1][b]==0 && uncovered[a+1][b]==0 &&
	mines[a-1][b]==0 && mines[a][b+1]==0 && mines[a+1][b]==0 && mines[a][b-1]==0 ){
		uncoverer(a+1,b);
	}
	if(b!=0 && flag[a][b-1]==0 && uncovered[a][b-1]==0 &&
	mines[a-1][b]==0 && mines[a][b+1]==0 && mines[a+1][b]==0 && mines[a][b-1]==0 ){
		uncoverer(a,b-1);
	}
	/*if(a!=0 && flag[a-1][b]==0 && uncovered[a-1][b]==0 && mines[a-1][b]==0 ){
		uncoverer(a-1,b);
	}
	if(b%9!=0 && flag[a][b+1]==0 && uncovered[a][b+1]==0 && mines[a][b+1]==0){
		uncoverer(a,b+1);
	}
	if(a!=8 && flag[a+1][b]==0 && uncovered[a+1][b]==0 && mines[a+1][b]==0 ){
		uncoverer(a+1,b);
	}
	if(b%9!=1 && flag[a][b+1]==0 && uncovered[a][b+1]==0 && mines[a][b+1]==0){
		uncoverer(a,b+1);
	}*/
	
	uncovered[a][b]=1;
	if(a!=0 && flag[a-1][b]==0 && uncovered[a-1][b]==0 && neighbour(b,a)==-16 ){
		uncoverer(a-1,b);
	}
	if(b!=l-1 && flag[a][b+1]==0 && uncovered[a][b+1]==0 && neighbour(b,a)==-16){
		uncoverer(a,b+1);
	}
	if(a!=h-1 && flag[a+1][b]==0 && uncovered[a+1][b]==0 && neighbour(b,a)==-16){
		uncoverer(a+1,b);
	}
	if(b!=0 && flag[a][b-1]==0 && uncovered[a][b-1]==0 && neighbour(b,a)==-16){
		uncoverer(a,b-1);
	}
	return;
}

int neighbour(int b,int a)
{	
	int sum=0;
	if(a!=0 && mines[a-1][b]==1){sum++;}
	if(b!=h-1 && mines[a][b+1]==1){sum++;}
	if(a!=l-1 && mines[a+1][b]==1){sum++;}
	if(b!=0 && mines[a][b-1]==1){sum++;}
	if(a!=0 && b!=0 && mines[a-1][b-1]==1){sum++;}
	if(a!=l-1 && b!=h-1 && mines[a+1][b+1]==1){sum++;}
	if(a!=l-1 && b!=0 && mines[a+1][b-1]==1){sum++;}
	if(a!=0 && b!=h-1 && mines[a-1][b+1]==1){sum++;}
	
	if(sum==0){sum=-16;}
	return sum;
}

void check()
{
	int sum=0;
	for(int i=0; i<l;i++){for(int j=0;j<h;j++){
		if(!uncovered[i][j]){
			sum++;
		}
		
	}}
	if(sum==mine_num){
		done=1;
	}
}

