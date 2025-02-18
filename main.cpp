#include <queue>
#include <string>
#include <iostream>
#include <stack>
#include <cmath>

struct Node {
    Node* left, *right;
    std::string val;
    Node(): left(0), right(0) {};
    Node(const std::string& data): val(data) {};
    Node(const std::string& data, Node* _left, Node* _right): val(data), left(_left), right(_right) {};
};
class Data {
public:
    size_t index;
    int value;
    Data(int data):value(data) {};
};
std::string hash(const Data& data) {return "hash" + std::to_string(data.value); };
std::string hash(const std::string& a) { return "hash"+a;};
class MerkleTree {
public:
    MerkleTree(std::vector<Data>& data) {
        build(data);
    }
    void build(std::vector<Data>& data) {
        if (data.empty()) return ;
        std::queue<Node*> inode;
        //Hash data and save their index
        for (int i = 0; i<data.size(); i++) {
            inode.push(new Node(hash(data[i])));
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
        n_leaves = data.size();
        m_root = inode.front();
    }
    std::stack<std::pair<bool, std::string>> Merkle_proof(const Data& data) {
        std::stack<bool> path;
        int n = data.index;
        int level = std::ceil(std::log2(n_leaves));
        for (int i = 0; i<level; i++) {
            path.push(n%2);
            n/=2;
        }
        std::stack<std::pair<bool,std::string>> ans;
        Node* cur = m_root;
        ans.push({true, cur->val});
        while (path.size()) {
            if (path.top()) {
                ans.push({true, cur->left->val});
                cur = cur->right;
            }
            else {
                if (cur->right) ans.push({false, cur->right->val});
                else ans.push({false, cur->left->val});
                cur = cur->left;
            }
            path.pop();
        }
        return ans;
    }
    void insert(Data& data) {
        if (float log = log2(n_leaves); log==std::floor(log))
            m_root = new Node("", m_root, 0);
        std::stack<bool> path;
        std::stack<Node*> inode;
        int n = n_leaves;
        while (n) {
            path.push(n%2);
            n/=2;
        }
        inode.push(m_root);
        for (int i = path.size()-1; i>=0; i--) {
            if (path.top()) { 
                if (!inode.top()->right) inode.top()->right = new Node();
                inode.push(inode.top()->right);
            }
            else {
                if (!inode.top()->left) inode.top()->left = new Node();
                inode.push(inode.top()->left);
            } 
            path.pop();
        }
        inode.top()->val = hash(data);
        data.index = n_leaves++;
        while (inode.size()>1) {
            Node* cur = inode.top();
            inode.pop();
            inode.top()->val = hash(inode.top()->left->val + ((inode.top()->right)?inode.top()->right->val:inode.top()->left->val));
        }
    }
    void showNode(Node* root,int indent) {
        if (!root) return ;
        std::cout.width(indent);
        std::cout<<"-";
        std::cout<<root->val<<std::endl;
        showNode(root->left, indent+5);
        showNode(root->right, indent+5);
    }
    void show() {
        showNode(m_root, 0);
    }
    void deleteWhole(Node*& tmp) {
        if (!tmp) return ;
        deleteWhole(tmp->left);
        deleteWhole(tmp->right);
        delete tmp;
        tmp = 0;
    }
    bool contains(Data& data) {
        auto path = Merkle_proof(data);
        std::string str = hash(data.value);
        while (path.size()>1) {
            auto i = path.top();
            if (path.top().first) str = hash(path.top().second + str);
            else str = hash(str + path.top().second);
            path.pop();
        }
        return str == path.top().second;
    }
    ~MerkleTree() {
        deleteWhole(m_root);
    }
private:
    Node* m_root;
    int n_leaves;
};
int main() {
    std::vector<Data> data({5,6,10});
    MerkleTree tree(data);
    tree.show();
    Data ins100(100);
    std::cout << "Insert" << 100 << std::endl;
    tree.insert(ins100);
    tree.show();
    Data ins55(55);
    std::cout << "Insert" << 55 << std::endl;
    tree.insert(ins55);
    tree.show();
    if (tree.contains(ins55)) std::cout << "100 is available!" << std::endl;
    else std::cout << "100 does not available!" << std::endl;
    return 0;
}