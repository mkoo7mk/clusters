all:
	gcc -std=c99 -Werror -Wextra -Wall -DNDEBUG -lm cluster.c -o cluster
val:
	gcc -std=c99 -Werror -Wextra -Wall -DNDEBUG -lm cluster.c -o cluster; valgrind ./cluster clusters.txt 8
temp:
	gcc -std=c99 -Werror -Wextra -Wall -DNDEBUG -lm cluster.c -o cluster ; ./cluster clusters.txt 8
