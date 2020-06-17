// unothetree.cpp : Defines the entry point for the console application.
//
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

#define BP (cout<<endl)

#define DOCK() do{                       \
                                  int dock;     \
                                  cin >> dock;    \
}while(0)

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Tree
{
public:
    TreeNode* createTreeFromArray(vector<int>& nodes)
    {
        int n = nodes.size();
        if (0 == n)
            return nullptr;

        vector<TreeNode*> tree_nodes(n, nullptr);
        int pi = 0;
        bool lc = true;

        for (int i = 0; i < n; i++)
        {
            if (!nodes[i])
            {
                if (!lc)
                    pi++;
                lc = !lc;
            }
            else
            {
                TreeNode *node = new TreeNode(nodes[i]);
                tree_nodes[i] = node;
                if (0 == i)
                    continue;

                if (lc)
                    tree_nodes[pi]->left = node;
                else
                    tree_nodes[pi++]->right = node;
                lc = !lc;
            }
            while (nullptr == tree_nodes[pi])
                pi++;
        }

        return tree_nodes[0];
    }

    int countTreeHeight(TreeNode *node)
    {
        if (!node)
            return 0;
        
        int lc = 1 + countTreeHeight(node->left);
        int rc = 1 + countTreeHeight(node->right);
        return max(lc, rc);
    }

    void printNodesArray(vector<int>& nodes)
    {
        int n = nodes.size();

        cout << "nodes: [ ";
        for (int i = 0; i < n; i++)
        {
            cout << nodes[i];
            if (i != n - 1)
                cout << ",";
            cout << " ";
        }
        cout << "]" << endl;
    }

    // 655. Print Binary Tree
    void renderMatrix(TreeNode *node, int k, int row, int col, vector<vector<string>>& matrix)
    {
        if (!node)
            return;
        
        matrix[row][col] = to_string(node->val);
        int offset = pow(2, k);
        renderMatrix(node->left, k-1, row+1, col-offset, matrix);
        renderMatrix(node->right, k-1, row+1, col+offset, matrix);
    }

    void fillMatrix(TreeNode *node, int row, int l, int r, vector<vector<string>>& matrix)
    {
        if (!node)
            return;

        int mid = l + (r - l)/2;
        matrix[row][mid] = to_string(node->val);
        fillMatrix(node->left, row+1, l, mid-1, matrix);
        fillMatrix(node->right, row+1, mid+1, r, matrix);
    }

    void printTree(TreeNode* root)
    {
        if (!root)
            return;

        int height = countTreeHeight(root);
        int col_num = pow(2, height) - 1;

        vector<vector<string>> matrix(height, vector<string>(col_num, "  "));

        // renderMatrix(root, height-2, 0, col_num/2, matrix);
        fillMatrix(root, 0, 0, col_num-1, matrix);

        cout << "tree:" << endl;
        for (auto row : matrix)
        {
            for (auto col : row)
                cout << col;
            cout << endl;
        }
        cout << endl;
    }
};

class Solution
{
private:
    vector<int> m_order;
public:
    // 94. Binary Tree Inorder Traversal
    void inorderTraversal_R(TreeNode* node)
    {
        if (!node)
            return;
        
        inorderTraversal_R(node->left);
        m_order.push_back(node->val);
        inorderTraversal_R(node->right);
    }

    void inorderTraversal_NR(TreeNode* node)
    {
        if (!node)
            return;

        stack<TreeNode*> mid_nodes;
        bool is_pop = false;

        while (1)
        {
            if (!is_pop)
            {
                if (node->left)
                {
                    mid_nodes.push(node);
                    node = node->left;
                    continue;
                }
            }
            
            m_order.push_back(node->val);

            if (node->right)
            {
                node = node->right;
                is_pop = false;
                continue;
            }

            if (mid_nodes.empty())
                return;

            node = mid_nodes.top();
            mid_nodes.pop();
            is_pop = true;
        }
    }

    vector<int> inorderTraversal(TreeNode* root)
	{
        m_order.clear();

        // inorderTraversal_R(root);
        inorderTraversal_NR(root);

        return m_order;
    }


    // 144. Binary Tree Preorder Traversal
    void preorderTraversal_R(TreeNode* node)
    {
        if (!node)
            return;

        m_order.push_back(node->val);
        preorderTraversal_R(node->left);
        preorderTraversal_R(node->right);
    }

    void preorderTraversal_NR(TreeNode* node)
    {
        if (!node)
            return;

        stack<TreeNode*> right_nodes;

        while (1)
        {
            m_order.push_back(node->val);

            if (node->right)
                right_nodes.push(node->right);

            if (node->left)
            {
                node = node->left;
                continue;
            }

            if (right_nodes.empty())
                return;
            
            node = right_nodes.top();
            right_nodes.pop();
        }
    }

    vector<int> preorderTraversal(TreeNode* root)
    {
        m_order.clear();

        // preorderTraversal_R(root);
        preorderTraversal_NR(root);

        return m_order;
    }

    // 145. Binary Tree Postorder Traversal
    // todo

    // 102. Binary Tree Level Order Traversal
    vector<vector<int>> levelOrder(TreeNode* root)
    {
        vector<vector<int>> lo;

        if (!root)
            return lo;
        
        deque<TreeNode*> level_nodes;
        int n = 1;
        level_nodes.push_back(root);

        while (n)
        {
            vector<int> level;

            while (n--)
            {
                TreeNode* node = level_nodes.front();
                level_nodes.pop_front();
                level.push_back(node->val);

                if (node->left)
                    level_nodes.push_back(node->left);
                if (node->right)
                    level_nodes.push_back(node->right);
            }

            lo.push_back(level);
            n = level_nodes.size();
        }

        return lo;
    }

    // 107. Binary Tree Level Order Traversal II
    vector<vector<int>> levelOrderBottom(TreeNode* root)
    {
        vector<vector<int>> lob;

        if (!root)
            return lob;

        deque<TreeNode*> level_nodes;
        int n = 1;
        level_nodes.push_back(root);

        while (n)
        {
            vector<int> level;

            while (n--)
            {
                TreeNode *node = level_nodes.front();
                level_nodes.pop_front();

                level.push_back(node->val);
                if (node->left)
                    level_nodes.push_back(node->left);
                if (node->right)
                    level_nodes.push_back(node->right);
            }

            lob.push_back(level);
            n = level_nodes.size();
        }
        
        n = lob.size();

        for (int i = 0; i < n/2; i++)
            swap(lob[i], lob[n-1-i]);

        return lob;
    }

    // 101. Symmetric Tree
    bool isSymmetric_R(TreeNode* lc, TreeNode* rc)
    {
        if (!lc || !rc)
        {
            if (lc == rc)
                return true;
            return false;
        }

        if (lc->val != rc->val)
            return false;

        return isSymmetric_R(lc->left, rc->right) && isSymmetric_R(lc->right, rc->left);
    }

    bool isSymmetric_NR(TreeNode* root)
    {
        if (!root)
            return true;

        deque<TreeNode*> level_nodes;
        int n = 1;
        level_nodes.push_back(root);

        while (n)
        {
            vector<TreeNode*> nodes;

            while (n--)
            {
                TreeNode *node = level_nodes.front();
                level_nodes.pop_front();

                if (node->left)
                    level_nodes.push_back(node->left);
                if (node->right)
                    level_nodes.push_back(node->right);
                nodes.push_back(node->left);
                nodes.push_back(node->right);
            }

            n = nodes.size();

            for (int i = 0; i < n/2; i++)
            {
                if (!nodes[i] && !nodes[n-1-i])
                    continue;
                else if ((nodes[i] && nodes[n-1-i]) && (nodes[i]->val == nodes[n-1-i]->val))
                    continue;

                return false;
            }

            n = level_nodes.size();
        }
        
        return true;
    }

    bool isSymmetric(TreeNode* root)
    {
        if (!root)
            return true;

        return isSymmetric_R(root->left, root->right);
        // return isSymmetric_NR(root);
    }

    // 101. Symmetric Tree
    int maxDepth(TreeNode* root)
    {
        if (!root)
            return 0;

        int ld = 1 + maxDepth(root->left);
        int rd = 1 + maxDepth(root->right);

        return max(ld, rd);
    }

    // 111. Minimum Depth of Binary Tree
    int minDepth(TreeNode* root)
    {
        if (!root)
            return 0;

        int ld = 1 + minDepth(root->left);
        int rd = 1 + minDepth(root->right);

        if (!root->left)
            return rd;
        else if (!root->right)
            return ld;

        return min(ld, rd); 
    }

    // 110. Balanced Binary Tree
    // todo: not optimal
    bool isBalanced(TreeNode* root)
    {
        if (!root)
            return true;
        
        int ld = 1 + maxDepth(root->left);
        int rd = 1 + maxDepth(root->right);

        if (abs(ld - rd) > 1)
            return false;
        
        return isBalanced(root->left) && isBalanced(root->right);
    }
};

int main()
{
    Tree tree;
    Solution solu;

    // 655. Print Binary Tree
    vector<int> nodes = { 56, 48, 59, 39, 52, 57, 69, 27, 44, 50, 54,
                            NULL, 58, 66, NULL, NULL, NULL, 40, NULL,
                            NULL, NULL, NULL, 55, NULL, NULL, NULL, 68, 38 };
    nodes = { 1, 2, 2, 3, NULL, NULL, 3, 4, NULL, NULL, 4};
    // nodes = { 5, 3, 6, 2, 4, NULL,7, NULL, 1 };
    // nodes = { 8, 3, 5, NULL, 9, NULL, NULL, 9, 5 };
    // nodes = { 1, 2, 3, NULL, NULL, NULL, -10, 11, 20 };
    // nodes = { -10, 9, 20, NULL, NULL, 15, 7 };
    // nodes = { 9, 6, -3, NULL, NULL, -6, 2, NULL, NULL, 2, NULL, -6, -6, -6 };
    // nodes = { 5, 4, 8, 11, NULL, 13, 4, 7, 2, NULL, NULL, NULL, 1 };
    // nodes = { 3, 4, 6, NULL, NULL, 7, 9, 11, NULL, NULL, 13 };
    tree.printNodesArray(nodes);
    TreeNode* root = tree.createTreeFromArray(nodes);
    tree.printTree(root);

    // 94. Binary Tree Inorder Traversal
    // vector<int> inorder = solu.inorderTraversal(root);
    // cout << "inorder traversal: ";
    // for (auto node : inorder)
    //     cout << node << ", ";
    // cout << endl;

    // 144. Binary Tree Preorder Traversal
    // vector<int> preorder = solu.preorderTraversal(root);
    // cout << "preorder traversal: ";
    // for (auto node : preorder)
    //     cout << node << ", ";
    // cout << endl;

    // 145. Binary Tree Postorder Traversal
    // vector<int> postorder = solu.postorderTraversal(root);
    // cout << "postorder traversal:";
    // for (auto node : postorder)
    //     cout << node << ", ";
    // cout << endl;

    // 102. Binary Tree Level Order Traversal
    // vector<vector<int>> levelOrder = solu.levelOrder(root);
    // cout << "level order: " << endl;
    // for (auto level : levelOrder)
    // {
    //     for (auto node : level)
    //         cout << node << " ";
    //     cout << endl;
    // }

    // 107. Binary Tree Level Order Traversal II
    // vector<vector<int>> levelOrderBottom = solu.levelOrderBottom(root);
    // cout << "level order bottom: " << endl;
    // for (auto level : levelOrderBottom)
    // {
    //     for (auto node : level)
    //         cout << node << " ";
    //     cout << endl;
    // }

    // 101. Symmetric Tree
    // cout << "isSymmetric: " << (solu.isSymmetric(root)? "true":"false") << endl;

    // 104. Maximum Depth of Binary Tree
    // cout << "max depth: " << solu.maxDepth(root) << endl;

    // 111. Minimum Depth of Binary Tree
    // cout << "min depth: " << solu.minDepth(root) << endl;

    // 110. Balanced Binary Tree
    cout << "isBalanced: " << (solu.isBalanced(root)? "true":"false") << endl;

    // 108. Convert Sorted Array to Binary Search Tree
    // vector<int> nums = {-10, -6, -4, -2, 0, 3, 4, 7 ,8};
    // pTreeRoot = solu.sortedArrayToBST(nums);
    // tree.printTree(pTreeRoot);

    // 99. Recover Binary Search Tree
    // solu.recoverTree(pTreeRoot);
    // cout << "Recover Binary Search Tree:" << endl;
    // tree.printTree(pTreeRoot);

    // 98. Validate Binary Search Tree
    // cout << "isValidBST: " << (solu.isValidBST(pTreeRoot) ? "true" : "false") << endl;

    // 103. Binary Tree Zigzag Level Order Traversal
    // vector<vector<int>> zigzagLevelOrder = solu.zigzagLevelOrder(pTreeRoot);
    // cout << "zigzagLevelOrder:" << endl;
    // for (auto zigzagLevel : zigzagLevelOrder)
    // {
    //     for (auto node : zigzagLevel)
    //         cout << node << " ";
    //     cout << endl;
    // }

    // 100. Same Tree
    // TreeNode *pTreeRoot2 = tree.createTreeFromNodeSet();
    // tree.printTree(pTreeRoot2);
    // cout << "isSameTree: " << (solu.isSameTree(pTreeRoot, pTreeRoot2) ? "true" : "false") << endl;
    
    // 112. Path Sum 
    // cout << "hasPathSum: " << (solu.hasPathSum(pTreeRoot, 230) ? "true" : "false") << endl;

    // 113. Path Sum II
    // vector<vector<int>> paths = solu.pathSum(pTreeRoot, 230);
    // cout << "paths where each equals to the pathsum:" << endl;
    // for (auto path : paths)
    // {
    //     for (auto node : path)
    //         cout << node << " ";
    //     cout << endl;
    // }

    // 450. Delete Node in a BST
    // int key = 5;
    // cout << "delete node: " << key << endl;
    // solu.deleteNode(pTreeRoot2, key);
    // tree.printTree(pTreeRoot2);

    // 617. Merge Two Binary Trees
    // solu.mergeTrees(pTreeRoot, pTreeRoot2);
    // tree.printTree(pTreeRoot);

    // 114. Flatten Binary Tree to Linked List
    // solu.flatten(pTreeRoot2);
    // cout << "flatten tree:" << endl;
    // tree.printTree(pTreeRoot2);
    
    // 129. Sum Root to Leaf Numbers
    // cout<< "Sum Root to Leaf Numbers: " << solu.sumNumbers(pTreeRoot2) << endl;

    // 173. Binary Search Tree Iterator
    // BSTIterator* treeIterator = new BSTIterator(pTreeRoot2);
    // int i = 0;
    // while (i < 25)
    // {
    //     cout << "hasNext(): ";
    //     if (treeIterator->hasNext())
    //     {
    //         cout << "true" << endl;
    //         cout << treeIterator->next() << endl;
    //     }
    //     else
    //         cout << "false" << endl;
    //     i++;
    // }
    // cout << treeIterator->next() << endl;
    // cout << "hasNext(): " << (treeIterator->hasNext()?"true":"false") << endl;

    // 199. Binary Tree Right Side View
    // vector<int> rightSideView = solu.rightSideView(pTreeRoot2);
    // cout << "right side view: ["; 
    // for (auto node : rightSideView)
    //     cout << node << " ";
    // cout << "]" << endl;

    // 226. Invert Binary Tree
    // solu.invertTree(pTreeRoot2);
    // cout << "invert tree:" << endl;
    // tree.printTree(pTreeRoot2);

    // 124. Binary Tree Maximum Path Sum
    // cout << "Maximum Path Sum: " << solu.maxPathSum(pTreeRoot2) << endl;

    // vector<int> preorderee = { 56, 48, 33, 27, 36, 34, 52, 50, 54, 55, 59, 57, 58, 69, 66, 68 };
    // vector<int> inorderee = { 27, 33, 34, 36, 48, 50, 52, 54, 55, 56, 57, 58, 59, 66, 68, 69 };
    // vector<int> postorderee = { 27, 34, 36, 33, 50, 55, 54, 52, 48, 58, 57, 68, 66, 69, 59, 56 };

    // 105. Construct Binary Tree from Preorder and Inorder Traversal
    // pTreeRoot2 = solu.buildTreeFromPreInOrder(preorderee, inorderee);
    // tree.printTree(pTreeRoot2);
    
    // 106. Construct Binary Tree from Inorder and Postorder Traversal
    // pTreeRoot2 = solu.buildTreeFromInPostOrder(inorderee, postorderee);
    // tree.printTree(pTreeRoot2);

    // 889. Construct Binary Tree from Preorder and Postorder Traversal
    // pTreeRoot2 = solu.constructFromPrePost(preorderee, postorderee);
    // tree.printTree(pTreeRoot2);

    DOCK();

    return 0;
}
