#ifndef GAME_OF_LIFE_CANVAS_H
#define GAME_OF_LIFE_CANVAS_H

typedef uint8_t** Canvas;

void canvasDisplay(Canvas canvas, size_t rows, size_t columns);

void canvasFree(Canvas canvas, size_t rows);

void canvasInit(char *seedFilename, Canvas *canvas, size_t *rows, size_t *columns);

int canvasIterate(Canvas currentCanvas, Canvas futureCanvas, size_t rows, size_t columns);

void emptyCanvas(Canvas *canvas, size_t rows, size_t columns);

void canvasesSwitch(Canvas *currentCanvas, Canvas *futureCanvas);

#endif //GAME_OF_LIFE_CANVAS_H
