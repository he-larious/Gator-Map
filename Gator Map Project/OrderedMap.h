#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;
using std::max;

class AVL {
private:
    // creates TreeNode structure containing the student's name and ID,
    // as well as left and right pointers.
    struct TreeNode {
        string name;
        int id;
        TreeNode* left;
        TreeNode* right;
        TreeNode() { name = ""; id = 0; left = nullptr; right = nullptr; }
        TreeNode(string studentName, int studentID) {
            name = studentName;
            id = studentID;
            left = nullptr;
            right = nullptr;
        }
        TreeNode(string studentName, int studentID, TreeNode* leftPtr, TreeNode* rightPtr) {
            name = studentName;
            id = studentID;
            left = leftPtr;
            right = rightPtr;
        }
    };

    // initializes root to be NULL inside the class
    TreeNode* root = nullptr;
    unsigned int nodeCount = 0;  // Added for part 3 testing

    // main helper functions meant to be called through external functions
    AVL::TreeNode* helperInsert(TreeNode* rootAVL, string studentName, int studentID);
    AVL::TreeNode* helperRemove(TreeNode* rootAVL, int studentID);
    void helperSearchID(TreeNode* rootAVL, int studentID, string& studentName);
    void helperSearchName(TreeNode* rootAVL, string studentName, bool& check);
    void helperInorder(TreeNode* rootAVL, TreeNode* maxTreeNode);
    string helperPreorder(TreeNode* rootAVL, TreeNode* maxTreeNode, string traverse);
    void helperPostorder(TreeNode* rootAVL, TreeNode* maxTreeNode);
    void helperLevelCt(TreeNode* rootAVL);
    void helperRemoveInorder(TreeNode* rootAVL, int n, int& studentID);

    // internal functions for rotation/finding successor/balancing
    AVL::TreeNode* findMax(TreeNode* rootAVL);
    AVL::TreeNode* findMin(TreeNode* rootAVL);
    AVL::TreeNode* leftRotate(TreeNode* rootAVL);
    AVL::TreeNode* rightRotate(TreeNode* rootAVL);
    AVL::TreeNode* leftRight(TreeNode* rootAVL);
    AVL::TreeNode* rightLeft(TreeNode* rootAVL);
    int height(TreeNode* rootAVL);
    int leftRightDiff(TreeNode* rootAVL);

public:
    // main functions
    bool insert(string studentName, int studentID);
    bool remove(int studentID);
    string searchID(int studentID);
    void searchName(string studentName);
    void inorderPrint();
    string preorderPrint();
    void postorderPrint();
    void levelCountPrint();
    void removeInorder(int n);
    unsigned int getNodeCount();
};

// helper function for inserting a TreeNode into the AVL
AVL::TreeNode* AVL::helperInsert(TreeNode* rootAVL, string studentName, int studentID) {
    // if root node is NULL before or after iteration, create a new
    // TreeNode that stores studentName and studentID parameters
    if (rootAVL == nullptr)
        return new TreeNode(studentName, studentID);

    // iterates recursively until an appropriate spot is found
    if (studentID < rootAVL->id)
        rootAVL->left = helperInsert(rootAVL->left, studentName, studentID);
    else if (studentID > rootAVL->id)
        rootAVL->right = helperInsert(rootAVL->right, studentName, studentID);
    else {
        return rootAVL;
    }

    // balancing part of helperInsert //
    int balanceValue = leftRightDiff(rootAVL);

    // left left
    if (balanceValue > 1 && leftRightDiff(rootAVL->left) >= 0)
        return rightRotate(rootAVL);

    // right right
    if (balanceValue < -1 && leftRightDiff(rootAVL->right) <= 0)
        return leftRotate(rootAVL);

    // left right
    if (balanceValue > 1 && leftRightDiff(rootAVL->left) < 0)
        return leftRight(rootAVL);

    // right left
    if (balanceValue < -1 && leftRightDiff(rootAVL->right) > 0)
        return rightLeft(rootAVL);

    return rootAVL;
}

// helper function for removing a TreeNode from the AVL
AVL::TreeNode* AVL::helperRemove(TreeNode* rootAVL, int studentID) {
    if (rootAVL == nullptr)
        return rootAVL;
    else if (studentID < rootAVL->id)
        rootAVL->left = helperRemove(rootAVL->left, studentID);
    else if (studentID > rootAVL->id)
        rootAVL->right = helperRemove(rootAVL->right, studentID);
    else { // found correct ID
        // no child case
        if (rootAVL->left == nullptr && rootAVL->right == nullptr) {
            return nullptr; // delete node
        }
        // one child case
        else if (rootAVL->left == nullptr || rootAVL->right == nullptr) {
            TreeNode* temp;
            if (rootAVL->left != nullptr)
                temp = rootAVL->left;
            else
                temp = rootAVL->right;
            *rootAVL = *temp; // overwrites root with left/right child, retains left/right child data
        }
        // two child case
        else {
            TreeNode* temp = findMin(rootAVL->right); // find successor: right node -> left most node
            rootAVL->id = temp->id; // overwrite root's id and name
            rootAVL->name = temp->name; // root still points to original left and right pointers
            rootAVL->right = helperRemove(rootAVL->right, temp->id); // remove successor node
        }
    }

    // balancing part of helperRemove //
    int balanceValue = leftRightDiff(rootAVL);

    // left left
    if (balanceValue > 1 && leftRightDiff(rootAVL->left) >= 0)
        return rightRotate(rootAVL);

    // right right
    if (balanceValue < -1 && leftRightDiff(rootAVL->right) <= 0)
        return leftRotate(rootAVL);

    // left right
    if (balanceValue > 1 && leftRightDiff(rootAVL->left) < 0)
        return leftRight(rootAVL);

    // right left
    if (balanceValue < -1 && leftRightDiff(rootAVL->right) > 0)
        return rightLeft(rootAVL);

    return rootAVL;
}

// helper function for searching for studentID in the AVL
void AVL::helperSearchID(TreeNode* rootAVL, int studentID, string& studentName) {
    // if root is NULL, return to original function call
    if (rootAVL == nullptr)
        return;

    // iterates recursively through AVL by making comparisons between desired ID and root's ID
    if (studentID < rootAVL->id)
        helperSearchID(rootAVL->left, studentID, studentName);
    if (studentID > rootAVL->id)
        helperSearchID(rootAVL->right, studentID, studentName);
    if (studentID == rootAVL->id) {
        studentName = rootAVL->name; // stores root's name into reference variable studentName that is defined in search();
        return;
    }
}

// helper function for searching for studentName in the AVL
void AVL::helperSearchName(TreeNode* rootAVL, string studentName, bool& check) {
    // used in the case that helperSearchName is called to an AVL with its root equal to NULL
    if (rootAVL == nullptr && !check) {
        cout << "unsuccessful" << endl;
        return;
    }
    check = true; // after checking the root, boolean variable check is set to true for the rest of the function call

    // if root is NULL, return to original function call
    if (rootAVL == nullptr)
        return;

    // because student names are not unique, continue iterating through the entire AVL to find each instance of studentName
    if (studentName == rootAVL->name)
        cout << rootAVL->id << endl; // print student's ID
    helperSearchName(rootAVL->left, studentName, check);
    helperSearchName(rootAVL->right, studentName, check);
}

// helper function for printing the AVL in-order
void AVL::helperInorder(TreeNode* rootAVL, TreeNode* maxTreeNode) {
    // if root is NULL, return to original function call
    if (rootAVL == nullptr)
        return;
    helperInorder(rootAVL->left, maxTreeNode); // keep iterating left until NULL
    cout << rootAVL->name;  // once NULL is reached, print the student's name
    if (rootAVL != maxTreeNode) // used to format the output. maxTreeNode refers to the largest node in the tree
        cout << ", ";
    helperInorder(rootAVL->right, maxTreeNode); // iterate right once unless NULL
}

// helper function for printing the AVL in pre-order
string AVL::helperPreorder(TreeNode* rootAVL, TreeNode* maxTreeNode, string traverse) {
    // FIXED: Preorder traversal doesn't always end with the largest node if it has left children.
    TreeNode* leftChild = findMin(maxTreeNode);
    // if root is NULL, return to original function call
    if (rootAVL == nullptr)
        return traverse;
    traverse += rootAVL->name;
    if (rootAVL != leftChild) // used to format the output. maxTreeNode refers to the largest node in the tree
        traverse += ", ";
    traverse = helperPreorder(rootAVL->left, maxTreeNode, traverse); // iterate left once unless NULL
    traverse = helperPreorder(rootAVL->right, maxTreeNode, traverse); // iterate right once unless NULL
    return traverse;
}

// helper function for printing the AVL in post-order
void AVL::helperPostorder(TreeNode* rootAVL, TreeNode* maxTreeNode) {
    // if root is NULL, return to original function call
    if (rootAVL == nullptr)
        return;
    helperPostorder(rootAVL->left, maxTreeNode); // keep iterating left until NULL
    helperPostorder(rootAVL->right, maxTreeNode); // iterate right once unless NULL
    cout << rootAVL->name; // print student's name
    if (rootAVL != maxTreeNode) // used to format the output. maxTreeNode refers to the largest node in the tree
        cout << ", ";
}

// helper function for printing the AVL's level count
void AVL::helperLevelCt(TreeNode* rootAVL) {
    int h = height(rootAVL); // calls the height function
    if (rootAVL == nullptr) // if root is NULL, level count is 0
        cout << "0" << endl;
    else // else, print height
        cout << h << endl;
}

// helper function for removing AVL nodes in-order
void AVL::helperRemoveInorder(TreeNode* rootAVL, int n, int& studentID) {
    static int count = 0; // count is only initialized once when called, meaning it will not change in recursive calls
    // iterates recursively in-order until count == n
    if (rootAVL != nullptr && count < n) {
        helperRemoveInorder(rootAVL->left, n, studentID);
        count++;
        if (count == n)
            studentID = rootAVL->id; // if count == n, stores root's name into reference variable studentName that is defined in removeInorder();
        helperRemoveInorder(rootAVL->right, n, studentID);
    }
}

// function used to find the right-most node from the called parameter
AVL::TreeNode* AVL::findMax(TreeNode* rootAVL) {
    if (rootAVL == nullptr)
        return rootAVL;
    while (rootAVL->right != nullptr) // continue iterating until root->right is NULL
        rootAVL = rootAVL->right;
    return rootAVL;
}

// function used to find the left-most node from the called parameter
AVL::TreeNode* AVL::findMin(TreeNode* rootAVL) {
    if (rootAVL == nullptr)
        return rootAVL;
    while (rootAVL->left != nullptr) // continue iterating until root->left is NULL
        rootAVL = rootAVL->left;
    return rootAVL;
}

// code from Aman
// rotate function used in right-right, left-right, and right-left, cases
AVL::TreeNode* AVL::leftRotate(TreeNode* rootAVL) {
    TreeNode* grandchild = rootAVL->right->left;
    TreeNode* newParent = rootAVL->right;
    newParent->left = rootAVL;
    rootAVL->right = grandchild;
    return newParent;
}

// code from Aman
// rotate function used in left-left, right-left, and left-right, cases
AVL::TreeNode* AVL::rightRotate(TreeNode* rootAVL) {
    TreeNode* grandchild = rootAVL->left->right;
    TreeNode* newParent = rootAVL->left;
    newParent->right = rootAVL;
    rootAVL->left = grandchild;
    return newParent;
}

// rotation function for left-right cases
AVL::TreeNode* AVL::leftRight(TreeNode* rootAVL) {
    rootAVL->left = leftRotate(rootAVL->left);
    return rightRotate(rootAVL);
}

// rotation function for right-left cases
AVL::TreeNode* AVL::rightLeft(TreeNode* rootAVL) {
    rootAVL->right = rightRotate(rootAVL->right);
    return leftRotate(rootAVL);
}

// function that is used in calculating the balance factor, and level count
int AVL::height(TreeNode* rootAVL) {
    int h = 0; // height is initialized as 0
    if (rootAVL != nullptr) {
        int leftHeight = height(rootAVL->left); // finds height of left sub-tree
        int rightHeight = height(rootAVL->right); // finds height of right sub-tree
        h = max(leftHeight, rightHeight) + 1; // sets height as the height of the greater of the two sub-trees + 1
    }
    return h; // returns height
}

// function that calculates the balance factor of a given node
int AVL::leftRightDiff(TreeNode* rootAVL) {
    int leftHeight = height(rootAVL->left); // finds height of the left sub-tree
    int rightHeight = height(rootAVL->right); // finds height of the right sub-tree
    int balanceFactor = leftHeight - rightHeight; // subtracts left height from right height
    return balanceFactor;
}

// public function that calls helper function helperInsert()
bool AVL::insert(string studentName, int studentID) {
    string name = "";
    helperSearchID(this->root, studentID, name); // checks if studentID is already initialized in AVL
    if (name.compare("") != 0) // if an ID is found, name will be some arbitrary string, meaning this operation cannot be executed
        return false;
    else { // if name is an empty string, the studentID is unique
        this->root = helperInsert(this->root, studentName, studentID);
        nodeCount++;
        return true;
    }
}

// public function that calls helper function helperRemove()
bool AVL::remove(int studentID) {
    string name = "";
    helperSearchID(this->root, studentID, name); // checks if studentID is already initialized in AVL
    if (name.compare("") == 0) // if name is an empty string, no such ID exists in the AVL, meaning there is no node to remove
        return false;
    else { // if name has some arbitrary string, the studentID exists in AVL
        this->root = helperRemove(this->root, studentID);
        nodeCount--;
        return true;
    }
}

// public function that calls helper function helperSearchID()
string AVL::searchID(int studentID) {
    string name = "";
    helperSearchID(this->root, studentID, name); // checks if studentID is already initialized in AVL
    if (name.compare("") == 0) // if name is an empty string, no such ID exists in the AVL
        return "";
    else // if name has some arbitrary string, the studentID exists in AVL
        return name;
}

// public function that calls helper function helperSearchName()
void AVL::searchName(string studentName) {
    bool iter = false; // have you iterated past first node
    helperSearchName(this->root, studentName, iter);
}

// public function that calls helper function helperInorder()
void AVL::inorderPrint() {
    helperInorder(this->root, findMax(this->root)); // second parameter is used to determine if the entire AVL has been reached
    cout << endl;
}

// public function that calls helper function helperPreorder()
string AVL::preorderPrint() {
    return helperPreorder(this->root, findMax(this->root), ""); // second parameter is used to determine if the entire AVL has been reached
}

// public function that calls helper function helperPostorder()
void AVL::postorderPrint() {
    helperPostorder(this->root, this->root); // second parameter is used to determine if the post-order traversal has finally reached the root node
    cout << endl;
}

// public function that calls helper function helperLevelCt()
void AVL::levelCountPrint() {
    helperLevelCt(this->root);
}

// public function that calls helper functions helperRemoveInorder() and helperRemove()
void AVL::removeInorder(int n) {
    int stuID = 0;
    int plusOne = n + 1; // because the helperRemoveInorder function begins counting at 1, we add 1 to the original parameter
    helperRemoveInorder(this->root, plusOne, stuID); // stores nth in-order node ID into stuID
    if (stuID == 0) // if stuID is not modified, it means there exists no nth ID
        cout << "unsuccessful" << endl;
    else { // else, it is stored with the desired nth ID, and it is removed using the helperRemove() function
        this->root = helperRemove(this->root, stuID);
        cout << "successful" << endl;
    }

}

unsigned int AVL::getNodeCount() {
    return nodeCount;
}

class OrderedMap {
private:
    AVL avlTree;

public:
    OrderedMap();
    ~OrderedMap();
    bool insert(const string ID, const string NAME);
    string search(const string ID);
    string traverse();
    bool remove(const string ID);
    unsigned int size();
};

OrderedMap::OrderedMap() {
    // Root node is already initialized to nullptr upon declaration in peer's code.
}

OrderedMap::~OrderedMap() {

}

bool OrderedMap::insert(const string ID, const string NAME) {
    return avlTree.insert(NAME, stoi(ID));
}

string OrderedMap::search(const string ID) {
    return avlTree.searchID(stoi(ID));
}

string OrderedMap::traverse() {
    return avlTree.preorderPrint();
}

bool OrderedMap::remove(const string ID) {
    return avlTree.remove(stoi(ID));
}

unsigned int OrderedMap::size() {
    return avlTree.getNodeCount();
}