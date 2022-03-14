# Main version
gcc main.c board.c priority.c test.c util.c play.c pathfinding.c binaryHeap.c dynamicArray.c element.c -o main.exe

# Old Pathfinding
gcc main.c board.c priority.c test.c util.c play.c linkedList.c pathfinding.c -o main.exe

# Perfomance Tests

gcc perftest.c board.c priority.c test.c util.c play.c pathfinding.c binaryHeap.c dynamicArray.c element.c -o perftest.exe

gcc perftest.c board.c priority.c test.c util.c play.c linkedList.c pathfinding.c -o perftest.exe