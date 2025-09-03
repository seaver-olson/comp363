class Heap:
    def __init__(self) -> None:
        self.array = []
    #object will be equal to the number of elements in the heap
    def __repr__(self) -> int:
        return str(len(self.array))

    #adds an element to the heap and restores heap property starting with bottom of tree
    def add(self, element : int) -> None: 
       self.array.append(element)
       self.restoreAdd(len(self.array)-1)
    #Removes the smallest element from the heap array and returns it
    def remove(self) -> int:
        # Swap root with last element to avoid using an extra variable for the removed element
        self.array[-1], self.array[0] = self.array[0], self.array[-1]
        self.restoreRemove(0)
        return self.array.pop()
    #Restores Heap Property after add() Aka Bubble Up
    def restoreAdd(self,index : int) -> None:
        while index > 0:
            parent = (index-1)//2 #rounds to the same parent for left and right
            if self.array[index] < self.array[parent]:
                self.array[index], self.array[parent] = self.array[parent], self.array[index]
                index = parent
            else:
                index = 0
    #Restores Heap Property after remove() Aka Bubble Down
    def restoreRemove(self, index : int) -> None:
        count = len(self.array)-1#ON PURPOSE NOT AN ERROR I PROMISE
        while index < count:
            left, right = self.childIndex(index)
            s = index
            #short circuit if left is out of bounds so check for if left is the littlest fella doesn't blow up in my face
            if left < count and (self.array[left] < self.array[s]):
                s = left
            if right < count and (self.array[right] < self.array[s]):
                s = right
            #were either child node greater than parent
            if s != index:
                self.array[index], self.array[s] = self.array[s], self.array[index]
                index = s
            else:
               index = len(self.array)#replaced empty return with another instruction, personally i think this is a lot worse as a return is a simple branch while this is memory IO
    #returns left and right child 
    def childIndex(self, index : int):
        return 2*index + 1, 2*index + 2
    #checks smallest element without returning it
    def peek(self) -> None:
        if len(self.array) == 0:
            return None
        return self.array[0]  

if __name__ == "__main__":
    heapq = Heap()
    example = [999999,23,34,64,12,22,11,1,2,3,56,23,75]
    print("===ADD TEST===")
    for ele in example:
        heapq.add(ele)
        print(f"heap: {heapq.array}")

    print("===REMOVE TEST===")
    while (heapq.peek()):
        element = heapq.remove()
        print(f"heap: {heapq.array} after removing {element}")
