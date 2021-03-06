#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "canvas.h"

int main(int argc, char *argv[]) {
    char *seedFilename;
    size_t rows, columns;
    Canvas canvas1 = NULL;
    Canvas canvas2 = NULL;
    Canvas currentCanvas = NULL;
    Canvas futureCanvas = NULL;

    // Validate and parse the args
    if (argc != 2) {
        fprintf(stderr, "Usage: `%s <filename>` where file is the initial seed.\n", argv[0]);
        exit(0);
    }
    seedFilename = argv[1];

    // Initialize program logic
    canvasInit(seedFilename, &canvas1, &rows, &columns);
    emptyCanvas(&canvas2, rows, columns);
    currentCanvas = canvas1;
    futureCanvas = canvas2;

    // Iterate until no more changes happen
    while (1) {
        canvasDisplay(currentCanvas, rows, columns);
        if (canvasIterate(currentCanvas, futureCanvas, rows, columns) == 0) {
            break;
        }

        canvasesSwitch(&currentCanvas, &futureCanvas);

        // Display Logic
        if(usleep(200000) == -1 || system("clear") != 0){
            fprintf(stderr, "Display has failed.\n");
            exit(1);
        }
    }

    printf("Terminating...\n");

    canvasFree(canvas1, rows);
    canvasFree(canvas2, rows);
    return 0;
}
