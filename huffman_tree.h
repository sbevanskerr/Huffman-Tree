#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <array>
#include <stack>

using namespace std;


class huffman_tree {

    struct huffNode {

        int freq;
        int value;

        huffNode* left = NULL;
        huffNode* right = NULL;

        //constructor
        huffNode(int freq, int value)
                : freq(freq), value(value)
        {
        }
    };
//overloaded struct derived from https://www.geeksforgeeks.org/stl-priority-queue-for-structure-or-class/
//simple instructions on how to make a priority queue that will accept multiple parameters and sort whichever you want
    struct compareFreq {
        bool operator()(huffNode * p1, huffNode * p2) {
            return p1->freq > p2->freq;
        }
    };


//modified from geeksforgeeks, function that ultimately didn't work out
    /* const bool findChar(struct huffNode* root, int arr[], string* fin, int top, char a)const {
        string * hoop =  nullptr;


        if (root->left) {

            arr[top] = 0;
            findChar(root->left, arr, hoop, top + 1, a);
        }


        if (root->right) {

            arr[top] = 1;
            findChar(root->right, arr, hoop, top + 1, a);
        }


        ;
        if (isLeaf(root)) {
           // cout << "Here's a leaf! " << (char)root->value << endl;
            if((char)root->value == a) {

                cout << "Found the code" << endl;
                cout << "It is " << endl;
                for(int i=0; i<10; i++) {
                    fin[i] = arr[i];
                    cout << fin[i] << endl;
                }
                return true;


            }

        }

    }*/



//using stepik 2.3 and 4.2, helped by Alexis Dougherty and TAs
//uses recursion and stack to find leaf nodes
    bool findChar(huffNode* head,bool isSingle, char charWanted, stack<int> &huffCode)const {
        if (head == nullptr) {
            return false;
        }
        else if (head->right == nullptr && head->left == nullptr) {
            if (charWanted == head->value) {
                if (isSingle) {
                    huffCode.push(0);
                }
                return true;
            }
        }
        else {
//traverses through right tree
            huffCode.push(1);

            bool p = findChar(head->right, false, charWanted, huffCode);
            if (p) { return true; }
            else {
                huffCode.pop();
            }
//traverses through left tree
            huffCode.push(0);

            p = findChar(head->left, false, charWanted, huffCode);
            if (p) { return true; }
            else {
                huffCode.pop();
            }
        }
        return false;
    }


    string  giveCode(huffNode* head, string huffCode)const { //finds a leaf using huffcode and adds it to a string - the string ends up being the decoded message

        string s = "";

        for(int i=0; i<huffCode.length()+1; i++) {

//if 0 - go left, if 1 - go right, if the node is a leaf (left and right == NULL) add node to the string and restart
            if (huffCode[i] == '0') {
                head = head->left;
                if (head->left == NULL && head->right == NULL) {

                    s = s + (char)head->value;
                    head = this->root;
                }
            }

            if (huffCode[i] == '1') {
                head = head->right;
                if (head->left == NULL && head->right == NULL) {

                    s = s + (char)head->value;
                    head = this->root;


                }
            }
        }

        return s;


    }


public:
		huffman_tree(const std::string &file_name);
		~huffman_tree();

		
		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;
    private:
        huffNode* root = nullptr;

};

#endif