#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#pragma warning(disable:4996)
#define VERTICIES 1000

typedef struct qblock {
	int val;
	struct qblock* next;
} qblock_t;

typedef struct queue {
	struct qblock* first;
	struct qblock* last;
} queue_t;

typedef struct edge {
	int first;
	int second;
} edge_t;

typedef struct Matrix {
	int size;
	int** matrix;
} matrix_t;

typedef struct graph {
	int vertex;
	matrix_t* matrix;
} graph_t;

queue_t* QueueCreate(int val) {
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	if (!queue) return NULL;
	queue->last = (qblock_t*)malloc(sizeof(qblock_t));
	if (!queue->last) return NULL;
	queue->last->val = val;
	queue->last->next = NULL;
	queue->first = queue->last;
	return queue;
};

void QueueDestroy(queue_t* queue) {
	qblock_t* iterblock = NULL;
	while (queue->first != queue->last) {
		iterblock = queue->last;
		queue->last = queue->last->next;
		free(iterblock);
	}
	free(queue->last);
};

queue_t* QueueAddEl(queue_t* queue, int val) {
	if (queue->last == NULL) {
		queue = QueueCreate(val);
		return queue;
	}
	queue->last->next = (qblock_t*)malloc(sizeof(qblock_t));
	if (queue->last->next == NULL) return queue;
	queue->last->val = val;
	queue->last = queue->last->next;
	queue->last->next = NULL;
	return queue;
};

int QueuePop(queue_t* queue) {
	int res = queue->first->val;
	qblock_t* mem = queue->first;
	queue->first = queue->first->next;
	if (queue->first == NULL) queue->last = NULL;
	free(mem);
	return res;
};

int QueueIsEmpty(queue_t* queue) {
	if (queue == NULL) return -1;
	if (queue->first == queue->last) return 1;
	return 0;
}

matrix_t* MatrixCreate(int size) {
	matrix_t* res = (matrix_t*)malloc(sizeof(matrix_t));

	if (!res) {
		return NULL;
	}

	res->size = size;
	res->matrix = (int**)malloc(size * sizeof(int*) + size * size * sizeof(int));

	if (!res->matrix) {
		return NULL;
	}

	char* start = (char*)res->matrix + size * sizeof(int*);

	for (int i = 0; i < size; i++) {
		res->matrix[i] = (int*)(start + i * size * sizeof(int));
	}

	return res;
}

void MatrixFill(matrix_t* matrix, int a) {

	if (!matrix) {
		return;
	}

	for (int i = 0; i < matrix->size; i++) {
		for (int j = 0; j < matrix->size; j++) {
			matrix->matrix[i][j] = a;
		}
	}
}

void MatrixDelete(matrix_t* matrix) {
	free(matrix->matrix);
	free(matrix);
}

void MatrixPrint(matrix_t* matrix) {
	if (matrix->matrix == NULL) return;
	for (int i = 0; i < matrix->size; i++) {
		for (int j = 0; j < matrix->size; j++) {
			printf("%d ", matrix->matrix[i][j]);
		}
		printf("\n");
	}
}

graph_t GraphCreate() {
	int check = 0;
	int a, b;

	graph_t graph;

	check = fscanf(stdin, "%d", &graph.vertex);
	if (check != 1) abort();

	graph.matrix = MatrixCreate(graph.vertex);
	MatrixFill(graph.matrix, 0);

	while (!0) {
		check = fscanf(stdin, "%d %d", &a, &b);
		
		if (check == EOF || feof(stdin) != 0) {
			break;
		}

		graph.matrix->matrix[a][b] = 1;
		graph.matrix->matrix[b][a] = 1;
	}
	return graph;
};

void GraphDelete(graph_t graph) {
	MatrixDelete(graph.matrix);
}

void GraphBFS(graph_t graph) {
	if (graph.vertex == 0 || graph.matrix->matrix == NULL) {
		return;
	}
	int* visibility = (int*)calloc(graph.vertex, sizeof(int));
	if (!visibility) {
		MatrixDelete(graph.matrix);
		abort();
	}
	queue_t* vert_queue = QueueCreate(graph.vertex);

	visibility[0] = 1;
	int num_visited = 1;
	QueueAddEl(vert_queue, 0);

	while (!QueueIsEmpty(vert_queue)) {
		int vert = QueuePop(vert_queue);
		printf("%d ", vert);
		for (int j = 1; j < graph.vertex; j++) {
			if ((graph.matrix->matrix[vert][j] == 1) && (visibility[j] == 0)) {
				visibility[j] = 1;
				QueueAddEl(vert_queue, j);
			}
		}
	}
	free(visibility);
	QueueDestroy(vert_queue);
};

void StressTest() {
	LARGE_INTEGER start, end, counter;

	FILE* F = fopen("Test.txt", "w+");
	if (!F) {
		return;
	}

	graph_t graph;
	graph.vertex = VERTICIES;
	graph.matrix = MatrixCreate(VERTICIES);
	MatrixFill(graph.matrix, 0);

	srand(time(NULL));

	fprintf(F, "%d", VERTICIES + 1);

	int rand_vertex = 0;
	for (int i = 0, vertex = 0; i < VERTICIES - 1; i++) {
		if ((vertex = (1 + rand()) % VERTICIES) != (i + 1)) {
			fprintf(F, "%d %d", i + 1, vertex);
			graph.matrix->matrix[i + 1][vertex] = 1;
			graph.matrix->matrix[vertex][i + 1] = 1;
			--i;
		}
		else {
			fprintf(F, "%d %d", vertex, (rand_vertex = (1 + rand()) % VERTICIES));
			graph.matrix->matrix[vertex][rand_vertex] = 1;
			graph.matrix->matrix[rand_vertex][vertex] = 1;
		}
	}
	fprintf(F, "\n");

	fseek(F, SEEK_SET, 0);

	QueryPerformanceFrequency(&counter);
	QueryPerformanceCounter(&start);

	GraphBFS(graph);

	QueryPerformanceCounter(&end);
	printf("\nTime: %lf", (double)((double)(end.QuadPart - start.QuadPart) / (double)counter.QuadPart));

	fclose(F);
}


int main() {
	/*graph_t graph = GraphCreate();
	GraphBFS(graph);
	GraphDelete(graph);*/
	
	StressTest();
	return 0;
}