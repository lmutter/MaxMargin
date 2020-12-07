#include <gtkmm.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <map>
#include <vector>
#include <iterator>
#include <cmath>
#include <chrono>

using namespace std;

int latestAVLspeed = 0;
int latestMapspeed = 0;

struct Sale {

    string region;
    string country;
    string itemType;
    string salesChannel;
    //char orderPriority;
    int orderID;
    double revenue;
    double cost;
    double profit;
};

Sale createSale(string region, string country, string itemType, int orderID, double revenue, double cost, double profit) {

    Sale sale;

    sale.region = region;
    sale.country = country;
    sale.itemType = itemType;
    sale.orderID = orderID;
    sale.revenue = revenue;
    sale.cost = cost;
    sale.profit = profit;

    return sale;

}

class SalesMap {

private:
    map<int, Sale> sales;
    map<int, Sale>::iterator iter;

public:
    void insert(Sale tempSale);
    void profitMargin(double &totalProfit, double &totalRevenue, Sale criteria);
    string calculateProfitMargin(Sale criteria);

};

void SalesMap::insert(Sale tempSale) {

    sales[tempSale.orderID] = tempSale;

}

string SalesMap::calculateProfitMargin(Sale criteria) {
    double totalProfit = 0;
    double totalRevenue = 0;
    double profitMargin = 0;

    //Start timer
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    SalesMap::profitMargin(totalProfit, totalRevenue, criteria);
    profitMargin = (totalProfit / totalRevenue) * 100.0;
    cout << fixed << setprecision(2) << profitMargin << "%" << endl;

    //end timer
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Map times" << endl;
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << endl;
    cout << "Time difference = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << " nanoseconds" << endl;

    latestMapspeed = chrono::duration_cast<chrono::nanoseconds> (end - begin).count();

    stringstream toOut;

    toOut << "Time taken: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds, " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << " nanoseconds\n";
    toOut << "Profit Margin: " << fixed << setprecision(2) << profitMargin << "%";

    return toOut.str();

}

void SalesMap::profitMargin(double &totalProfit, double &totalRevenue, Sale criteria) {
    for (iter = sales.begin(); iter != sales.end(); ++iter) {

        bool match = true;

        //Check here if everything matches the criteria
        //if (criteria matches)
        //don't change anything
        //if criteria doesn't match
        //set match = false;

//        criteria.region = "All Regions";
//        criteria.country = "All Countries";
//        criteria.itemType = "All Item Types";
//        criteria.salesChannel = "All Sales Channels";

        //Checks if the node's region is not equal to the critera, and that the criteria's region isn't set to "All Regions"
        //If both are true, set match to false

        if (iter->second.region != criteria.region && criteria.region != "All Regions")
            match = false;

        if (iter->second.country != criteria.country && criteria.country != "All Countries")
            match = false;

        if (iter->second.itemType != criteria.itemType && criteria.itemType != "All Item Types")
            match = false;

        if (iter->second.salesChannel != criteria.salesChannel && criteria.salesChannel != "All Sales Channels")
            match = false;

        if (match == true) {
            totalProfit += iter->second.profit;
            totalRevenue += iter->second.revenue;
            //            cout << root->sale.orderID;
        }
    }
}

class AVLtree {

public:

    //Node class that connects the AVL tree together
    class Node
    {
    public:
        Node* left = nullptr;
        Node* right = nullptr;
        Sale sale;
        int height = 1;
    };

    Node* successor(Node* root);
    int height(Node* node);
    int getBalance(Node* node);
    Node* rotateLeft(Node *node);
    Node* rotateRight(Node *node);
    Node* rotateLeftRight(Node *node);
    Node* rotateRightLeft(Node *node);
    Node* insert(Node* root, Sale tempSale);
    string calculateProfitMargin(Node* root, Sale criteria);
    void profitMargin(Node* root, double &totalProfit, double &totalRevenue, Sale criteria);

};


//Returns the leftmost node of the inputted node
AVLtree::Node* AVLtree::successor(Node* root)
{
    Node* tempNode = root;
    while (tempNode->left != nullptr)
    {
        tempNode = tempNode->left;
    }
    return tempNode;
}

//Method that returns that height of an inputted node
int AVLtree::height(Node* node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

//Method that returns the balance of the inputted node
int AVLtree::getBalance(Node* node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return height(node->left) - height(node->right); //returns calculated node balance
}

//Method that performs a left rotation to specified nodes in the tree
AVLtree::Node* AVLtree::rotateLeft(Node *node)
{
    //Rearrange nodes
    Node* tempChild = node->right->left;
    Node* newRoot = node->right;
    newRoot->left = node;
    node->right = tempChild;
    //Re-adjusting heights after rotation
    node->height = max(height(node->left), height(node->right) + 1);
    newRoot->height = max(height(newRoot->left), height(newRoot->right) + 1);

    return newRoot;
}

//Method that performs a right rotation to specified nodes in the tree
AVLtree::Node* AVLtree::rotateRight(Node *node)
{
    //Rearrange nodes
    Node* tempChild = node->left->right;
    Node* newRoot = node->left;
    newRoot->right = node;
    node->left = tempChild;
    //Re-adjusting heights after rotation
    node->height = max(height(node->left), height(node->right) + 1);
    newRoot->height = max(height(newRoot->left), height(newRoot->right) + 1);

    return newRoot;
}

//Method that performs a left-right rotation to specified nodes in the tree
AVLtree::Node* AVLtree::rotateLeftRight(Node *node)
{
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

//Method that performs a right-left rotation to specified nodes in the tree
AVLtree::Node* AVLtree::rotateRightLeft(Node *node)
{
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

//Method that inserts a new node into the AVL tree with the specified NAME and ID
//Also automatically auto balances the tree as a new item is added
AVLtree::Node* AVLtree::insert(Node* root, Sale tempSale)
{
    if (root == nullptr)
    {
        Node* temp = new Node();
        temp->sale = tempSale;

        return temp;
    }
    if (tempSale.orderID < root->sale.orderID)
    {
        root->left = insert(root->left, tempSale);
    }
    else if (tempSale.orderID > root->sale.orderID)
    {
        root->right = insert(root->right, tempSale);
    }

    root->height = 1 + max(height(root->left), height(root->right));

    //Code after this balances the tree

    int nodeBalance = (getBalance(root));

    if (nodeBalance > 1 && tempSale.orderID < root->left->sale.orderID)
    {
        return rotateRight(root);
    }

    if (nodeBalance < -1 && tempSale.orderID > root->right->sale.orderID)
    {
        return rotateLeft(root);
    }

    if (nodeBalance > 1 && tempSale.orderID > root->left->sale.orderID)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (nodeBalance < -1 && tempSale.orderID < root->right->sale.orderID)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

string AVLtree::calculateProfitMargin(Node* root, Sale criteria) {

    double totalProfit = 0;
    double totalRevenue = 0;
    double profitMargin = 0;

    //Start timer
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    AVLtree::profitMargin(root, totalProfit, totalRevenue, criteria);
    profitMargin = (totalProfit / totalRevenue) * 100.0;
    cout << fixed << setprecision(2) << profitMargin << "%" << endl;

    //end timer
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Tree times" << endl;
    cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds" << endl;
    cout << "Time difference = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << " nanoseconds" << endl;

    latestAVLspeed = chrono::duration_cast<chrono::nanoseconds> (end - begin).count();

    stringstream toOut;

    toOut << "Time taken: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds, " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << " nanoseconds\n";
    toOut << "Profit Margin: " << fixed << setprecision(2) << profitMargin << "%";

    return toOut.str();

}

void AVLtree::profitMargin(Node* root, double &totalProfit, double &totalRevenue, Sale criteria) {

    if (root != nullptr)
    {
        if (root->left != nullptr) {
            profitMargin(root->left, totalProfit, totalRevenue, criteria);
        }

        bool match = true;

        //Check here if everything matches the criteria
        //if (criteria matches)
        //don't change anything
        //if criteria doesn't match
        //set match = false;

//        The below criteria is used when the user doesn't want to specify something specific
//        criteria.region = "All Regions";
//        criteria.country = "All Countries";
//        criteria.itemType = "All Item Types";
//        criteria.salesChannel = "All Sales Channels";

        //Checks if the node's region is not equal to the critera, and that the criteria's region isn't set to "All Regions"
        //If both are true, set match to false
        if (root->sale.region != criteria.region && criteria.region != "All Regions")
            match = false;

        if (root->sale.country != criteria.country && criteria.country != "All Countries")
            match = false;

        if (root->sale.itemType != criteria.itemType && criteria.itemType != "All Item Types")
            match = false;

        if (root->sale.salesChannel != criteria.salesChannel && criteria.salesChannel != "All Sales Channels")
            match = false;

        if (match) {
            totalProfit += root->sale.profit;
            totalRevenue += root->sale.revenue;
        }

        if (root->right != nullptr) {
            profitMargin(root->right, totalProfit, totalRevenue, criteria);
        }
    }
}

Gtk::Window* pDialog = nullptr;
Gtk::Label* pLabelAVL = nullptr;
Gtk::Label* pLabelMap = nullptr;
Gtk::Button* pButtonAVL = nullptr;
Gtk::Button* pButtonMap = nullptr;
Gtk::Button* pButtonClear = nullptr;
Gtk::Entry* pEntryRegion = nullptr;
Gtk::Entry* pEntryCountry = nullptr;
Gtk::Entry* pEntryItems = nullptr;
Gtk::Entry* pEntrySales = nullptr;
Gtk::Label* pLabelDiff = nullptr;

void labelCompare(){
    if(pLabelAVL->get_text() != "" && pLabelMap->get_text() != ""){
        if(latestAVLspeed < latestMapspeed){
            stringstream toOut;
            toOut << "The AVL Tree is faster by " << latestMapspeed - latestAVLspeed << " nanoseconds.";
            pLabelDiff->set_text(toOut.str());
        }
        else{
            stringstream toOut;
            toOut << "The Map is faster by " << latestAVLspeed - latestMapspeed << " nanoseconds.";
            pLabelDiff->set_text(toOut.str());

        }
    }
}

static void on_AVL_button_clicked(){
    std::string strOut = "";

    std::string regionInput = pEntryRegion->get_text();
    std::string countryInput = pEntryCountry->get_text();
    std::string itemsInput = pEntryItems->get_text();
    std::string salesInput = pEntrySales->get_text();

    strOut += "Calculating AVL tree with:\n"
              + regionInput + ", " + countryInput + ", " + itemsInput + ", " + salesInput + "\n";

    AVLtree avlTree;
    AVLtree::Node* root = nullptr;

    // my code is in this block
    // HUGE warning: you'll have to make sure the filepath is accurate for your current environment
    // different IDEs do it differently, if you need help you can look it up/ask me/fiddle with it a bit
    // if you're debugging, it'll probably depend on where your IDE actually does the debugging from, which can cause some issues
    // if need be, try using a complete filepath, starting from C:, and that should work
    // note: if you're trying to put a backslash (\), you have to put two, since C++ uses it for special characters like endlines and such (ie. '\n')
    fstream inputFile;
    inputFile.open("100000 Sales Records.csv", std::fstream::in);
    char temp[300];
    string region, country, itemType, salesChannel;
    string holder;
    int ID;
    double revenue, cost, profit;
    double arithmetic = 0.0;

    // skips the first line of the file, the one that has the titles of the columns
    inputFile.ignore(300, '\n');

    int count = 0;
    for (int i = 0; i < 100000; i++) {
//    while (!inputFile.eof()) {
        inputFile.getline(temp, 300, ',');
        region = std::string(temp);
        inputFile.getline(temp, 300, ',');
        country = std::string(temp);
        inputFile.getline(temp, 300, ',');
        itemType = std::string(temp);
        inputFile.getline(temp, 300, ',');
        salesChannel = std::string(temp);

        // skip the "Priority" and the "Order Date" fields
        inputFile.ignore(300, ',');
        inputFile.ignore(300, ',');

        inputFile.getline(temp, 300, ',');
        holder = std::string(temp);
        ID = stoi(holder);

        // skip the "Ship Date", "Units Sold", "Unit Price", and "Unit Cost" fields
        inputFile.ignore(300, ',');
        inputFile.ignore(300, ',');
        inputFile.ignore(300, ',');
        inputFile.ignore(300, ',');

        // a little more trickery is needed to get a double, since there's two decimal places
        // here, the two decimal places are stored as another double, divided by 100 (since there are always two places), and added to revenue to get the Total Revenue
        // this process will be copied for the other doubles we need: cost and profit
        inputFile.getline(temp, 300, '.');
        holder = std::string(temp);
        revenue = 0.0 + stoi(holder);
        inputFile.getline(temp, 300, ',');
        holder = std::string(temp);
        arithmetic = 0.0 + stoi(holder);
        arithmetic /= 100.0;
        revenue += arithmetic;

        inputFile.getline(temp, 300, '.');
        holder = std::string(temp);
        cost = 0.0 + stoi(holder);
        inputFile.getline(temp, 300, ',');
        holder = std::string(temp);
        arithmetic += stoi(holder);
        arithmetic /= 100.0;
        cost += arithmetic;

        inputFile.getline(temp, 300, '.');
        holder = std::string(temp);
        profit = 0.0 + stoi(holder);
        inputFile.getline(temp, 300, '\n');
        holder = std::string(temp);
        arithmetic = 0.0 + stoi(holder);
        arithmetic /= 100.0;
        profit += arithmetic;

        Sale toAdd;
        toAdd.region = region;
        toAdd.country = country;
        toAdd.itemType = itemType;
        toAdd.salesChannel = salesChannel;
        toAdd.orderID = ID;
        toAdd.revenue = revenue;
        toAdd.cost = cost;
        toAdd.profit = profit;

        root = avlTree.insert(root, toAdd);
    }

    // my code ends here

    Sale criteria;
    criteria.region = regionInput;
    criteria.country = countryInput;
    criteria.itemType = itemsInput;
    criteria.salesChannel = salesInput;

    strOut += avlTree.calculateProfitMargin(root, criteria);

    pLabelAVL->set_text(strOut);

    labelCompare();

}

static void on_clear_button_clicked(){
    pLabelAVL->set_text("");
    pLabelMap->set_text("");
}

static void on_Map_button_clicked(){
    std::string strOut = "";

    std::string regionInput = pEntryRegion->get_text();
    std::string countryInput = pEntryCountry->get_text();
    std::string itemsInput = pEntryItems->get_text();
    std::string salesInput = pEntrySales->get_text();

    strOut += "Calculating Map with:\n"
              + regionInput + ", " + countryInput + ", " + itemsInput + ", " + salesInput + "\n";

    SalesMap sales;

    // my code is in this block
    // HUGE warning: you'll have to make sure the filepath is accurate for your current environment
    // different IDEs do it differently, if you need help you can look it up/ask me/fiddle with it a bit
    // if you're debugging, it'll probably depend on where your IDE actually does the debugging from, which can cause some issues
    // if need be, try using a complete filepath, starting from C:, and that should work
    // note: if you're trying to put a backslash (\), you have to put two, since C++ uses it for special characters like endlines and such (ie. '\n')
    fstream inputFile;
    inputFile.open("100000 Sales Records.csv", std::fstream::in);
    char temp[300];
    string region, country, itemType, salesChannel;
    string holder;
    int ID;
    double revenue, cost, profit;
    double arithmetic = 0.0;

    // skips the first line of the file, the one that has the titles of the columns
    inputFile.ignore(300, '\n');

    int count = 0;
    for (int i = 0; i < 100000; i++) {
//    while (!inputFile.eof()) {
        //cout << "Made it in" << endl;
        inputFile.getline(temp, 300, ',');
        region = std::string(temp);
        inputFile.getline(temp, 300, ',');
        country = std::string(temp);
        inputFile.getline(temp, 300, ',');
        itemType = std::string(temp);
        inputFile.getline(temp, 300, ',');
        salesChannel = std::string(temp);

        // skip the "Priority" and the "Order Date" fields
        inputFile.ignore(300, ',');
        inputFile.ignore(300, ',');

        inputFile.getline(temp, 300, ',');
        holder = std::string(temp);
        ID = stoi(holder);

        // skip the "Ship Date", "Units Sold", "Unit Price", and "Unit Cost" fields
        inputFile.ignore(300, ',');
        inputFile.ignore(300, ',');
        inputFile.ignore(300, ',');
        inputFile.ignore(300, ',');

        // a little more trickery is needed to get a double, since there's two decimal places
        // here, the two decimal places are stored as another double, divided by 100 (since there are always two places), and added to revenue to get the Total Revenue
        // this process will be copied for the other doubles we need: cost and profit
        inputFile.getline(temp, 300, '.');
        holder = std::string(temp);
        revenue = 0.0 + stoi(holder);
        inputFile.getline(temp, 300, ',');
        holder = std::string(temp);
        arithmetic = 0.0 + stoi(holder);
        arithmetic /= 100.0;
        revenue += arithmetic;

        inputFile.getline(temp, 300, '.');
        holder = std::string(temp);
        cost = 0.0 + stoi(holder);
        inputFile.getline(temp, 300, ',');
        holder = std::string(temp);
        arithmetic += stoi(holder);
        arithmetic /= 100.0;
        cost += arithmetic;

        inputFile.getline(temp, 300, '.');
        holder = std::string(temp);
        profit = 0.0 + stoi(holder);
        inputFile.getline(temp, 300, '\n');
        holder = std::string(temp);
        arithmetic = 0.0 + stoi(holder);
        arithmetic /= 100.0;
        profit += arithmetic;

        Sale toAdd;
        toAdd.region = region;
        toAdd.country = country;
        toAdd.itemType = itemType;
        toAdd.salesChannel = salesChannel;
        toAdd.orderID = ID;
        toAdd.revenue = revenue;
        toAdd.cost = cost;
        toAdd.profit = profit;

        sales.insert(toAdd);
    }

    // my code ends here

    Sale criteria;
    criteria.region = regionInput;
    criteria.country = countryInput;
    criteria.itemType = itemsInput;
    criteria.salesChannel = salesInput;

    strOut += sales.calculateProfitMargin(criteria);

    pLabelMap->set_text(strOut);

    labelCompare();
}


int main (int argc, char **argv)
{

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    //Load the GtkBuilder file and instantiate its widgets:
    auto refBuilder = Gtk::Builder::create();
    try
    {
        //refBuilder->add_from_file("basic.glade");
        refBuilder->add_from_file("marginmax5_8.glade");
    }
    catch(const Glib::FileError& ex)
    {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    }
    catch(const Glib::MarkupError& ex)
    {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    }
    catch(const Gtk::BuilderError& ex)
    {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    //Get the GtkBuilder-instantiated Dialog:
    refBuilder->get_widget("window", pDialog);
    if(pDialog)
    {
        //Get the GtkBuilder-instantiated Button, and connect a signal handler:
        refBuilder->get_widget("button2", pButtonAVL);
        refBuilder->get_widget("button3", pButtonMap);
        refBuilder->get_widget("button1", pButtonClear);
        refBuilder->get_widget("label1", pLabelAVL);
        refBuilder->get_widget("label2", pLabelMap);
        refBuilder->get_widget("entry1", pEntryRegion);
        refBuilder->get_widget("entry2", pEntryCountry);
        refBuilder->get_widget("entry3", pEntryItems);
        refBuilder->get_widget("entry4", pEntrySales);
        refBuilder->get_widget("label3", pLabelDiff);

        pButtonAVL->signal_clicked().connect( sigc::ptr_fun(on_AVL_button_clicked) );
        pButtonMap->signal_clicked().connect( sigc::ptr_fun(on_Map_button_clicked) );
        pButtonClear->signal_clicked().connect( sigc::ptr_fun(on_clear_button_clicked) );

        app->run(*pDialog);
    }

    delete pDialog;

    return 0;
}
