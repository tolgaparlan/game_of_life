#include <stdio.h>
#include <stdlib.h>
#include "canvas.h"

int main(int argc, char *argv[]) {
    char *seedFilename;

    size_t rows, columns;
    uint8_t **canvas1 = NULL;
    uint8_t **canvas2 = NULL;
    uint8_t **currentCanvas = NULL;
    uint8_t **futureCanvas = NULL;

    // Validate and parse the args
    if (argc != 2) {
        fprintf(stderr, "Usage: `%s <filename>` where file is the initial seed.\n", argv[0]);
        exit(0);
    }
    seedFilename = argv[1];

    canvasInit(seedFilename, &canvas1, &rows, &columns);
    emptyCanvas(&canvas2, rows, columns);
    currentCanvas = canvas1;
    futureCanvas = canvas2;

    // Iterate until no more changes happen
    while (1) {
        canvasDisplay(currentCanvas, rows, columns);
        if (canvasIterate(currentCanvas, futureCanvas, rows, columns) == 1) {
            break;
        }

        canvasesSwitch(&currentCanvas, &futureCanvas);
    }

    printf("Terminating...\n");

    canvasFree(canvas1, rows);
    canvasFree(canvas2, rows);
    return 0;
}
