#include <stdio.h>
#include <stdlib.h>
#include "canvas.h"

void canvasDisplay(uint8_t **canvas, size_t rows, size_t columns) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            printf("%c", ((canvas[i][j] & 0x01) ? 'O' : '.'));
        }
        printf("\n");
    }
}

void canvasFree(uint8_t **canvas, size_t rows) {
    for (int i = 0; i < rows; ++i) {
        free(canvas[i]);
    }
    free(canvas);
}

void emptyCanvas(uint8_t ***canvas, size_t rows, size_t columns) {
    *canvas = malloc(sizeof(uint8_t *) * rows);
    for (size_t i = 0; i < rows; ++i) {
        (*canvas)[i] = malloc(sizeof(uint8_t) * columns);
        for (size_t j = 0; j < columns; ++j) {
            (*canvas)[i][j] = 0;
        }
    }
}

void canvasInit(char *seedFilename, uint8_t ***canvas, size_t *rows, size_t *columns) {
    FILE *fp;
    char *lineBuffer;
    size_t bufferLength;

    // Open the file
    fp = fopen(seedFilename, "r");
    if (fp == NULL) {
        fprintf(stderr, "File %s does not exist.\n", seedFilename);
        exit(1);
    }

    // Parse the file

    // First line has the row/column sizes
    if (fscanf(fp, "%zu %zu\n", columns, rows) != 2) {
        fprintf(stderr, "Seed file is incorrect.\n");
        exit(1);
    }

    lineBuffer = malloc(sizeof(char) * (*columns + 2));
    bufferLength = *columns + 2;

    // Read the starting canvas
    *canvas = (uint8_t **) malloc(sizeof(uint8_t *) * *rows);
    for (size_t i = 0; i < *rows; ++i) {
        if (fgets(lineBuffer, bufferLength, fp) == NULL) {
            fprintf(stderr, "Seed file is incorrect.\n");
            exit(1);
        }

        (*canvas)[i] = malloc(sizeof(uint8_t) * *columns);

        for (size_t j = 0; j < *columns; ++j) {
            (*canvas)[i][j] = lineBuffer[j] == '0' ? 0 : 1;
        }

    }

    free(lineBuffer);
    fclose(fp);
}

void canvasesSwitch(uint8_t ***currentCanvas, uint8_t ***futureCanvas) {
    uint8_t **temp = *currentCanvas;
    *currentCanvas = *futureCanvas;
    *futureCanvas = temp;
}

static uint8_t countLiveNeighbors(uint8_t **canvas, size_t rows, size_t columns, size_t row, size_t column) {
    uint8_t count = 0;
    size_t rightIndex = column < (columns - 1) ? (column + 1) : -1;
    size_t leftIndex = column > 0 ? (column - 1) : -1;
    size_t aboveIndex = row > 0 ? (row - 1) : -1;
    size_t underIndex = row < (rows - 1) ? (row + 1) : -1;

    // up&down&left&right
    count += (leftIndex != -1) ? canvas[row][leftIndex] : 0;
    count += (rightIndex != -1) ? canvas[row][rightIndex] : 0;
    count += (aboveIndex != -1) ? canvas[aboveIndex][column] : 0;
    count += (underIndex != -1) ? canvas[underIndex][column] : 0;

    // diagonals
    count += (leftIndex != -1 && aboveIndex != -1) ? canvas[aboveIndex][leftIndex] : 0;
    count += (leftIndex != -1 && underIndex != -1) ? canvas[underIndex][leftIndex] : 0;
    count += (rightIndex != -1 && aboveIndex != -1) ? canvas[aboveIndex][rightIndex] : 0;
    count += (rightIndex != -1 && underIndex != -1) ? canvas[underIndex][rightIndex] : 0;

    return count;
}

int canvasIterate(uint8_t **currentCanvas, uint8_t **futureCanvas, size_t rows, size_t columns) {
    int changed = 0; // track whether we have changed anything

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            uint8_t c = countLiveNeighbors(currentCanvas, rows, columns, i, j);
            if (currentCanvas[i][j]) { // alive
                futureCanvas[i][j] = c == 2 || c == 3;
            } else { // dead
                futureCanvas[i][j] = c == 3;
            }

            if (futureCanvas[i][j] != currentCanvas[i][j]) changed = 1;
        }
    }

    return changed;
}
