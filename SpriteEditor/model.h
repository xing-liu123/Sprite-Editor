#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QColor>
#include <QPen>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QStack>


//We add enum (jinwen)
enum class SelectedTool{
    Undefined,
    SC_Pen,
    SC_Eraser,
    SC_Bucket,
    SC_ShapeCreator
};

enum class ShapeCreator{
    SC_Line,
    SC_Ciecle,
    SC_Rectangle
};

class model : public QObject
{
    Q_OBJECT
public:
    explicit model(QObject *parent = nullptr);

private:
    SelectedTool currentTool;
    ShapeCreator currentShape;
    QList<QImage> frames;
    QList<QPoint> startEndLoc;
    QColor penColor;
    int canvasSize;
    int zoomSize;
    int zoomIndex;
    int framesPerSec;
    int currentFrame;
    int penSize;
    int eraserSize;
    double ratio;

    // redo and undo
    QStack<QList<QImage>> undoStack;
    QStack<QList<QImage>> redoStack;
    //
    int numOfPixels;
    int pixelLength;

    int currentIndex; //index counter to access frames.
    bool isChanged = false; // checks if there were any changes on the frame.

    void updatePixelsByPen(int, int);
    void updatePixelsByEraser(int, int);
    void updatePixelsByBucketFiller(int, int);
    void updatePixelsByShapeCreator(int, int, int, int);

    //Read/Open file //still in testing phase, need to work on.
    void read(QString fileName);
    void initializeFrame();

signals:

    // control buttons
    void disableDeleteButton();
    void enableDeleteButton();
    void disableNextButton();
    void enableNextButton();
    void disableLastButton();
    void enableLastButton();
    void enableSwapUp();
    void enableSwapDown();
    void disableSwapUp();
    void disableSwapDown();
    void sendCurrentColor(QColor);
    void sendList(QList<QImage>);
    void updateFrameNumberCombo(int, int);
    void updateFrameNumberLabel(int, int);
    void setColorLabel(QColor);
    void enableUndo();
    void disableUndo();
    void enableRedo();
    void disableRedo();
    void startButtons();
    void setCanvas(QPixmap);

    void showSprite(QImage);

    //zoom
    void toZoomIn(QPixmap, int, int);
    void toZoomOut(QPixmap, int, int);
    void disableZoom(std::string);
    void enableZoomIn();
    void enableZoomOut();

    //upload and save warning message
    void errorMessage(QString errorMsg);

public slots:
    void addNewFrame();
    void insertNewFrame();
    void nextFrame();
    void lastFrame();
    void deleteFrame();
    void zoomIn();
    void zoomOut();
    //void zoomOut();
    void updateFPS(int);
    void updatePenSize(int);
    void updateEraserSize(int);
    void setStartingArea(int, int);
    void updatePenColor(QColor);
    void updateTool(std::string);
    void getList(QList<QImage>);
    void checkCurrentColor(QColor);
    void clearCanvas();
    void copyFrame();
    void swapUp();
    void swapDown();

    //Added
    void undo(); //need change parameters?
    void redo(); //need change parameters?
    void selectedFrame(int);
    void updatePixels(int,int,int = 0,int = 0);
    void updateToolSize(int);
    void drawOnCanvas(QPoint);
    void updateCanvasSize();

    void initializeCanvasSize(int);
    void initializeShapeTool(int);

    //Preview
    void previewOfFrames();
    void updateActualLabel();


    //Save file
    void save(QString fileName);

    void open(QString fileName);

    void saveFrameToStack();

    //Release Button
    void mouseRelease(QPoint& loc);

};

#endif // MODEL_H
