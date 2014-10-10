/****************************************************************************
**
** Copyright (C) 2004-2008 NIISI RAS. All rights reserved.
**
** This file is part of the KuMir.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pult.h"
#include "robotmodule.h"


RoboPult::RoboPult ( QWidget* parent, Qt::WFlags fl )
	: QWidget ( parent, fl ), Ui::RoboPult()
{
	
	setupUi ( this );
	Logger=new pultLogger(ActorRobot::RobotModule::self->myResourcesDir(),this);
	Logger->setSizes(164,150);
	Logger->Move(40,5);	
        greenLight=new linkLight(this);
        greenLight->move(15,29);
        greenLight->resize(12,104);
    ClearLog->setIcon(QIcon(ActorRobot::RobotModule::self->myResourcesDir().absoluteFilePath("edit-delete.png")));
    ToKumir->setIcon(QIcon(ActorRobot::RobotModule::self->myResourcesDir().absoluteFilePath("kumir.png")));
        UpB->hide();
        buttUp=new MainButton(ActorRobot::RobotModule::self->myResourcesDir(),this);
        buttUp->move(90,170);

        DownB->hide();
        buttDown=new MainButton(ActorRobot::RobotModule::self->myResourcesDir(),this);
        buttDown->move(DownB->pos());
	buttDown->setDirection(DOWN);

        LeftB->hide();
        buttLeft=new MainButton(ActorRobot::RobotModule::self->myResourcesDir(),this);
        buttLeft->move(LeftB->pos());
        buttLeft->setDirection(LEFT);
        RightB->hide();
        buttRight=new MainButton(ActorRobot::RobotModule::self->myResourcesDir(),this);
        buttRight->move(RightB->pos());
        buttRight->setDirection(RIGHT);
        
        StenaB->hide();
        askStena=new MainButton(ActorRobot::RobotModule::self->myResourcesDir(),this);
        askStena->move(StenaB->pos());
        askStena->setCheckable(true);
	askStena->setText(trUtf8("  "));
	askStena->loadIcon(ActorRobot::RobotModule::self->myResourcesDir().absoluteFilePath("stena.png"));
	askStena->setCheckable(true);

        SvobodnoB->hide();
        askFree=new MainButton(ActorRobot::RobotModule::self->myResourcesDir(),this);
        askFree->move(SvobodnoB->pos());
        askFree->setCheckable(true);
	askFree->setText(trUtf8(" "));
	askFree->loadIcon(ActorRobot::RobotModule::self->myResourcesDir().absoluteFilePath("svobodno.png"));
     askFree->setIconOffset(10);
	askFree->setCheckable(true);

        RadB->hide();
        buttRad=new MainButton(ActorRobot::RobotModule::self->myResourcesDir(),this);
        buttRad->move(RadB->pos());
  
	buttRad->setText(trUtf8(" "));
     buttRad->setIconOffset(10);
	if(!buttRad->loadIcon(ActorRobot::RobotModule::self->myResourcesDir().absoluteFilePath("radiation.png")))qWarning("Image not loaded!");
	TempB->hide();
        buttTemp=new MainButton(ActorRobot::RobotModule::self->myResourcesDir(),this);
        buttTemp->move(TempB->pos());
    
	buttTemp->setText(" ");
    buttTemp->setIconOffset(10);
	buttTemp->loadIcon(ActorRobot::RobotModule::self->myResourcesDir().absoluteFilePath("tC.png"));

	CenterB->setIcon(QIcon(ActorRobot::RobotModule::self->myResourcesDir().absoluteFilePath("robo_field.png")));
	connect(buttUp,SIGNAL(clicked()),this,SLOT(Up()));
	connect(buttDown,SIGNAL(clicked()),this,SLOT(Down()));
	connect(buttLeft,SIGNAL(clicked()),this,SLOT(Left()));
	connect(buttRight,SIGNAL(clicked()),this,SLOT(Right()));

	connect(buttRad,SIGNAL(clicked()),this,SLOT(RadS()));
	connect(buttTemp,SIGNAL(clicked()),this,SLOT(TempS()));

	connect(askStena,SIGNAL(clicked()),this,SLOT(SwStena()));
	connect(askFree,SIGNAL(clicked()),this,SLOT(SwSvobodno()));
	connect(CenterB,SIGNAL(clicked()),this,SLOT(CenterButton()));

	connect(ClearLog,SIGNAL(clicked()),Logger,SLOT(ClearLog()));

	connect(ToKumir,SIGNAL(clicked()),Logger,SLOT(CopyLog()));

	link=true;
};

void RoboPult::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.save();
	p.setPen(Qt::NoPen);
	p.setBrush(QColor("#8C877E"));
	p.drawRect(0,0,width(),height());
	p.restore();
	QWidget::paintEvent(event);
}

void RoboPult::noLink(){
	link=false;
	greenLight->setLink(link);greenLight->repaint();qWarning("NoLINK");};
void RoboPult::LinkOK(){link=true;greenLight->setLink(link);greenLight->repaint();};

void RoboPult::Up()
  {
  if(askStena->isChecked () ){emit hasUpWall(); askStena->setChecked(false);qWarning("Up Stena");;return;};
  if(askFree->isChecked () ){emit noUpWall(); askFree->setChecked(false);return;};
  emit goUp();
  };
void RoboPult::Down()
  {
  if(askStena->isChecked () ){emit hasDownWall(); askStena->setChecked(false);return;};
  if(askFree->isChecked () ){emit noDownWall(); askFree->setChecked(false);return;};
  emit goDown();
  };
void RoboPult::Left()
  {
  if(askStena->isChecked () ){emit hasLeftWall(); askStena->setChecked(false);return;};
  if(askFree->isChecked () ){emit noLeftWall(); askFree->setChecked(false);return;};
  emit goLeft();
  };
void RoboPult::Right()
  {
  if(askStena->isChecked () ){emit hasRightWall(); askStena->setChecked(false);return;};
  if(askFree->isChecked () ){emit noRightWall(); askFree->setChecked(false);return;};
  emit goRight();
  };

void RoboPult::SwStena()
  {
   if(askFree->isChecked () ) askFree->setChecked(false);	
  };
void RoboPult::SwSvobodno()
  {
   if(askStena->isChecked () )askStena->setChecked(false);
  };
void RoboPult::TempS()
  {
   askStena->setChecked(false);	
   askFree->setChecked(false);
  emit Temp();
  };
void RoboPult::RadS()
  {
   askStena->setChecked(false);	
   askFree->setChecked(false);
  emit Rad();
  };

void RoboPult::CenterButton()
  {
  if(askStena->isChecked () ){emit Colored(); askStena->setChecked(false);return;};
  if(askFree->isChecked () ){emit Clean(); askFree->setChecked(false);return;};
  emit Color();
  };
