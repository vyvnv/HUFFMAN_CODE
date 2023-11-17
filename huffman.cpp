#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

class HuffmanTree
{
public:
    class Node;                                                                                        

    HuffmanTree();                                                                                   

    ~HuffmanTree();                                                                             

    void BuildHuffmanTree(const string& text);                                              

    string EncodeSym(char symbol) const;                                                           

    string Encode(const string& text) const;                              

    string Decode(const string& text) const;                                                 

private:
    Node* m_root = nullptr;

    void DestructorHelp(Node* node);                                                              

    void CountFrequencies(Node* node, unordered_map<char, int>& frequencyMap) const;           

    void EncodeHelp(Node* node, char symbol, string currentCode, string& encodedSymbol) const;
};

class HuffmanTree::Node
{
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

void HuffmanTree::BuildHuffmanTree(const string& text)
{
    unordered_map<char, int> frequencyMap;

    for (char huffmanChar : text)
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

    list<Node*> nodeList;

    for (auto& element : frequencyMap)
    {
        nodeList.push_back(new Node(element.first, element.second));
    }

    while (nodeList.size() != 1)
    {
        nodeList.sort([](Node* left, Node* right)
            {
                return left->m_frequency < right->m_frequency;
            });

        Node* node1 = nodeList.front();
        nodeList.pop_front();
        Node* node2 = nodeList.front();
        nodeList.pop_front();

        Node* newNode = new Node('\0', node1->m_frequency + node2->m_frequency, node1, node2);
        nodeList.push_back(newNode);
    }

    m_root = nodeList.front();
    nodeList.pop_front();
}

string HuffmanTree::EncodeSym(char symbol) const
{
    string encodedSymbol = "";
    EncodeHelp(m_root, symbol, "", encodedSymbol);

    return encodedSymbol;
}

string HuffmanTree::Encode(const string& text) const
{
    string encodedText = "";

    for (char huffmanChar : text)
    {
        encodedText += EncodeSym(huffmanChar);
    }

    return encodedText;
}

void HuffmanTree::EncodeHelp(Node* node, char symbol, string currentCode, string& encodedSymbol) const
{
    if (!node)
    {
        return;
    }

    if (node->m_char == symbol)
    {
        encodedSymbol = currentCode;

        return;
    }

    EncodeHelp(node->m_left, symbol, currentCode + "0", encodedSymbol);
    EncodeHelp(node->m_right, symbol, currentCode + "1", encodedSymbol);
}

string HuffmanTree::Decode(const string& text) const
{
    string decodedText = "";
    Node* currentNode = m_root;

    for (char huffmanChar : text)
    {
        if (huffmanChar == '0')
        {
            currentNode = currentNode->m_left;
        }
        else
        {
            currentNode = currentNode->m_right;
        }

        if (!currentNode->m_left && !currentNode->m_right)
        {
            decodedText += currentNode->m_char;
            currentNode = m_root;
        }
    }

    return decodedText;
}

void HuffmanTree::CountFrequencies(Node* node, unordered_map<char, int>& frequencyMap) const
{
    if (!node)
    {
        return;
    }

    if (!node->m_left && !node->m_right)
    {
        if (frequencyMap.count(node->m_char) == 0)
        {
            frequencyMap[node->m_char] = 1;
        }
        else
        {
            frequencyMap[node->m_char]++;
        }
    }

    CountFrequencies(node->m_left, frequencyMap);
    CountFrequencies(node->m_right, frequencyMap);
}

int main()
{
    setlocale(LC_ALL, "Russian");

    ifstream inputFile("input.txt");
    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    HuffmanTree labHuffmanTree;
    labHuffmanTree.BuildHuffmanTree(text);

    auto result = labHuffmanTree.Encode(text);
    string encodedText = result;

    ofstream encodedFile("encoded.txt");
    encodedFile << encodedText;
    encodedFile.close();

    ifstream encodedInputFile("encoded.txt");
    string encodedInputText((istreambuf_iterator<char>(encodedInputFile)), istreambuf_iterator<char>());

    string decodedText = labHuffmanTree.Decode(encodedInputText);
    ofstream decodedFile("decoded.txt");
    decodedFile << decodedText;
    decodedFile.close();

    cout << ((text == decodedText) ? "decoded" : "not decoded") << endl;

    return 0;
}