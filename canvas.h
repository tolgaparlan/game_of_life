#ifndef GAME_OF_LIFE_CANVAS_H
#define GAME_OF_LIFE_CANVAS_H

void canvasDisplay(uint8_t **canvas, size_t rows, size_t columns);

void canvasFree(uint8_t **canvas, size_t rows);

void canvasInit(char *seedFilename, uint8_t ***canvas, size_t *rows, size_t *columns);

int canvasIterate(uint8_t **currentCanvas, uint8_t **futureCanvas, size_t rows, size_t columns);

void emptyCanvas(uint8_t ***canvas, size_t rows, size_t columns);

void canvasesSwitch(uint8_t ***currentCanvas, uint8_t ***futureCanvas);

#endif //GAME_OF_LIFE_CANVAS_H
