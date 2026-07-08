#include "graph.h"
#include <stdio.h>



int main() {
    Graph G;
    CreateGraph(&G);
    ShowVertex(&G);
    ShowGraph(&G);
    DestroyGraph(&G);
    return 0;
}