#pragma once

struct node
{
    int key;
    unsigned char height;
    node* left;
    node* right;

    node(int k) : key(k), height(1), left(0), right(0) {}
};

class AVLTree
{
public:
    AVLTree();
    ~AVLTree();


    void insertValue(int k);
    void removeValue(int k);
    void clearTree();
    node* getRoot();

private:
    node* root;
    bool first;

    unsigned char height(node* p);
    int bfactor(node* p);
    void fixheight(node* p);

    node* rotateright(node* p);
    node* rotateleft(node* q);

    node* balance(node* p);
    node* insert(node* p, int k);
    node* remove(node* p, int k);

    node* findmin(node* p);
    node* removemin(node* p);

};
