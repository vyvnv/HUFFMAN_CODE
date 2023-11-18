#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <cstring>
#include <algorithm>

using namespace std;

class HuffmanTree {
   public:

    class Node; 

    HuffmanTree(); 

    ~HuffmanTree(); 


    Node* m_root = nullptr;

    void DestructorHelp(Node* node);

    unordered_map<char, int> FillTheMap(string fileName);

    void BuildHuffmanTree(unordered_map<char, int>frequencyMap);
    

    void TreeTour(Node * m_root, string code, unordered_map<char, string>& codes); // аргументики

    Node * getRoot()const{
        return m_root;
        }
};

class HuffmanTree::Node {

public:
    char m_char;
    int m_frequency;
    Node* m_left;
    Node* m_right;

    Node(char huffmanChar, int frequency, Node* m_left = nullptr, Node* m_right = nullptr)
        : m_char(huffmanChar), m_frequency(frequency), m_left(m_left), m_right(m_right) {}
    

};

HuffmanTree::HuffmanTree()
{
    m_root = nullptr;
}

HuffmanTree::~HuffmanTree()
{
    DestructorHelp(m_root);
}

void HuffmanTree::DestructorHelp(Node* node)
{
    if (node)
    {
        DestructorHelp(node->m_left);
        DestructorHelp(node->m_right);
        delete node;
    }
}

 unordered_map<char, int> HuffmanTree::FillTheMap(string fileName){

    unordered_map<char, int> frequencyMap;

    ifstream inputFile(fileName);

     if (!inputFile.is_open()) {
        cerr << "Could not open the file." << endl;
        frequencyMap['0'] = -1;
        return frequencyMap;
    }

    char huffmanChar;

    while(inputFile.get(huffmanChar))
    {
        if (frequencyMap.count(huffmanChar) == 0)
        {
            frequencyMap[huffmanChar] = 1;
        }
        else
        {
            frequencyMap[huffmanChar]++;
        }
    }
	inputFile.close();
    return frequencyMap;

 }

void HuffmanTree::BuildHuffmanTree(unordered_map<char, int> frequencyMap)
{
    list<Node*> nodeList;

    for (auto& element : frequencyMap)
    {
        nodeList.push_back(new Node(element.first, element.second));
    } // занесли неотсортированные данные о частотах символов в список

    while (nodeList.size() > 1)
    {
        nodeList.sort([](Node* left, Node* right)
            {
                return left->m_frequency > right->m_frequency; // сортирую по убыванию частот
            });

		if (!nodeList.empty()) { // проверяем список на пустоту
            Node* node1 = nodeList.front(); // заносим в узел верхний  элемент из списка
            nodeList.pop_front(); // вытаскиваем из списка верхний элемент
            Node* node2 = nodeList.front();
            nodeList.pop_front();

            Node* newNode = new Node('\0', node1->m_frequency + node2->m_frequency, node1, node2);
            nodeList.push_back(newNode);
	    }
	}
        if (!nodeList.empty()){
          	m_root = nodeList.front();
		    nodeList.pop_front();// убираем вершину списка  
            cout << "Tree is done!";
        }
        else cout << "File is empty, unable to build the tree";
}



void HuffmanTree::TreeTour(Node * m_root, string code, unordered_map<char, string>& codes){


    if ((m_root -> m_left == nullptr) && (m_root -> m_right = nullptr)) {

        codes[m_root -> m_char] = '0';
        return;
        }

        else {

        TreeTour(m_root->m_left, code + "0", codes);
        TreeTour(m_root->m_right, code + "1", codes);
    }

}

int nbits, current_byte, nbytes;

void saveTable (ofstream& outputFile, unordered_map<char, string>& codes) // в откртый файл помещаем информацию о соответствии кодируемых символов и кодирующих цепочек
{
    outputFile << "{ ";

    for(const auto& pair : codes){
        char key = pair.first;
        string value = pair.second;
        outputFile << key << " : " << value;
    }

    outputFile << endl << " }" << endl;
    cout << "Table is written correctly";
}

/*void bitOut (ofstream& outputFile, char b) { // записать 

	current_byte <<= 1;

	if (b == '1') current_byte |= 1;

	nbits++;

	if (nbits == 8) {
        outputFile << current_byte;
		nbytes++;
		nbits = 0;
		current_byte = 0;
	}
}*/

void writeBitToFile(const string& filename, bool bit) {
    ofstream file(filename, ios::binary | ios::app);

    if (!file.is_open()) {
        std::cerr << "Unable to open the file" << std::endl;
        return;
    }
    // Буфер для хранения одного байта
    char buffer = 0;

    // Установка нужного бита в буфере
    if (bit) {
        buffer |= 1; // Устанавливаем младший бит в 1
    } else {
        buffer &= ~1; // Устанавливаем младший бит в 0
    }

    // Запись буфера в файл
    file.write(&buffer, 1);

    file.close();
}

bool Encode() {

    HuffmanTree object;

    unordered_map Text = object.FillTheMap("input.txt");

    object.BuildHuffmanTree(Text);

    unordered_map<char, string> codes;

    HuffmanTree::Node * root = object.getRoot(); cout << "I'm here!";

    string code;

    object.TreeTour(root, code, codes);

	ifstream plainText("input.txt", ios_base::binary); // это текст из которого читаем и который нужно закодировать
	if (!plainText.is_open()) {
        cout << "Couldn't open the plaintext";
        return 0;
    }
    cout << "I'm here";
    const char * encodedFileName = "encoded.bin";
    ofstream readFile(encodedFileName, ios_base::binary); //а это файл в который кодируем
	if (!readFile.is_open()) {
        cout << "Couldn't create a file";
        return 0;
    }

    saveTable(readFile, codes); // занесли коды символов в файл
   // readFile.close();

    current_byte = 0;
	nbits = 0;
	nbytes = 0;
    
	char characterfromFile;
	char * charIndex;

	while(!plainText.eof()) {
		plainText.get(characterfromFile);
		int len = codes[characterfromFile].length();
		for (int i = 0;(codes[characterfromFile])[i] < len; i++) { // перебираем символы строки кода символа.
			if ((codes[characterfromFile])[i] == '0') writeBitToFile(encodedFileName, false);
			if ((codes[characterfromFile])[i] == '1') writeBitToFile(encodedFileName, true);
			else {
				cout << "Error: wrong sequence of bits";
				return 1;
			}
		
		//for (charIndex = codes[characterfromFile]; * charIndex; charIndex++) bitOut(readFile, *charIndex);
		
		}
	}
	//while (nbits) bitOut (readFile, '0');
	readFile.close();
	plainText.close();
	return 1;
    
}

//bool Test(){

//}

int main(){
  //  if (Test()) cout << "Encoded and decoded back sucsessfully";
  //  else cout << "Something went wrong";
  if (!Encode()) cout << "Couldn't open the file";
  return 0;
}
