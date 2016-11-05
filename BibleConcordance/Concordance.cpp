//Matthew Thompson
//Honors Comp 170
//Bible Concordance Project

#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;
void getNextVerse(ifstream& fin, string& bookName, int& chapter, int& verse, string& verseText);
void writeToConcordance(int argumentCount, char* words[], int& printedVerseCount);
void searchForMatches(int argumentCount, char* words[], int& printedVerseCount, string& verseText, ofstream& fout, string& bookName, int& chapter, int& verse);

void main(int argc, char* argv[])
{
	if (argc == 1)
	{
		cout << "Usage:  concordance word ..." << endl;
		cout << "Searches bible.txt for any verses that contain one of the words provided." << endl;
		cout << "Results will be added to the end of concordance.txt." << endl;
	}
	else
	{
		cout << "|-------------------------------|" << endl;
		cout << "|";
		int printedVerseCount = 0;
		writeToConcordance(argc, argv, printedVerseCount);
		cout << "|" << endl;
		cout << "Added " << printedVerseCount << " verses." << endl;
	}
}

void writeToConcordance(int argumentCount, char* words[], int& printedVerseCount)
{
	ifstream fin("bible.txt");
	ofstream fout("concordance.txt", ios_base::app);
	for (int i = 1; i < argumentCount; i++)
	{
		fout << "\"" << words[i] << "\"" << ' ';
	}
	fout << endl;
	string bookName;
	int chapter;
	int verse;
	string verseText;
	int verseCount = 0;
	bool isInVerse = false;
	do
	{
		verseText = ' ';
		getNextVerse(fin, bookName, chapter, verse, verseText);
		verseCount++;
		searchForMatches(argumentCount, words, printedVerseCount, verseText, fout, bookName, chapter, verse);
		if (verseCount % 1000 == 0)
		{
			cout << '*';
		}
	} while (!fin.eof());
	fin.close();
	fout.close();
}

void getNextVerse(ifstream& fin, string& bookName, int& chapter, int& verse, string& verseText)
{
	string line;
	string nextline;
	string bookLine;
	char colon;
	do
	{
		line = nextline;
		istringstream sin(line);
		if (line[0] == 'B')
		{
			bookName = line.substr(8, bookLine.length() - 9);
		}
		else if (isdigit(line[0]))
		{
			sin >> chapter;
			sin >> colon;
			sin >> verse;
			verseText = line.substr(8, line.length() - 8);
			verseText += ' ';
		}
		else if (line.length() > 1)
		{
			string verseExtension;
			verseExtension = line.substr(8, line.length() - 8);
			verseText += verseExtension;
			verseText += ' ';
		}
		getline(fin, nextline);
	} while (nextline.length() > 1);
}

void searchForMatches(int argumentCount, char* words[], int& printedVerseCount, string& verseText, ofstream& fout, string& bookName, int& chapter, int& verse)
{
	for (int i = 1; i < argumentCount; i++)
	{
		int searchIndex = 0;
		if (verseText.find(words[i]) != string::npos)
		{
			bool hasPrintedVerse = false;
			while (verseText.find(words[i], searchIndex) != string::npos && !hasPrintedVerse)
			{
				char letterBefore = verseText[verseText.find(words[i], searchIndex) - 1];
				char letterAfter = verseText[verseText.find(words[i], searchIndex) + strlen(words[i]) ];
				if (!isalpha(letterBefore) && !isalpha(letterAfter))
				{
					fout << "\t" << bookName << ' ' << chapter << ':' << verse << endl;
					hasPrintedVerse = true;
					printedVerseCount++;
				}
				searchIndex = verseText.find(words[i], searchIndex) + strlen(words[i]) + 1;
			}
		}
	}
}
