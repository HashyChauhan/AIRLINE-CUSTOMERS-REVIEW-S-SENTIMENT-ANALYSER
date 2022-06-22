#include<iostream>
#include <string.h>				 // For string handling
#include <cstdlib>               // Header Files 
#include<conio.h>              	 // For getch() function 
#include<fstream>			  	 // For file handling
#include<queue>				 // Using built-in Queue
#include<bits/stdc++.h>
#include<time.h>
#include<unistd.h>
#include<dos.h>
#include<stdlib.h>     		 //for using the function sleep
#include<windows.h>              // Necessary when treating consoles  
using namespace std;
void gotoxy (int x, int y){     // Function for displaying entities accordingly
	COORD coordinates;     		// coordinates is declared as COORD
	coordinates.X = x;    		// defining x-axis
	coordinates.Y = y;     		//defining  y-axis
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , coordinates);
}

class node{
	public:

		string data;
		node *left;
		node *right;
		int height;

		node(string d):left(0),right(0)
		{
			height=1;
			data=d;
		}

};

class sentiment
{
	public:

		node *positiveroot , *negativeroot; 	// Nodes to create balanced AVL trees for positive and negative words
		string userinput;					// String entered by user
		string words[1000];				// Words extracted from user input
		int wordcount;					// Count for the number of words entered by the user
		int positive;						// Count for the number of positive words entered by the user
		int negative;						// Count for the number of negative words entered by the user
		int notflag;						// Flag set if not is in the input string
		int noneflag;						// Flag set if none is in the input string
		int noflag;						// Flag set if no is in the input string 
		int dontflag;						// Flag set if dont || don't is in the input string 
		int didntflag;					// Flag set if didnt || didn't is in the input string 
		int wontflag;						// Flag set if wont || won't is in the input string 
//		int veryflag;
		int stat; 
		int len;
		int flag;
		double polarity;
		queue<string> postv;
		queue<string> negtv;

		sentiment():positiveroot(0),negativeroot(0)
		{
			notflag=noneflag=noflag=dontflag=didntflag=wontflag=stat=wordcount=positive=negative=flag=len=0;
		}

		~sentiment()
		{
			if(positiveroot!=0)
			{
				positiveroot=0;
				delete positiveroot;
			}

			else if(negativeroot!=0)
			{
				negativeroot=0;
				delete negativeroot;
			}
		}

		int max(int a,int b)
		{
			return (a>b)? a : b;
		}

		int height(node *temp)
		{
			if(temp==NULL)
				return 0;
	
			return temp->height;
		}
		
		node *rightrotate(node *y)
		{
			node *x=y->left;	
			node *t2=x->right;
			x->right=y;
			y->left=t2;
			y->height=max(height(y->left),height(y->right)) + 1;
			x->height=max(height(x->left),height(x->right)) + 1;
			return x;
		}

		node *leftrotate(node *x)
		{	
			node *y=x->right;	
			node *t2=y->left;
			y->left=x;
			x->right=t2;
			x->height=max(height(x->left),height(x->right)) + 1;
			y->height=max(height(y->left),height(y->right)) + 1;
			return y;
		}

		int getbalance(node *temp)
		{
			if(temp==NULL)
				return 0;
			
			return height(temp->left)-height(temp->right);	
		}
		
		node *insert(node *n,string d)
		{
			if(n==NULL)
				return new node(d);
		
			if(d<n->data)
				n->left=insert(n->left,d);	
			else if(d>=n->data)
				n->right=insert(n->right,d);


			n->height=1+max(height(n->left),height(n->right));
			int balance	= getbalance(n);
			if(balance>1 && d<n->left->data)
				return rightrotate(n);

			if(balance<-1 && d>=n->right->data)
				return leftrotate(n);

			if(balance>1 && d>=n->left->data)
			{
				n->left=leftrotate(n->left);
				return rightrotate(n);
			}			
			
			if(balance<-1 && d<n->right->data)
			{
				n->right=rightrotate(n->right);
				return leftrotate(n);
			}
			return n;
		}

		node *negtree()								// Creates balanced AVL tree for negative words
		{
			fstream neg("Negative.txt");
			string str;
			while(!neg.eof())
			{
				getline(neg,str);
				negativeroot=insert(negativeroot,str);
			}
			neg.close();
		}

		node *postree()								// Creates balanced AVL tree for positive words
		{
			fstream pos("Positive.txt");
			string str;
			if(!pos.is_open())
			{
				cout<<"File Not Opened"<<endl;
				exit(0);
			}

			while(!pos.eof())
			{
				getline(pos,str);
				positiveroot=insert(positiveroot,str);
			}
			pos.close();
		}
		
		void wordbreak()						// Breaks words by " " || "." || "," which can used in searching
		{
			len=userinput.length();
			if(userinput[len-1]!='.')
			{
				userinput[len]='.';
				len++;
			}
			int x=0;
			for(int i=0;i<len;i++)
			{
				if(userinput[i]==' ')
				{
					x++;
					wordcount++;
					continue;
				}
				if(userinput[i]=='.'||userinput[i]==',')
				{
					x++;
					wordcount++;
					continue;
				}
				words[x]+=userinput[i];
			}
		}
		
		void search(node *root,string temp,int n) 			// Function used for searching the extracted words
		{
			if(root==NULL)
				return;
			
			search(root->left,temp,n);
			if(root->data==temp)
			{
				if(n==1)
				{
					if(notflag==1)
					{
						negative++;
						string t="not ";
						t+=temp;
						negtv.push(t);
						notflag=0;
						flag++;
						return;
					}
					
					else if(noflag==1)
					{
						negative++;
						string t="no ";
						t+=temp;
						negtv.push(t);
						noflag=0;
						flag++;
						return;
					}

					else if(noneflag==1)
					{
						negative++;
						string t="none ";
						t+=temp;
						negtv.push(t);
						noneflag=0;
						flag++;
						return;
					}

					else if(wontflag==1)
					{
						negative++;
						string t="won't ";
						t+=temp;
						negtv.push(t);
						wontflag=0;
						flag++;
						return;
					}

					else if(dontflag==1)
					{
						negative++;
						string t="don't ";
						t+=temp;
						negtv.push(t);
						dontflag=0;
						flag++;
						return;
					}
					
					else if(didntflag==1)
					{
						negative++;
						string t="didn't ";
						t+=temp;
						negtv.push(t);
						didntflag=0;
						flag++;
						return;
					}
					
				/*	else if(veryflag==1)
					{
						negative++;
						string t="very ";
						t+=temp;
						negtv.push(t);
						veryflag=0;
						flag++;
						return;
					}*/
					
					else
					{
						positive++;
						postv.push(temp);
						flag++;
						return;
					}
				}

				if(n==2)
				{
					if(notflag==1)
					{
						positive++;
						string t="not ";
						t+=temp;
						postv.push(t);
						notflag=0;
						flag++;
						return;
					}
					
					else if(noflag==1)
					{
						positive++;
						string t="no ";
						t+=temp;
						postv.push(t);
						noflag=0;
						flag++;
						return;
					}
					
					else if(noneflag==1)
					{
						positive++;
						string t="none ";
						t+=temp;
						postv.push(t);
						noneflag=0;
						flag++;
						return;
					}
					
					else if(wontflag==1)
					{
						positive++;
						string t="won't ";
						t+=temp;
						postv.push(t);
						wontflag=0;
						flag++;
						return;
					}
					
					else if(dontflag==1)
					{
						positive++;
						string t="don't ";
						t+=temp;
						postv.push(t);
						dontflag=0;
						flag++;
						return;
					}
					
					else if(didntflag==1)
					{
						positive++;
						string t="didn't ";
						t+=temp;
						postv.push(t);
						didntflag=0;
						flag++;
						return;
					}
					
				/*	else if(veryflag==1)
					{
						positive++;
						string t="very ";
						t+=temp;
						postv.push(t);
						veryflag=0;
						flag++;
						return;
					}*/
					
					else
					{
						negative++;
						negtv.push(temp);
						flag++;
						return;
					}
				}
			}
			search(root->right,temp,n);
		}
		
		void word_search()                          // Searching throughout input words to increment values of various flags
		{
			for(int i=0;i<wordcount;i++)
			{
				if(words[i]=="not")
				{
					notflag++;
					continue;
				}
				if(words[i]=="no")
				{
					noflag++;
					continue;
				}
				if(words[i]=="none")
				{
					noneflag++;
					continue;
				}
				if(words[i]=="wont"||words[i]=="won't")
				{
					wontflag++;
					continue;
				}
				if(words[i]=="dont"||words[i]=="don't")
				{
					dontflag++;
					continue;
				}
				if(words[i]=="didnt"||words[i]=="didn't")
				{
					didntflag++;
					continue;
				}
			/*	if(words[i]=="very")
				{
					veryflag++;
					continue;
				}*/
				flag=0;
				search(positiveroot,words[i],1);
				if(!flag)	
					search(negativeroot,words[i],2);
				else
					continue;	
			}
		}
		
		void result(char choice)
		{
			if(choice=='1')
				result_user();
				
			if(choice=='2')
				result_file();	
			
			for(int i=0;i<wordcount;i++)
			{
				words[i].clear();
			}
		}
		
		int result_user()
		{
			system("cls");
			ofstream out;
			out.open("Output.txt",ios_base::app);
			gotoxy(51,3);
			cout<<"===========================================================";
			gotoxy(58,4);
			cout<<"AIRLINE CUSTOMER'S REVIEW'S ANALYSIS REPORT";
			gotoxy(51,5);
			cout<<"===========================================================";
			gotoxy(0,11);
			cout<<"\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2";
			out<<"\n******************************************************************************************************"<<endl;
			out<<"Analysed Review: "<<endl<<userinput<<endl;
			if(!postv.empty())
			{
				cout<<endl<<"\n Positive Words:"<<endl;
				out<<endl<<"\n Positive Words:"<<endl;
				while(!postv.empty())
				{
					cout<<" "<<postv.front()<<endl;
					out<<postv.front()<<endl;
					postv.pop();
				}
			}
			if(!negtv.empty())
			{
				cout<<endl<<"\n Negative Words:"<<endl;
				out<<endl<<"\n Negative Words:"<<endl;
				while(!negtv.empty())
				{
					cout<<" "<<negtv.front()<<endl;
					out<<negtv.front()<<endl;
					negtv.pop();
				}
			}
			cout<<endl;
			out<<endl;
			double total=positive+negative;
			cout<<"\n Positive Sentiments: "<<positive;
			out<<"\n Positive Sentiments: "<<positive;
			cout<<"\n\n Negative Sentiments: "<<negative;
			out<<"\n\n Negative Sentiments: "<<negative;
			cout<<"\n\n Total Sentimental References: "<<total;
			out<<"\n\n Total Sentimental References: "<<total;
			polarity=positive/total;
			cout<<"\n\n Polarity: "<<polarity;
			out<<"\n\nPolarity: "<<polarity;
			if(positive == negative)
			{
				cout<<"\n\n Sentiment possessed is NEUTRAL ";
				out<<"\n\n Sentiment possessed is NEUTRAL ";
			}
			
			else if(polarity > 0)
			{
				cout<<"\n\n Sentiment possessed is POSITIVE ";
				out<<"\n\n Sentiment possessed is POSITIVE ";
			}
			
			else if(polarity <= 0)
			{
				cout<<"\n\n Sentiment possessed is NEGATIVE ";
				out<<"\n\n Sentiment possessed is NEGATIVE ";
			}
			gotoxy(0,31);
			cout<<"\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2"; 
		}
		
		void result_file()
		{
			cout<<"******************************************************************************************************"<<endl;
			cout<<" Analysed Review: "<<endl<<" "<<userinput<<endl;
			if(!postv.empty())
			{
				cout<<"\n Positive Words:"<<endl;
				while(!postv.empty())
				{
					cout<<" "<<postv.front();
					postv.pop();
				}
			}
			if(!negtv.empty())
			{
				cout<<"\n\n Negative Words:"<<endl;
				while(!negtv.empty())
				{
					cout<<" "<<negtv.front();
					negtv.pop();
				}
			}
			cout<<endl;
			double total=positive+negative;
			cout<<"\n\n Positive Sentiments: "<<positive;
			cout<<"\n\n Negative Sentiments: "<<negative;
			cout<<"\n\n Total Sentimental References: "<<total;
			polarity=positive/total;
			cout<<endl<<"\n Polarity: "<<polarity;
			if(positive == negative)
			{
				cout<<endl<<"\n Sentiment possessed is NEUTRAL ";
			}
			else if(polarity > 0)
			{
				cout<<endl<<"\n Sentiment possessed is POSITIVE ";
			}
			
			else if(polarity <= 0)
			{
				cout<<endl<<"\n Sentiment possessed is NEGATIVE ";
			}
			cout<<"\n******************************************************************************************************"<<endl;
		}

		void upperToLowerCase()
		{
			transform(userinput.begin(),userinput.end(),userinput.begin(),::tolower);
		}
		
		void Title()
		{
			gotoxy(51,3);
			cout<<"===========================================================";
			gotoxy(58,4);
			cout<<"AIRLINE CUSTOMER'S REVIEW'S SENTIMENT ANALYSER";
			gotoxy(51,5);
			cout<<"===========================================================";
			gotoxy(51,9);
			cout<<"Enter Customer's Review For Analysis ";
			gotoxy(51,11);
		}

		void user_input(int choice)
		{
			system("cls");
			Title();
			gotoxy(51,11);
			cout<<"\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2";
			gotoxy(51,20);
			cout<<"\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2"; 
			gotoxy(51,13);
			fflush(stdin);
			getline(cin,userinput);
			ofstream out;
			out.open("Input.txt",ios_base::app);
			upperToLowerCase();
			out<<userinput<<endl;
			wordbreak();
			word_search();
			result(choice);
			out.close();
		}
		
		void file_input(int choice)
		{
			system("cls");
			gotoxy(51,3);
			cout<<"===========================================================";
			gotoxy(58,4);
			cout<<"AIRLINE CUSTOMER'S REVIEW'S ANALYSIS REPORT";
			gotoxy(51,5);
			cout<<"===========================================================\n\n\n";
			fstream inp("Input.txt");	
			while(!inp.eof())
			{
				stat++;
				positive=0;
				negative=0;
				userinput.clear();
				getline(inp,userinput);
				if(userinput=="\n"||userinput=="\0")
					return;
				
				upperToLowerCase();
				wordbreak();
				word_search();
				result(choice);
			}	
			inp.close();
		}
		
		void view_all()
		{
			ifstream out("Output.txt");
			system("cls");
			gotoxy(51,3);
			cout<<"===========================================================";
			gotoxy(58,4);
			cout<<"AIRLINE CUSTOMER'S REVIEW'S ANALYSIS REPORT";
			gotoxy(51,5);
			cout<<"===========================================================\n\n\n";
			string outString;
			while(!out.eof())
			{
				getline(out,outString);	
				cout<<outString<<endl;
			}
			cout<<"******************************************************************************************************"<<endl;
			out.close();
		}
};

void Main_Menu(){
	sentiment sen;
	sen.negtree();
	sen.postree();
	gotoxy(23,8); char choice;
	cout<<"\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2";
	gotoxy(23,10);
	cout<<"Press [1]. Place Customer's Review For Sentiment Analysis";
	gotoxy(23,12);
	cout<<"Press [2]. Place Multiple Customer's Review For Sentiment Analysis from file";
	gotoxy(23,14);
	cout<<"Press [3]. View Sentiment Analysis Of All The Customers's Reviews";
	gotoxy(23,16);
	cout<<"Press [4]. Exit";
	gotoxy(23,18);
	cout<<"\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2";
	a:gotoxy(23,21);
	cout<<"Enter Your Choice: ";
	choice=getch();
	if(choice=='1'){
		system("cls");
		sen.user_input(choice);
		cout<<endl<<endl;
		system("pause");
		system("cls");
	}	
	
	else if(choice=='2'){
		system("cls");
		sen.file_input(choice);
		cout<<endl;
		system("pause");
		system("cls");
	}
	
	else if(choice=='3'){
		system("cls");
		sen.view_all();
		cout<<endl;
		system("pause");
		system("cls");
	}
	
	else if(choice=='4'){
		system("cls");
		cout<<"\nThank You!";
		exit(0);
	}
	
	else{
		gotoxy(46,31);
		cout<<"Please Enter Option 1 To 4 Only...";
		goto a;
	}
}

int main(){
	Main_Menu();
	return 0;
}
