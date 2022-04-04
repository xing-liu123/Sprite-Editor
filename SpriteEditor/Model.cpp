#include "model.h"
#include "qtimer.h"

model::model(QObject *parent)
    : QObject{parent}
{

    penColor.setRgb(0, 0, 0, 255);

    zoomIndex = 0;
    ratio = 400/20; //change to have it initialized
    framesPerSec = 1;
    currentFrame = 1;
    penSize = 1;
    eraserSize = 1;
    currentIndex = 0; //for keeping track of our current index in the list when displaying a sprite.
    canvasSize = 0; //Initialize with 0
    currentTool == SelectedTool::Undefined;

}


void model::initializeCanvasSize(int index){
    switch(index){
    case 0:
        canvasSize = 2;
        zoomSize = 2;
        break;
    case 1:
        canvasSize = 4;
        zoomSize = 4;
        break;
    case 2:
        canvasSize = 5;
        zoomSize = 5;
        break;
    case 3:
        canvasSize = 8;
        zoomSize = 8;
        break;
    case 4:
        canvasSize = 10;
        zoomSize = 10;
        break;
    case 5:
        canvasSize = 16;
        zoomSize = 16;
        break;
    case 6:
        canvasSize = 20;
        zoomSize = 20;
        break;
    case 7:
        canvasSize = 25;
        zoomSize = 25;
        break;
    case 8:
        canvasSize = 40;
        zoomSize = 40;
        break;
    case 9:
        canvasSize = 50;
        zoomSize = 50;
        break;
    case 10:
        canvasSize = 80;
        zoomSize = 80;
        break;
    case 11:
        canvasSize = 100;
        zoomSize = 100;
        break;
    case 12:
        canvasSize = 200;
        zoomSize = 200;
        break;
    case 13:
        canvasSize = 400;
        zoomSize = 400;
        break;
    }

    initializeFrame();
}


void model::initializeFrame(){

    //Set the canvas Size
    if(canvasSize == 0){
        //Default Values
        canvasSize = 20;
    }
    zoomSize = canvasSize;
    //Set the tool
    if(currentTool == SelectedTool::Undefined)
        currentTool = SelectedTool::SC_Pen;

    //set up default values
    frames.append(QImage (canvasSize, canvasSize, QImage::Format_ARGB32));

    //Make the first inage to have a white background
    frames[currentFrame-1].fill(Qt::white);
    undoStack.push(frames);
    emit startButtons();

}


void model::initializeShapeTool(int index)
{
    currentTool = SelectedTool::SC_ShapeCreator;
    switch(index){
    case 0:
        currentShape = ShapeCreator::SC_Line;
        break;
    case 1:
        currentShape = ShapeCreator::SC_Ciecle;
        break;
    case 2:
        currentShape = ShapeCreator::SC_Rectangle;
        break;
    }

}



// Add a new frame to the position next to the current frame
void model::addNewFrame(){
    QImage frame(canvasSize, canvasSize, QImage::Format_ARGB32);
    frame.fill(Qt::white);
    frames.insert(currentFrame, frame);

    // move to the new frame
    currentFrame++;

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());
    emit enableDeleteButton();
    emit enableLastButton();
    emit enableSwapUp();

    emit enableUndo();
    undoStack.push(frames);

    // if the new frame is at the end of the list, disale next button
    if(currentFrame == frames.size()){
        emit disableNextButton();
    }

    //frames[currentFrame-1].fill(Qt::white);
    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit setCanvas(map);
}

// insert a new frame to the position before current frame
void model::insertNewFrame(){
    QImage frame(canvasSize, canvasSize, QImage::Format_ARGB32);
    frame.fill(Qt::white);
    frames.insert(currentFrame - 1, frame);

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());
    emit enableDeleteButton();
    emit enableNextButton();
    emit enableSwapDown();

    emit enableUndo();
    undoStack.push(frames);

    if(currentFrame == 1){
        emit disableLastButton();
    }

    //frames[currentFrame-1].fill(Qt::white);
    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit setCanvas(map);
}

void model::nextFrame(){
    emit updateFrameNumberCombo(++currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());

    emit enableLastButton();
    emit enableSwapUp();

    if(currentFrame == frames.size()){
        emit disableNextButton();
        emit disableSwapDown();
    }

     QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
     emit setCanvas(map);
}

void model::lastFrame(){
    emit updateFrameNumberCombo(--currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());

    emit enableNextButton();
    emit enableSwapDown();

    if(currentFrame == 1){
        emit disableLastButton();
        emit disableSwapUp();
    }

    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit setCanvas(map);
}

void model::deleteFrame(){
    frames.removeAt(currentFrame - 1);

    if(frames.size() == 1){
        emit disableNextButton();
        emit disableLastButton();
        emit disableDeleteButton();
        emit disableSwapDown();
        emit disableSwapUp();
    }

    // if the last frame is deleted
    if(currentFrame - 1 == frames.size()){
        currentFrame--;
        emit disableNextButton();
        emit disableSwapDown();
    }

    emit enableUndo();
    undoStack.push(frames);

    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit setCanvas(map);

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());
}

void model::swapUp(){
    frames.swapItemsAt(currentFrame - 1, currentFrame - 2);
    currentFrame--;

    undoStack.push(frames);
    emit enableUndo();

    emit enableNextButton();
    emit enableSwapDown();

    if(currentFrame == 1){
        emit disableLastButton();
        emit disableSwapUp();
    }

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());

    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit setCanvas(map);
}

void model::swapDown(){
    frames.swapItemsAt(currentFrame - 1, currentFrame);
    currentFrame++;

    undoStack.push(frames);
    emit enableUndo();

    emit enableLastButton();
    emit enableSwapUp();

    if(currentFrame == frames.size()){
        emit disableNextButton();
        emit disableSwapDown();
    }

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());

    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit setCanvas(map);
}

void model::clearCanvas(){
    QImage frame(canvasSize, canvasSize, QImage::Format_ARGB32);
    frame.fill(Qt::white);
    frames.replace(currentFrame - 1, frame);

    emit enableUndo();
    undoStack.push(frames);

    QPixmap map = QPixmap::fromImage(frame);
    emit setCanvas(map); 
}

void model::copyFrame(){
    QImage frame = QImage(frames.at(currentFrame - 1));
    frames.insert(currentFrame++, frame);

    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit updateFrameNumberLabel(currentFrame, frames.size());
    emit enableDeleteButton();
    emit enableSwapUp();
    emit enableLastButton();
    emit enableUndo();
    undoStack.push(frames);

    //frames[currentFrame-1].fill(Qt::white);
    QPixmap map = QPixmap::fromImage(frame);
    emit setCanvas(map);
}

//Need to implement the change of pixels to edit
//This method increases the size of the image, and sends it back to the
//view  to be displayed in the canvas
void model::zoomIn(){

    //Check if it is within bounds
    if(zoomSize > 2 ){
        //canvas height and width changes (the display will be different)
        zoomSize = zoomSize -2;
        zoomIndex++;

        QPixmap currentPic;
        //Convert QImage to QPixmap
        currentPic.convertFromImage(frames[currentFrame-1]); //maybe resize?
        //Return the pixmap of our QImage with the scaled version
        emit toZoomIn(currentPic, zoomSize, zoomIndex);
        emit enableZoomOut();
    }
    else
        emit disableZoom("zoomIn");

}


void model::zoomOut(){

    //Check if is within bounds
    if(zoomSize < canvasSize){
        zoomSize = zoomSize +2;
        zoomIndex--;

        QPixmap currentPic;
        //Convert QImage to QPixmap
        currentPic.convertFromImage(frames[currentFrame-1]);
        //Return the pixmap of our QImage with the scaled version
        emit toZoomOut(currentPic, zoomSize, zoomIndex);
        emit enableZoomIn();
    }
    else
        emit disableZoom("zoomOut");

}



void model::updateFPS(int fps){
    framesPerSec = fps;
}

void model::updatePenSize(int size){
    penSize = size;
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
    redoStack.push(undoStack.pop());
    emit enableRedo();
    if(undoStack.size() == 1){
        emit disableUndo();
    }


    QList<QImage> previousFrames = undoStack.at(undoStack.size() - 1);

    if(previousFrames.size() > frames.size()){
        currentFrame++;
    }

    if(frames.size() > previousFrames.size() && currentFrame == previousFrames.size() + 1){
        currentFrame--;
    }

    frames = previousFrames;

    if(frames.size() == 1){
        emit disableDeleteButton();
    }
    else{
        emit enableDeleteButton();
    }


    if(frames.size() > currentFrame){
        emit enableNextButton();
        emit enableSwapDown();
    }
    else if(frames.size() == currentFrame){
        emit disableNextButton();
        emit disableSwapDown();
    }

    if(currentFrame > 1){
        emit enableLastButton();
        emit enableSwapUp();
    }
    else{
        emit disableLastButton();
        emit disableSwapUp();
        emit disableSwapDown();
    }

    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit updateFrameNumberLabel(currentFrame, frames.size());
    emit updateFrameNumberCombo(currentFrame, frames.size());
    emit setCanvas(map);
}

//need change parameters?
void model::redo(){
    QList<QImage> previousFrame = redoStack.pop();

    if(previousFrame.size() > frames.size()){
        currentFrame++;
        emit disableNextButton();
        emit disableSwapDown();
    }
    frames = previousFrame;

    undoStack.push(frames);

    emit enableUndo();


    if(frames.size() == currentFrame){
        emit disableNextButton();
        emit disableSwapDown();
    }
    else if (frames.size() < currentFrame){
        currentFrame = frames.size();
        emit disableSwapDown();
    }

    if(frames.size() == 1){
        emit disableDeleteButton();
        emit disableSwapUp();
        emit disableSwapDown();
    }
    else{
        emit enableDeleteButton();
    }

    if(redoStack.size() == 0){
        emit disableRedo();
    }

    if(currentFrame > 1){
        emit enableLastButton();
        emit enableSwapUp();
    }
    else{
        emit disableLastButton();
    }

    emit updateFrameNumberLabel(currentFrame, frames.size());
    emit updateFrameNumberCombo(currentFrame, frames.size());
    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit setCanvas(map);
}

void model::saveFrameToStack(){
    emit enableUndo();
    undoStack.push(frames);
}

void model::mouseRelease(QPoint &loc)
{
    if(startEndLoc.size() ==1 ){
    startEndLoc.insert(1,loc);
    }
    switch(currentTool){
        case SelectedTool::SC_ShapeCreator: // might need to delete
                if(startEndLoc.size() ==2 ){
                        updatePixelsByShapeCreator((int) (startEndLoc[0].x()/ratio) + zoomIndex,
                                (int) (startEndLoc[0].y()/ratio) + zoomIndex,
                                (int) (startEndLoc[1].x()/ratio) + zoomIndex,
                                (int) (startEndLoc[1].y()/ratio) + zoomIndex);
                 }
        break;
    }
}

//Frame that we are currently in
void model::selectedFrame(int index){
    currentFrame = index + 1;

    QPixmap map = QPixmap::fromImage(frames.at(currentFrame - 1));
    emit setCanvas(map);
    emit updateFrameNumberLabel(currentFrame, frames.size());

    if(currentFrame == frames.size()){
        emit disableNextButton();
        emit disableSwapDown();
    }
    else if(currentFrame < frames.size()){
        emit enableNextButton();
        emit enableSwapDown();
    }

    if(currentFrame == 1){
        emit disableLastButton();
        emit disableSwapUp();
    }
    else if(currentFrame > 1){
        emit enableLastButton();
        emit enableSwapUp();
    }
}

//updates the toolsize, we first check our selected tool
void model::updateToolSize(int size){

    //change to a switch case if we add more brushes
    if(currentTool == SelectedTool::SC_Eraser)
        updateEraserSize(size);
    else if(currentTool == SelectedTool::SC_Pen)
        updatePenSize(size);
    else if(currentTool == SelectedTool::SC_ShapeCreator)
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
            updatePixelsByBucketFiller(initialX, initialY);
            break;
//        case SelectedTool::SC_ShapeCreator: // might need to delete
//            if(startEndLoc.size() == 2){
//                std::cout << "hit" << std::endl;

//                std::cout << startEndLoc.size() << std::endl;

//            updatePixelsByShapeCreator(initialX, initialY, endX, endY);
//            break;
          //  }
        default:
            break;
    }
}

void model::updatePixelsByPen(int x, int y){
    QImage* AFrame = &frames[currentFrame -1];
    QPainter Painter(AFrame);
    QPen Pen(penColor);
    Pen.setWidth(penSize);
    Painter.setPen(Pen);
    Painter.drawPoint(x,y);
    Painter.end();
    startEndLoc.clear();

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
            Painter.drawEllipse(initialX,initialY,endX-initialX,endY-initialY);
            Painter.end();
            break;
        case ShapeCreator::SC_Rectangle:
            Painter.drawRect(initialX,initialY,endX-initialX,endY-initialY);
            Painter.end();
            break;
        default:
            break;

    }
    startEndLoc.clear();
    QPixmap currentPic;
    //Convert QImage to QPixmap
    currentPic.convertFromImage(frames[currentFrame-1]);
    //Return the pixmap of our QImage
    emit setCanvas(currentPic);
}

void model::updatePixelsByBucketFiller(int x, int y){
    QColor colorToBeChanged = (frames[currentFrame -1]).pixelColor(x,y);

    QSize sizeOfFrame = frames[currentFrame-1].size();

    for(int pixelX=0; pixelX<sizeOfFrame.width(); pixelX++){
        for(int pixelY=0; pixelY<sizeOfFrame.height(); pixelY++){
            if(frames[currentFrame-1].pixelColor(pixelX, pixelY) == colorToBeChanged)
                frames[currentFrame-1].setPixelColor(pixelX, pixelY, penColor);
        }
    }
}


//This method obtains where the current position of the mouse is in our canvas
//then it optains the ratio
void model::drawOnCanvas(QPoint pixelPoint){
    //if there have been any changes on the canvas
    isChanged = true;

    //Check it it has been initialized with a size
    if(canvasSize == 0){
        initializeFrame();
    }

    //Get the position to paint
     ratio = 400/ zoomSize;
     int x = (pixelPoint.x()/ratio) + zoomIndex;
     int y = (pixelPoint.y()/ratio) + zoomIndex;

     if (startEndLoc.size() < 1){ // assures to get only one
         startEndLoc.insert(0,pixelPoint);
     }

     updatePixels(x,y);

    //Create a Pixmap to return to view
    QPixmap currentPic;
    //Convert QImage to QPixmap
    currentPic.convertFromImage(frames[currentFrame-1]);
    //Return the pixmap of our QImage
    emit setCanvas(currentPic);
}


void model::updateCanvasSize()
{
    canvasSize += 1;
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
    //Since the file is beening saved, we change this to false
    isChanged = false;

    QJsonObject json;

    int n = 0;
    json["height"] = canvasSize;
    json["width"] = canvasSize;
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
        for(int rowNum = 0 ; rowNum < canvasSize; rowNum++)
        {
            QJsonArray row;
            for(int pixelNum = 0; pixelNum < canvasSize; pixelNum++)
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
        double height = jsonFromLoadFile["height"].toDouble();
        double width = jsonFromLoadFile["width"].toDouble();
        if(height == width)
        {
            canvasSize = width;
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

    //Select a tool and initialize frame
    currentTool = SelectedTool::Undefined;
    initializeFrame();

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
    int frameCounter = 0;
    int colorCounter = 0;
    while(frameCounter < numberOfFrames)
    {
        frames.append(QImage(canvasSize,canvasSize,QImage::Format_ARGB32));
        frames[frameCounter].fill(Qt::white);

        for(int y=0; y < canvasSize; y++)
        {
            for(int x=0; x <canvasSize; x++)
            {
                frames[frameCounter].setPixelColor(x,y,colorList[colorCounter]);
                colorCounter++;
            }
        }

        frameCounter++;
        emit updateFrameNumberCombo(frameCounter, frames.size());
    }

    undoStack.push(frames);
    //update in the view :
    emit updateFrameNumberLabel(1, frames.size());
    emit updateFrameNumberCombo(1,frames.size());

    //should we make this a helper method? (same method was used in drawCanvas method)
    //Create a Pixmap to return to view
    QPixmap currentPic;
    //Convert QImage to QPixmap
    currentPic.convertFromImage(frames[0]);
    //Return the pixmap of our QImage
    emit setCanvas(currentPic);

    //enables the next button and swapdown button
    if(frames.size() > 1)
    {
        emit enableNextButton();
        emit enableSwapDown();

    }

}
