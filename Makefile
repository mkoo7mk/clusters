all:
	gcc -std=c99 -Werror -Wextra -Wall -DNDEBUG -Og -lm cluster.c -o cluster
val:
	gcc -std=c99 -Werror -Wextra -Wall -DNDEBUG -Og -lm cluster.c -o cluster; valgrind ./cluster clusters.txt 10
temp:
	gcc -std=c99 -Werror -Wextra -Wall -DNDEBUG -Og -lm cluster.c -o cluster ; ./cluster clusters.txt 10
