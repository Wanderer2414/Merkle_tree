#include <vector>
#include <string>
#include <iostream>
struct Node {
    Node* left, *right;
    std::string val;
    Node(std::string& data): val(data) {};

};
class Data {
public:
    size_t index;
};
std::string hash(Data&);

Node* build(std::vector<Data>& data) {
    if (data.empty()) return nullptr;
    std::vector<Node*> inode;
    //Hash data and save their index
    for (int i = 0; i<data.size(); i++) {
        inode.push_back(new Node(hash(data[i])));
        data[i].index = i;
    }
    while (inode.size()!=1) {
       
    }
}
