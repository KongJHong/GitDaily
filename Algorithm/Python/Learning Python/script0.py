# A first Python script

class TreeNode:
    def __init__(self,x):
        self.val = x
        self.left = None
        self.right = None

    def __str__(self):
        return "value:%d"%(self.val)

    

class BinaryTree:
    def __init__(self,root=None):
        self.root = root

    def inTraverse(self,ansLst,node):
        if node != None:
            self.inTraverse(ansLst,node.left)
            ansLst.append(node.val)
            self.inTraverse(ansLst,node.right)
        return ansLst
    

class Solution:
    def buildTree(self, inorder, postorder) -> TreeNode:
        if not inorder or not postorder: return None
        
        node = TreeNode(postorder[-1])
        index = inorder.index(node.val)
        node.left = self.buildTree(inorder[:index],postorder[:index])
        node.right = self.buildTree(inorder[index+1:],postorder[index:-1])
        
        return node

if __name__ == "__main__":
    s = Solution()
    inorder = [9,3,15,20,7]
    postorder = [9,15,7,20,3]
    node = s.buildTree(inorder,postorder)

    b = BinaryTree(node)
    lst = b.inTraverse([],b.root)
    print(lst)