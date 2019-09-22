#include "huffman_tree.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <sstream>
#include <array>
#include <stack>

//Sam Bevans-Kerr


//Personally, I had problems with ASCII value 26, but the rest (0-25 & 27-127) are fair game. - Joe
using namespace std;





/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/
huffman_tree::huffman_tree(const std::string &file_name){


    ifstream inFile(file_name);
    int charar[128] = {0};

    stringstream buff;
    char character;

    buff >> std::noskipws;
    buff << inFile.rdbuf();

    while(buff >> character) {

        charar[(int)character]++;
        if((int)character > 127) {
//      cout << "Invalid ASCII Code Given." << endl;
        }

        /*cout << character << endl;
        cout << (int)character << endl;*/
    }

    priority_queue<huffNode*, vector<huffNode*>, compareFreq> Q;

    for (int i=0; i<128; i++) {
        if(charar[i] != 0) {
            Q.push(new huffNode(charar[i], i));
        }
    }

    //prints queue
    /*while (!Q.empty()) {
        huffNode p = Q.top();
        Q.pop();
        cout << p.freq << " " << p.value << "\n";
    }*/

    //special case for only 1 character
    if (Q.size() == 1) {
        huffNode *r = new huffNode(Q.top()->freq, 0);
        r->left = Q.top();
        this->root = r;
        return;
    }

    while (Q.size() > 1 ) {

        huffNode *p = Q.top();
        Q.pop();

        huffNode *q = Q.top();
        Q.pop();

        huffNode *r = new huffNode(p->freq + q->freq, 0);

        r->left = p;
        r->right = q;

        Q.push(r);
        //cout << "This is combined frequency: " << r->freq << " it points left to " << (char)r->left->value << " and right to " << (char)r->right->value << endl;
    }
    this->root = Q.top();

//    cout <<"final frequency is " << this->root->freq << endl;
}

huffman_tree::~huffman_tree(){
}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const {
//calls findChar, which modifies a stack. Said stack has the huffman code, but backwards, so the code is transferred to another stack before the string

    stack <int> oldStack;
    stack <int> newStack;
    bool hasCode;
    string s = "";


    if (findChar(this->root, hasCode, character, oldStack)) {

        int x = oldStack.size();

        for (int i = 0; i < x; i++) {

            // cout << oldStack.top() << endl;
            newStack.push(oldStack.top());
            oldStack.pop();
        }

 //       cout << "This is the huffman code for " << character << ":" << endl;
        for (int i = 0; i < x; i++) {

            // cout << newStack.top() << endl;
            s = s + to_string(newStack.top());
            newStack.pop();

        }

 //       cout << s << endl;
        return s;
    }
    else{
        s = "";
//        cout << "Character " << character << " not in tree." << endl;
        return s;
    }

}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const {
    ifstream inFile(file_name);
    string huffCode = "";

    stringstream buff;
    char character;

    buff >> std::noskipws;
    buff << inFile.rdbuf();

    while(buff >> character) {

        huffCode = huffCode + get_character_code(character);
        if(get_character_code(character) == "") { //breaks and sends empty string if any character is invalid
            break;
        }

    }

 //   cout << "This is the full huffman code: " << huffCode << endl;

	return huffCode;
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const {


   // cout << "This is string to decode " << string_to_decode << endl;
    // cout << "Code size is " << codeSize << endl;

//    cout << giveCode(this->root, string_to_decode) << endl;

    return giveCode(this->root, string_to_decode);
}
