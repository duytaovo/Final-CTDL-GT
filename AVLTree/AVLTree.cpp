#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

int num_fields; 

class dataRow {
public:
    string fields[10]; // max = 10 fields

    void create() {
        cout << "Enter number of fields: ";
        cin >> num_fields;
        for (int i = 0; i < num_fields; i++) {
            cout << "Enter field[" << i << "]: ";
            cin.ignore();
            cin >> fields[i];
        }
    }
};

struct info {
    int index;
    dataRow data;
};

// An AVL tree node
class Node
{
public:
    info info;
    Node* left;
    Node* right;
    int height;
};

Node* root = NULL;

int max(int a, int b);

int max(int a, int b)
{
    return (a > b) ? a : b;
}
// A utility function to get the height of the tree
int height(Node* N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
/* Helper function that allocates a
   new node with the given key and
   NULL left and right pointers. */
Node* newNode(info info)
{
    Node* node = new Node();
    node->info.index = info.index;
    for (int i = 0; i < num_fields; i++) {
        node->info.data.fields[i] = info.data.fields[i];
    }
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // new node is initially
                      // added at leaf
    return(node);
}

// A utility function to right
// rotate subtree rooted with y
// See the diagram given above.
Node* rightRotate(Node* y)
{
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left),
        height(y->right)) + 1;
    x->height = max(height(x->left),
        height(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left
// rotate subtree rooted with x
// See the diagram given above.
Node* leftRotate(Node* x)
{
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left),
        height(x->right)) + 1;
    y->height = max(height(y->left),
        height(y->right)) + 1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalance(Node* N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Recursive function to insert a key
// in the subtree rooted with node and
// returns the new root of the subtree.
Node* insert(Node* node, info info)
{
    /* 1. Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(info));

    if (info.index < node->info.index)
        node->left = insert(node->left, info);
    else if (info.index > node->info.index)
        node->right = insert(node->right, info);
    else // Equal keys are not allowed in BST
        return node;

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
        height(node->right));

    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && info.index < node->left->info.index)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && info.index > node->right->info.index)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && info.index > node->left->info.index)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && info.index < node->right->info.index)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

// A utility function to print preorder
// traversal of the tree.
// The function also prints height
// of every node
void preOrder(Node* node = root)
{
    if (root == NULL) {
        cout << "Nothing to display" << endl;
    }
    else if (node != NULL) {
        cout << node->info.index << "\t\t";
        for (int i = 0; i < num_fields; i++) {
            cout << node->info.data.fields[i] << "\t\t";
        }
        cout << "\n";
        preOrder(node->left);
        preOrder(node->right);
    }
}
void inOrder(Node* node = root) {
    if (root == NULL) {
        cout << "Nothing to display" << endl;
    }
    else if (node != NULL) {
        inOrder(node->left);
        cout << node->info.index << "\t\t";
        for (int i = 0; i < num_fields; i++) {
            cout << node->info.data.fields[i] << "\t\t";
        }
        cout << "\n";
        inOrder(node->right);
    }
}
void postOrder(Node*node = root) {
    if (root == NULL) {
        cout << "Nothing to display" << endl;
    }
    else if (node != NULL) {
        postOrder(node->left);
        postOrder(node->right);
        cout << node->info.index << "\t\t";
        for (int i = 0; i < num_fields; i++) {
            cout << node->info.data.fields[i] << "\t\t";
        }
        cout << "\n";
    }
}
/* Given a non-empty binary search tree,
return the node with minimum key value
found in that tree. Note that the entire
tree does not need to be searched. */
Node* minValueNode(Node* node)
{
    Node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

// Recursive function to delete a node
// with given key from subtree with
// given root. It returns root of the
// modified subtree.
Node* deleteNode(Node* root, int key)
{

    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
        return root;

    // If the key to be deleted is smaller
    // than the root's key, then it lies
    // in left subtree
    if (key < root->info.index)
        root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater
    // than the root's key, then it lies
    // in right subtree
    else if (key > root->info.index)
        root->right = deleteNode(root->right, key);

    // if key is same as root's key, then
    // This is the node to be deleted
    else
    {
        // node with only one child or no child
        if ((root->left == NULL) ||
            (root->right == NULL))
        {
            Node* temp = root->left ?
                root->left :
                root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
                *root = *temp; // Copy the contents of
                               // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            Node* temp = minValueNode(root->right);

            // Copy the inorder successor's
            // data to this node
            root->info.index = temp->info.index;

            // Delete the inorder successor
            root->right = deleteNode(root->right,
                temp->info.index);
        }
    }

    // If the tree had only one node
    // then return
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
        height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF
    // THIS NODE (to check whether this
    // node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced,
    // then there are 4 cases

    // Left Left Case
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 &&
        getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 &&
        getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
Node* search(Node* node, int key)
{
    if (node == NULL)
    {
        /* Element is not found */
        return NULL;
    }
    if (key > node->info.index)
    {
        /* Search in the right sub tree. */
        return search(node->right, key);
    }
    else if (key < node->info.index)
    {
        /* Search in the left sub tree. */
        return search(node->left, key);
    }
    else
    {
        /* Element Found */
        return node;
    }
}
void searchData(int key) {
    Node* result = search(root, key);
    if (result == NULL)
        cout << "Data is not found" << endl;
    else
    {
        cout << result->info.index << "\t\t";
        for (int i = 0; i < num_fields; i++) {
            cout << result->info.data.fields[i] << "\t\t";
        }
    }
}
void updateData(int key) {
    Node* result = search(root, key);
    if (result == NULL)
        cout << "Data is not found" << endl;
    else
    {
        string temp[100];
        cout << "Enter item update: ";
        for (int i = 0; i < num_fields; i++) {
            cin.ignore();
            cin >> temp[i];
        }
        for (int i = 0; i < num_fields; i++) {
            result->info.data.fields[i] = temp[i];
        }
    }
}
void saveData(Node* node = root)
{
    if (root == NULL) {
        cout << "Nothing to save" << endl;
    }
    else if (node != NULL) {
        saveData(node->left);

        //viet save vs fstream
        fstream f;
        f.open("data.txt", ios::app);
        f << endl << node->info.index << "|";
        for (int i = 0; i < num_fields - 1; i++) {
            f << node->info.data.fields[i] << "|";
        }
        f << node->info.data.fields[num_fields - 1];
        f.close();

        saveData(node->right);
    }
}

void menu(dataRow data, info info)
{
    int choice, key;
    cout << "\n\t1. Insert\n\t2. Delete\n\t3. Preorder Traversal\n\t4. Inorder Traversal\n\t5. Postorder Traversal\n\t6. Search\n\t7. Update\n\t8. Save Data\n\t9. Exit" << endl;
    while (1) {
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "Enter item: ";
            cin >> info.index;
            for (int i = 0; i < num_fields; i++) {
                cin.ignore();
                cin >> info.data.fields[i];
            }
            root = insert(root, info);
            break;
        case 2:
            cout << "Enter element to delete: ";
            cin >> key;
            root = deleteNode(root, key);
            break;
        case 3:
            cout << endl;
            cout << "index" << "\t\t";
            for (int i = 0; i < num_fields; i++) {
                cout << data.fields[i] << "\t\t";
            }
            cout << endl;
            preOrder(root);
            break;
        case 4:
            cout << endl;
            cout << "index" << "\t\t";
            for (int i = 0; i < num_fields; i++) {
                cout << data.fields[i] << "\t\t";
            }
            cout << endl;
            inOrder(root);
            break;
        case 5:
            cout << endl;
            cout << "index" << "\t\t";
            for (int i = 0; i < num_fields; i++) {
                cout << data.fields[i] << "\t\t";
            }
            cout << endl;
            postOrder(root);
            break;
        case 6:
            cout << "Enter element to search: ";
            cin >> key;
            cout << "index" << "\t\t";
            for (int i = 0; i < num_fields; i++) {
                cout << data.fields[i] << "\t\t";
            }
            cout << endl;
            searchData(key);
            cout << endl;
            break;
        case 7:
            cout << "Enter element to update: ";
            cin >> key;
            updateData(key);
            break;
        case 8:
        {
            fstream f;
            f.open("data.txt", ios::out);
            f << num_fields;
            f << endl << "index" << "|";
            for (int i = 0; i < num_fields; i++) {
                if (i == num_fields - 1)
                    f << data.fields[i];
                else
                    f << data.fields[i] << "|";
            }
            f.close();
            saveData();
            cout << "Save successful!" << endl;
            break;
        }
        case 9:
            exit(0);
        }
    }
}

int main()
{

    cout << "Enter SQL statement: ";
    string sql;
    getline(cin, sql);

    dataRow data;
    info info;

    if (sql == "create table") {
        data.create();

        sql = "---------Statements complete---------";
        cout << endl << sql << endl;

        menu(data, info);

    }

    // read data from file
    if (sql == "read data") {
        fstream f;
        f.open("data.txt", ios::in);
        if (!f.is_open())
        {
            cout << "Can't open this file" << endl;
        }
        else {
            f >> num_fields;
            string temp_index;
            getline(f, temp_index, '|');

            //dataRow data;
            for (int i = 0; i < num_fields - 1; i++) {
                getline(f, data.fields[i], '|');
            }
            getline(f, data.fields[num_fields - 1], '\n');

            //int choice, key;
            //info info;
            while (!f.eof())
            {
                //f >> info.index;
                string temp_value;
                getline(f, temp_value, '|');
                stringstream ss;
                ss << temp_value;
                ss >> info.index;
                for (int i = 0; i < num_fields - 1; i++) {
                    getline(f, info.data.fields[i], '|');
                }
                getline(f, info.data.fields[num_fields - 1], '\n');
                root = insert(root, info);
            }
            cout << "Read data success!" << endl;
            
            menu(data, info);
        }

    }

    if (sql != "create table" || sql != "read data")
        cout << "\n----------Statement error!. Please restart program!----------\n";

    return 0;
}


// Giải thích các cấu trcu1 dữ liệu mảng - binary tree - AVL Tree / Tài
// Các chức năng
/*
Insert, search C , T
Delete, update, C, T
save read TB
*/


Node* deleteNode(Node* root, int key)
{

    if (root == NULL)
        return root;
    if (key < root->info.index)
        root->left = deleteNode(root->left, key);
    else if (key > root->info.index)
        root->right = deleteNode(root->right, key);
    else
    {
        // node with only one child or no child
        if ((root->left == NULL) ||
            (root->right == NULL))
        {
            Node* temp = root->left ?
                root->left :
                root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
                *root = *temp; // Copy the contents of
            free(temp);
        }
        else
        {

            Node* temp = minValueNode(root->right);
            root->info.index = temp->info.index;
            root->right = deleteNode(root->right,
                temp->info.index);
        }
    }
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
        height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF
    int balance = getBalance(root);
    // Left Left Case
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);
    // Left Right Case
    if (balance > 1 &&
        getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // Right Right Case
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return leftRotate(root);
    // Right Left Case
    if (balance < -1 &&
        getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}
Node* search(Node* node, int key)
{
    if (node == NULL)
    {
        /* Element is not found */
        return NULL;
    }
    if (key > node->info.index)
    {
        /* Search in the right sub tree. */
        return search(node->right, key);
    }
    else if (key < node->info.index)
    {
        /* Search in the left sub tree. */
        return search(node->left, key);
    }
    else
    {
        /* Element Found */
        return node;
    }
}








