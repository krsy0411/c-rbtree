# bst에서 보통의 노드 구조
class Node:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None
        self.parent = None

def bst_search(x, k):
    # 기저조건 : 노드가 null이거나 키값이 일치하는 경우
    if x == None or k == x.key:
        return x
    
    if k < x.key:
        return bst_search(x.left, k)
    else:
        return bst_search(x.right, k)
    
def bst_search_iterative(x, k):
    while ((x.key != None) and (x.key != k)):
        # 1. x.key와 k를 비교 -> k가 작으면 왼쪽 서브트리로 이동
        if k < x.key:
            x = x.left
        # 2. x.key와 k를 비교 -> k가 크면 오른쪽 서브트리로 이동
        else:
            x = x.right
    
    return x

def bst_maximum(x):
    # 최대한 오른쪽 서브트리로 이동
    while(x.right != None):
        x = x.right
    
    return x

def bst_minimum(x):
    # 최대한 왼쪽 서브트리로 이동
    while(x.left != None):
        x = x.left
    
    return x

# 주어진 노드의 다음(오름차순) 노드를 찾는 함수
def bst_successor_iterative(x):
    # 오른쪽 자식이 있는 경우
    if x.right is not None:
        current = x.right
        # 오른쪽 서브트리의 최솟값을 찾아서 반환
        while current.left != None:
            current = current.left
        
        return current
    else:
        # 오른쪽 자식이 없는 경우 : 현재 노드가 부모 노드의 왼쪽 노드가 될 때까지 부모 따라 거슬러 올라감
        parent = x.parent
        
        while (parent is not None) and (x == parent.right):
            x = parent
            parent = parent.parent

        # 최대한 거슬러 올라간 경우에서 부모 노드 반환
        return parent
    
# 삽입 구현
# tree : 이진탐색트리, x : 삽입할 노드
def bst_insert(tree, x):
    temp_node = tree.root
    parent_node = None
    
    # 최대한 내려가기
    while temp_node != None:
        parent_node = temp_node
        
        # 값에 따라 좌/우 방향따라 이동
        if x.key < temp_node.key:
            temp_node = temp_node.left
        else:
            temp_node = temp_node.right
    
    # 삽입 위치 결정
    x.parent = parent_node
    
    # 1. 트리가 비어있는 경우
    if parent_node == None:
        tree.root = x
    elif x.key < parent_node.key:
        # 부모 노드의 값보다 작은 경우 -> 왼쪽
        parent_node.left = x
    else:
        # 부모 노드의 값보다 큰 경우 -> 오른쪽
        parent_node.right = x
        
# 이진탐색 트리 삭제 연산 : 왼쪽 서브트리의 최댓값을 이용해 키와 교환하는 방법
# tree : 이진탐색트리, x : 삭제할 노드
def bst_delete_recursive(current_node, target_node):
    if not current_node:
        return None
    elif current_node.key < target_node.key:
        current_node.left = bst_delete_recursive(current_node.left, target_node)
    elif current_node.key > target_node.key:
        current_node.right = bst_delete_recursive(current_node.right, target_node)
    else:
        # 1) 삭제 노드가 리프 노드인 경우 -> 그냥 null 처리
        if (not current_node.left) and (not current_node.right):
            current_node = None
        # 2) 삭제 노드의 자식이 하나 있는 경우
        elif (not current_node.left):
            current_node = current_node.right
        elif (not current_node.right):
            current_node = current_node.left
        # 3) 삭제 노드의 자식이 두 개 있는 경우
        else:
            # 왼쪽 서브트리의 최대값을 찾아야함
            replace_node = current_node.left
            
            while replace_node.right:
                replace_node = replace_node.right
            
            current_node.key, replace_node.key = replace_node.key, current_node.key
            current_node.left = bst_delete_recursive(current_node.left, replace_node.key)
    
    return current_node

# 이진탐색 트리 삭제 연산 : 오른쪽 서브트리의 최소값을 이용해 키와 교환하는 방법
# tree : 이진탐색트리, x : 삭제할 노드