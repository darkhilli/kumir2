/*
This file is generated, but you can safely change it
until you run "gen_actor_source.py" with "--project" flag.

Generated file is just a skeleton for module contents. 
You should change it corresponding to functionality.
*/

#ifndef ROBOTMODULE_H
#define ROBOTMODULE_H

#include <QtCore>
#include <QtGui>

#include <kumir2-libs/extensionsystem/kplugin.h>
#include "robotmodulebase.h"
#include "pult.h"

namespace ActorRobot {

    
#define NORMAL_MODE 0
#define NEDIT_MODE 1
#define TEMP_MODE 2
#define RAD_MODE 3
#define TEXT_MODE 4

    
    
#define LEFT_WALL 	0x1
#define RIGHT_WALL 	0x2
#define DOWN_WALL 	0x4
#define UP_WALL   	0x8
#define MAX_CLICK_DELTA 7
    
#define NO_CRASH 0
#define UP_CRASH 1
#define DOWN_CRASH 2
#define LEFT_CRASH 3
#define RIGHT_CRASH 4

    //Robot
    //#define FIELD_SIZE 20
#define FIELD_SIZE_SMALL 33
#define BUTTON_SIZE 20
    
#define NOT_LEFT_WALL   14
#define NOT_RIGHT_WALL   13
#define NOT_DOWN_WALL   11
#define NOT_UP_WALL   7
#define DEFAULT_X 16
#define DEFAULT_Y 9
#define BORT 5
#define MAX_SIZE 256
#define MIN_SIZE 3
    // NEW V.K. 05-10
#define MIN_TEMP -273 
#define DPANEL_HIGTH 0
    //#define PANEL_HIGTH 0
#define BTNXSDVIG 0
#define SEE_MODE 0
#define RUN_MODE 1
#define EDIT_MODE 2
#define ANALYZE_MODE 3
#define PAUSE_MODE 4
#define DEFAULT_SIZEX 400
#define DEFAULT_SIZEY 400    
#define MAX_COLUMNS 255
#define MAX_ROWS 128
    
    class EditLine;
    class RobotModule;

    class  CFieldItem
    {
     public:
        CFieldItem();
        void setWalls(int wallByte);
        
        
        bool IsColored;
        float radiation;
        float temperature;
        QChar upChar;
        QChar downChar;
        bool mark;
        bool upWall;
        bool downWall;
        bool rightWall;
        bool leftWall;
        
    };
    
    class ConsoleField
    {
     public:
        ConsoleField(int w,int h);
        void createField(int w,int h);
        CFieldItem* getItem(int row,int col);
        CFieldItem* getCurItem(); //Item with robot
        bool goLeft();
        bool goRight();
        bool goUp();
        bool goDown();
        bool isUpWall();
        bool isDownWall();
        bool isLeftWall();
        bool isRightWall();
        
        int  Columns(){return rows.at(0).count();}
        int  Rows(){return rows.count();}
        int  robotRow(){return roboRow;}
        int  robotCol(){return roboCol;}
        int loadFromFile(QString filename);
        int consoleLoadFromDataStream(QIODevice *stream);
    
    private:
        QList< QList<CFieldItem*> > rows;
        int roboRow;
        int roboCol;
    };
    
    class SimpleRobot:
    public QGraphicsObject
    {
        Q_OBJECT;
        
    public:
        SimpleRobot(QGraphicsItem *parent = 0);
        QRectF boundingRect() const;
        
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);
        QGraphicsPolygonItem* RoboItem();
        void setCrash(uint dirct);
        void move(QPoint point);
        bool isMoving()
        {
            return moving;
        }
        void  setMoving(bool flag)
        {
             moving=flag;
        }
        //void show();
    signals:
        void moved(QPointF point);
    protected:
        void dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
        void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
        void mousePressEvent ( QGraphicsSceneMouseEvent * event );
        void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
        void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    private:
        
        QGraphicsPolygonItem *Robot;
        bool ready,moving;
        uint crash;
        
    };    

    
    
    class FieldItm: public QGraphicsWidget
    {
        Q_OBJECT
    public:
        
        FieldItm(QGraphicsItem *parent = 0,QGraphicsScene* scene=0);
        
        
        ~FieldItm();
        FieldItm* Copy();
        void cleanSelf();
        
        
        void setLeftsepItem(FieldItm* ItmLeft);
        
        void setRightsepItem(FieldItm* ItmRight);
        
        void setUpsepItem(FieldItm* ItmUp);
        
        void setDownsepItem(FieldItm* ItmDown);
        void removeDownsepItem();
        void removeLeftsepItem();
        void removeRightsepItem();
        void setWalls(int wallByte);
        
        int wallByte();
        void showCharFld(qreal upLeftCornerX,qreal upLeftCornerY,int field_size);
        void hideCharFld();
        void setUpLine(QGraphicsLineItem* Line,QPen pen);
        
        void showCharMark(qreal upLeftCornerX,qreal upLeftCornerY,int size);
        void showUpChar(qreal upLeftCornerX,qreal upLeftCornerY,int size);
        void showDownChar(qreal upLeftCornerX,qreal upLeftCornerY,int size);
        void showRTItm(qreal upLeftCornerX, qreal upLeftCornerY, int size,int mode);
        void hideRTItm();
        
        void showMark(qreal upLeftCornerX,qreal upLeftCornerY,int size);
        
        void setColorRect(QGraphicsRectItem* Rect,QColor color);
        void setDownLine(QGraphicsLineItem* Line,QPen pen);
        void setLeftLine(QGraphicsLineItem* Line,QPen pen);
        void setRightLine(QGraphicsLineItem* Line,QPen pen);
        bool hasUpWall();
        bool hasDownWall();
        
        bool hasLeftWall();
        bool hasRightWall();
        
        bool canUp();
        bool canDown();
        bool canLeft();
        bool canRight();
        bool hasUpSep();
        bool hasDownSep();
        
        bool hasLeftSep();
        bool hasRightSep();
        QGraphicsLineItem* UpWallLine() const;
        QGraphicsLineItem* DownWallLine() const;
        QGraphicsLineItem* LeftWallLine() const ;
        QGraphicsLineItem* RightWallLine() const;
        bool isColored() const;
        
        void removeUpWall();
        
        void setUpWall(QGraphicsLineItem* Line,QPen pen);
        void setDownWall(QGraphicsLineItem* Line,QPen pen);
        void removeDownWall();
        
        void removeLeftWall();
        void removeRightWall();
        
        
        
        void setLeftWall(QGraphicsLineItem* Line,QPen pen);
        void setRightWall(QGraphicsLineItem* Line,QPen pen);
        
        void removeColor();
        
        void removeMark();
        
        void removeUpChar();
        
        void removeDownChar();
        bool emptyCell();
        void setScene(QGraphicsScene* scene);
        void wbWalls();
        void colorWalls();
        void setTextColor();        
    public:
        bool IsColored;
        float radiation;
        float temperature;
        QChar upChar;
        QChar downChar;
        bool mark;
        bool upWall;
        bool downWall;
        bool rightWall;
        bool leftWall;
    private:
        
        QFont font;
        
        
        QPen wallPen;
        QGraphicsScene* Scene;
        QGraphicsLineItem* upWallLine;
        QGraphicsLineItem* downWallLine;
        QGraphicsLineItem* leftWallLine;
        QGraphicsLineItem* rightWallLine;
        QGraphicsRectItem * ColorRect;
        
        QGraphicsTextItem * upCharItm;
        QGraphicsTextItem * downCharItm;
        QGraphicsTextItem * markItm;
        EditLine * radItm;
        EditLine * tempItm;
        QGraphicsRectItem * upCharFld;
        QGraphicsRectItem * downCharFld;
        
        QColor TextColor;
        
        FieldItm* sepItmUp;
        FieldItm* sepItmDown;
        FieldItm* sepItmLeft;
        FieldItm* sepItmRight;
        QFont charFnt;
        ExtensionSystem::SettingsPtr sett;
    };
    
    class RoboField: public QGraphicsScene
    {
        Q_OBJECT
    public:
        RoboField(QWidget *parent,RobotModule* actor);
        RoboField* Clone() const ;
        ~RoboField();
        
    public:
        void drawField(uint cellSize);//TODO Document
        void destroyField();
        void setTextEditMode(bool flag);
       // void setRadEditMode(bool flag);
        void redrawEditFields();
        void redrawRTFields();
        void destroyRobot();
        
    
        inline int rows() const { return Items.count(); }
 
        inline int columns() const { return Items.isEmpty()? 0 : Items.last().size();  }
        void drawNet();
        qreal m_height() const;
        qreal m_width() const;
        void setItem(FieldItm* item,uint str,uint stlb);
        void destroyNet();
        void destroyScene();
        void createField(int shirina,int visota);
        FieldItm* getFieldItem(int str,int stlb) const;
        void debug() const;
        void setColorFromSett();
        const int loadFromFile(const QString& fileName);
        const int loadFromDataStream(QIODevice * l_File);
        int saveToFile(QString fileName);
        void createRobot();
        void UpdateColors();
        void showCursorUp(int row,int col);
        void showCursorDown(int row,int col);   
        
        /**
         * –£–¥–∞–ª—è–µ—Ç / —Å—Ç–∞–≤–∏—Ç –≤–µ—Ä—Ö–Ω—é—é —Å—Ç–µ–Ω—É
         * @param row –†—è–¥
         * @param col —Å—Ç–æ–ª–±–µ—Ü
         *
         */
        void reverseUpWall(int row,int col);
        
        /**
         * –£–¥–∞–ª—è–µ—Ç / —Å—Ç–∞–≤–∏—Ç –Ω–∏–∂–Ω—é—é —Å—Ç–µ–Ω—É
         * @param row –†—è–¥
         * @param col —Å—Ç–æ–ª–±–µ—Ü
         *
         */
        void reverseDownWall(int row,int col);
        void reverseLeftWall(int row,int col);
        void reverseRightWall(int row,int col);
        
        
        
        void showUpWall(int row,int col);
        
        /**
         * –£–¥–∞–ª—è–µ—Ç / —Å—Ç–∞–≤–∏—Ç –Ω–∏–∂–Ω—é—é —Å—Ç–µ–Ω—É
         * @param row –†—è–¥
         * @param col —Å—Ç–æ–ª–±–µ—Ü
         *
         */
        void showDownWall(int row,int col);
        void showLeftWall(int row,int col);
        void showRightWall(int row,int col);
        
        
        void reverseColor(int row,int col);
        void reverseColorCurrent();
  
        void reverseMark(int row,int col);
        
        bool isEditMode() const
        {
            return mode>0;
        }
        
        inline QList<QList<FieldItm * > > FieldItems() { return Items; }
        void setFieldItems(QList<QList<FieldItm * > > FieldItems);
        
        QPointF roboPosF() const
        {
            if(robot)return robot->scenePos () ;
            return QPointF(0,0);
        };
        bool stepUp();
        bool stepDown();
        bool stepLeft();
        bool stepRight();
        void editField();
        void setMode(int Mode);//swich mode (show/hide controls etc.)
        void setModeFlag(int Mode)
        {
            mode=Mode; 
        }
      
        inline FieldItm * currentCell()const  { return getFieldItem(robo_y,robo_x); }
        inline FieldItm * cellAt(int x,int y)const  { return getFieldItem(x,y); }
        
        inline void setMarkMode(bool isColor) { markMode=isColor; }
        
        inline bool WasEdit() const { return wasEdit; }
        
        void setRoboPos(int roboX,int roboY);
        inline int robotX() const { return robo_x; }
        inline int robotY() const { return robo_y; }
        
        
        inline void dropWasEdit() { wasEdit=false; }
        SimpleRobot * robot;
        void wbMode();
        void colorMode();
         void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
        public
        
    slots:
        void roboMoved(QPointF pos);
        void cellDialogOk();
        void timerTic();
        void addRow();
        void remRow();
        void remCol();
        void addCol();
    signals:
        void MousePress(qreal x,qreal y, bool Flag);
       // void was_edit();
        
        
        
    private:
        
        void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
       
        void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
        void keyPressEvent ( QKeyEvent * keyEvent );
        void showButtons(bool yes);
        void createResizeButtons();
        QTimer * timer;
        QList<QList<FieldItm * > > Items;
        QList<QGraphicsLineItem*> setka;
        //QGraphicsView * scena;
        bool markMode,wasEdit;
        QColor LineColor,EditLineColor,WallColor,EditColor,NormalColor,Color;
        uint fieldSize;
        uint robo_x,robo_y;
        QWidget *Parent;
        QPen BortLine,StLine,showLine,WallLine;
       // CellDialog* cellDialog;
        bool WasEditFlag;
        //TOOLS
        QPoint upLeftCorner(int str,int stlb);
        bool pressed;
       // RobotModule *m_robot;
        ExtensionSystem::SettingsPtr sett;
        QRectF	oldRect;
        qreal perssX,pressY;
        QGraphicsLineItem* showWall,*keyCursor;
        QPair<int,int> old_cell,clickCell;
        bool pressD;
        QDoubleSpinBox * radSpinBox;
        QSpinBox * tempSpinBox;
        QGraphicsProxyWidget *wAddRow,*wAddCol,*wRemCol,*wRemRow;
        QToolButton *btnAddRow,*btnAddCol,*btnRemCol,*btnRemRow;
        int mode;
        RobotModule* Actor;
          
    };
  
    
    
    
    

    
    
    
    class RobotView:
    public QGraphicsView
    {
        Q_OBJECT
    public:
        RobotView(RoboField * roboField);
        void  FindRobot();
        void showButtons(bool flag);
        QSize	sizeHint () const;
        void setField (RoboField* field)
        {
            robotField=field;
        }
        void setWindowSize(const QSize newGeometry);
    public slots:
        void handleDocked();
        void changeEditMode(bool state);
        void setDock(bool);
        void reloadSett(ExtensionSystem::SettingsPtr settings);
      signals:
        void resizeRequest(const QSize &newGeometry);
    protected:
        void mousePressEvent ( QMouseEvent * event );
        void mouseReleaseEvent ( QMouseEvent * event );
        void mouseMoveEvent ( QMouseEvent * event );
        void wheelEvent ( QWheelEvent * event );
        void showEvent ( QShowEvent * event );
    private:
        bool pressed,inDock,firstShow;
        int pressX,pressY;
        RoboField* robotField;
        QToolButton * textEditBtn;
        QToolButton * radEditBtn;
        QToolButton * tmpEditBtn;
        double c_scale;
        int CurCellSize;
        
    };
    
    class RobotModule
	: public RobotModuleBase
    {
        Q_OBJECT
    public:
        // Constructor
        RobotModule(ExtensionSystem::KPlugin * parent);
        static RobotModule * self;
        QSize minimumSize()const;
        
        public slots:
        // Reset actor state before program starts
        void reset();
        inline void terminateEvaluation() {}
        void loadActorData(QIODevice * source);
        // Set animation flag
  
        void setAnimationEnabled(bool enabled);
        static QList<ExtensionSystem::CommandLineParameter> acceptableCommandLineParameters();
        QString initialize(const QStringList &configurationParameters, const ExtensionSystem::CommandLine & runtimeParameters);
        // Actor methods
        void runGoUp();
        void runGoDown();
        void runGoLeft();
        void runGoRight();
        void runDoPaint();
        bool runIsWallAtTop();
        bool runIsWallAtBottom();
        bool runIsWallAtLeft();
        bool runIsWallAtRight();
        bool runIsFreeAtTop();
        bool runIsFreeAtBottom();
        bool runIsFreeAtLeft();
        bool runIsFreeAtRight();
        bool runIsColor();
        bool runIsClear();
        qreal runRadiation();
        int runTemperature();
        void runFieldSize(int& rows, int& cols);
        bool runMark(const int row, const int col) ;
        bool runColored(const int row, const int col);
        void runRobotPos(int& row, int& col);
        int runCellTemp(const int row, const int col);
        qreal runCellRad(const int row, const int col);
        QChar runUpChar(const int row, const int col);
        QChar runDownChar(const int row, const int col);
        bool isSafeToQuit();

    public:
        // GUI access methods
        QWidget* mainWidget() const;
        QWidget* pultWidget() const;
        static ExtensionSystem::SettingsPtr robotSettings();
   public slots:        
        void reloadSettings(ExtensionSystem::SettingsPtr settings, const QStringList & keys);
        void changeGlobalState(ExtensionSystem::GlobalState old, ExtensionSystem::GlobalState current);
        void loadEnv();
        void resetEnv();
        void save2png();
        void saveEnv();
        void editEnv();
        void newEnv();
        void createNewField();
        void createRescentMenu();
        void updateLastFiles(const QString newFile );
        void openRecent();
        void setWindowSize();
        void copyFromPult(QString log);
        void getTimer();
        
    private:
        void createGui();
        void updateRobot();
        void createEmptyField(int rows,int cols);
        int LoadFromFile(QString p_FileName);
        int SaveToFile(QString p_FileName);
        QWidget* m_mainWidget;
       // class QDeclarativeView * m_pultWidget;
        RoboPult* m_pultWidget;
        RoboField* field;
        RoboField * startField;
        bool animation;
        RobotView * view;
        QString curDir,curPDir;
        bool pressed;
        bool inDock;
        QPushButton * btnOK1;
        QPushButton * btnCancel1; //Кнопки диалога новая обстановка
        QSpinBox *eXSizeEdit;
        QSpinBox *eYSizeEdit;
        QWidget* NewWindow;
        QMenu * rescentMenu;
        void prepareNewWindow();
        int CurCellSize;
        bool DISPLAY;
        ExtensionSystem::SettingsPtr curSettings;
        ConsoleField* curConsoleField;
        QMutex mutex;
        ExtensionSystem::GlobalState currentState;
        QTimer *redrawTimer;
        
    signals:
        void sendToPultLog(const QVariant &);
      
    }; // RobotModule

    class EditLine:public QGraphicsObject
    {
        Q_OBJECT;
    public:
        EditLine(QGraphicsItem *parent = 0);
        QRectF boundingRect() const;
        bool isTemp()
        {
            return istemp;
        }
        bool isRad()
        {
            return !istemp;
        }
        void setRad()
        {
            istemp=false;

            iconPath=QUrl::fromLocalFile(RobotModule::self->myResourcesDir()
                                         .absoluteFilePath("btn_radiation.png")
                                         );
           // rad=QImage(iconPath.toLocalFile ());

           // rad.load(iconPath.toLocalFile ());
        }
        void setTemp()
        {
            istemp=true;
            iconPath=QUrl::fromLocalFile(RobotModule::self->myResourcesDir()
                                         .absoluteFilePath("temp.png")
                                         );
            rad=QImage(iconPath.toLocalFile ());

           // rad.load(iconPath.toLocalFile ());
        }
        void setValue(float value)
        {
            Value=value;
        }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);
        private:
        QUrl iconPath;
        float Value;
        bool istemp;
        QImage rad;
        QPicture radiation,temp;
    };

} // ActorRobot

#endif // ROBOTMODULE_H
