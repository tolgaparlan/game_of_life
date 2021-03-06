#include <stdio.h>
#include <stdlib.h>
#include "canvas.h"

/**
 * Print out the given canvas to stdout line by line
 * @param canvas
 * @param rows
 * @param columns
 */
void canvasDisplay(Canvas canvas, size_t rows, size_t columns) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            printf("%c", ((canvas[i][j] & 0x01) ? 'O' : '.'));
        }
        printf("\n");
    }
}

/**
 * Free the canvas from the heap
 * @param canvas An allocated canvas
 * @param rows Rows length
 */
void canvasFree(Canvas canvas, size_t rows) {
    for (int i = 0; i < rows; ++i) {
        free(canvas[i]);
    }
    free(canvas);
}

/**
 * Generate a canvas that is all dead squares
 * @param canvas Pointer to an non-allocated canvas
 * @param rows Rows length
 * @param columns Columns length
 */
void emptyCanvas(Canvas *canvas, size_t rows, size_t columns) {
    *canvas = malloc(sizeof(uint8_t *) * rows);
    for (size_t i = 0; i < rows; ++i) {
        (*canvas)[i] = malloc(sizeof(uint8_t) * columns);
        for (size_t j = 0; j < columns; ++j) {
            (*canvas)[i][j] = 0;
        }
    }
}

/**
 * Initialize a canvas from the given seed file
 * @param seedFilename name of the seed file. Should exist and have the proper format
 * @param canvas Pointer to an non-allocated canvas
 * @param rows Rows length
 * @param columns Columns length
 */
void canvasInit(char *seedFilename, Canvas *canvas, size_t *rows, size_t *columns) {
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

    lineBuffer = (char *) malloc(sizeof(char) * (*columns + 2));
    bufferLength = *columns + 2;

    // Read the starting canvas
    *canvas = (Canvas) malloc(sizeof(uint8_t *) * *rows);
    for (size_t i = 0; i < *rows; ++i) {
        if (fgets(lineBuffer, bufferLength, fp) == NULL) {
            fprintf(stderr, "Seed file is incorrect.\n");
            exit(1);
        }

        (*canvas)[i] = (uint8_t *) malloc(sizeof(uint8_t) * *columns);

        for (size_t j = 0; j < *columns; ++j) {
            (*canvas)[i][j] = lineBuffer[j] == '0' ? 0 : 1;
        }

    }

    free(lineBuffer);
    fclose(fp);
}

/**
 * Switch the canvas pointers
 * @param a Pointer to one canvas
 * @param b Pointer to another canvas
 */
void canvasesSwitch(Canvas *a, Canvas *b) {
    Canvas temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Return the number of live neighbors the cell at (column, row) has
 * @param canvas Currently valid canvas
 * @param rows Rows length
 * @param columns Columns length
 * @param row Row index
 * @param column Column index
 * @return the count
 */
static uint8_t countLiveNeighbors(Canvas canvas, size_t rows, size_t columns, size_t row, size_t column) {
    uint8_t count = 0;
    size_t rightIndex = column < (columns - 1) ? (column + 1) : -1;
    size_t leftIndex = column > 0 ? (column - 1) : -1;
    size_t aboveIndex = row > 0 ? (row - 1) : -1;
    size_t underIndex = row < (rows - 1) ? (row + 1) : -1;

    // Make sure corners don't cause out-of-bound array access

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

/**
 * Given the current state of the canvas, calculates the next iteration of the game of life
 * on the future canvas.
 * @param currentCanvas Current Canvas
 * @param futureCanvas Where the store the next iteration
 * @param rows Rows length
 * @param columns Columns length
 * @return returns 1 if any changes were made during the calculation, 0 otherwise
 */
int canvasIterate(Canvas currentCanvas, Canvas futureCanvas, size_t rows, size_t columns) {
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
