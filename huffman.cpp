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

        codes[m_root -> m_char] = code;
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
}

void bitOut (ofstream& outputFile, char b) { // записать 

	current_byte <<= 1;

	if (b == '1') current_byte |= 1;

	nbits++;

	if (nbits == 8) {
        outputFile << current_byte;
		nbytes++;
		nbits = 0;
		current_byte = 0;
	}
}

bool Encode() {
    HuffmanTree example;

    unordered_map Text = example.FillTheMap("input.txt");

    example.BuildHuffmanTree(Text);

    unordered_map<char, string> codes;

    HuffmanTree::Node * root = example.getRoot();

    string code;

    example.TreeTour(root, code, codes);

	ifstream plainText("input.txt", ios_base::binary);
    const char * encodedFileName = "encoded.bin";
    ofstream readFile(encodedFileName, ios_base::binary);
    saveTable(readFile, codes);
    readFile.close();

    current_byte = 0;
	nbits = 0;
	nbytes = 0;
    
	unsigned char characterfromFile;
	char * charIndex;
	while()
    



}

//bool Test(){

//}

int main(){
  //  if (Test()) cout << "Encoded and decoded back sucsessfully";
  //  else cout << "Something went wrong";
  Encode();
}
