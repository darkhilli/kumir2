//****************************************************************************
//**
//** Copyright (C) 2004-2008 IMPB RAS. All rights reserved.
//**
//** This file is part of the KuMir.
//**
//** This file may be used under the terms of the GNU General Public
//** License version 2.0 as published by the Free Software Foundation
//** and appearing in the file LICENSE.GPL included in the packaging of
//** this file.
//**
//** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
//** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//**
//****************************************************************************/


#include <QtGui>
#include "vodoley.h"
#include "pult.h"
#include <QSizePolicy>
#include <QtGlobal>
#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

        static const double Pi = 3.14159265358979323846264338327950288419717;

Menzurka::Menzurka(int x,int y,uint size,float lsize,QMutex* mutex)
{
    SizeInLiters=size;
    literSize=lsize;
    offsetX=x+5;
    offsetY=y;
    Gp=23;
    GpY=4;
    curFil=1;
    needFill=-1;
    M=mutex;

};

void Menzurka::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{

	Q_UNUSED(option); Q_UNUSED(widget);
	M->lock();
	//qDebug()<<"Repaint menzurka "<<Gp<<" curFil"<<curFil;
 	QBrush black(Qt::black);
    float size=(literSize*SizeInLiters);
	// qDebug()<<"Repaint menzurka "<<Gp<<" gpy"<<GpY <<" size"<<size;
	QPointF points[4] = {
		QPointF(offsetX,offsetY+size-3),
		QPointF(offsetX-3,offsetY+size+2),
		QPointF(offsetX+X_SIZE+3,offsetY+size+2),
		QPointF(offsetX+X_SIZE, offsetY+size-3)
 	};



    painter->setBrush(black);
    painter->drawConvexPolygon(points, 4);


    QLinearGradient glassGradient(offsetX,offsetY,offsetX+X_SIZE,offsetY+size);
    QColor niceBlue(142, 140+26, 180+25);
    QColor semiWhite(230, 230, 250);

    glassGradient.setColorAt(0.0, Qt::white);
    glassGradient.setColorAt(23/20, niceBlue);
    glassGradient.setColorAt(4/20, semiWhite);
    // glassGradient.setColorAt(0.4, Qt::white);

    //Menzurka
    painter->setBrush(glassGradient);
    painter->drawRoundedRect(offsetX, offsetY-5,X_SIZE,size+5, 5, 5);
    //Birka
    QString text=QString::number(curFil);
    QPointF birka[5] = {
        QPointF(offsetX+X_SIZE+3,offsetY+size-literSize*curFil),
        QPointF(offsetX+X_SIZE+11,offsetY+size-literSize*curFil+8),
        QPointF(offsetX+X_SIZE+17+text.length()*10,offsetY+size-literSize*curFil+8),
        QPointF(offsetX+X_SIZE+17+text.length()*10,offsetY+size-literSize*curFil-8),
        QPointF(offsetX+X_SIZE+11,offsetY+size-literSize*curFil-8)
 	};
    painter->drawConvexPolygon(birka, 5);
    painter->drawText(offsetX+X_SIZE+13,offsetY+size-literSize*curFil+7,text);//Birka text
	QString sizeT=QString::number(SizeInLiters);
	painter->drawText(offsetX+X_SIZE/2-sizeT.length()*3,offsetY+size+20,sizeT);
	
  	//Metki
    if ( ((uint)needFill>0)&&((uint)needFill<= SizeInLiters))
    {


        painter->setPen(QColor(20,237,17));
        painter->drawLine(offsetX+1,offsetY+size-literSize*needFill,offsetX+X_SIZE/3,offsetY+size-literSize*needFill);
        //QString sizeN=QString::number(needFill);
        //painter->drawText(offsetX-sizeN.length()*10,offsetY+size-literSize*needFill+7,sizeN);
        //QBrush gold(QColor(2,237,17));
        painter->setPen(QColor(150,150,150));
        painter->drawLine(offsetX+X_SIZE*3/4,offsetY+size-literSize*SizeInLiters,offsetX+X_SIZE-1,offsetY+size-literSize*SizeInLiters);
	};
	painter->setPen(QColor(0,0,0));


 	//VODA
    if(curFil>0){
        QLinearGradient waterGradient(offsetX+1, offsetY+size-literSize*curFil,offsetX+X_SIZE-2,offsetY+size);
        QColor W1(78, 127, 248);
        QColor W2(18, 173, 253);
        qDebug()<<"cur"<<curFil<<" need"<<needFill;
        if(curFil==(uint)needFill)
		{
			qDebug()<<"OK!!!!!!";
			W1.setRgb(250,250,35);
			W1.setRgb(210,230,10);
		};
		//waterGradient.setColorAt(0.0, Qt::);
		waterGradient.setColorAt(0.0, W1);
        // waterGradient.setColorAt(0.3, W2);
        if(curFil==(uint)needFill)waterGradient.setColorAt(1, W2);else waterGradient.setColorAt(0.3, W2);
        painter->setBrush(waterGradient);
        painter->setOpacity(0.5);
        painter->drawRoundedRect(offsetX, offsetY+size-literSize*curFil,X_SIZE,literSize*curFil, 2, 2);
    };


	//painter->drawLine(offsetX+1, offsetY+size-literSize*curFil,offsetX+X_SIZE-2,offsetY+size);
    M->unlock();
};

Vodoley::Vodoley()
{
//	vodHeader=new WHeader();
	//createActions();
	autoClose=false;
    pult=new VodoleyPult();
    pult->VodoleyObj=this;


	scene = new QGraphicsScene(this);
	view=new QGraphicsView(this);
        view->setSceneRect(0,0,354,220);
        view->resize(370,245);

	//Asize()=80;Bsize()=7;Csize()=9; //Размер емкости
	Maxfill.append(8);
	Maxfill.append(5);
	Maxfill.append(3);


	Afill=0;Bfill=0;Cfill=0;//Сколько  налито изначально
	Curfill.append(Afill);//Сколько сейчас налито
	Curfill.append(Bfill);//Сколько сейчас налито
	Curfill.append(Cfill);//Сколько сейчас налито


	AfillR=4,BfillR=4,CfillR=5;


	//	view->resize(500,500);
	view->setScene(scene);
	setCentralWidget(view);
//	view->move(0,menuBar->height ()+HEADER_SIZE);
	//qDebug()<<menuBar->height ();
        this->setBaseSize (364,235);
        this->resize(160,145+1);



	this->setMinimumSize(this->size());
	//this->setMaximumSize(this->size());
    this->setSizePolicy(QSizePolicy::Expanding,
                        QSizePolicy::Expanding);
	this->statusBar()->hide();
//	vodHeader->setChildWidget(this);


	scene->setBackgroundBrush(QBrush(QColor(175,200,203)));


	ang=0;
	step=30;
	tail=true;
	curX=0;
	curY=0;

	zoom=0.2;
	setWindowTitle(QString::fromUtf8("Водолей"));
//	vodHeader->setWMTitle(QString::fromUtf8("Водолей"));

	delta=100*zoom;

	QPen redPen(QColor(250,10,10));

	QSettings vSett("NIISI RAS","Vodoley");
	curDir=vSett.value("Dir",QApplication::applicationDirPath()+"/Addons/vodoley/resources/").toString();
	QString curFile=vSett.value("File","").toString();
	qDebug()<<"curFile:"<<curFile;
	//	drawTail();

	needFrame=new QFrame(view);
	needFrame->setGeometry ( 165, 5, 40, 25 );
	needFrame->setFrameShadow(QFrame::Sunken);
	needFrame->setFrameShape(QFrame::Panel);
	needFrame->setPalette(QPalette(QColor(140,140,140),QColor(100,100,100)));
	needFrame->setBackgroundRole(QPalette::Window);
	needFrame->setAutoFillBackground(true);
	needFrame->show();
	needLabel=new QLabel(needFrame);
	needLabel->setText("999");
	needLabel->move(2,2);
	needLabel->setGeometry(1,5,39,24);
	needLabel->setAlignment(Qt::AlignHCenter);
	needLabel->show();
	CreateVodoley();
      CreateDummyTask();
	if(curFile!="" && QFile::exists(curFile)){
		//loadFile(curFile);
    }
	else {
        CreateDummyTask();
#ifndef Q_OS_MAC
		//loadFile(QApplication::applicationDirPath()+"/Addons/vodoley/resources/test.vod");
#endif
#ifdef Q_OS_MAC
		//loadFile(QApplication::applicationDirPath()+"/../Resources/Addons/vodoley/resources/test.vod");
#endif
	}
}
//--------------------------------
void Vodoley::CreateDummyTask()
{
    Maxfill[0]=3;
    Maxfill[1]=5;
    Maxfill[2]=7;
    qDebug()<<"ASize"<<Asize();
    
    
    
    AfillR=1;
    //  BfillR=newZdialog->BNeed();
    //CfillR=newZdialog->CNeed();
    
    Amen->setNeedFill(1);
    Bmen->setNeedFill(1);
    Cmen->setNeedFill(1);
    
    setBaseFill(0,0,0);
    Afill=0;Bfill=0;Cfill=0;
    Curfill[0]=0;
    Curfill[1]=0;
    Curfill[2]=0;
    updateMenzur();
 
}
//-----------------------------------
void Vodoley::CreateVodoley(void)
{
    float literSize = MAX_SIZE/maxSize();
    qDebug()<<"Liter Size:"<<literSize;
    Amen=new Menzurka(30,30+(maxSize()-Asize())*literSize,Asize(),literSize,&mutex);
    Bmen=new Menzurka(140,30+(maxSize()-Bsize())*literSize,Bsize(),literSize,&mutex);
    Cmen=new Menzurka(250,30+(maxSize()-Csize())*literSize,Csize(),literSize,&mutex);

    Amen->setCurFill(CurA());
    Bmen->setCurFill(CurB());
    Cmen->setCurFill(CurC());

    Amen->setNeedFill(AfillR);
    Bmen->setNeedFill(BfillR);
    Cmen->setNeedFill(CfillR);


    qDebug()<<CurB();
    scene->addItem(Amen);
    scene->addItem(Bmen);
    scene->addItem(Cmen);

    Atext=scene->addSimpleText("A");
    Atext->setPos(35,15+(maxSize()-Asize())*literSize);
    //Atext->hide();
    Btext=scene->addSimpleText("B");
    Btext->setPos(145,15+(maxSize()-Bsize())*literSize);
    Ctext=scene->addSimpleText("C");
    Ctext->setPos(255,15+(maxSize()-Csize())*literSize);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Amen->move(20,20);
    
}

//-----------------------------------
void Vodoley::createActions(QList<QAction*> actions)
{
    if(actions.count()<3){qDebug()<<"Vodoley: cant create actions!";return;}
    
    actNew=actions.at(0);
    connect(actNew, SIGNAL(triggered()), this, SLOT(newZ()));
    actLoad=actions.at(1);
    connect(actLoad, SIGNAL(triggered()), this, SLOT(loadZ()));
    actSave=actions.at(2);
    connect(actSave, SIGNAL(triggered()), this, SLOT(saveZ()));
    

};

void Vodoley::rotateImages()
{




};

void Vodoley::resizeEvent ( QResizeEvent * event )
{
    this->setSizePolicy(QSizePolicy::Expanding,
                        QSizePolicy::Expanding);
    view->resize(event->size());
    QRectF sizeRect=QRectF(scene->sceneRect().topLeft(),QSize(qMax(scene->sceneRect().width(),scene->sceneRect().height())+43,qMax(scene->sceneRect().width(),scene->sceneRect().height())));
    view->fitInView(sizeRect, Qt::KeepAspectRatio);
    needFrame->move(view->width()/2-10,5);

    QWidget::resizeEvent(event);
}

void Vodoley::reset()
{
    mutex.tryLock(30);
    setBaseFill(Afill,Bfill,Cfill);
    mutex.unlock();
    updateMenzur();
};


void Vodoley::FillA()
{
    mutex.lock();
    Curfill[0]=Asize();
    mutex.unlock();
   // updateMenzur();
};
void Vodoley::FillB()
{    mutex.lock();
    Curfill[1]=Bsize();
    mutex.unlock();
   // updateMenzur();
};
void Vodoley::FillC()
{
    mutex.unlock();
    Curfill[2]=Csize();
    mutex.unlock();
    //updateMenzur();
};

void Vodoley::MoveFromTo(uint from,uint to)
{
    mutex.lock();
    if(to>2){Curfill[from]=0;mutex.unlock();updateMenzur();return;};//Выливаем
    int svobodno=Maxfill[to]-Curfill[to];
    Curfill[to]=Curfill[to]+Curfill[from];
    if(Curfill[to]>Maxfill[to])Curfill[to]=Maxfill[to];


    if ((uint)svobodno>Curfill[from])
        Curfill[from]=0;
    else
        Curfill[from]=Curfill[from]-svobodno;
    mutex.unlock();
   // updateMenzur();
    QApplication::processEvents();
    
};


Vodoley::~Vodoley()
{

}

void Vodoley::newZ()
{
    Dialog* newZdialog=new Dialog();
      
    newZdialog->setSizes(Asize(),Bsize(),Csize());
    newZdialog->setFill(Afill,Bfill,Cfill);
    newZdialog->setNeed(AfillR);

 	if(!newZdialog->exec())return;
    Maxfill[0]=newZdialog->ASize();
    Maxfill[1]=newZdialog->BSize();
    Maxfill[2]=newZdialog->CSize();




    AfillR=newZdialog->ANeed();
    //  BfillR=newZdialog->BNeed();
    //CfillR=newZdialog->CNeed();

    Amen->setNeedFill(AfillR);
    Bmen->setNeedFill(AfillR);
    Cmen->setNeedFill(AfillR);

    setBaseFill(newZdialog->AFill(),newZdialog->BFill(),newZdialog->CFill());
    Afill=newZdialog->AFill();Bfill=newZdialog->BFill();Cfill=newZdialog->CFill();
    updateMenzur();
//    vodHeader->setWMTitle(QString::fromUtf8("Водолей - новое"));
    setWindowTitle(QString::fromUtf8("Водолей - новое"));
};

bool Vodoley::loadIoDevice(QIODevice * source)
{
    QString curLine;
	bool size=false;
	bool fill=false;
	bool need=false;
    
	while(!source->atEnd()) //Читаем файл
	{
		curLine = source->readLine().simplified();
		if((curLine.isEmpty()) || (curLine[0]==';')){qDebug()<<"Continue";continue;};
        //		int A,B,C;
        
		QStringList data=curLine.split(" ");
		if(data.count()!=3)
			QMessageBox::information( 0, "", trUtf8("Ошибка чтения задания! "), 0,0,0);
		if(!size)//Читаем размер
		{
			bool ok;
			Maxfill[0]=data[0].toInt(&ok);
			if(!ok)
				QMessageBox::information( 0, "", trUtf8("Ошибка чтения задания: размер не верен!"), 0,0,0);
            
			Maxfill[1]=data[1].toInt(&ok);
            if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения задания: размер не верен! ") , 0,0,0);
			Maxfill[2]=data[2].toInt(&ok);
			if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения задания: размер не верен!"), 0,0,0);
			size=true;
			continue;
		};
        
		if(!fill)//Читаем наполненность
		{
			bool ok;
			Afill=data[0].toInt(&ok);
			if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения:наполненость A не верна! ") , 0,0,0);
			Bfill=data[1].toInt(&ok);
			if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения:наполненость B не верна!") , 0,0,0);
			Cfill=data[2].toInt(&ok);
			if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения:наполненость C не верна!") , 0,0,0);
			fill=true;
			continue;
		};
		if(!need)//Читаем задание
		{
			bool ok;
			AfillR=data[0].toInt(&ok);
			if(!ok){
				QMessageBox::information( 0, "", trUtf8("Ошибка чтения: необходимое количество!") , 0,0,0);
				return false;
			};
			BfillR=data[1].toInt(&ok);
			if(!ok){
				QMessageBox::information( 0, "", trUtf8("Ошибка чтения:необходимое количество! ") , 0,0,0);
				return false;
			};
			CfillR=data[2].toInt(&ok);
			if(!ok){
				QMessageBox::information( 0, "", trUtf8("Ошибка чтения: необходимое количество!") , 0,0,0);
				return false;
			};
			need=true;
			continue;
		};
	};

    return true;
};
bool Vodoley::loadFile(QString fileName)
{
    QFile VodFile(fileName);
 	if  (!VodFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);
        return false;
	};
	if(VodFile.atEnd())QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);

	QString curLine;
	bool size=false;
	bool fill=false;
	bool need=false;

	while(!VodFile.atEnd()) //Читаем файл
	{
		curLine = VodFile.readLine().simplified();
		if((curLine.isEmpty()) || (curLine[0]==';')){qDebug()<<"Continue";continue;};
//		int A,B,C;

		QStringList data=curLine.split(" ");
		if(data.count()!=3)
			QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);
		if(!size)//Читаем размер
		{
			bool ok;
			Maxfill[0]=data[0].toInt(&ok);
			if(!ok)
				QMessageBox::information( 0, "", trUtf8("Ошибка чтения размера: ") + fileName, 0,0,0);

			Maxfill[1]=data[1].toInt(&ok);
                        if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения размера:  ") + fileName, 0,0,0);
			Maxfill[2]=data[2].toInt(&ok);
			if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения размера:  ") + fileName, 0,0,0);
			size=true;
			continue;
		};

		if(!fill)//Читаем наполненость
		{
			bool ok;
			Afill=data[0].toInt(&ok);
			if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);
			Bfill=data[1].toInt(&ok);
			if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);
			Cfill=data[2].toInt(&ok);
			if(!ok)QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);
			fill=true;
			continue;
		};
		if(!need)//Читаем задание
		{
			bool ok;
			AfillR=data[0].toInt(&ok);
			if(!ok){
				QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);
				return false;
			};
			BfillR=data[1].toInt(&ok);
			if(!ok){
				QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);
				return false;
			};
			CfillR=data[2].toInt(&ok);
			if(!ok){
				QMessageBox::information( 0, "", trUtf8("Ошибка чтения: ") + fileName, 0,0,0);
				return false;
			};
			need=true;
			continue;
		};
	};
	VodFile.close();
	QSettings vSett("NIISI RAS","Vodoley");
	QFileInfo fi(VodFile);
	curDir=fi.absolutePath ();
	vSett.setValue("Dir",fi.absolutePath());
	vSett.setValue("File",fileName);

	setBaseFill(Afill,Bfill,Cfill);
	Amen->setNeedFill(AfillR);
	Bmen->setNeedFill(BfillR);
	Cmen->setNeedFill(CfillR);
	updateMenzur();
//	vodHeader->setWMTitle(QString::fromUtf8("Водолей - ") +  fi.bundleName());
        setWindowTitle(QString::fromUtf8("Водолей - ") +  fi.baseName());
	emit FileLoaded(fileName);
    return true;
};

void Vodoley::loadZ()
{
//    KumFileDialog dialog(this,trUtf8("Открыть файл"),curDir, "(*.vod)",false);
//    dialog.setAcceptMode(QFileDialog::AcceptOpen);
//    if(!dialog.exec())return;
//    QString VodFileN=dialog.selectedFiles().first();


    QString	VodFileN=QFileDialog::getOpenFileName(this, QString::fromUtf8 ("Открыть файл"), curDir, "(*.vod)");



    if ( VodFileN.isEmpty())return;


    loadFile(VodFileN);
};

void Vodoley::saveZ()
{
//    KumFileDialog dialog(this,trUtf8("Сохранить файл"),curDir, "(*.vod)",false);
//    dialog.setAcceptMode(QFileDialog::AcceptSave);
//    if(!dialog.exec())return;

//    QString VodFile=dialog.selectedFiles().first();


    QString	VodFile=QFileDialog::getSaveFileName(this, QString::fromUtf8 ("Сохранить файл"), curDir, "(*.vod)");

    if(VodFile.right(4)!=".vod")VodFile+=".vod";
    QFile vFile(VodFile);
  	if  (!vFile.open(QIODevice::WriteOnly))
	{
		QMessageBox::information( 0, "", trUtf8("Ошибка записи: ") + VodFile, 0,0,0);
        return;
	};

	//Zapis v fayl
	vFile.write(";Capacity\n");
	vFile.write((QString::number(Asize())+" "+QString::number(Bsize())+" "+QString::number(Csize())).toLatin1());
	vFile.write("\n;Fullness\n");
	vFile.write((QString::number(Afill)+" "+QString::number(Bfill)+" "+QString::number(Cfill)).toLatin1());
	vFile.write("\n;Need\n");
	vFile.write((QString::number(AfillR)+" "+QString::number(BfillR)+" "+QString::number(CfillR)).toLatin1());

	vFile.close();

	QSettings vSett("NIISI RAS","Vodoley");
	QFileInfo fi(vFile);
	curDir=fi.absolutePath ();
	vSett.setValue("Dir",fi.absolutePath());
	vSett.setValue("File",fi.filePath());
//	vodHeader->setWMTitle(QString::fromUtf8("Водолей - ") +  fi.baseName());
	setWindowTitle(QString::fromUtf8("Водолей - ") +  fi.baseName());
};


void Vodoley::updateMenzur()
{

    //qDebug()<<"Liter size"<<literSize;
    //mutex.lock();
    float literSize = MAX_SIZE/maxSize();
    if(Asize()==0){Amen->hide();Atext->hide();}//TODO LockPult
    else{Amen->show();Atext->show();};
    Amen->setSize(Asize());
    Amen->setLiterSize(literSize);
    Amen->setYoffset(40+(maxSize()-Asize())*literSize);
    Amen->setCurFill(CurA());

    if(Bsize()==0){Bmen->hide();Btext->hide();}//TODO LockPult
	else{Bmen->show();Btext->show();};
	Bmen->setSize(Bsize());
	Bmen->setLiterSize(literSize);
	Bmen->setYoffset(40+(maxSize()-Bsize())*literSize);
	Bmen->setCurFill(CurB());

	if(Csize()==0){Cmen->hide();Ctext->hide();}//TODO LockPult
	else{Cmen->show();Ctext->show();};
	Cmen->setSize(Csize());
	Cmen->setLiterSize(literSize);
	Cmen->setYoffset(40+(maxSize()-Csize())*literSize);
	Cmen->setCurFill(CurC());

	Atext->setPos(35,15+(maxSize()-Asize())*literSize);

	Btext->setPos(145,15+(maxSize()-Bsize())*literSize);

	Ctext->setPos(255,15+(maxSize()-Csize())*literSize);

  //  mutex.unlock();
    
    
	updateNeedBirka();
   // redraw();
//	if(Csize()<1)emit CNull();else emit CNotNull();
};
void Vodoley::redraw()
{
    mutex.lock();
    updateMenzur();
    view->update();
	scene->update();
    mutex.unlock();
}

void Vodoley::updateNeedBirka()
{
//mutex.lock();
    if(needFrame)
	{
		if((CurA()==AfillR)||(CurB()==AfillR)||(CurC()==AfillR)){needFrame->setPalette(QPalette(QColor(50,90,50),QColor(100,190,100)));}
		else needFrame->setPalette(QPalette(QColor(140,140,160),QColor(140,140,160)));

	}else {qDebug()<<"updateNeedBirka():No needFrame";};

	needLabel->setText(" "+QString::number(AfillR));
	qDebug()<<"NEED:"<<QString::number(AfillR);
   // mutex.unlock();
};
void Vodoley::mousePressEvent(QMouseEvent *event)
{
	QPointF point = event->pos() - rect().center();
	qDebug()<<"TailPoint:"<<point;
	//Curfill[0]=CurA()-1;
	Amen->setGp(point.x());
    redraw();
    qDebug()<<"View geometrey"<<view->geometry();
};
NewDialog::NewDialog()
{
	setWindowFlags(Qt::Dialog);
	QVBoxLayout *layout = new QVBoxLayout;
	sizeFrame=new QFrame(this);
	fillFrame=new QFrame(this);;
	valueFrame=new QFrame(this);
	buttonFrame=new QFrame(this);

	btnOK=new QToolButton(buttonFrame);
	btnOK->move(150,5);
	btnOK->setText(trUtf8("OK"));
	btnCancel=new QToolButton(buttonFrame);
	btnCancel->setText(trUtf8("Отмена"));
	btnCancel->move(50,5);
	connect(btnCancel,SIGNAL(pressed()),this,SLOT(close()));

	sizeFrame->setFrameStyle ( QFrame::Panel | QFrame::Raised );
	fillFrame->setFrameStyle ( QFrame::Panel | QFrame::Raised );
	valueFrame->setFrameStyle ( QFrame::Panel | QFrame::Raised );
	buttonFrame->setFrameStyle ( QFrame::Panel | QFrame::Raised );

	resize(200,170);
	sizeFrame->setMaximumSize( 200,70 );
	fillFrame->setMaximumSize( 200,70 );
	valueFrame->setMaximumSize( 200,70 );
	buttonFrame->setMaximumSize (200,40);

	layout->addWidget(sizeFrame);
	layout->addWidget(fillFrame);
	layout->addWidget(valueFrame);
	layout->addWidget(buttonFrame);
	this->setLayout(layout);
    
};

