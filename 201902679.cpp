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

// ���ǿ� �´� �����͸� ������ vector�� ��ȯ�ϴ� ��ɵ��� ������ class
class vectorAction {
public:
	//���Ͽ������� ���� �о�� ���ǿ� �˸°� ������ ���Ϳ� �����Ѵ�. file -> line
	vector<string> fileRnS(string fileName) {
		ifstream ifile;
		ifile.open(fileName);

		char* line;	//���Ͽ��� �о� �� ������ �����ϴ� ���ڿ�
		int maxSize = 75; // �� �ٴ� �ִ� ũ��

		vector<string> vec; // ���Ͽ��� �о� �� ���ڿ����� ������ vextor
		int lastSpace = 0; // ���Ͱ� ���� ��� index ����.

		line = new char[76]; // ����� �� ��

		if (ifile.is_open()) {
			int count = 0; // vector�� index�� �ľ��ϱ� ���� counting value
			int wordNumCheck = 0; // maxSize�� �Ѿ�� �ʰ� üũ

			while (!ifile.eof()) {

				ifile.get(line[wordNumCheck]); // ���Ͽ��� �� ���ھ� �о�ͼ� �ӽ� ���� ���ڿ��� line �� �Ҵ��Ѵ�.
				wordNumCheck++; // line�� index ���� counting �ϱ� ���� ����
				if (line[wordNumCheck - 1] == ' ') { // ���Ŀ� ������ ���� ������ ������ index+1 ��ġ�� �����Ѵ�.
					lastSpace = wordNumCheck;
				}
				if (wordNumCheck == maxSize) { // line�� ������ �� �ִ� ������ ũ�Ⱑ �ִ밡 �Ǿ��� ��
					char save; // ������ ���� ������ ���� ����
					ifile.get(save);

					if (line[maxSize - 1] == ' ') { // line�� ������ ==' ' ==> pass
						line[maxSize - 1] = '\0'; // ���ڿ��� �������� ǥ��
						vec.push_back(line); // vector�� �������� line�� �߰��Ѵ�.

						line = new char[76]; // �ӽ� ���� �迭�� �ʱ�ȭ �Ѵ�.
						line[0] = save; // �ʱ�ȭ ���� �̸� �����ߴ� ���� ���ڸ� �迭 ���� ó���� ����.
						wordNumCheck = 1; // index ����
						continue; //����
					}
					else if (save == ' ') {
						line[maxSize] = '\0';
						vec.push_back(line);
						line = new char[76]; // �迭 �ʱ�ȭ
						wordNumCheck = 0;
						continue;
					}
					else { // ó���ؾ� �� ���
						int saveSize = 75 - lastSpace; // �߸� �ܾ��� �պκ� ����
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
	//�Ķ���ͷ� �Է¹��� ���Ͽ� �����Ͽ� ���� �ȿ� �ִ� �ܾ�� �̷�����ִ� vector�� ��ȯ�Ѵ�.
	vector<string> fileLineToWord(string fileName) { //�ܾ������ ���Ϳ� ����
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
	//�ܾ��� �����Ǿ��ִ� vector�� �Ķ���ͷ� �޾� ���ǿ� �´� ������ ���� vector�� ��ȯ�Ѵ�. word-> line
	vector<string> fileWordtoLine(vector<string> prevVec) {

		string word;
		string saveLine;	//���Ϳ��� �޾ƿ� �ܾ��� ���� ���ڿ�
		vector<string> returnVec; //return �� vector
		int count = 0;
		int wordLengCheck = 0; // maxSize�� �Ѿ�� �ʰ� üũ

		while (true)
		{
			if (count >= (int)prevVec.size()) {
				returnVec.push_back(saveLine);
				break;
			}//vector�� ��� ������ ���

			word = prevVec[count++] + " ";

			wordLengCheck += word.length();

			if (wordLengCheck - 1 >= 75) { //������ ���� ���( -1)
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
	//�Ķ���ͷ� �Է¹��� ���͸� �ܾ� ������ ������ string vector�� ��ȯ�Ѵ�. vec line -> vec word
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
	// �Ķ���ͷ� ���� ���ڿ��� �ܾ�� �����ϴ� vector�� ��ȯ�Ѵ�. string(line) -> vec word
	vector<string> strTowv(string vectorLine) { // ���� ���Ұ��� wordvector�� ��ȯ.
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
	//����ڷκ��� �Է¹��� ���� vector���·� ��ȯ�Ѵ�.
	vector<string> getInputs(int reNum, char* userInput) {
		vector<string> returnVec;

		string inputValue(userInput); //c(word1,word2)
		if (userInput[1] != '(' || userInput[inputValue.size() - 1] != ')') {
			consolePrint("(console) �Է��� ���Ŀ� ���� �ʽ��ϴ�. �ٽ� �Է� �� �ּ��� ( ex : i(1,2,Hello) )");
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
				consolePrint("(console) �ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է� �� �ּ���.");

				controller();
			}
			returnVec.push_back(string(storeInputs));

		}
		storeInputs = strtok(NULL, ",");
		if (storeInputs != NULL) {
			consolePrint("(console) �ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է� �� �ּ���.");

			controller();
		}


		return returnVec;

	}
};
class checking {
public:
	bool checkMaxbyte(string beCheckedString) { //���ڼ� 75����Ʈ üũ

		if (beCheckedString.size() >= 75) {
			return false;
		}
		else {
			return true;
		}
	}
	bool checkIndex(int start, int lineLength, int lineNum, int wordNum) {
		if (lineNum >= start + 20 || lineNum < start) {
			consolePrint("(console) ���� ��� â�� " + to_string(lineNum) + "��° ������ ���������ʽ��ϴ�.");
			return false;
		}
		else if (wordNum > lineLength) {
			consolePrint("(console)" + to_string(lineNum) + "��° ���ο�" + to_string(wordNum) + "��° �ܾ�� �������� �ʽ��ϴ�.");
			return false;
		}
		else {
			return true;
		}
	}

};

// 1. fileAction : ����ڷκ��� �Է¹��� �ϵ��� ó���ϴ� class���� ���� Ŭ����
class fileAction {
protected:
	//��� fil_Action���� ����ǰ� ���������� ����
	void afterFileAction(vector<string> vec, int startPoint) {
		vectorAction* va = new vectorAction;
		vec = va->fileVecLineToWord(vec);
		vec = va->fileWordtoLine(vec);
		//��� ��Ŀ� �˸´� �����͸������ϰ� �ִ� vector�� ��ȯ
		copyTop::instance()->set_vec(vec); // ���� �����͸� ������ �ִ� singletone�� vector�� ����

		readVec(copyTop::instance()->get_vec(), startPoint);
	}
public:
	static fileAction* doingAct(char* userInput);
	virtual void doFileAction() = 0;

};
// 2. ���� ������ ���� ��� Ŭ����(n,p) fileAction�� ��ӹ޴´�.(doFileAction)
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
// 3. insert ���� Ŭ����. fileAction�� ��ӹ޴´�.(doFileAction)
class file_insert : public fileAction {
private:
	vector<string> changedVec = copyTop::instance()->get_vec();
	int lineNum = 0;
	int wordNum = 0;
	string saveWord; //���� �� ���ڿ�
public:
	void doFileAction() {
		checking* ck = new checking;
		string targetString = changedVec[lineNum - 1];
		vectorAction* va = new vectorAction;
		//�Է� ���� string�� �ܾ������ vector�� �����Ѵ�.
		vector<string> lineWord = va->strTowv(targetString);

		int start = file_page::instance()->getStart();
		if (!ck->checkIndex(start, (int)lineWord.size(), lineNum, wordNum)) {
			return;
		}
		if (!ck->checkMaxbyte(saveWord)) {
			return;
		}

		string insertedLine; //�ܾ �߰� �� ���ڿ�

		for (int i = 0; i < (int)lineWord.size(); i++) {
			if (i == wordNum) {
				insertedLine += saveWord + " ";
			}
			insertedLine += lineWord[i] + " ";
		}
		if (wordNum == (int)(int)lineWord.size()) { //��� ������ ���� �������� ������ ���.
			insertedLine += saveWord + " ";
		}
		//�����Ͱ� ����Ǿ��ִ� vector�� �����Ѵ�.
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
// 4, delete ���� Ŭ���� fileAction�� ��ӹ޴´�.(doFileAction)
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
// 5. change ���� Ŭ���� fileAction�� ��ӹ޴´�.(doFileAction)
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
// 6. ���� �� ���� ���� Ŭ����fileAction�� ��ӹ޴´�.(doFileAction)
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
		consolePrint("(console) �۾��� ����Ǿ����ϴ�.");
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
		consolePrint("(console) ã���� �ϴ� ���� �������� �ʽ��ϴ�.");
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
	cout << "n:���� ������, p:���� ������, i:����, d:����, c:����, s:ã��, t:���� �� ����" << endl;;
	printSepLine();

	while (1) {
		cout << "�Է�:";
		cin >> userInput;
		printSepLine();
		fileAction* currentAction = fileAction::doingAct(userInput);
		currentAction->doFileAction();
		if (userInput[0] == 't') {
			break;
		}
		printSepLine();
		cout << "n:���� ������, p:���� ������, i:����, d:����, c:����, s:ã��, t:���� �� ����" << endl;;
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
	else if (userInput[0] == 't') { // ����
		return new file_Save_Quit;
	}
	else if (userInput[0] == 'c') {
		file_change* fc = new file_change;
		vector<string> inputs = va->getInputs(2, userInput);
		fc->setWord1(inputs[0]);
		bool wordCheck_1 = chk->checkMaxbyte(inputs[0]);
		bool wordCheck_2 = chk->checkMaxbyte(inputs[1]);
		if (!wordCheck_1 || !wordCheck_2) {
			consolePrint("(consol) �ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է� �� �ּ���.");
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
			consolePrint("(console) �ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է� �� �ּ���.");
			controller();
		}

		if (!chk->checkMaxbyte(inputs[2])) {
			consolePrint("(consol) �ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է� �� �ּ���.");
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
			consolePrint("(console) �ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է� �� �ּ���.");
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
		consolePrint("(console) ��ġ�ϴ� ����� �����ϴ�. �ٽ� �Է� �� �ּ���.");
		controller();
	}
}

int main() {
	vector<string> infoVec; // ���Ͽ� ���� ������ �����ϰ� ���� vector�� �����Ѵ�.
	vectorAction* va = new vectorAction;

	infoVec = va->fileRnS(FILENAME); // ���Ͽ��� �о� �� vector ���� �����Ѵ�.
	copyTop::instance()->set_vec(infoVec);
	readVec(copyTop::instance()->get_vec(), 0);
	controller();
}
