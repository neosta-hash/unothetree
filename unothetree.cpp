// unothetree.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
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
                                  cin>>dock;    \
}while(0)

#define nullnode 1202

struct TNode
{
    int val;
    int lval;
    int rval;
};

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
    TreeNode* createTreeFromNodeSet(vector<TNode>& nodeSet)
    {
        vector<TreeNode*> treeNodes;
        int iCursor = 0;

        for (int i = 0; i < nodeSet.size(); i++)
        {
            TreeNode* pstNode = new TreeNode(nodeSet[i].val);

            /*
            //from bottom to top level, from left to right in each level
            if (nullnode != nodeSet[i].lval)
            {
            pstNode->left = treeNodes[iCursor];
            iCursor++;
            }
            if (nullnode != nodeSet[i].rval)
            {
            pstNode->right = treeNodes[iCursor];
            iCursor++;
            }
            */

            //from bottom to top level, from right to left in each level
            if (nullnode != nodeSet[i].rval)
            {
                pstNode->right = treeNodes[iCursor];
                iCursor++;
            }
            if (nullnode != nodeSet[i].lval)
            {
                pstNode->left = treeNodes[iCursor];
                iCursor++;
            }
            treeNodes.push_back(pstNode);
        }

        return treeNodes[treeNodes.size() - 1];
    }

    void printNodesArray(vector<int>& nodes)
    {
        cout << "nodes array: [";
        for (int i = 0; i < nodes.size(); i++)
        {
            if (NULL == nodes[i])
                cout << "null";
            else
                cout << nodes[i];
            if (nodes.size() != i + 1)
                cout << ", ";
        }
        cout << "]" << endl;
    }

    TreeNode* createTreeFromArray(vector<int>& nodes, int parentIdx, int childIdx, vector<TreeNode*>& parentNodes)
    {
        if (childIdx >= nodes.size())
            return nullptr;

        while (NULL == nodes[parentIdx])
            parentIdx++;

        createTreeFromArray(nodes, parentIdx + 1, childIdx + 2, parentNodes);

        TreeNode* pParent = new TreeNode(nodes[parentIdx]);

        if (NULL != nodes[childIdx])
        {
            if (childIdx < parentNodes.size() && nullptr != parentNodes[childIdx])
            {
                pParent->left = parentNodes[childIdx];
            }
            else
                pParent->left = new TreeNode(nodes[childIdx]);
        }

        if (nodes.size() > childIdx + 1)
        {
            if (NULL != nodes[childIdx + 1])
            {
                if (childIdx + 1 < parentNodes.size() && nullptr != parentNodes[childIdx + 1])
                {
                    pParent->right = parentNodes[childIdx + 1];
                }
                else
                    pParent->right = new TreeNode(nodes[childIdx + 1]);
            }
        }

        parentNodes[parentIdx] = pParent;

        return pParent;
    }

    int countTreeHeight(TreeNode *pNode)
    {
        if (nullptr != pNode)
        {
            int leftSubTreeHeight = 1 + countTreeHeight(pNode->left);
            int rightSubTreeHeight = 1 + countTreeHeight(pNode->right);
            return (leftSubTreeHeight > rightSubTreeHeight) ? leftSubTreeHeight : rightSubTreeHeight;
        }
        return 0;
    }

    void fillMatrix(TreeNode *pNode, int treeHeight, int row, int col, vector<vector<string>>& matrix)
    {
        if (nullptr != pNode)
        {
            matrix[row][col] = to_string(pNode->val);
            int offset = pow(2, treeHeight - (row + 1 + 1)); // pow(2, height-i);  i:[1,height]
            fillMatrix(pNode->left, treeHeight, row + 1, col - offset, matrix);
            fillMatrix(pNode->right, treeHeight, row + 1, col + offset, matrix);
        }
    }

    // 655. Print Binary Tree
    void printTree(TreeNode* root)
    {
        int treeHeight = countTreeHeight(root);
        int colNum = pow(2, treeHeight) - 1;
        vector<vector<string>> matrix;

        for (int i = 0; i < treeHeight; i++)
        {
            vector<string> row(colNum, "  ");
            matrix.push_back(row);
        }

        fillMatrix(root, treeHeight, 0, colNum / 2, matrix);

        cout << "Tree:" << endl;
        for (auto row : matrix)
        {
            for (auto col : row)
            {
                cout << col;
            }
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
    // inorder traversal
    void rInorderTraversal(TreeNode *pNode)
    {
        if (nullptr != pNode)
        {
            rInorderTraversal(pNode->left);
            m_order.emplace_back(pNode->val);
            rInorderTraversal(pNode->right);
        }
    }
    void nrInorderTraversal(TreeNode *pNode)
    {
        std::stack<TreeNode*> middleNodes;
        bool bLeftTraveled = false;
        while (1)
        {
            if (!bLeftTraveled)
            {
                if (nullptr != pNode->left)
                {
                    middleNodes.push(pNode);
                    pNode = pNode->left;
                    continue;
                }
            }
            m_order.emplace_back(pNode->val);

            if (nullptr != pNode->right)
            {
                pNode = pNode->right;
                bLeftTraveled = false;
                continue;
            }
            if (middleNodes.empty())
                break;
            pNode = middleNodes.top();
            middleNodes.pop();
            bLeftTraveled = true;
        }
    }
    vector<int> inorderTraversal(TreeNode* root)
    {
        m_order.clear();
        if (nullptr != root)
        {
            //rInorderTraversal(root);
            nrInorderTraversal(root);
        }
        return m_order;
    }

    // preorder traversal
    void rPreorderTraversal(TreeNode *pNode)
    {
        if (nullptr != pNode)
        {
            m_order.emplace_back(pNode->val);
            rPreorderTraversal(pNode->left);
            rPreorderTraversal(pNode->right);
        }
    }

    void nrPreorderTraversal(TreeNode *pNode)
    {
        stack<TreeNode*> rightChildNodes;

        while (1)
        {
            m_order.emplace_back(pNode->val);

            if(nullptr != pNode->right)
                rightChildNodes.push(pNode->right);

            if (nullptr != pNode->left)
            {
                pNode = pNode->left;
                continue;
            }

            if (rightChildNodes.empty())
                break;

            pNode = rightChildNodes.top();
            rightChildNodes.pop();
        }
    }

    vector<int> preorderTraversal(TreeNode* root)
    {
        m_order.clear();
        if (nullptr != root)
        {
            //rPreorderTraversal(root);
            nrPreorderTraversal(root);
        }
        return m_order;
    }

    // postorder traversal
    void rPostorderTraversal(TreeNode *pNode)
    {
        if (nullptr != pNode)
        {
            rPostorderTraversal(pNode->left);
            rPostorderTraversal(pNode->right);
            m_order.emplace_back(pNode->val);
        }
    }

    void nrPostorderTraversal(TreeNode *pNode)
    {
        stack<TreeNode*> middleNodes;
        bool bLeftTraveled = false;
        stack<TreeNode*> rightTraveledNodes;

        while (1)
        {
            if (!bLeftTraveled)
            {
                if (nullptr != pNode->left)
                {
                    middleNodes.push(pNode);
                    pNode = pNode->left;
                    continue;
                }
            }

            if (!rightTraveledNodes.empty())
            {
                if (pNode == rightTraveledNodes.top())
                {
                    rightTraveledNodes.pop();
                    goto end;
                }
            }

            if (nullptr != pNode->right)
            {
                middleNodes.push(pNode);
                rightTraveledNodes.push(pNode);
                pNode = pNode->right;
                bLeftTraveled = false;
                continue;
            }

        end:
            m_order.emplace_back(pNode->val);
            if (middleNodes.empty())
                break;
            pNode = middleNodes.top();
            middleNodes.pop();
            bLeftTraveled = true;
        }
    }

    vector<int> postorderTraversal(TreeNode* root)
    {
        m_order.clear();
        if (nullptr != root)
        {
            //rPostorderTraversal(root);
            nrPostorderTraversal(root);
        }
        return m_order;
    }

    // level order
    vector<vector<int>> levelOrder(TreeNode* root)
    {
        vector<vector<int>> nodesInLevels;
        if (nullptr == root)
            return nodesInLevels;

        queue<TreeNode*> nodes;
        nodes.push(root);

        TreeNode* pNode = nullptr;
        int nodesCount = 0;
        int levelCount = 0;
        while (!nodes.empty())
        {
            nodesInLevels.emplace_back();
            nodesCount = nodes.size();
            while (0 < nodesCount)
            {
                pNode = nodes.front();
                nodes.pop();
                nodesCount--;
                nodesInLevels[levelCount].emplace_back(pNode->val);
                if (nullptr != pNode->left)
                    nodes.push(pNode->left);
                if (nullptr != pNode->right)
                    nodes.push(pNode->right);
            }
            levelCount++;
        }

        return nodesInLevels;
    }

    // level order from bottom
    vector<vector<int>> levelOrderBottom(TreeNode* root)
    {
        vector<vector<int>> nodesInLevels = levelOrder(root);

        for (int i = 0, j = nodesInLevels.size() - 1; i < j; i++, j--)
        {
            swap(nodesInLevels[i], nodesInLevels[j]);
        }

        return nodesInLevels;
    }

    // symmetric tree
    bool rIsSymmetricTree(TreeNode* pLeftNode, TreeNode* pRightNode)
    {
        if (nullptr == pLeftNode || nullptr == pRightNode)
        {
            if (pLeftNode == pRightNode)
                return true;
            return false;
        }
        else if (pLeftNode->val != pRightNode->val)
            return false;

        return (rIsSymmetricTree(pLeftNode->left, pRightNode->right) && rIsSymmetricTree(pLeftNode->right, pRightNode->left));
    }

    bool nrIsSymmetricTree(TreeNode* pNode)
    {
        deque<TreeNode*> nodes;
        nodes.push_back(pNode);
        int nodesCount = 0;
        bool bIsBottomLevel;

        while (1)
        {
            nodesCount = nodes.size();
            bIsBottomLevel = true;

            while (0 < nodesCount)
            {
                pNode = nodes.front();
                nodes.pop_front();
                nodesCount--;

                if (nullptr == pNode)
                    continue;

                if (nullptr == pNode->left)
                    nodes.push_back(nullptr);
                else
                {
                    bIsBottomLevel = false;
                    nodes.push_back(pNode->left);
                }

                if (nullptr == pNode->right)
                    nodes.push_back(nullptr);
                else
                {
                    bIsBottomLevel = false;
                    nodes.push_back(pNode->right);
                }
            }

            if (bIsBottomLevel)
                return true;

            nodesCount = nodes.size();

            for (int i = 0; i < nodesCount / 2; i++)
            {
                if (nullptr == nodes[i] || nullptr == nodes[nodesCount - 1 - i])
                {
                    if (nodes[i] != nodes[nodesCount - 1 - i])
                        return false;
                }
                else if (nodes[i]->val != nodes[nodesCount - 1 - i]->val)
                    return false;
            }
        }

        return true;
    }

    bool isSymmetric(TreeNode* root)
    {
        if (nullptr == root)
            return true;

        //return nrIsSymmetricTree(root);
        return rIsSymmetricTree(root->left, root->right);
    }

    // Maximum Depth of Binary Tree
    int maxDepth(TreeNode* root)
    {
        if (nullptr == root)
            return 0;

        int leftSubtreeDepth = 1 + maxDepth(root->left);
        int rightSubtreeDepth = 1 + maxDepth(root->right);

        return (leftSubtreeDepth > rightSubtreeDepth ? leftSubtreeDepth : rightSubtreeDepth);
    }

    // Minimum Depth of Binary Tree
    int minDepth(TreeNode* root)
    {
        if (nullptr == root)
            return 0;

        int leftSubtreeDepth = 1 + minDepth(root->left);
        int rightSubtreeDepth = 1 + minDepth(root->right);

        if (1 == leftSubtreeDepth)
            return rightSubtreeDepth;
        if (1 == rightSubtreeDepth)
            return leftSubtreeDepth;

        return (leftSubtreeDepth > rightSubtreeDepth ? rightSubtreeDepth : leftSubtreeDepth);
    }

    // Balanced Binary Tree
    bool isBalanced(TreeNode* root)
    {
        if (nullptr == root)
            return true;

        int leftSubtreeDepth = maxDepth(root->left);
        int rightSubtreeDepth = maxDepth(root->right);

        if (1 < abs(leftSubtreeDepth - rightSubtreeDepth))
            return false;

        return isBalanced(root->left) && isBalanced(root->right);
    }

    // Convert Sorted Array to Balanced Binary Search Tree
    TreeNode* createBalancedBST(vector<int>& nums, int left, int right)
    {
        if (left <= right)
        {
            int middle = (left + right) / 2;

            TreeNode* root = new TreeNode(nums[middle]);
            root->left = createBalancedBST(nums, left, middle - 1);
            root->right = createBalancedBST(nums, middle + 1, right);

            return root;
        }

        return nullptr;
    }

    TreeNode* sortedArrayToBST(vector<int>& nums)
    {
        return createBalancedBST(nums, 0, nums.size() - 1);
    }

    // 222. Count Complete Tree Nodes
    int countNodes(TreeNode* pNode)
    {
        if (nullptr == pNode)
            return 0;

        return 1 + countNodes(pNode->left) + countNodes(pNode->right);
    }

    // Recover Binary Search Tree
    vector<int> levelTraversal(TreeNode* pNode)
    {
        vector<int> levelOrder;

        if (nullptr == pNode)
            return levelOrder;

        queue<TreeNode*> nodes;
        nodes.push(pNode);
        int nodesCount;

        while (!nodes.empty())
        {
            nodesCount = nodes.size();
            while (0 < nodesCount)
            {
                pNode = nodes.front();
                nodes.pop();
                nodesCount--;
                levelOrder.push_back(pNode->val);

                if (nullptr != pNode->left)
                    nodes.push(pNode->left);
                if (nullptr != pNode->right)
                    nodes.push(pNode->right);
            }
        }
        return levelOrder;
    }

    void traversalTree(TreeNode* pNode, vector<int>& sortedNodes, int start)
    {
        if (nullptr == pNode)
            return;

        int leftSubtreeNodeNum = countNodes(pNode->left);
        //cout << "number of nodes of left subtree:" << leftSubtreeNodeNum << " for node:" << pNode->val << endl;
        pNode->val = sortedNodes[start + leftSubtreeNodeNum];
        traversalTree(pNode->left, sortedNodes, start);
        traversalTree(pNode->right, sortedNodes, start + leftSubtreeNodeNum + 1);
    }

    void recoverTree(TreeNode* root)
    {
        if (nullptr == root)
            return;

        vector<int> levelOrder = levelTraversal(root);

        /*cout << "level order: [";
        for (auto node : levelOrder)
        cout << node << " ";
        cout << "]" << endl;*/

        sort(levelOrder.begin(), levelOrder.end());

        /*cout << "sorted nodes: [";
        for (auto node : levelOrder)
        cout << node << " ";
        cout << "]" << endl;*/

        traversalTree(root, levelOrder, 0);
    }

    // Validate Binary Search Tree
    bool isValidBSTNode(TreeNode* pNode, vector<int>& leftNodes, vector<int>& rightNodes)
    {
        if (nullptr == pNode)
            return true;

        for (auto val : leftNodes)
            if (pNode->val <= val)
                return false;

        for (auto val : rightNodes)
            if (pNode->val >= val)
                return false;

        rightNodes.push_back(pNode->val);
        bool bRet = isValidBSTNode(pNode->left, leftNodes, rightNodes);
        rightNodes.pop_back();
        if (!bRet)
            return false;

        leftNodes.push_back(pNode->val);
        bRet = isValidBSTNode(pNode->right, leftNodes, rightNodes);
        leftNodes.pop_back();

        return bRet;
    }

    bool isValidBST(TreeNode* root)
    {
        if (nullptr == root)
            return true;

        vector<int> leftNodes, rightNodes;

        return isValidBSTNode(root, leftNodes, rightNodes);
    }

    // 103. Binary Tree Zigzag Level Order Traversal
    vector<vector<int>> zigzagLevelOrder(TreeNode* root)
    {
        vector<vector<int>> nodesInLevels;
        if (nullptr == root)
            return nodesInLevels;

        TreeNode* pNode = nullptr;
        stack<TreeNode*> curtLevelNodes;
        stack<TreeNode*> nextLevelNodes;
        curtLevelNodes.push(root);
        int levelCount = 0;

        while (!curtLevelNodes.empty())
        {
            nodesInLevels.emplace_back();

            while (!curtLevelNodes.empty())
            {
                pNode = curtLevelNodes.top();
                curtLevelNodes.pop();

                nodesInLevels[levelCount].push_back(pNode->val);

                if (0 == levelCount % 2)
                {
                    if (nullptr != pNode->left)
                        nextLevelNodes.push(pNode->left);
                    if (nullptr != pNode->right)
                        nextLevelNodes.push(pNode->right);
                }
                else
                {
                    if (nullptr != pNode->right)
                        nextLevelNodes.push(pNode->right);
                    if (nullptr != pNode->left)
                        nextLevelNodes.push(pNode->left);
                }
            }

            curtLevelNodes = std::move(nextLevelNodes);
            levelCount++;
        }

        return nodesInLevels;
    }

    // 100. Same Tree
    bool isSameTree(TreeNode* p, TreeNode* q)
    {
        if (nullptr == p && nullptr == q)
            return true;
        if (nullptr == p && nullptr != q)
            return false;
        if (nullptr != p && nullptr == q)
            return false;

        if (p->val != q->val)
            return false;

        return (isSameTree(p->left, q->left) && isSameTree(p->right, q->right));
    }

    // 112. Path Sum
    bool isPathSumEqual(TreeNode* pNode, int& targetSum, int pathSum)
    {
        if (nullptr == pNode)
            return false;

        pathSum += pNode->val;

        if (pathSum == targetSum && nullptr == pNode->left && nullptr == pNode->right)
            return true;

        return isPathSumEqual(pNode->left, targetSum, pathSum) || isPathSumEqual(pNode->right, targetSum, pathSum);
    }

    bool hasPathSum(TreeNode* root, int sum)
    {
        return isPathSumEqual(root, sum, 0);
    }

    // 113. Path Sum II
    void isPathMatched(TreeNode* pNode, int& targetSum, int& pathSum, vector<int>& path, vector<vector<int>>& paths)
    {
        if (nullptr == pNode)
            return;

        pathSum += pNode->val;
        path.push_back(pNode->val);

        if (pathSum == targetSum && nullptr == pNode->left && nullptr == pNode->right)
        {
            paths.push_back(path);
            path.pop_back();
            pathSum -= pNode->val;
            return;
        }

        isPathMatched(pNode->left, targetSum, pathSum, path, paths);
        isPathMatched(pNode->right, targetSum, pathSum, path, paths);
        path.pop_back();
        pathSum -= pNode->val;
    }

    void isPathMatched2(TreeNode* pNode, int& targetSum, int& pathSum, int& pathCount, vector<vector<int>>& paths)
    {
        if (nullptr == pNode)
            return;

        pathSum += pNode->val;
        paths[pathCount].push_back(pNode->val);

        if (pathSum == targetSum && nullptr == pNode->left && nullptr == pNode->right)
        {
            paths.push_back(paths[pathCount]);
            pathCount++;
            paths[pathCount].pop_back();
            pathSum -= pNode->val;
            return;
        }

        isPathMatched2(pNode->left, targetSum, pathSum, pathCount, paths);
        isPathMatched2(pNode->right, targetSum, pathSum, pathCount, paths);
        paths[pathCount].pop_back();
        pathSum -= pNode->val;
    }

    vector<vector<int>> pathSum(TreeNode* root, int sum)
    {
        int pathSum = 0;
        vector<int> path;
        vector<vector<int>> paths;
        isPathMatched(root, sum, pathSum, path, paths);
        return paths;

        /*int pathSum = 0;
        int pathCount = 0;
        vector<vector<int>> paths;
        paths.emplace_back();
        isPathMatched2(root, sum, pathSum, pathCount, paths);
        if (paths[paths.size() - 1].empty())
        paths.pop_back();
        return paths;*/
    }

    // 450. Delete Node in a BST
    int deleteMostLeftNodeOfRightSubtree(TreeNode*& rpNode)
    {
        if(nullptr == rpNode->left)
        {
            int val = rpNode->val;
            rpNode = rpNode->right;
            return val;
        }
        
        return deleteMostLeftNodeOfRightSubtree(rpNode->left);
    }

    TreeNode*rDeleteNode(TreeNode* root, int key)
    {
        if(nullptr == root)
            return nullptr;
        
        if(key == root->val)
        {
            if(nullptr == root->left && nullptr == root->right)
                return nullptr;

            if(nullptr != root->left && nullptr == root->right)
                return root->left;

            if(nullptr == root->left && nullptr != root->right)
                return root->right;

            root->val = deleteMostLeftNodeOfRightSubtree(root->right);
            return root;
        }
        else if(key < root->val)
            root->left = rDeleteNode(root->left, key);
        else if(key > root->val)
            root->right = rDeleteNode(root->right, key);

        return root;
    }

    TreeNode* findNode(TreeNode* pNode, const int& key, TreeNode*& rpParentNode)
    {
        rpParentNode = pNode;

        while(nullptr != pNode)
        {
            if(key == pNode->val)
                return pNode;
            else if(key < pNode->val)
            {
                rpParentNode = pNode;
                pNode = pNode->left;
            }
            else if(key > pNode->val)
            {
                rpParentNode = pNode;
                pNode = pNode->right;
            }
        }

        return nullptr;
    }

    TreeNode* nrDeleteNode(TreeNode* root, int key)
    {   
        TreeNode* pNode = root;
        TreeNode* pParentNode = nullptr;
        pNode = findNode(pNode, key, pParentNode);
        if(nullptr == pNode)
            return root;

        TreeNode** ppNode = nullptr;

        if(root == pNode)
        {
            ppNode = &root;    
        }
        else
        {
            if(pNode == pParentNode->left)
                ppNode = &(pParentNode->left);
            else
                ppNode = &(pParentNode->right);
        }

        if(nullptr == pNode->left && nullptr == pNode->right)
            *ppNode = nullptr;
        else if(nullptr != pNode->left && nullptr == pNode->right)
            *ppNode = pNode->left;
        else if(nullptr == pNode->left && nullptr != pNode->right)
            *ppNode = pNode->right;
        else
        {
            TreeNode* pRSTMostLeftNode = pNode->right;
            pParentNode = pRSTMostLeftNode;
            while(nullptr != pRSTMostLeftNode->left)
            {
                pParentNode = pRSTMostLeftNode;
                pRSTMostLeftNode = pRSTMostLeftNode->left;
            }
            pNode->val = pRSTMostLeftNode->val;
            if(pParentNode == pRSTMostLeftNode)
                pNode->right = pRSTMostLeftNode->right;
            else
                pParentNode->left = pRSTMostLeftNode->right;
        }

        return root;
    }

    TreeNode* deleteNode(TreeNode* root, int key)
    {
        return rDeleteNode(root,key);
        //return nrDeleteNode(root, key);
    }

    // 617. Merge Two Binary Trees
    TreeNode* rMergeTrees(TreeNode* t1, TreeNode* t2)
    {
        if(nullptr == t1 && nullptr == t2)
            return nullptr;
        else if(nullptr != t1 && nullptr == t2)
            return t1;
        else if(nullptr == t1 && nullptr != t2)
            return t2;
        else
        {
            t1->val += t2->val;
            t1->left = mergeTrees(t1->left, t2->left);
            t1->right = mergeTrees(t1->right, t2->right);
            return t1;
        }
    }

    TreeNode* nrMergeTrees(TreeNode* t1, TreeNode* t2)
    {
        if(!t1 && !t2)
            return nullptr;
        else if(t1 && !t2)
            return t1;
        else if(!t1 && t2)
            return t2;
        
        // todo: 
    }

    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2)
    {
        return rMergeTrees(t1, t2);
    }

    // 114. Flatten Binary Tree to Linked List
    void flatten(TreeNode* root)
    {
        if(!root)
            return;
        
        TreeNode* pNode = root;
        TreeNode* pParentNode = nullptr;
        stack<TreeNode*> middleNodes;

        while(1)
        {
            if(pNode->right)
                middleNodes.push(pNode->right);

            if(pParentNode)
            {
                pParentNode->left = nullptr;
                pParentNode->right = pNode;
            }
            pParentNode = pNode;

            if(pNode->left)
            {
                pNode = pNode->left;
                continue;
            }

            if(middleNodes.empty())
                break;

            pNode = middleNodes.top();
            middleNodes.pop();
        }
    }

    // 129. Sum Root to Leaf Numbers
    void sumPath(TreeNode* pNode, int& preNumber, int& sum)
    {
        if(!pNode)
            return;

        int num = preNumber*10+pNode->val;

        if(!pNode->left && !pNode->right)
        {
            sum += num;
            return;
        }

        preNumber = num;
        sumPath(pNode->left, preNumber, sum);
        preNumber = num;
        sumPath(pNode->right, preNumber, sum);
    }

    int sumNumbers(TreeNode* root)
    {
        int preNumber = 0;
        int sum = 0;
        sumPath(root, preNumber, sum);
        return sum;
    }

    // 199. Binary Tree Right Side View
    vector<int> rightSideView(TreeNode* root)
    {
        vector<int> rightSideNodes;
        if(nullptr == root)
            return rightSideNodes;

        TreeNode* pNode = root;
        queue<TreeNode*> levelNodes;
        int levelNodesCount;
        levelNodes.push(pNode);

        while(!levelNodes.empty())
        {
            levelNodesCount = levelNodes.size();

            while(0 < levelNodesCount)
            {
                pNode = levelNodes.front();
                levelNodes.pop();
                if(nullptr != pNode->left)
                    levelNodes.push(pNode->left);
                if(nullptr != pNode->right)
                    levelNodes.push(pNode->right);

                levelNodesCount--;
            }
            rightSideNodes.push_back(pNode->val);
        }

        return rightSideNodes;
    }

    // 226. Invert Binary Tree
    // TreeNode* invertTree(TreeNode* root)
    // {
    //     if(nullptr == root)
    //         return nullptr;

    //     swap(root->left, root->right);
    //     invertTree(root->left);
    //     invertTree(root->right);

    //     return root;
    // }
    TreeNode* invertTree(TreeNode* root)
    {
        if(nullptr == root)
            return nullptr;

        TreeNode* left = root->left;
        root->left = invertTree(root->right);
        root->right = invertTree(left);

        return root;
    }

    // 230. Kth Smallest Element in a BST
    int kthSmallest(TreeNode* root, int k)
    {
        if(nullptr == root)
            return -1;
        
        TreeNode* pNode = root;
        stack<TreeNode*> middleNodes;

        while(1)
        {
            if(nullptr != pNode->left)
            {
                middleNodes.push(pNode);
                pNode = pNode->left;
                continue;
            }

        findOne:
            if(0 == --k)
                break;
            
            if(nullptr != pNode->right)
            {
                pNode = pNode->right;
                continue;
            }

            if(middleNodes.empty())
                break;

            pNode = middleNodes.top();
            middleNodes.pop();
            goto findOne;
        }

        if(0 < k)
            return -1;

        return pNode->val;
    }

    // 124. Binary Tree Maximum Path Sum
    int findMaxPathSum(TreeNode* pNode, int& maxSum)
    {
        if(nullptr == pNode)
            return 0;

        int leftPathSum = findMaxPathSum(pNode->left, maxSum);
        int rightPathSum = findMaxPathSum(pNode->right, maxSum); 

        int pathSum = max(pNode->val, pNode->val + max(leftPathSum, rightPathSum));
        int currentMaxSum = max(pathSum, pNode->val + leftPathSum + rightPathSum);
        if(maxSum < currentMaxSum)
            maxSum = currentMaxSum;

        return pathSum;
    }

    int maxPathSum(TreeNode* root)
    {
        if(nullptr == root)
            return 0;
        
        int maxSum = INT_MIN;
        findMaxPathSum(root, maxSum);
        return maxSum;
    }

    // 105. Construct Binary Tree from Preorder and Inorder Traversal
    TreeNode* buildTreeFromPreInOrder(vector<int>& preorder, vector<int>& inorder)
    {
        TreeNode* root = nullptr;
        deque<TreeNode*> nodes;
        map<int, int> nodeInorderIndex;

        for(int i = 0; i < inorder.size(); i++)
            nodeInorderIndex.insert({inorder[i], i});

        for(int i = 0; i < preorder.size(); i++)
        {
            TreeNode* pNode = new TreeNode(preorder[i]);

            if(nodes.empty())
            {
                root = pNode;
                nodes.push_back(pNode);
                continue;
            }

            auto itParent = nodes.end() - 1;
            if(nodeInorderIndex[pNode->val] < nodeInorderIndex[(*itParent)->val])
            {
                (*itParent)->left = pNode;
                nodes.push_back(pNode);
                continue;
            }

            int minOffset = INT_MAX;

            for(auto it = nodes.end()-1; it >= nodes.begin(); it--)
            {
                int offset = nodeInorderIndex[pNode->val] - nodeInorderIndex[(*it)->val];

                if(0 < offset && offset < minOffset)
                {
                    itParent = it;
                    if(1 == offset)
                        break;
                    minOffset = offset;
                }
            }

            (*itParent)->right = pNode;
            if(nullptr != (*itParent)->left)
                nodes.erase(itParent);

            nodes.push_back(pNode);
        }

        return root;
    }

    // 106. Construct Binary Tree from Inorder and Postorder Traversal
    TreeNode* buildTreeFromInPostOrder(vector<int>& inorder, vector<int>& postorder)
    {
        TreeNode* root = nullptr;
        deque<TreeNode*> nodes;
        map<int, int> nodeInorderIndex;

        for(int i = 0; i < inorder.size(); i++)
            nodeInorderIndex.insert({inorder[i], i});

        for(int i = postorder.size()-1; i >= 0; i--)
        {
            TreeNode* pNode = new TreeNode(postorder[i]);

            if(nodes.empty())
            {
                root = pNode;
                nodes.push_back(pNode);
                continue;
            }

            auto itParent = nodes.end() - 1;

            if(nodeInorderIndex[pNode->val] > nodeInorderIndex[(*itParent)->val])
            {
                (*itParent)->right = pNode;
                nodes.push_back(pNode);
                continue;
            }

            int minOffset = INT_MAX;

            for(auto it = nodes.end()-1; it >= nodes.begin(); it--)
            {
                int offset = nodeInorderIndex[(*it)->val] - nodeInorderIndex[pNode->val];

                if(0 < offset && offset < minOffset)
                {
                    itParent = it;
                    if(1 == offset)
                        break;
                    minOffset = offset;
                }
            }

            (*itParent)->left = pNode;
            if(nullptr != (*itParent)->right)
                nodes.erase(itParent);

            nodes.push_back(pNode);
        }

        return root;
    }

    // 889. Construct Binary Tree from Preorder and Postorder Traversal
    TreeNode* constructFromPrePost(vector<int>& pre, vector<int>& post)
    {
        TreeNode* root = nullptr;
        deque<TreeNode*> nodes;
        map<int, int> nodePostorderIndex;

        for(int i = 0; i < post.size(); i++)
            nodePostorderIndex.insert({post[i], i});
        
        for(int i = 0; i < pre.size(); i++)
        {
            TreeNode* pNode = new TreeNode(pre[i]);

            if(nodes.empty())
            {
                root = pNode;
                nodes.push_back(pNode);
                continue;
            }

            auto itParent = nodes.end() - 1;

            if(nodePostorderIndex[pNode->val] < nodePostorderIndex[(*itParent)->val])
            {
                (*itParent)->left = pNode;
                nodes.push_back(pNode);
                continue;
            }

            for(auto it = nodes.end()-2; it >= nodes.begin(); it--)
            {
                if(nodePostorderIndex[pNode->val] < nodePostorderIndex[(*it)->val])
                {
                    itParent = it;
                    break;
                }
            }
            
            (*itParent)->right = pNode;
            if(nullptr != (*itParent)->left)
                nodes.erase(itParent);

            nodes.push_back(pNode);
        }

        return root;
    }

};

// 173. Binary Search Tree Iterator
class BSTIterator_Queue {
public:
    stack<TreeNode*> m_middleNodes;
    vector<int> m_inorderNodes;
    int m_index = 0;

    BSTIterator_Queue(TreeNode* root) {
        if(nullptr == root)
            return;
        
        TreeNode* pNode = root;

        while(1)
        {
            if(nullptr != pNode->left)
            {
                m_middleNodes.push(pNode);
                pNode = pNode->left;
                continue;
            }

        in:
            m_inorderNodes.push_back(pNode->val);
            if(nullptr != pNode->right)
            {
                pNode = pNode->right;
                continue;
            }

            if(m_middleNodes.empty())
                break;

            pNode = m_middleNodes.top();
            m_middleNodes.pop();
            goto in;
        }
    }
    
    /** @return the next smallest number */
    int next() {
        return m_inorderNodes[(m_index++)%m_inorderNodes.size()];
    }
    
    /** @return whether we have a next smallest number */
    bool hasNext() {
        return (m_index < m_inorderNodes.size());
    }
};

class BSTIterator {
public:
    stack<TreeNode*> m_middleNodes;
    TreeNode* m_root = nullptr;

    BSTIterator(TreeNode* root) {
        if(nullptr == root)
            return;

        m_root = root;
        pushStack(m_root);
    }
    
    void pushStack(TreeNode* pNode)
    {
        while(nullptr != pNode->left)
        {
            m_middleNodes.push(pNode);
            pNode = pNode->left;
        }
        m_middleNodes.push(pNode);
    }

    /** @return the next smallest number */
    int next() {
        if(nullptr == m_root)
            return -1;

        if(m_middleNodes.empty())
            pushStack(m_root);

        TreeNode* pNode = m_middleNodes.top();
        m_middleNodes.pop();
        if(nullptr != pNode->right)
            pushStack(pNode->right);

        return pNode->val;
    }
    
    /** @return whether we have a next smallest number */
    bool hasNext() {
        return (!m_middleNodes.empty());
    }
};

int main()
{
    // vector<TNode> nodeSet = {
    //    /*
    //    //from bottom to top level, from left to right in each level
    //    {0,nullnode,nullnode},
    //    {5,nullnode,nullnode},{6,nullnode,nullnode},{ 7,0,nullnode },
    //    {8,5,6},{9,nullnode,7},
    //    {4,nullnode,nullnode},{5,8,nullnode},{6,nullnode,9},{7,nullnode ,nullnode },
    //    {2,4,5},{3,6,7},
    //    {1,2,3}
    //    */

    //    //from bottom to top level, from right to left in each level
    //    { 68,nullnode,nullnode },{ 55,nullnode,nullnode },{ 34,nullnode,nullnode },
    //    { 66,nullnode,68 },{ 58,nullnode,nullnode },{ 54,nullnode,55 },{ 50,nullnode,nullnode },{ 36,34,nullnode },{ 27,nullnode,nullnode },
    //    /*{ 66,nullnode,nullnode },{ 58,nullnode,nullnode },{ 54,nullnode,nullnode },{ 50,nullnode,nullnode },{ 36,nullnode,nullnode },{ 27,nullnode,nullnode },*/
    //    { 69,66,nullnode },{ 57,nullnode,58 },{ 52,50,54 },{ 33,27,36 },
    //    { 59,57,69 },{ 48,33,52 },
    //    { 56,48,59 }
    // };
    Tree tree;
    // TreeNode *pTreeRoot = tree.createTreeFromNodeSet(nodeSet);
    // tree.printTree(pTreeRoot);
    Solution solu;

    // vector<int> inorder = solu.inorderTraversal(pTreeRoot);
    // cout << "non-recursive inorder traversal:";
    // for (auto node : inorder)
    //     cout << node << ", ";
    // cout << endl;

    // vector<int> preorder = solu.preorderTraversal(pTreeRoot);
    // cout << "non-recursive preorder traversal:";
    // for (auto node : preorder)
    //     cout << node << ", ";
    // cout << endl;

    // vector<int> postorder = solu.postorderTraversal(pTreeRoot);
    // cout << "non-recursive postorder traversal:";
    // for (auto node : postorder)
    //     cout << node << ", ";
    // cout << endl;

    /*vector<vector<int>> levelOrder = solu.levelOrder(pTreeRoot);
    cout << "levelOrder:" << endl;
    for (auto level : levelOrder)
    {
    for (auto node : level)
    cout << node << " ";
    cout << endl;
    }*/

    // vector<vector<int>> levelOrderBottom = solu.levelOrderBottom(pTreeRoot);
    // cout << "levelOrderBottom:" << endl;
    // for (auto level : levelOrderBottom)
    // {
    //  for (auto node : level)
    //      cout << node << " ";
    //  cout << endl;
    // }

    //cout << "isSymmetric: " << (solu.isSymmetric(pTreeRoot)? "true":"false") << endl;

    //cout << "max depth: " << solu.maxDepth(pTreeRoot) << endl;

    //cout << "min depth: " << solu.minDepth(pTreeRoot) << endl;

    //cout << "isBalanced: " << (solu.isBalanced(pTreeRoot)? "true":"false") << endl;

    // vector<int> nums = {-10, -6, -4, -2, 0, 3, 4, 7 ,8};
    // pTreeRoot = solu.sortedArrayToBST(nums);
    // tree.printTree(pTreeRoot);

    // solu.recoverTree(pTreeRoot);
    // cout << "Recover Binary Search Tree:" << endl;
    // tree.printTree(pTreeRoot);

    //cout << "isValidBST: " << (solu.isValidBST(pTreeRoot) ? "true" : "false") << endl;

    /*vector<vector<int>> zigzagLevelOrder = solu.zigzagLevelOrder(pTreeRoot);
    cout << "zigzagLevelOrder:" << endl;
    for (auto zigzagLevel : zigzagLevelOrder)
    {
    for (auto node : zigzagLevel)
    cout << node << " ";
    cout << endl;
    }*/

    /*
    TreeNode *pTreeRoot2 = tree.createTreeFromNodeSet();
    tree.printTree(pTreeRoot2);
    cout << "isSameTree: " << (solu.isSameTree(pTreeRoot, pTreeRoot2) ? "true" : "false") << endl;
    */

    //cout << "hasPathSum: " << (solu.hasPathSum(pTreeRoot, 230) ? "true" : "false") << endl;

    /*vector<vector<int>> paths = solu.pathSum(pTreeRoot, 230);
    cout << "paths where each equals to the pathsum:" << endl;
    for (auto path : paths)
    {
    for (auto node : path)
    cout << node << " ";
    cout << endl;
    }*/

    // vector<int> nodes = { 1, NULL, 2, NULL, 3, NULL, 4 };
    // vector<TreeNode*> parentNodes(nodes.size(), nullptr);
    TreeNode* pTreeRoot2 = nullptr;

    // tree.printNodesArray(nodes);
    // pTreeRoot2 = tree.createTreeFromArray(nodes, 0, 1, parentNodes);
    // tree.printTree(pTreeRoot2);

    // nodes = { 3, 4, 6, NULL, NULL, 7, 9, 11, NULL, NULL, 13 };
    // parentNodes.assign(nodes.size(), nullptr);
    // tree.printNodesArray(nodes);
    // pTreeRoot2 = tree.createTreeFromArray(nodes, 0, 1, parentNodes);
    // tree.printTree(pTreeRoot2);

    //nodes = { 56, 48, 59, 39, 52, 57, 69, 27, 44, 50, 54, NULL, 58, 66, NULL, NULL, NULL, 40, NULL, NULL, NULL, NULL, 55, NULL, NULL, NULL, 68, 38 };
    //nodes = { 5, 3, 6, 2, 4, NULL,7, NULL, 1 };
    //nodes = { 8, 3, 5, NULL, 9, NULL, NULL, 9, 5 };
    //nodes = { 1, 2, 3, NULL, NULL, NULL, -10, 11, 20 };
    //nodes = { -10,9,20,NULL,NULL,15,7 };
    // nodes = { 9,6,-3,NULL,NULL,-6,2,NULL,NULL,2,NULL,-6,-6,-6 };
    // //nodes = { 5,4,8,11,NULL,13,4,7,2,NULL,NULL,NULL,1 };
    // parentNodes.assign(nodes.size(), nullptr);
    // tree.printNodesArray(nodes);
    // pTreeRoot2 = tree.createTreeFromArray(nodes, 0, 1, parentNodes);
    // tree.printTree(pTreeRoot2);

    // int key = 5;
    // cout << "delete node: " << key << endl;
    // solu.deleteNode(pTreeRoot2, key);
    // tree.printTree(pTreeRoot2);

    // solu.mergeTrees(pTreeRoot, pTreeRoot2);
    // tree.printTree(pTreeRoot);

    // solu.flatten(pTreeRoot2);
    // cout << "flatten tree:" << endl;
    // tree.printTree(pTreeRoot2);
    
    // cout<< "Sum Root to Leaf Numbers: " << solu.sumNumbers(pTreeRoot2) << endl;

    // BSTIterator* treeIterator = new BSTIterator(pTreeRoot2);
    // int i = 0;
    // while(i<25)
    // {
    //     cout << "hasNext(): ";
    //     if(treeIterator->hasNext())
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

    // vector<int> rightSideView = solu.rightSideView(pTreeRoot2);
    // cout << "right side view: ["; 
    // for (auto node : rightSideView)
    //     cout << node << " ";
    // cout << "]" << endl;

    // solu.invertTree(pTreeRoot2);
    // cout << "invert tree:" << endl;
    // tree.printTree(pTreeRoot2);

    // cout << "Maximum Path Sum: " << solu.maxPathSum(pTreeRoot2) << endl;

    vector<int> preorderee = { 56, 48, 33, 27, 36, 34, 52, 50, 54, 55, 59, 57, 58, 69, 66, 68 };
    // vector<int> inorderee = { 27, 33, 34, 36, 48, 50, 52, 54, 55, 56, 57, 58, 59, 66, 68, 69 };
    vector<int> postorderee = { 27, 34, 36, 33, 50, 55, 54, 52, 48, 58, 57, 68, 66, 69, 59, 56 };

    // pTreeRoot2 = solu.buildTreeFromPreInOrder(preorderee, inorderee);
    // tree.printTree(pTreeRoot2);
    
    // pTreeRoot2 = solu.buildTreeFromInPostOrder(inorderee, postorderee);
    // tree.printTree(pTreeRoot2);

    pTreeRoot2 = solu.constructFromPrePost(preorderee, postorderee);
    tree.printTree(pTreeRoot2);

    DOCK();

    return 0;
}
