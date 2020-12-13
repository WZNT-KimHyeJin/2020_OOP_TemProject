#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
void consolePrint(string messsage);
void printSepLine();
void controller();
void readVec(vector<string> vec, int start);
static string FILENAME = "test.txt";

class copyTop {
private:
	vector<string> c_vector;
	static copyTop* ct_instance;

public:
	vector<string> get_vec() {
		return this->c_vector;
	}
	void set_vec(vector<string> new_vec) {
		this->c_vector.swap(new_vec);
	}
	static copyTop* instance()
	{
		if (!ct_instance) {
			ct_instance = new copyTop;
		}
		return ct_instance;
	}
};
copyTop* copyTop::ct_instance = 0;

// 조건에 맞는 데이터를 저장한 vector를 반환하는 기능들을 저장한 class
class vectorAction {
public:
	//파일에서부터 값을 읽어와 조건에 알맞게 문장을 벡터에 저장한다. file -> line
	vector<string> fileRnS(string fileName) {
		ifstream ifile;
		ifile.open(fileName);

		char* line;	//파일에서 읽어 올 문장을 저장하는 문자열
		int maxSize = 75; // 한 줄당 최대 크기

		vector<string> vec; // 파일에서 읽어 온 문자열들을 저장할 vextor
		int lastSpace = 0; // 엔터가 있을 경우 index 저장.

		line = new char[76]; // 출력할 한 줄

		if (ifile.is_open()) {
			int count = 0; // vector의 index를 파악하기 위한 counting value
			int wordNumCheck = 0; // maxSize를 넘어가지 않게 체크

			while (!ifile.eof()) {

				ifile.get(line[wordNumCheck]); // 파일에서 한 글자씩 읽어와서 임시 저장 문자열인 line 에 할당한다.
				wordNumCheck++; // line의 index 수를 counting 하기 위함 변수
				if (line[wordNumCheck - 1] == ' ') { // 이후에 개행을 위해 마지막 공백의 index+1 위치를 저장한다.
					lastSpace = wordNumCheck;
				}
				if (wordNumCheck == maxSize) { // line에 저장할 수 있는 문자의 크기가 최대가 되었을 때
					char save; // 개행을 위해 저장할 다음 문자
					ifile.get(save);

					if (line[maxSize - 1] == ' ') { // line의 마지막 ==' ' ==> pass
						line[maxSize - 1] = '\0'; // 문자열이 끝났음을 표시
						vec.push_back(line); // vector의 마지막에 line을 추가한다.

						line = new char[76]; // 임시 저장 배열을 초기화 한다.
						line[0] = save; // 초기화 이후 미리 저장했던 다음 문자를 배열 가장 처음에 저장.
						wordNumCheck = 1; // index 수정
						continue; //진행
					}
					else if (save == ' ') {
						line[maxSize] = '\0';
						vec.push_back(line);
						line = new char[76]; // 배열 초기화
						wordNumCheck = 0;
						continue;
					}
					else { // 처리해야 할 경우
						int saveSize = 75 - lastSpace; // 잘린 단어의 앞부분 길이
						char store[20];
						for (int i = lastSpace; i < 75; i++) {
							store[i - lastSpace] = line[i];
						}
						store[saveSize] = '\0';
						line[lastSpace] = '\0';
						vec.push_back(line);

						line = new char[76];
						for (int i = 0; i < saveSize; i++) {
							line[i] = store[i];
						}
						line[saveSize] = save;
						wordNumCheck = saveSize + 1;
						continue;

					}
				}
			}
			line[wordNumCheck] = '\0';
			vec.push_back(line);
		}
		ifile.close();
		return vec;
	}
	//파라미터로 입력받은 파일에 접근하여 파일 안에 있는 단어로 이루어져있는 vector를 반환한다.
	vector<string> fileLineToWord(string fileName) { //단어단위로 벡터에 저장
		string allContext;
		ifstream ifile;
		ifile.open(fileName);

		vector<string> returnVec;
		if (ifile.is_open()) {
			getline(ifile, allContext);
			ifile.close();

			vector<char> file_read(allContext.begin(), allContext.end());
			file_read.push_back('\0');
			char* ptr = &file_read[0];
			char* words = strtok(ptr, " ");

			int count = 0;
			while (words != NULL) {

				returnVec.push_back(words);
				words = strtok(NULL, " ");
			}
			return returnVec;
		}
		else {
			cout << "Unable to open file";
		}
	}
	//단어들로 구성되어있는 vector를 파라미터로 받아 조건에 맞는 문장을 갖는 vector를 반환한다. word-> line
	vector<string> fileWordtoLine(vector<string> prevVec) {

		string word;
		string saveLine;	//벡터에서 받아온 단어들로 만든 문자열
		vector<string> returnVec; //return 할 vector
		int count = 0;
		int wordLengCheck = 0; // maxSize를 넘어가지 않게 체크

		while (true)
		{
			if (count >= (int)prevVec.size()) {
				returnVec.push_back(saveLine);
				break;
			}//vector를 모두 돌았을 경우

			word = prevVec[count++] + " ";

			wordLengCheck += word.length();

			if (wordLengCheck - 1 >= 75) { //마지막 띄어쓰기 고려( -1)
				returnVec.push_back(saveLine);
				saveLine = word;
				wordLengCheck = word.length();
			}
			else {
				saveLine += word;
			}
		}
		return returnVec;
	}
	//파라미터로 입력받은 벡터를 단어 단위로 저장한 string vector로 반환한다. vec line -> vec word
	vector<string> fileVecLineToWord(vector<string> lineVector) {
		int count = 0;
		vector<string> wordsVec;
		while (true)
		{
			if (count >= (int)lineVector.size()) {
				break;
			}
			else {
				string line = lineVector[count];
				int lineSize = line.length();
				char* charLine = 0;
				charLine = new char[lineSize + 1];

				strcpy(charLine, line.c_str());
				charLine[lineSize] = '\0';

				char* word = strtok(charLine, " ");
				while (word != NULL) {
					wordsVec.push_back(string(word));
					word = strtok(NULL, " ");
				}
				count++;
			}

		}
		return wordsVec;
	}
	// 파라미터로 받은 문자열을 단어로 저장하는 vector를 반환한다. string(line) -> vec word
	vector<string> strTowv(string vectorLine) { // 벡터 원소값을 wordvector로 변환.
		vector<string> wordVec;

		int lineLength = vectorLine.length();
		char* charLine = 0;
		charLine = new char[lineLength + 1];
		strcpy(charLine, vectorLine.c_str());
		charLine[lineLength] = '\0';
		char* word = strtok(charLine, " ");
		while (word != NULL) {
			wordVec.push_back((string)word);
			word = strtok(NULL, " ");
		}
		return wordVec;

	}
	//사용자로부터 입력받은 값을 vector형태로 반환한다.
	vector<string> getInputs(int reNum, char* userInput) {
		vector<string> returnVec;

		string inputValue(userInput); //c(word1,word2)
		if (userInput[1] != '(' || userInput[inputValue.size() - 1] != ')') {
			consolePrint("(console) 입력이 형식에 맞지 않습니다. 다시 입력 해 주세요 ( ex : i(1,2,Hello) )");
			printSepLine();
			readVec(copyTop::instance()->get_vec(), 0);
			controller();
		}
		string subValue = inputValue.substr(2, inputValue.size() - 3);

		char* val2ch = new char[subValue.size() + 1];
		strcpy(val2ch, subValue.c_str());
		val2ch[subValue.size()] = '\0';

		char* storeInputs = strtok(val2ch, ",");
		returnVec.push_back(string(storeInputs));
		for (int i = 0; i < reNum - 1; i++) {
			storeInputs = strtok(NULL, ",");
			if (storeInputs == NULL) {
				consolePrint("(console) 올바르지 않은 입력입니다. 다시 입력 해 주세요.");

				controller();
			}
			returnVec.push_back(string(storeInputs));

		}
		storeInputs = strtok(NULL, ",");
		if (storeInputs != NULL) {
			consolePrint("(console) 올바르지 않은 입력입니다. 다시 입력 해 주세요.");

			controller();
		}


		return returnVec;

	}
};
class checking {
public:
	bool checkMaxbyte(string beCheckedString) { //글자수 75바이트 체크

		if (beCheckedString.size() >= 75) {
			return false;
		}
		else {
			return true;
		}
	}
	bool checkIndex(int start, int lineLength, int lineNum, int wordNum) {
		if (lineNum >= start + 20 || lineNum < start) {
			consolePrint("(console) 현재 출력 창에 " + to_string(lineNum) + "번째 라인은 존재하지않습니다.");
			return false;
		}
		else if (wordNum > lineLength) {
			consolePrint("(console)" + to_string(lineNum) + "번째 라인에" + to_string(wordNum) + "번째 단어는 존재하지 않습니다.");
			return false;
		}
		else {
			return true;
		}
	}

};

// 1. fileAction : 사용자로부터 입력받은 일들을 처리하는 class들의 상위 클래스
class fileAction {
protected:
	//모든 fil_Action들이 종료되고 공통적으로 실행
	void afterFileAction(vector<string> vec, int startPoint) {
		vectorAction* va = new vectorAction;
		vec = va->fileVecLineToWord(vec);
		vec = va->fileWordtoLine(vec);
		//출력 양식에 알맞는 데이터를저장하고 있는 vector로 변환
		copyTop::instance()->set_vec(vec); // 최종 데이터를 가지고 있는 singletone의 vector에 저장

		readVec(copyTop::instance()->get_vec(), startPoint);
	}
public:
	static fileAction* doingAct(char* userInput);
	virtual void doFileAction() = 0;

};
// 2. 파일 페이지 관련 명령 클래스(n,p) fileAction을 상속받는다.(doFileAction)
class file_page : public fileAction {

private:
	vector<string> fileReadVec = copyTop::instance()->get_vec();
	int startingPoint = 0;
	static file_page* file_instance;
	char check_mode = ' ';

public:
	void setMode(char mode) {
		this->check_mode = mode;
	}
	char getMode() {
		return this->check_mode;
	}
	void setStart(int start) {
		startingPoint = start;
	}
	int getStart() {
		return startingPoint;
	}
	void doFileAction() {
		if (check_mode == 'n') { //nextPage
			if (startingPoint + 20 < (int)fileReadVec.size()) {
				startingPoint += 20;
			}
			if (startingPoint + 20 >= (int)fileReadVec.size()) {
				startingPoint = (int)fileReadVec.size() - 20;
			}
			readVec(fileReadVec, startingPoint);
		}
		else if (check_mode == 'p') {//prevpage
			if (startingPoint - 20 >= 0) {
				startingPoint -= 20;
				readVec(fileReadVec, startingPoint);
			}
			else {
				startingPoint = 0;
				readVec(fileReadVec, startingPoint);
				printSepLine();

				consolePrint("this is the first page!");

			}
		}
	}
	static file_page* instance() {
		if (!file_instance) {
			file_instance = new file_page;
		}
		return file_instance;
	}

};
file_page* file_page::file_instance = 0;
// 3. insert 관련 클래스. fileAction을 상속받는다.(doFileAction)
class file_insert : public fileAction {
private:
	vector<string> changedVec = copyTop::instance()->get_vec();
	int lineNum = 0;
	int wordNum = 0;
	string saveWord; //삽입 할 문자열
public:
	void doFileAction() {
		checking* ck = new checking;
		string targetString = changedVec[lineNum - 1];
		vectorAction* va = new vectorAction;
		//입력 받은 string을 단어단위의 vector로 변경한다.
		vector<string> lineWord = va->strTowv(targetString);

		int start = file_page::instance()->getStart();
		if (!ck->checkIndex(start, (int)lineWord.size(), lineNum, wordNum)) {
			return;
		}
		if (!ck->checkMaxbyte(saveWord)) {
			return;
		}

		string insertedLine; //단어가 추가 된 문자열

		for (int i = 0; i < (int)lineWord.size(); i++) {
			if (i == wordNum) {
				insertedLine += saveWord + " ";
			}
			insertedLine += lineWord[i] + " ";
		}
		if (wordNum == (int)(int)lineWord.size()) { //출력 문장의 가장 마지막에 삽입할 경우.
			insertedLine += saveWord + " ";
		}
		//데이터가 저장되어있는 vector를 수정한다.
		changedVec[lineNum - 1] = insertedLine;

		afterFileAction(changedVec, 0);
		file_page::instance()->setStart(0);

	}
	void setLineNum(int newLine) {
		this->lineNum = newLine;
	}
	int getLineNum() {
		return this->lineNum;
	}
	void setWordNum(int newWordNum) {
		this->wordNum = newWordNum;
	}
	int getWordNum() {
		return this->wordNum;
	}
	void setString(string newInsertWord) {
		this->saveWord = newInsertWord;
	}
	string getString() {
		return this->saveWord;
	}

};
// 4, delete 관련 클래스 fileAction을 상속받는다.(doFileAction)
class file_delete : public fileAction {
private:
	vector<string> deleteVec = copyTop::instance()->get_vec();
	int lineNum = 0;
	int wordNum = 0;
public:
	void doFileAction() {
		vectorAction* va = new vectorAction;
		checking* ck = new checking;

		vector<string> changedLine = va->strTowv(deleteVec[lineNum - 1]);
		string changed;
		int start = file_page::instance()->getStart();
		if (!ck->checkIndex(start, (int)changedLine.size(), lineNum, wordNum)) {
			return;
		}
		for (int i = 0; i < (int)changedLine.size(); i++) {
			if (i != wordNum - 1) {
				changed += changedLine[i] + " ";
			}
		}
		deleteVec.erase(deleteVec.begin() + lineNum - 1);
		deleteVec.insert(deleteVec.begin() + lineNum - 1, changed);

		afterFileAction(deleteVec, 0);
		file_page::instance()->setStart(0);

	}
	void setLineNum(int newLineNum) {
		this->lineNum = newLineNum;
	}
	int getLineNum() {
		return this->lineNum;
	}
	void setwordNum(int newWordNum) {
		this->wordNum = newWordNum;
	}
	int getWordNum() {
		return this->wordNum;
	}
};
// 5. change 관련 클래스 fileAction을 상속받는다.(doFileAction)
class file_change : public fileAction {
private:
	vector<string> unChangeVec = copyTop::instance()->get_vec();
	vector<string> changedVec;
	string org;
	string change;

	vector<string> wordVec;

public:
	void doFileAction() {
		checking* ck = new checking;
		if (!ck->checkMaxbyte(org) || !ck->checkMaxbyte(change)) {
			return;
		}
		vectorAction* va = new vectorAction;

		wordVec = va->fileVecLineToWord(unChangeVec);
		for (int i = 0; i < (int)wordVec.size(); i++) {
			if (wordVec[i] == org) {
				changedVec.push_back(change);
			}
			else {
				changedVec.push_back(wordVec[i]);
			}
		}
		changedVec = va->fileWordtoLine(changedVec);
		copyTop::instance()->set_vec(changedVec);
		readVec(copyTop::instance()->get_vec(), 0);
		file_page::instance()->setStart(0);
	}
	void setWord1(string newWord1) {
		this->org = newWord1;
	}
	void setWord2(string newWord2) {
		this->change = newWord2;
	}

};
// 6. 저장 후 종료 관련 클래스fileAction을 상속받는다.(doFileAction)
class file_Save_Quit : public fileAction {
	vector<string> saveVec = copyTop::instance()->get_vec();
public:
	void doFileAction() {
		ofstream of(FILENAME);
		if (of.is_open()) {
			for (int i = 0; i < (int)saveVec.size(); i++) {
				of << saveVec[i];
			}
			of.close();
		}
		consolePrint("(console) 작업이 종료되었습니다.");
	}

};
// 7. search 
class file_search : public fileAction {
private:
	vector<string> searchVec = copyTop::instance()->get_vec();
	string findCon;
public:
	void doFileAction() {
		checking* ck = new checking;
		if (!ck->checkMaxbyte(findCon)) {
			return;
		}
		for (int i = 0; i < (int)searchVec.size(); i++) {
			if (searchVec[i].find(findCon) != string::npos) {
				readVec(copyTop::instance()->get_vec(), i);
				return;
			}
		}
		consolePrint("(console) 찾고자 하는 값이 존재하지 않습니다.");
		printSepLine();
		readVec(copyTop::instance()->get_vec(), 0);
		return;
	}
	void setFind(string cont) {
		this->findCon = cont;
	}
	string getFine() {
		return this->findCon;
	}
};

void printSepLine() {
	cout << "---------------------------------------------------------------------------------\n";
}
void consolePrint(string message) {
	cout << message << endl;
}
void controller() {
	char userInput[20];

	printSepLine();
	cout << "n:다음 페이지, p:이전 페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장 후 종료" << endl;;
	printSepLine();

	while (1) {
		cout << "입력:";
		cin >> userInput;
		printSepLine();
		fileAction* currentAction = fileAction::doingAct(userInput);
		currentAction->doFileAction();
		if (userInput[0] == 't') {
			break;
		}
		printSepLine();
		cout << "n:다음 페이지, p:이전 페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장 후 종료" << endl;;
		printSepLine();
	}
}
void readVec(vector<string> vec, int start) {
	int count = 0;
	try {
		while (1) {
			if (start >= (int)vec.size()) {
				printSepLine();
				consolePrint("this is the last page!");
				break;
			}
			if (count == 20) {
				break;
			}
			cout.width(2);
			cout << start + 1;
			cout << " | " << vec[start] << endl;
			start++;
			count++;
		}
	}
	catch (const std::out_of_range) {
	}
}

fileAction* fileAction::doingAct(char* userInput) {
	vectorAction* va = new vectorAction;
	checking* chk = new checking;

	if (userInput[0] == 'n' || userInput[0] == 'p') {
		file_page::instance()->setMode(userInput[0]);
		return file_page::instance();
	}
	else if (userInput[0] == 't') { // 종료
		return new file_Save_Quit;
	}
	else if (userInput[0] == 'c') {
		file_change* fc = new file_change;
		vector<string> inputs = va->getInputs(2, userInput);
		fc->setWord1(inputs[0]);
		bool wordCheck_1 = chk->checkMaxbyte(inputs[0]);
		bool wordCheck_2 = chk->checkMaxbyte(inputs[1]);
		if (!wordCheck_1 || !wordCheck_2) {
			consolePrint("(consol) 올바르지 않은 입력입니다. 다시 입력 해 주세요.");
			controller();
		}
		else {
			fc->setWord2(inputs[1]);
			return fc;
		}


	}
	else if (userInput[0] == 'i') {
		vector<string> inputs = va->getInputs(3, userInput);
		file_insert* fi = new file_insert;
		try {
			fi->setLineNum(stoi(inputs[0]));
			fi->setWordNum(stoi(inputs[1]));
		}
		catch (exception e) {
			consolePrint("(console) 올바르지 않은 입력입니다. 다시 입력 해 주세요.");
			controller();
		}

		if (!chk->checkMaxbyte(inputs[2])) {
			consolePrint("(consol) 올바르지 않은 입력입니다. 다시 입력 해 주세요.");
			controller();
		}
		else {
			fi->setString(inputs[2]);
			return fi;
		}
	}
	else if (userInput[0] == 'd') {
		vector<string> inputs = va->getInputs(2, userInput);
		file_delete* fd = new file_delete;
		try {
			fd->setLineNum(stoi(inputs[0]));
			fd->setwordNum(stoi(inputs[1]));
		}
		catch (exception e) {
			consolePrint("(console) 올바르지 않은 입력입니다. 다시 입력 해 주세요.");
			controller();
		}
		return fd;
	}
	else if (userInput[0] == 's') {
		string inputValue(userInput); //c(word1,word2)
		string storeInputs = inputValue.substr(2, inputValue.size() - 3);

		file_search* fs = new file_search;
		fs->setFind(storeInputs);
		return fs;
	}
	else {
		consolePrint("(console) 일치하는 명령이 없습니다. 다시 입력 해 주세요.");
		controller();
	}
}

int main() {
	vector<string> infoVec; // 파일에 대한 정보를 저장하고 있을 vector를 생성한다.
	vectorAction* va = new vectorAction;

	infoVec = va->fileRnS(FILENAME); // 파일에서 읽어 온 vector 값을 저장한다.
	copyTop::instance()->set_vec(infoVec);
	readVec(copyTop::instance()->get_vec(), 0);
	controller();
}
