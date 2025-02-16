#include <queue>
#include <string>
#include <iostream>
#include <stack>

struct Node {
    Node* left, *right;
    std::string val;
    Node(): left(0), right(0) {};
    Node(std::string data): val(data) {};
};
class Data {
public:
    size_t index;
    int value;
    Data(int data):value(data) {};
};
std::string hash(Data& data) {return std::to_string(data.value); };
std::string hash(const std::string& a) { return a;};

Node* build(std::vector<Data>& data) {
    if (data.empty()) return nullptr;
    std::queue<Node*> inode;
    //Hash data and save their index
    for (int i = 0; i<data.size(); i++) {
        std::string str = hash(data[i]);
        inode.push(new Node(str));
        data[i].index = i;
    }
    //Make inode
    while (inode.size() > 1) {
        int size = inode.size();
        for (int i = 0; i<size/2; i++) {
            Node* tmp = new Node();
            tmp->left = inode.front();
            inode.pop();
            tmp->right = inode.front();
            inode.pop();
            tmp->val = hash(tmp->left->val + tmp->right->val);
            inode.push(tmp);
        }
        if (size%2) {
            Node* tmp = new Node();
            tmp->left = inode.front();
            tmp->val = hash(tmp->left->val + tmp->left->val);
            inode.pop();
            inode.push(tmp);
        }
    }
    return inode.front();
}

std::stack<Node*> Merkle_proof(Node* root, int index) { 
    if (!index) return {};
    std::stack<bool> bin;
    while (index) {
        bin.push(index%2);
        index /= 2;
    }
    std::stack<Node*> ans;
    while (!bin.empty()) {
        if (bin.top()) {
            ans.push(root->left);
            root = root->right;
        } 
        else { 
            ans.push(root->right);
            root = root->left;
        }
        bin.pop();
    }
    return ans;
}
void showNode(Node* tmp, int indent) {
    std::cout<<tmp->val<<std::endl;
    std::cout.width(indent);
    showNode(tmp->left, indent+1);
    showNode(tmp->right, indent+1);
}
void deleteWhole(Node*& tmp) {
    if (!tmp) return ;
    deleteWhole(tmp->left);
    deleteWhole(tmp->right);
    delete tmp;
    tmp = 0;

}
int main() {
    std::vector<Data> data = {5,9,10};
    auto out = build(data);
    showNode(out, 0);
    deleteWhole(out);
    return 0;
}