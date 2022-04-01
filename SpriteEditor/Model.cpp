#include "model.h"
#include "qtimer.h"

model::model(QObject *parent)
    : QObject{parent}
{
    penColor.setRgb(0, 0, 0, 255);
    canvasHeight = 20;
    canvasWidth = 20;
    zoomHeight = 20;
    zoomWidth = 20;
    framesPerSec = 1;
    currentFrame = 1;
    penSize = 1;
    eraserSize = 1;
    currentIndex = 0; //for keeping track of our current index in the list when displaying a sprite.
    // default values to be determined
    frames.append(QImage (canvasHeight, canvasWidth, QImage::Format_ARGB32));

//    //gon
//    frames.append(QImage (360, 360, QImage::Format_ARGB32));
//    //

    //Make the first inage to have a white background
    frames[currentFrame-1].fill(Qt::white);



}

// add a new frame to the position next to the current frame
void model::addNewFrame(){
    QImage frame(canvasHeight, canvasWidth, QImage::Format_ARGB32);
    frames.insert(currentFrame, frame);

    // move to the new frame
    currentFrame++;

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());
    emit enableDeleteButton();
    emit enableLastButton();

    // if the new frame is at the end of the list, disale next button
    if(currentFrame == frames.size()){
        emit disableNextButton();
    }
    frames[currentFrame-1].fill(Qt::white);
    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit goToFrame(map);
}

// insert a new frame to the position before current frame
void model::insertNewFrame(){
    QImage frame(canvasHeight, canvasWidth, QImage::Format_ARGB32);
    frames.insert(currentFrame - 1, frame);

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());
    emit enableDeleteButton();
    emit enableNextButton();

    if(currentFrame == 1){
        emit disableLastButton();
    }

    frames[currentFrame-1].fill(Qt::white);
    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit goToFrame(map);
}

void model::nextFrame(){
    emit updateFrameNumberCombo(++currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());

    emit enableLastButton();
    if(currentFrame == frames.size()){
        emit disableNextButton();
    }

     QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
     emit goToFrame(map);
}

void model::lastFrame(){
    emit updateFrameNumberCombo(--currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());

    emit enableNextButton();
    if(currentFrame == 1){
        emit disableLastButton();
    }

    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit goToFrame(map);
}

void model::deleteFrame(){
    frames.removeAt(currentFrame - 1);

    if(frames.size() == 1){
        emit disableNextButton();
        emit disableLastButton();
        emit disableDeleteButton();
    }

    if(currentFrame == frames.size()){
        emit disableNextButton();
    }

    // if the last frame is deleted
    if(currentFrame - 1 == frames.size()){
        currentFrame--;
        QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
        emit goToFrame(map);
    }
    else{
        QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
        emit goToFrame(map);
    }

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());
}

//Need to Fix
//This method increases the size of the image, and sends it back to the
//view  to be displayed in the canvas
void model::zoomIn(){    

    //Decrease our height and Width if is not out of bounds
    //CHANGE: Make another emit that sends message to vew and it desable zoom Out button
    if(zoomHeight - 5 < 4 || zoomWidth - 5 < 4){
        zoomHeight = zoomHeight - 5;
        zoomWidth = zoomWidth - 5;
    }

    //Set our Image to the scaled version
    //frames[currentFrame-1] = frames[currentFrame-1].scaled(canvasWidth, canvasHeight);

    //setWorldTransform
    //Create a Pixmap to return to view
    QPixmap currentPic;
    //Convert QImage to QPixmap
    currentPic.convertFromImage(frames[currentFrame-1]);
    //Return the pixmap of our QImage with the scaled version

    emit setCanvas(currentPic.scaled(zoomWidth, zoomHeight));
    //emit setCanvas(frames[currentFrame -1]);
}

//Need to Fix
//This method decreases the size of the image, and sends it back to the
//view  to be displayed in the canvas
void model::zoomOut(){
    //Increase our height and Width if is not out of bounds
    //CHANGE: Make another emit that sends message to vew and it desable zoom Out button
    if(zoomHeight + 5 > canvasHeight || zoomWidth + 5 > canvasWidth){
        zoomHeight = zoomHeight + 5;
        zoomWidth = zoomWidth + 5;
    }

    //Set our Image to the scaled version
    //frames[currentFrame-1] = frames[currentFrame-1].scaled(canvasWidth, canvasHeight);

    //Create a Pixmap to return to view
    QPixmap currentPic;
    //Convert QImage to QPixmap
    currentPic.convertFromImage(frames[currentFrame-1]);
    //Return the pixmap of our QImage with the scaled version
    emit setCanvas(currentPic.scaled(zoomWidth, zoomHeight));
    //emit setCanvas(frames[currentFrame -1]);


}

void model::updateFPS(int fps){
    framesPerSec = fps;
}

void model::updatePenSize(int size){
    //penSize = size;
    penSize += size;
}

void model::updateEraserSize(int size){
    eraserSize = size;   
}

void model::setStartingArea(int, int){
    //TODO:
}

void model::checkCurrentColor(QColor){
    emit sendCurrentColor(penColor);
}

void model::updatePenColor(QColor color){
    penColor = color;
    emit setColorLabel(penColor);
}
//updates our current tool we are using
void model::updateTool(std::string tool){
    //Should we do a switch case? if we do, we have to change parameters (bri)
    if(tool == "pen")
        currentTool = SelectedTool::SC_Pen;
    else if(tool == "eraser")
        currentTool = SelectedTool::SC_Eraser;
    else if(tool == "bucket")
        currentTool = SelectedTool::SC_Bucket;
    else if(tool == "shapeCreator")
        currentTool = SelectedTool::SC_ShapeCreator;
}

//Don't need the QList as Parameter??
void model::getList(QList<QImage>){
    //TODO:
    emit sendList(frames);
}

//need change parameters?
void model::undo(){
    //TODO
}

//need change parameters?
void model::redo(){
    //TODO
}

//Frame that we are currently in
void model::selectedFrame(int index){
    currentFrame = index + 1;
    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit goToFrame(map);
    emit updateFrameNumberLabel(currentFrame, frames.size());

    if(currentFrame == frames.size()){
        emit disableNextButton();
    }
    else if(currentFrame == 1){
        emit disableLastButton();
    }
    else{
        emit enableNextButton();
        emit enableLastButton();
    }


}

//updates the toolsize, we first check our selected tool
void model::updateToolSize(int size){

    //change to a switch case if we add more brushes
    if(currentTool == SelectedTool::SC_Eraser)
        updateEraserSize(size);
    else if(currentTool == SelectedTool::SC_Pen)
        updatePenSize(size);
}

void model::updatePixels(int initialX, int initialY, int endX, int endY){
    switch(currentTool){
        case SelectedTool::SC_Pen:
            updatePixelsByPen(initialX,initialY);
            break;
        case SelectedTool::SC_Eraser:
            updatePixelsByEraser(initialX, initialY);
            break;
        case SelectedTool::SC_Bucket:
            //updatePixelsByBucket(x,y);
            break;
        case SelectedTool::SC_ShapeCreator:
            updatePixelsByShapeCreator(initialX, initialY, endX, endY);
            break;
        default:
            break;
    }

    //emit or call another method?
}

// gon

//void model::updatePixels2(int sx, int sy, int ex, int ey){
//    std::cout << "hit" << std::endl;

//    switch(currentTool){
//        case SelectedTool::Tool_Pen:
//            updatePixelsByPen2(sx,sy,ex,ey);
//            break;
//        case SelectedTool::Tool_Eraser:
//            //updatePixelsByEraser(x,y);
//            break;
//        case SelectedTool::Tool_Bucket:
//            //updatePixelsByBucket(x,y);
//            break;
//        case SelectedTool::Tool_ShapeCreator:
//            //updatePixelsByShapeCreator(x,y);
//            break;
//        default:
//            break;
//    }

//    //emit or call another method?
//}


void model::updatePixelsByPen(int x, int y){
    QImage* AFrame = &frames[currentFrame -1];
    QPainter Painter(AFrame);
    QPen Pen(penColor);
    Pen.setWidth(penSize);
    Painter.setPen(Pen);
    Painter.drawPoint(x,y);
    Painter.end();
}

void model::updatePixelsByEraser(int x, int y){
    QImage* AFrame = &frames[currentFrame -1];
    QPainter Painter(AFrame);
    // Give QPen a white color to act as eraser
    QPen Pen(QColor(255,255,255));
    Pen.setWidth(eraserSize);
    Painter.setPen(Pen);
    Painter.drawPoint(x,y);
    Painter.end();
}

void model::updatePixelsByShapeCreator(int initialX, int initialY, int endX, int endY){
    // Set pen specs for shape creator
    QImage* AFrame = &frames[currentFrame -1];
    QPainter Painter(AFrame);
    QPen Pen(penColor);
    Pen.setWidth(penSize);
    Painter.setPen(Pen);

    switch(currentShape){
        case ShapeCreator::SC_Line:
            Painter.drawLine(initialX, initialY, endX, endY);
            Painter.end();
            break;
        case ShapeCreator::SC_Ciecle:
            Painter.drawEllipse(initialX, initialY, initialX-endX, initialY-endY);
            Painter.end();
            break;
        case ShapeCreator::SC_Rectangle:
            Painter.drawRect(initialX, initialY, initialX-endX, initialY-endY);
            Painter.end();
            break;
        default:
            break;
    }
}

void model::updatePixelsByBucketFiller(int x, int y){
    QList<std::tuple<int,int>> pixelsToBeFilled;
    pixelsToBeFilled.append(std::tuple<int,int>(x,y));
    QColor colorToBeChanged = (frames[currentFrame -1]).pixelColor(x,y);
    pixelsToBeFilled = FindPixelsWithTheSameColorInBound(pixelsToBeFilled,colorToBeChanged,x,y);
}

QList<std::tuple<int,int>> model::FindPixelsWithTheSameColorInBound(QList<std::tuple<int,int>> coordinates,
                                                                    const QColor colorToBeChanged,
                                                                    int x,
                                                                    int y){

}

//void model::updatePixelsByPen2(int sx, int sy, int ex, int ey){
//    //QPen Pen(penColor);
//    for(int x = sx; x < ex; x++){
//        for(int y = sy; y < ey; y++){
//            frames[currentFrame -1].setPixel(x,y,penColor.rgba());
//        }
//    }
//}


//This method obtains where the current position of the mouse is in our canvas
//then it optains the ratio
void model::drawOnCanvas(QPoint pixelPoint){
    //Set the canvas ratio -> Canvas Label Size / number of pixels
    //(Choose height, but height and width should all be the same)
    std::cout << canvasHeight << std::endl;
    ratio = 360/canvasHeight;
    //Calculate x and y position of our QImage pixels
    int x = pixelPoint.x()/ratio;
    int y = pixelPoint.y()/ratio;

    //gon added
    int pointStartX = x * ratio;
    int pointStartY = y * ratio;
    int pointEndX = pointStartX + (penSize * ratio);
    int pointEndY = pointStartY + (penSize * ratio);

    std::cout << pointStartX << " " << pointStartY << std::endl;
    std::cout << pointEndX << " " << pointEndY << std::endl;

    //Edit the pixels of the current QImage
    //frames[currentFrame -1].setPixelColor(x, y,penColor);

    updatePixels(x,y);
    //Create a Pixmap to return to view
    QPixmap currentPic;
    //Convert QImage to QPixmap
    currentPic.convertFromImage(frames[currentFrame-1]);
    //Return the pixmap of our QImage
    emit setCanvas(currentPic);

//    //gon//
//    updatePixels2(pointStartX, pointStartY,pointEndX, pointEndY);
//    QPixmap currentPic;
//    //Convert QImage to QPixmap
//    currentPic.convertFromImage(frames[currentFrame -1]);
//    //Return the pixmap of our QImage
//    emit setCanvas(currentPic);
//    //////




}

void model::updateCanvasSize()
{
    canvasHeight += 1;
    canvasWidth += 1;
}

void model::previewOfFrames(){
    int time = 2000/framesPerSec;

    for(int i = 0; i < frames.size(); i++)
    {
        QTimer::singleShot(time * i, this, &model::updateActualLabel);
    }
}

void model::updateActualLabel(){

    emit showSprite(frames[currentIndex]);

    currentIndex++;
    if(currentIndex == frames.size())
        currentIndex = 0;
}

//save file
void model::save(QString fileName){//QJsonObject &json) const{ //change parameters
    QJsonObject json;

    int n = 0;
    json["height"] = canvasHeight;
    json["width"] = canvasWidth;
    json["numberOfFrames"] = frames.size();

    //*******Important******//
    // x and y coordinate are switched, x is columns and y is rows, to fix this
    // i will have rowNum go in the Y in the forloo for pixel when we get pixelcolor
    // and pixelNum will go in the X in the forloop for pixe when we get pixelcolor.
    //I'm doing this so it can fit the requirements in Q6 //
    //*******Important*******//
    QJsonObject framesObj;
    for(QImage a : frames)
    {
        QJsonArray frame;
        for(int rowNum = 0 ; rowNum < canvasHeight; rowNum++)
        {
            QJsonArray row;
            for(int pixelNum = 0; pixelNum < canvasWidth; pixelNum++)
            {
                QJsonArray pixelColor;
                QString pixelName = "pixel" + QString::number(pixelNum);
                pixelColor.append(a.pixelColor(pixelNum,rowNum).red());
                pixelColor.append(a.pixelColor(pixelNum,rowNum).green());
                pixelColor.append(a.pixelColor(pixelNum,rowNum).blue());
                pixelColor.append(a.pixelColor(pixelNum,rowNum).alpha());
                row.append(pixelColor);
            }
            frame.append(row);
        }
        QString frameName = "Frame" + QString::number(n);
        framesObj[frameName] = frame;
        n++;
    }

    json["frames"] = framesObj;
    QJsonDocument doc(json);

    QFile jsonFile(fileName + ".ssp");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
}

void model::open(QString fileName){
    read(fileName);
}

//Read/Open file
void model::read(QString fileName){
    //opening the file and saving the information to a QByteArray
    QJsonDocument doc;
    QByteArray saveData;
    QFile loadFile(fileName);
    QList<QColor> colorList;
    if(loadFile.open(QIODevice::ReadOnly))
    {
        saveData = loadFile.readAll();
    }
    else
        qWarning("Couldn't open save File");

    //convert the information to a QJsonDocument, and open it with QJsonObject
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject jsonFromLoadFile = loadDoc.object();

    int frameCount = 0; //starting number of frames
    int numberOfFrames = 0;
    if(jsonFromLoadFile.contains("height") && jsonFromLoadFile["height"].isDouble() && jsonFromLoadFile.contains("width") && jsonFromLoadFile["width"].isDouble())
    {
        double somevalue = jsonFromLoadFile["height"].toDouble();
        double othervalue = jsonFromLoadFile["width"].toDouble();
        if(somevalue == othervalue)
        {
            canvasHeight = somevalue;
            canvasWidth = somevalue; //canvasSize = somevalue; //uncomment this code
        }
        else
        {
            qWarning("File size is not valid, needs to be a perfect square"); //TODO, I need to stop the upload if the canvas size isn't a perfect square.
        }

    }
    if(jsonFromLoadFile.contains("numberOfFrames") && jsonFromLoadFile["numberOfFrames"].isDouble())
    {
        numberOfFrames = jsonFromLoadFile["numberOfFrames"].toDouble();
    }

    if(jsonFromLoadFile.contains("frames") && jsonFromLoadFile["frames"].isObject())
    {
        QJsonObject frame = jsonFromLoadFile["frames"].toObject();
        while(frameCount < numberOfFrames)
        {
            if(frame.contains("Frame"+ QString::number(frameCount)) && frame["Frame" + QString::number(frameCount)].isArray())
            {
                QJsonArray rowArray = frame["Frame" + QString::number(frameCount)].toArray();
                for(QJsonValue value: rowArray)
                {
                    QJsonArray pixelArray = value.toArray();
                    for(QJsonValue valueP : pixelArray)
                    {
                        QJsonArray colorArray = valueP.toArray();
                        QColor color;
                        int cIndex =0;
                        for(QJsonValue colorVal : colorArray)
                        {
                            int colorValues = colorVal.toDouble();
                            if(cIndex == 0)
                                color.setRed(colorValues);
                            else if(cIndex ==1)
                                color.setGreen(colorValues);
                            else if(cIndex ==2)
                                color.setBlue(colorValues);
                            else
                                color.setAlpha(colorValues);
                            cIndex++;
                        }
                        colorList.append(color);
                    }
                }
                frameCount++;
            }
        }
    }

    //emptys our current qlist<Qimage> list in model
    if(!frames.isEmpty())
    {
        QList<QImage> empty;
        frames = empty;
    }

    //This is where we add everything to the model, such as : frames, canvassize
    int frameCounter =0;
    int colorCounter =0;
    while(frameCounter < numberOfFrames)
    {
        frames.append(QImage(canvasHeight,canvasHeight,QImage::Format_ARGB32));
        frames[frameCounter].fill(Qt::white);

        for(int y=0; y < canvasHeight; y++)
        {
            for(int x=0; x <canvasHeight; x++)
            {
                frames[frameCounter].setPixelColor(x,y,colorList[colorCounter]);
                colorCounter++;
            }
        }
        frameCounter++;
    }

    //update in the view : TODO
}
